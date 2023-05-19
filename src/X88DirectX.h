////////////////////////////////////////////////////////////
// X88000 DirectX Manager
//
// Written by Manuke

#ifndef X88DirectX_DEFINED
#define X88DirectX_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88DirectX;

////////////////////////////////////////////////////////////
// declaration of CX88DirectX

#ifdef X88_GUI_WINDOWS

class CX88DirectX {
// enum
public:
	// palette
	enum {
		ALLPAL_COUNT = 256,
		SYSPAL_COUNT = 20
	};

// attribute
protected:
	// DirectDraw library module
	static HMODULE m_hlibDDraw;
	// DirectSound library module
	static HMODULE m_hlibDSound;
	// DirectInput library module
	static HMODULE m_hlibDInput;

	// DirectDraw active
	static bool m_bDirectDrawActive;
	// screen bit per pixel
	static int m_nScreenBPP;

	// DirectDraw object
	static IDirectDraw* m_pDD;

	// DirectSound active
	static bool m_bDirectSoundActive;

	// DirectSound object
	static IDirectSound* m_pDS;
	// primary sound buffer
	static IDirectSoundBuffer* m_pDSBPrimary;
	// secondary sound buffers
	static std::list<IDirectSoundBuffer*> m_listDSBSecondaries;

	// DirectInput active
	static bool m_bDirectInputActive;

	// DirectInput object
	static IDirectInput* m_pDI;
	// keyboard device
	static IDirectInputDevice* m_pDIDKeyboard;

public:
	// is DirectDraw active
	static bool IsDirectDrawActive() {
		return m_bDirectDrawActive;
	}
	// get screen bit per pixel
	static int GetScreenBPP() {
		return m_nScreenBPP;
	}
	// get DirectDraw
	static IDirectDraw* GetDD() {
		return m_pDD;
	}

	// is DirectSound active
	static bool IsDirectSoundActive() {
		return m_bDirectSoundActive;
	}
	// get DirectSound object
	static IDirectSound* GetDS() {
		return m_pDS;
	}
	// get primary sound buffer
	static IDirectSoundBuffer* GetDSBPrimary() {
		return m_pDSBPrimary;
	}

	// is DirectInput active
	static bool IsDirectInputActive() {
		return m_bDirectInputActive;
	}
	// get DirectInput object
	static IDirectInput* GetDI() {
		return m_pDI;
	}
	// get keyboard device
	static IDirectInputDevice* GetDIDKeyboard() {
		return m_pDIDKeyboard;
	}

// create & destroy
public:
	// default constructor
	CX88DirectX();
	// destructor
	~CX88DirectX();

	// initialize
	static bool Initialize();
	// dispose
	static bool Dispose();

// DirectDraw
public:
	// start DirectDraw full-screen
	static bool StartDirectDraw(
		HWND hwndFrame);
	// end DirectDraw full-screen
	static bool EndDirectDraw();

// DirectSound
public:
	// start DirectSound
	static bool StartDirectSound(
		HWND hwndFrame,
		int nFrequency, int nBits, int nChannels);
	// stop DirectSound
	static bool EndDirectSound();
	// create secondary sound buffer
	static HRESULT CreateDSBSecondary(
		const DSBUFFERDESC* pDSBD,
		IDirectSoundBuffer** ppDSBSecondary,
		IUnknown* pUnkOuter);
	// release secondary sound buffer
	static bool ReleaseDSBSecondary(
		IDirectSoundBuffer* pDSBSecondary);

// DirectInput
public:
	// start DirectInput
	static bool StartDirectInput(HWND hwndFrame);
	// stop DirectInput
	static bool EndDirectInput();
};

#else // !X88_GUI_WINDOWS

class CX88DirectX {
// create & destroy
public:
	// default constructor
	CX88DirectX();
	// destructor
	~CX88DirectX();

	// initialize
	static bool Initialize();
	// dispose
	static bool Dispose();
};

#endif // X88_GUI

#endif // X88DirectX_DEFINED
