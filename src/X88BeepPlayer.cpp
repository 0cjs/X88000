////////////////////////////////////////////////////////////
// X88000 Beep Player
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88BeepPlayer.h"

#include "X88DirectX.h"

////////////////////////////////////////////////////////////
// implementation of CX88BeepPlayer

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88BeepPlayer::CX88BeepPlayer() {
}

// destructor

CX88BeepPlayer::~CX88BeepPlayer() {
}

////////////////////////////////////////////////////////////
// implementation of CX88BeepPlayerDirectSound

#ifdef X88_GUI_WINDOWS

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88BeepPlayerDirectSound::CX88BeepPlayerDirectSound(CX88DirectX& DX) :
	CX88BeepPlayer(),
	m_pDX(&DX),
	m_nBufferBits(8),
	m_nBufferChannels(2),
	m_nFrequency(BEEP_FREQUENCY),
	m_nVolume(50),
	m_nBeepSampleCount(0),
	m_bPlaying(false),
	m_bMute(false),
	m_pDSBBeep(NULL)
{
}

// destructor

CX88BeepPlayerDirectSound::~CX88BeepPlayerDirectSound() {
	Destroy();
}

////////////////////////////////////////////////////////////
// implementation

// create beep player

bool CX88BeepPlayerDirectSound::Create() {
	HRESULT hResult;
	Destroy();
	if (!GetDX().IsDirectSoundActive()) {
		return false;
	}
	uint32_t dwSizeWFX = 0;
	hResult = GetDX().GetDSBPrimary()->GetFormat(
		NULL, 0, &dwSizeWFX);
	if (hResult == DS_OK) {
		WAVEFORMATEX* pwfxPrimary =
			(WAVEFORMATEX*)(new uint8_t[dwSizeWFX]);
		hResult = GetDX().GetDSBPrimary()->GetFormat(
			pwfxPrimary, dwSizeWFX, NULL);
		if (hResult == DS_OK) {
			m_nBufferBits = pwfxPrimary->wBitsPerSample;
			m_nBufferChannels = pwfxPrimary->nChannels;
		}
		delete [] (uint8_t*)pwfxPrimary;
	}
	WAVEFORMATEX wfx;
	memset(&wfx, 0, sizeof(wfx));
	wfx.wFormatTag      = WAVE_FORMAT_PCM;
	wfx.nChannels       = (uint16_t)m_nBufferChannels;
	wfx.nSamplesPerSec  = BUFFER_FREQUENCY;
	wfx.wBitsPerSample  = (uint16_t)m_nBufferBits;
	wfx.nBlockAlign     = (uint16_t)(((wfx.wBitsPerSample+7)/8)*wfx.nChannels);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;
	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
		DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2;
	m_nBeepSampleCount = BUFFER_FREQUENCY/WAVE_FREQUENCY;
	if (m_nBeepSampleCount < SAMPLE_MIN) {
		if (m_nBeepSampleCount < 1) {
			m_nBeepSampleCount = 1;
		}
		int nWaveCount = (SAMPLE_MIN+m_nBeepSampleCount-1)/m_nBeepSampleCount;
		m_nBeepSampleCount =
			(nWaveCount*BUFFER_FREQUENCY)/WAVE_FREQUENCY;
	}
	dsbd.dwBufferBytes = m_nBeepSampleCount*wfx.nBlockAlign;
	dsbd.lpwfxFormat = &wfx;
	hResult = GetDX().CreateDSBSecondary(&dsbd, &m_pDSBBeep, NULL);
	if (hResult != DS_OK) {
		return false;
	}
	if (!InitBuffer() ||
		!SetFrequency(m_nFrequency) ||
		!SetVolume(m_nVolume))
	{
		return false;
	}
	return true;
}

// destroy beep player

bool CX88BeepPlayerDirectSound::Destroy() {
	if (m_pDSBBeep != NULL) {
		if (GetDX().IsDirectSoundActive()) {
			GetDX().ReleaseDSBSecondary(m_pDSBBeep);
		}
		m_pDSBBeep = NULL;
	}
	return true;
}

// get beep frequency

int CX88BeepPlayerDirectSound::GetFrequency() const {
	return m_nFrequency;
}

// set beep frequency

bool CX88BeepPlayerDirectSound::SetFrequency(int nFrequency) {
	m_nFrequency = nFrequency;
	if (m_pDSBBeep != NULL) {
		int nPlayFrequency =
			(BUFFER_FREQUENCY*m_nFrequency)/WAVE_FREQUENCY;
		if (nPlayFrequency < DSBFREQUENCY_MIN) {
			nPlayFrequency = DSBFREQUENCY_MIN;
		} else if (nPlayFrequency > DSBFREQUENCY_MAX) {
			nPlayFrequency = DSBFREQUENCY_MAX;
		}
		HRESULT hResult;
		hResult = m_pDSBBeep->SetFrequency(nPlayFrequency);
		if (hResult != DS_OK) {
			return false;
		}
		if (m_bPlaying) {
			if (IsLost()) {
				if (!Restore() || !Play(true)) {
					return false;
				}
			}
		}
	}
	return true;
}

