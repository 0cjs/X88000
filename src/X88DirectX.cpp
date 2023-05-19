////////////////////////////////////////////////////////////
// X88000 DirectX Manager
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88DirectX.h"

////////////////////////////////////////////////////////////
// implementation of CX88DirectX

#ifdef X88_GUI_WINDOWS

////////////////////////////////////////////////////////////
// attribute

// DirectDraw library module

HMODULE CX88DirectX::m_hlibDDraw;

// DirectSound library module

HMODULE CX88DirectX::m_hlibDSound;

// DirectInput library module

HMODULE CX88DirectX::m_hlibDInput;

// DirectDraw active

bool CX88DirectX::m_bDirectDrawActive;

// screen bit per pixel

int CX88DirectX::m_nScreenBPP;

// DirectDraw

IDirectDraw* CX88DirectX::m_pDD;

// DirectSound active

bool CX88DirectX::m_bDirectSoundActive;

// DirectSound object

IDirectSound* CX88DirectX::m_pDS;

// primary sound buffer

IDirectSoundBuffer* CX88DirectX::m_pDSBPrimary;

// secondary sound buffers

std::list<IDirectSoundBuffer*> CX88DirectX::m_listDSBSecondaries;

// DirectInput active

bool CX88DirectX::m_bDirectInputActive;

// DirectInput object

IDirectInput* CX88DirectX::m_pDI;

// keyboard device

IDirectInputDevice* CX88DirectX::m_pDIDKeyboard;

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88DirectX::CX88DirectX() {
	m_bDirectDrawActive = false;
	m_nScreenBPP = 0;

	m_pDD = NULL;

	m_bDirectSoundActive = false;

	m_pDS = NULL;
	m_pDSBPrimary = NULL;

	m_bDirectInputActive = false;

	m_pDI = NULL;
	m_pDIDKeyboard = NULL;
}

// destructor

CX88DirectX::~CX88DirectX() {
}

// initialize

bool CX88DirectX::Initialize() {
	m_hlibDDraw = LoadLibrary("ddraw.dll");
	m_hlibDSound = LoadLibrary("dsound.dll");
	m_hlibDInput = LoadLibrary("dinput.dll");
	return true;
}

// dispose

bool CX88DirectX::Dispose() {
	if (IsDirectDrawActive()) {
		EndDirectDraw();
	}
	if (IsDirectSoundActive()) {
		EndDirectSound();
	}
	if (IsDirectInputActive()) {
		EndDirectInput();
	}
	if (m_hlibDDraw != NULL) {
		FreeLibrary(m_hlibDDraw);
		m_hlibDDraw = NULL;
	}
	if (m_hlibDSound != NULL) {
		FreeLibrary(m_hlibDSound);
		m_hlibDSound = NULL;
	}
	if (m_hlibDInput != NULL) {
		FreeLibrary(m_hlibDInput);
		m_hlibDInput = NULL;
	}
	return true;
}

////////////////////////////////////////////////////////////
// DirectDraw

// start DirectDraw full-screen

bool CX88DirectX::StartDirectDraw(
	HWND hwndFrame)
{
	if (m_hlibDDraw == NULL) {
		return false;
	}
	typedef HRESULT WINAPI DirectDrawCreateProc(
		GUID*, IDirectDraw**, IUnknown*);
	DirectDrawCreateProc* pprocDirectDrawCreate = (DirectDrawCreateProc*)
		GetProcAddress(m_hlibDDraw, "DirectDrawCreate");
	if (pprocDirectDrawCreate == NULL) {
		return false;
	}
	HRESULT hResult;
	// DirectDraw
	hResult = pprocDirectDrawCreate(NULL, &m_pDD, NULL);
	if (hResult != DD_OK) {
		return false;
	}
	hResult = m_pDD->SetCooperativeLevel(
		hwndFrame,
		DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);
	if (hResult != DD_OK) {
		return false;
	}
	int anBPPs[] = {32, 24, 16, 8};
	for (int n = 0; n < sizeof(anBPPs)/sizeof(anBPPs[0]); n++) {
		m_nScreenBPP = anBPPs[n];
		hResult = m_pDD->SetDisplayMode(640, 480, m_nScreenBPP);
		if (hResult == DD_OK) {
			break;
		}
	}
	if (hResult != DD_OK) {
		return false;
	}
	m_bDirectDrawActive = true;
	return true;
}

