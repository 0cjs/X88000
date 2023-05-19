////////////////////////////////////////////////////////////
// X88000 Beep Player
//
// Written by Manuke

#ifndef X88BeepPlayer_DEFINED
#define X88BeepPlayer_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88BeepPlayer;

////////////////////////////////////////////////////////////
// include

#include "X88DirectX.h"

////////////////////////////////////////////////////////////
// declaration of CX88BeepPlayer

class CX88BeepPlayer {
// enum
public:
	// frequency
	enum {
		BEEP_FREQUENCY = 2400
	};

// create & destroy
public:
	// default constructor
	CX88BeepPlayer();
	// destructor
	virtual ~CX88BeepPlayer();

// implementation
public:
	// create beep player
	virtual bool Create() = 0;
	// destroy beep player
	virtual bool Destroy() = 0;
	// get beep frequency
	virtual int GetFrequency() const = 0;
	// set beep frequency
	virtual bool SetFrequency(int nFrequency) = 0;
	// get beep volume
	virtual int GetVolume() const = 0;
	// set beep volume
	virtual bool SetVolume(int nVolume) = 0;
	// is muting
	virtual bool IsMute() const = 0;
	// set muting
	virtual bool SetMute(bool bMute) = 0;
	// is playing
	virtual bool IsPlaying() const = 0;
	// play start/stop
	virtual bool Play(bool bPlay) = 0;
	// is losted buffer
	virtual bool IsLost() const = 0;
	// restore buffer
	virtual bool Restore() = 0;
};

////////////////////////////////////////////////////////////
// declaration of CX88BeepPlayerDirectSound

#ifdef X88_GUI_WINDOWS

class CX88BeepPlayerDirectSound :
	public CX88BeepPlayer
{
// enum
protected:
	// frequency
	enum {
		BUFFER_FREQUENCY = 22050,
		WAVE_FREQUENCY   = 2450,
		SAMPLE_MIN       = BUFFER_FREQUENCY/10
	};

// attribute
protected:
	// DirectX manager
	CX88DirectX* m_pDX;
	// sound buffer bit count
	int m_nBufferBits;
	// sound buffer channel count
	int m_nBufferChannels;
	// beep frequency
	int m_nFrequency;
	// beep volume
	int m_nVolume;
	// beep sample count
	int m_nBeepSampleCount;
	// playing
	bool m_bPlaying;
	// muting
	bool m_bMute;
	// DirectSound buffer
	IDirectSoundBuffer* m_pDSBBeep;

public:
	// get DirectX manager
	CX88DirectX& GetDX() {
		return *m_pDX;
	}

// create & destroy
public:
	// standard constructor
	CX88BeepPlayerDirectSound(CX88DirectX& DX);
	// destructor
	virtual ~CX88BeepPlayerDirectSound();

// implementation
public:
	// create beep player
	virtual bool Create();
	// destroy beep player
	virtual bool Destroy();
	// get beep frequency
	virtual int GetFrequency() const;
	// set beep frequency
	virtual bool SetFrequency(int nFrequency);
	// get beep volume
	virtual int GetVolume() const;
	// set beep volume
	virtual bool SetVolume(int nVolume);
	// is muting
	virtual bool IsMute() const;
	// set muting
	virtual bool SetMute(bool bMute);
	// is playing
	virtual bool IsPlaying() const;
	// play start/stop
	virtual bool Play(bool bPlay);
	// is losted buffer
	virtual bool IsLost() const;
	// restore buffer
	virtual bool Restore();

// operation
protected:
	// initialize buffer
	virtual bool InitBuffer();
};

#endif // X88_GUI

#endif // X88BeepPlayer_DEFINED