// get beep volume

int CX88BeepPlayerDirectSound::GetVolume() const {
	return m_nVolume;
}

// set beep volume

bool CX88BeepPlayerDirectSound::SetVolume(int nVolume) {
	m_nVolume = nVolume;
	if (m_pDSBBeep != NULL) {
		int nPlayVolume = -(100-m_nVolume)*25;
		if (nPlayVolume < DSBVOLUME_MIN) {
			nPlayVolume = DSBVOLUME_MIN;
		} else if (nPlayVolume > DSBVOLUME_MAX) {
			nPlayVolume = DSBVOLUME_MAX;
		}
		if (IsMute()) {
			nPlayVolume = DSBVOLUME_MIN;
		}
		HRESULT hResult;
		hResult = m_pDSBBeep->SetVolume(nPlayVolume);
		if (hResult != DS_OK) {
			return false;
		}
		if (m_bPlaying) {
			if (IsLost()) {
				if (!Restore() || !Play(true)) {
					return false;
				}
			}
		}
	}
	return true;
}

// is playing

bool CX88BeepPlayerDirectSound::IsPlaying() const {
	return m_bPlaying;
}

// play start/stop

bool CX88BeepPlayerDirectSound::Play(bool bPlay) {
	HRESULT hResult;
	if (bPlay) {
		hResult = m_pDSBBeep->Play(0, 0, DSBPLAY_LOOPING);
		if (hResult == DSERR_BUFFERLOST) {
			if (Restore()) {
				hResult = m_pDSBBeep->Play(0, 0, DSBPLAY_LOOPING);
			}
		}
	} else {
		hResult = m_pDSBBeep->Stop();
	}
	if (hResult != DS_OK) {
		return false;
	}
	m_bPlaying = bPlay;
	return true;
}

// is muting

bool CX88BeepPlayerDirectSound::IsMute() const {
	return m_bMute;
}

// set muting

bool CX88BeepPlayerDirectSound::SetMute(bool bMute) {
	bool bResult = true;
	if (bMute != m_bMute) {
		m_bMute = bMute;
		bResult = SetVolume(GetVolume());
	}
	return bResult;
}

// is losted buffer

bool CX88BeepPlayerDirectSound::IsLost() const {
	uint32_t dwStatus = 0;
	HRESULT hResult = m_pDSBBeep->GetStatus(&dwStatus);
	return (hResult == DS_OK) &&
		((dwStatus & DSBSTATUS_BUFFERLOST) != 0);
}

// restore buffer

bool CX88BeepPlayerDirectSound::Restore() {
	HRESULT hResult = m_pDSBBeep->Restore();
	if (hResult != DS_OK) {
		return false;
	}
	return InitBuffer();
}

////////////////////////////////////////////////////////////
// operation

// initialize buffer

bool CX88BeepPlayerDirectSound::InitBuffer() {
	uint8_t* pbtBuf1 = NULL;
	uint8_t* pbtBuf2 = NULL;
	int nBufferBytes = (m_nBufferBits+7)/8;
	uint32_t dwBufSize = m_nBeepSampleCount*nBufferBytes*m_nBufferChannels;
	uint32_t dwBufSize1 = 0, dwBufSize2 = 0;
	HRESULT hResult = DS_OK;
	for (int i = 0; i < 2; i++) {
		hResult = m_pDSBBeep->Lock(
			0, dwBufSize,
			(void**)&pbtBuf1, &dwBufSize1,
			(void**)&pbtBuf2, &dwBufSize2,
			0);
		if (hResult != DSERR_BUFFERLOST) {
			break;
		}
		m_pDSBBeep->Restore();
	}
	if (hResult != DS_OK) {
		return false;
	}
	uint8_t* pbtBuf1Last = pbtBuf1+dwBufSize1;
	uint8_t* pbtWave = pbtBuf1;
	uint8_t abtData[2];
	bool bInBuffer1 = true;
	for (int nSample = 0; nSample < m_nBeepSampleCount; nSample++) {
		int nHalfWave =
			((nSample*WAVE_FREQUENCY*2)/BUFFER_FREQUENCY)%2;
		if (m_nBufferBits <= 8) {
			abtData[0] = (uint8_t)((nHalfWave <= 0)? 0xE0: 0x20);
		} else {
			int nData = (nHalfWave <= 0)? 16384: -16384;
			abtData[0] = LOBYTE(nData);
			abtData[1] = HIBYTE(nData);
		}
		for (int nChannel = 0; nChannel < m_nBufferChannels; nChannel++) {
			for (int nByte = 0; nByte < nBufferBytes; nByte++) {
				*(pbtWave++) = abtData[nByte];
				if (bInBuffer1 && (pbtWave >= pbtBuf1Last)) {
					bInBuffer1 = false;
					pbtWave = pbtBuf2;
				}
			}
		}
	}
	m_pDSBBeep->Unlock(pbtBuf1, dwBufSize1, pbtBuf2, dwBufSize2);
	return true;
}

#endif // X88_GUI