// end DirectDraw full-screen

bool CX88DirectX::EndDirectDraw() {
	m_bDirectDrawActive = false;
	m_nScreenBPP = 0;
	if (m_pDD != NULL) {
		m_pDD->RestoreDisplayMode();
		m_pDD->Release();
		m_pDD = NULL;
	}
	return true;
}

////////////////////////////////////////////////////////////
// DirectSound

// start DirectSound

bool CX88DirectX::StartDirectSound(
	HWND hwndFrame,
	int nFrequency, int nBits, int nChannels)
{
	if (m_hlibDSound == NULL) {
		return false;
	}
	typedef HRESULT WINAPI DirectSoundCreateProc(
		GUID*, IDirectSound**, IUnknown*);
	DirectSoundCreateProc* pprocDirectSoundCreate = (DirectSoundCreateProc*)
		GetProcAddress(m_hlibDSound, "DirectSoundCreate");
	if (pprocDirectSoundCreate == NULL) {
		return false;
	}
	HRESULT hResult;
	// DirectSound
	hResult = pprocDirectSoundCreate(NULL, &m_pDS, NULL);
	if (hResult != DS_OK) {
		return false;
	}
	int nLevel = DSSCL_PRIORITY;
	for (int i = 0; i < 3; i++) {
		switch (i) {
		case 0:
			nLevel = DSSCL_PRIORITY;
			break;
		case 1:
			nLevel = DSSCL_NORMAL;
			break;
		case 2:
			nLevel = DSSCL_EXCLUSIVE;
			break;
		}
		hResult = m_pDS->SetCooperativeLevel(hwndFrame, nLevel);
		if (hResult == DS_OK) {
			break;
		}
	}
	if (hResult != DS_OK) {
		return false;
	}
	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	hResult = m_pDS->CreateSoundBuffer(&dsbd, &m_pDSBPrimary, NULL);
	if (hResult != DS_OK) {
		return false;
	}
	WAVEFORMATEX wfx;
	memset(&wfx, 0, sizeof(wfx));
	if (nLevel != DSSCL_NORMAL) {
		wfx.wFormatTag      = WAVE_FORMAT_PCM;
		wfx.nChannels       = (uint16_t)nChannels;
		wfx.nSamplesPerSec  = nFrequency;
		wfx.wBitsPerSample  = (uint16_t)nBits;
		wfx.nBlockAlign     = (uint16_t)(((nBits+7)/8)*nChannels);
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;
		hResult = m_pDSBPrimary->SetFormat(&wfx);
		if (hResult != DS_OK) {
			// ignore error
		}
	} else {
		// normal format
		wfx.wFormatTag      = WAVE_FORMAT_PCM;
		wfx.nChannels       = 2;
		wfx.nSamplesPerSec  = 22050;
		wfx.wBitsPerSample  = 8;
		wfx.nBlockAlign     = (uint16_t)(((8+7)/8)*2);
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;
	}
	// blank sound(for windows sound drive bug)
	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS;
	dsbd.dwBufferBytes = wfx.nSamplesPerSec*wfx.nBlockAlign;
	dsbd.lpwfxFormat = &wfx;
	IDirectSoundBuffer* pDSBBlank = NULL;
	hResult = CreateDSBSecondary(&dsbd, &pDSBBlank, NULL);
	if (hResult == DS_OK) {
		void* pAudioBuf1 = NULL;
		void* pAudioBuf2 = NULL;
		uint32_t dwAudioBytes1 = 0, dwAudioBytes2 = 0;
		uint8_t btFill = (uint8_t)((wfx.wBitsPerSample > 8)? 0x00: 0x80);
		hResult = pDSBBlank->Lock(
			0, dsbd.dwBufferBytes,
			&pAudioBuf1, &dwAudioBytes1,
			&pAudioBuf2, &dwAudioBytes2,
			0);
		if (hResult == DSERR_BUFFERLOST) {
			hResult = pDSBBlank->Restore();
			if (hResult == DS_OK) {
				hResult = pDSBBlank->Lock(
					0, dsbd.dwBufferBytes,
					&pAudioBuf1, &dwAudioBytes1,
					&pAudioBuf2, &dwAudioBytes2,
					0);
			}
		}
		if (hResult == DS_OK) {
			if (dwAudioBytes1 > 0) {
				memset(pAudioBuf1, btFill, dwAudioBytes1);
			}
			if (dwAudioBytes2 > 0) {
				memset(pAudioBuf2, btFill, dwAudioBytes2);
			}
			hResult = pDSBBlank->Unlock(
				pAudioBuf1, dwAudioBytes1,
				pAudioBuf2, dwAudioBytes2);
		}
		if (hResult == DS_OK) {
			pDSBBlank->Play(0, 0, DSBPLAY_LOOPING);
		}
	}
	m_bDirectSoundActive = true;
	return true;
}

