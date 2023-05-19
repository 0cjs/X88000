////////////////////////////////////////////////////////////
// X88000 Environment Settings Dialog
//
// Written by Manuke

#ifndef X88EnvSetDlg_DEFINED
#define X88EnvSetDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88EnvSetDlg;

////////////////////////////////////////////////////////////
// include

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88EnvSetDlg

class CX88EnvSetDlg :
	public CX88Dialog
{
// attribute
protected:
	// dialog
	// page window handle
	static CX88WndHandle m_ahPageHandles[4];

	// general
	// UI language
	static int m_nUILanguage;
	// boost limiter
	static int m_nBoostLimiter;
	// floppy disk drive count
	static int m_nDriveCount;
	// hi-resolution mode
	static bool m_bHiresolution;
	// wait emulation
	static bool m_bWaitEmulation;
	// old-machine compatible mode
	static bool m_bOldCompatible;
	// PCG enable
	static bool m_bPcgEnable;
	// international display mode
	static bool m_bOptionFont;

	// display
	// frame-rate
	static int m_nScreenFrameRate;
	// interlace mode
	static bool m_bInterlace;

	// input
	// using DirectInput
	static bool m_bDirectInputUse;
	// is IME disable
	static bool m_bIMEDisable;

	// sound
	// using DirectSound
	static bool m_bDirectSoundUse;
	// DirectSound frequency
	static int m_nDirectSoundFrequency;
	// DirectSound bit count
	static int m_nDirectSoundBits;
	// DirectSound channel count
	static int m_nDirectSoundChannels;
	// beep player volume
	static int m_nBeepPlayerVolume;
	// beep player muting
	static bool m_bBeepPlayerMute;
	// PCG sound player volume
	static int m_nPcgPlayerVolume;
	// PCG sound player muting
	static bool m_bPcgPlayerMute;

public:
	// general
	// get UI language
	static int GetUILanguage() {
		return m_nUILanguage;
	}
	// set UI language
	static void SetUILanguage(int nUILanguage) {
		m_nUILanguage = nUILanguage;
	}
	// get boost limiter
	static int GetBoostLimiter() {
		return m_nBoostLimiter;
	}
	// set boost limiter
	static void SetBoostLimiter(int nBoostLimiter) {
		m_nBoostLimiter = nBoostLimiter;
	}
	// get floppy disk drive count
	static int GetDriveCount() {
		return m_nDriveCount;
	}
	// set floppy disk drive count
	static void SetDriveCount(int nDriveCount) {
		m_nDriveCount = nDriveCount;
	}
	// is hi-resolution mode
	static bool IsHiresolution() {
		return m_bHiresolution;
	}
	// set hi-resolution mode
	static void SetHiresolution(bool bHiresolution) {
		m_bHiresolution = bHiresolution;
	}
	// is wait emulation
	static bool IsWaitEmulation() {
		return m_bWaitEmulation;
	}
	// set wait emulation
	static void SetWaitEmulation(bool bWaitEmulation) {
		m_bWaitEmulation = bWaitEmulation;
	}
	// is old-machine compatible mode
	static bool IsOldCompatible() {
		return m_bOldCompatible;
	}
	// set old-machine compatible mode
	static void SetOldCompatible(bool bOldCompatible) {
		m_bOldCompatible = bOldCompatible;
	}
	// is PCG enable
	static bool IsPcgEnable() {
		return m_bPcgEnable;
	}
	// set PCG enable
	static void SetPcgEnable(bool bPcgEnable) {
		m_bPcgEnable = bPcgEnable;
	}
	// is international display mode
	static bool IsOptionFont() {
		return m_bOptionFont;
	}
	// set international display mode
	static void SetOptionFont(bool bOptionFont) {
		m_bOptionFont = bOptionFont;
	}
	// display
	// get frame-rate
	static int GetScreenFrameRate() {
		return m_nScreenFrameRate;
	}
	// set frame-rate
	static void SetScreenFrameRate(int nScreenFrameRate) {
		m_nScreenFrameRate = nScreenFrameRate;
	}
	// is interlace mode
	static bool IsInterlace() {
		return m_bInterlace;
	}
	// set interlace mode
	static void SetInterlace(bool bInterlace) {
		m_bInterlace = bInterlace;
	}
	// input
	// is using DirectInput
	static bool IsDirectInputUse() {
		return m_bDirectInputUse;
	}
	// set using DirectInput
	static void SetDirectInputUse(bool bDirectInputUse) {
		m_bDirectInputUse = bDirectInputUse;
	}
	// is IME disable
	static bool IsIMEDisable() {
		return m_bIMEDisable;
	}
	// set IME disable
	static void SetIMEDisable(bool bIMEDisable) {
		m_bIMEDisable = bIMEDisable;
	}
	// sound
	// is using DirectSound
	static bool IsDirectSoundUse() {
		return m_bDirectSoundUse;
	}
	// set using DirectSound
	static void SetDirectSoundUse(bool bDirectSoundUse) {
		m_bDirectSoundUse = bDirectSoundUse;
	}
	// get DirectSound frequency
	static int GetDirectSoundFrequency() {
		return m_nDirectSoundFrequency;
	}
	// set DirectSound frequency
	static void SetDirectSoundFrequency(int nDirectSoundFrequency) {
		m_nDirectSoundFrequency = nDirectSoundFrequency;
	}
	// get DirectSound bit count
	static int GetDirectSoundBits() {
		return m_nDirectSoundBits;
	}
	// set DirectSound bit count
	static void SetDirectSoundBits(int nDirectSoundBits) {
		m_nDirectSoundBits = nDirectSoundBits;
	}
	// get DirectSound channel count
	static int GetDirectSoundChannels() {
		return m_nDirectSoundChannels;
	}
	// set DirectSound channel count
	static void SetDirectSoundChannels(int nDirectSoundChannels) {
		m_nDirectSoundChannels = nDirectSoundChannels;
	}
	// get beep player volume
	static int GetBeepPlayerVolume() {
		return m_nBeepPlayerVolume;
	}
	// set beep player volume
	static void SetBeepPlayerVolume(int nBeepPlayerVolume) {
		m_nBeepPlayerVolume = nBeepPlayerVolume;
	}
	// is beep player muting
	static bool IsBeepPlayerMute() {
		return m_bBeepPlayerMute;
	}
	// set beep player muting
	static void SetBeepPlayerMute(bool bBeepPlayerMute) {
		m_bBeepPlayerMute = bBeepPlayerMute;
	}
	// get PCG sound player volume
	static int GetPcgPlayerVolume() {
		return m_nPcgPlayerVolume;
	}
	// set PCG sound player volume
	static void SetPcgPlayerVolume(int nPcgPlayerVolume) {
		m_nPcgPlayerVolume = nPcgPlayerVolume;
	}
	// is PCG sound player muting
	static bool IsPcgPlayerMute() {
		return m_bPcgPlayerMute;
	}
	// set PCG sound player muting
	static void SetPcgPlayerMute(bool bPcgPlayerMute) {
		m_bPcgPlayerMute = bPcgPlayerMute;
	}

// create & destroy
public:
	// standard constructor
	CX88EnvSetDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88EnvSetDlg();

// implementation
protected:

#ifdef X88_GUI_WINDOWS

	// general dialog procedure
	static BOOL CALLBACK GeneralDlgProc(
		HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam);
	// display dialog procedure
	static BOOL CALLBACK DisplayDlgProc(
		HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam);
	// input dialog procedure
	static BOOL CALLBACK InputDlgProc(
		HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam);
	// sound dialog procedure
	static BOOL CALLBACK SoundDlgProc(
		HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam);

#elif defined(X88_GUI_GTK)

	// dialog procedure(initialize & dispose)
	static bool DlgProc(
		GtkWidget* pDialog,
		bool bInitialize,
		int nID);
	// UI language changed signal
	static void OnChangedSignalUILanguage(
		GtkWidget* pWidget, gpointer pData);
	// boost limiter changed signal
	static void OnChangedSignalBoostLimiter(
		GtkWidget* pWidget, gpointer pData);
	// floppy disk drive count toggled signal
	static void OnToggledSignalDriveCount(
		GtkToggleButton* pToggleButton, gpointer pData);
	// hi-resolution mode toggled signal
	static void OnToggledSignalHiresolution(
		GtkToggleButton* pToggleButton, gpointer pData);
	// wait emulation mode toggled signal
	static void OnToggledSignalWaitEmulation(
		GtkToggleButton* pToggleButton, gpointer pData);
	// old-machine compatible mode signal
	static void OnToggledSignalOldCompatible(
		GtkToggleButton* pToggleButton, gpointer pData);
	// PCG enable signal
	static void OnToggledSignalPcgEnable(
		GtkToggleButton* pToggleButton, gpointer pData);
	// international display signal
	static void OnToggledSignalOptionFont(
		GtkToggleButton* pToggleButton, gpointer pData);
	// frame-rate changed signal
	static void OnChangedSignalScreenFrameRate(
		GtkWidget* pWidget, gpointer pData);
	// interlace mode toggled signal
	static void OnToggledSignalInterlace(
		GtkToggleButton* pToggleButton, gpointer pData);
	// DirectInput toggled signal
	static void OnToggledSignalDirectInput(
		GtkToggleButton* pToggleButton, gpointer pData);
	// IME toggled signal
	static void OnToggledSignalIME(
		GtkToggleButton* pToggleButton, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// general
	// set general children
	static void SetChildrenGeneral(CX88WndHandle hdlg);
	// on changed UI language
	static void OnUILanguageChange(CX88WndHandle hdlg);
	// on changed boost limiter
	static void OnBoostLimiterChange(CX88WndHandle hdlg);
	// on changed floppy disk drive count
	static void OnDriveCountChange(CX88WndHandle hdlg);
	// on changed hi-resolution mode
	static void OnHiresolutionChange(CX88WndHandle hdlg);
	// on chabnged wait emulation mode
	static void OnWaitEmulationChange(CX88WndHandle hdlg);
	// on changed old-machine compatible mode
	static void OnOldCompatibleChange(CX88WndHandle hdlg);
	// on changed PCG enable
	static void OnPcgChange(CX88WndHandle hdlg);
	// on changed international display mode
	static void OnOptFontChange(CX88WndHandle hdlg);
	// display
	// set display children
	static void SetChildrenDisplay(CX88WndHandle hdlg);
	// on changed frame-rate
	static void OnScreenFrameRateChange(CX88WndHandle hdlg);
	// on changed interlace mode
	static void OnInterlaceChange(CX88WndHandle hdlg);
	// input
	// set input children
	static void SetChildrenInput(CX88WndHandle hdlg);
	// on changed using DirectInput
	static void OnDirectInputUseChange(CX88WndHandle hdlg);
	// on changed IME disable
	static void OnIMEDisableChange(CX88WndHandle hdlg);
	// sound
	// set sound children
	static void SetChildrenSound(CX88WndHandle hdlg);
	// on changed using DirectSound
	static void OnDirectSoundUseChange(CX88WndHandle hdlg);
	// on changed DirectSound frequency
	static void OnDirectSoundFrequencyChange(CX88WndHandle hdlg);
	// on changed DirectSound bit count
	static void OnDirectSoundBitsChange(CX88WndHandle hdlg);
	// on changed DirectSound channel count
	static void OnDirectSoundChannelsChange(CX88WndHandle hdlg);
	// on changed beep player volume
	static void OnBeepPlayerVolumeChange(CX88WndHandle hdlg);
	// on changed beep player mute state
	static void OnBeepPlayerMuteChange(CX88WndHandle hdlg);
	// on changed PCG sound player volume
	static void OnPcgPlayerVolumeChange(CX88WndHandle hdlg);
	// on changed PCG sound player mute state
	static void OnPcgPlayerMuteChange(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88EnvSetDlg_DEFINED