// stop DirectSound

bool CX88DirectX::EndDirectSound() {
	m_bDirectSoundActive = false;
	while (m_listDSBSecondaries.size() > 0) {
		(m_listDSBSecondaries.back())->Release();
		m_listDSBSecondaries.pop_back();
	}
	if (m_pDSBPrimary != NULL) {
		m_pDSBPrimary->Release();
		m_pDSBPrimary = NULL;
	}
	if (m_pDS != NULL) {
		m_pDS->Release();
		m_pDS = NULL;
	}
	return true;
}

// create secondary sound buffer

HRESULT CX88DirectX::CreateDSBSecondary(
	const DSBUFFERDESC* pDSBD,
	IDirectSoundBuffer** ppDSBSecondary,
	IUnknown* pUnkOuter)
{
	HRESULT hResult = m_pDS->CreateSoundBuffer(
		pDSBD, ppDSBSecondary, pUnkOuter);
	if (hResult == DS_OK) {
		m_listDSBSecondaries.push_back(*ppDSBSecondary);
	}
	return hResult;
}

// release secondary sound buffer

bool CX88DirectX::ReleaseDSBSecondary(
	IDirectSoundBuffer* pDSBSecondary)
{
	std::list<IDirectSoundBuffer*>::iterator itFind = std::find(
		m_listDSBSecondaries.begin(), m_listDSBSecondaries.end(),
		pDSBSecondary);
	if (itFind == m_listDSBSecondaries.end()) {
		return false;
	}
	m_listDSBSecondaries.erase(itFind);
	pDSBSecondary->Release();
	return true;
}

////////////////////////////////////////////////////////////
// DirectInput

// start DirectInput

bool CX88DirectX::StartDirectInput(HWND hwndFrame) {
	if (m_hlibDInput == NULL) {
		return false;
	}
	typedef HRESULT WINAPI DirectInputCreateProc(
		HINSTANCE, DWORD, IDirectInput**, IUnknown*);
	DirectInputCreateProc* pprocDirectInputCreate = (DirectInputCreateProc*)
		GetProcAddress(m_hlibDInput, "DirectInputCreateA");
	if (pprocDirectInputCreate == NULL) {
		return false;
	}
	HRESULT hResult;
	// DirectInput
	hResult = pprocDirectInputCreate(
		(HINSTANCE)GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, &m_pDI, NULL);
	if (hResult != DI_OK) {
		return false;
	}
	// DirectInputDevice
	hResult = m_pDI->CreateDevice(
		GUID_SysKeyboard, &m_pDIDKeyboard, NULL);
	if (hResult != DI_OK) {
		return false;
	}
	hResult = m_pDIDKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (hResult != DI_OK) {
		return false;
	}
	hResult = m_pDIDKeyboard->SetCooperativeLevel(
		hwndFrame, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (hResult != DI_OK) {
		return false;
	}
	m_bDirectInputActive = true;
	return true;
}

// stop DirectInput

bool CX88DirectX::EndDirectInput() {
	m_bDirectInputActive = false;
	if (m_pDIDKeyboard != NULL) {
		m_pDIDKeyboard->Release();
		m_pDIDKeyboard = NULL;
	}
	if (m_pDI != NULL) {
		m_pDI->Release();
		m_pDI = NULL;
	}
	return true;
}

#else // !X88_GUI_WINDOWS

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88DirectX::CX88DirectX() {
}

// destructor

CX88DirectX::~CX88DirectX() {
}

// initialize

bool CX88DirectX::Initialize() {
	return true;
}

// dispose

bool CX88DirectX::Dispose() {
	return true;
}

#endif // X88_GUI
