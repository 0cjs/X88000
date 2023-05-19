////////////////////////////////////////////////////////////
// X88000 Environment Settings Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88EnvSetDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88EnvSetDlg

////////////////////////////////////////////////////////////
// attribute

// dialog

// page window handle

CX88WndHandle CX88EnvSetDlg::m_ahPageHandles[4];

// general

// UI language

int CX88EnvSetDlg::m_nUILanguage;

// boost limiter

int CX88EnvSetDlg::m_nBoostLimiter;

// floppy disk drive count

int CX88EnvSetDlg::m_nDriveCount;

// hi-resolution mode

bool CX88EnvSetDlg::m_bHiresolution;

// wait emulation

bool CX88EnvSetDlg::m_bWaitEmulation;

// old-machine compatible mode

bool CX88EnvSetDlg::m_bOldCompatible;

// PCG enable

bool CX88EnvSetDlg::m_bPcgEnable;

// international display mode

bool CX88EnvSetDlg::m_bOptionFont;

// display

// frame-rate

int CX88EnvSetDlg::m_nScreenFrameRate;

// interlace mode

bool CX88EnvSetDlg::m_bInterlace;

// input

// using DirectInput

bool CX88EnvSetDlg::m_bDirectInputUse;

// IME disable

bool CX88EnvSetDlg::m_bIMEDisable;

// sound

// using DirectSound

bool CX88EnvSetDlg::m_bDirectSoundUse;

// DirectSound frequency

int CX88EnvSetDlg::m_nDirectSoundFrequency;

// DirectSound bit count

int CX88EnvSetDlg::m_nDirectSoundBits;

// DirectSound channel count

int CX88EnvSetDlg::m_nDirectSoundChannels;

// beep player volume

int CX88EnvSetDlg::m_nBeepPlayerVolume;

// beep player muting

bool CX88EnvSetDlg::m_bBeepPlayerMute;

// PCG sound player volume

int CX88EnvSetDlg::m_nPcgPlayerVolume;

// PCG sound player muting

bool CX88EnvSetDlg::m_bPcgPlayerMute;

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88EnvSetDlg::CX88EnvSetDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	memset(m_ahPageHandles, 0, sizeof(m_ahPageHandles));
	m_nUILanguage = 0;
	m_nBoostLimiter = 0;
	m_nDriveCount = 2;
	m_bHiresolution = true;
	m_bWaitEmulation = false;
	m_bOldCompatible = false;
	m_bPcgEnable = false;
	m_bOptionFont = false;
	m_nScreenFrameRate = 20;
	m_bInterlace = false;
	m_bDirectInputUse = true;
	m_bIMEDisable = false;
	m_bDirectSoundUse = true;
	m_nDirectSoundFrequency = 44100;
	m_nDirectSoundBits = 8;
	m_nDirectSoundChannels = 1;
	m_nBeepPlayerVolume = 50;
	m_bBeepPlayerMute = false;
	m_nPcgPlayerVolume = 50;
	m_bPcgPlayerMute = false;
}

// destructor

CX88EnvSetDlg::~CX88EnvSetDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// general dialog procedure

BOOL CALLBACK CX88EnvSetDlg::GeneralDlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		{ // dummy block
			m_ahPageHandles[0] = hdlg;
			HWND hsheet = GetParent(hdlg);
			Centering(hsheet);
			SetWindowLong(
				hsheet, GWL_EXSTYLE,
				GetWindowLong(hsheet, GWL_EXSTYLE) & ~WS_EX_CONTEXTHELP);
			SetChildrenGeneral(hdlg);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			bResult = FALSE;
			break;
		case IDC_UI_LANGUAGE:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				OnUILanguageChange(hdlg);
			}
			break;
		case IDC_BOOST_LIMITER:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				OnBoostLimiterChange(hdlg);
			}
			break;
		case IDC_DISK_DRIVE1:
		case IDC_DISK_DRIVE2:
		case IDC_DISK_DRIVE3:
		case IDC_DISK_DRIVE4:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnDriveCountChange(hdlg);
			}
			break;
		case IDC_VSYNC_24KHZ:
		case IDC_VSYNC_15KHZ:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnHiresolutionChange(hdlg);
			}
			break;
		case IDC_WAIT_EMULATION:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnWaitEmulationChange(hdlg);
			}
			break;
		case IDC_OLD_COMPATIBLE:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnOldCompatibleChange(hdlg);
			}
			break;
		case IDC_PCG_ENABLE:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnPcgChange(hdlg);
				if (m_ahPageHandles[3] != NULL) {
					SetChildrenSound(m_ahPageHandles[3]);
				}
			}
			break;
		case IDC_OPTION_FONT:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnOptFontChange(hdlg);
			}
			break;
		}
		break;
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

// display dialog procedure

BOOL CALLBACK CX88EnvSetDlg::DisplayDlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		{ // dummy block
			m_ahPageHandles[1] = hdlg;
			SetChildrenDisplay(hdlg);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			bResult = FALSE;
			break;
		case IDC_FRAME_RATE:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				OnScreenFrameRateChange(hdlg);
			}
			break;
		case IDC_INTERLACE_DO:
		case IDC_INTERLACE_NODO:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnInterlaceChange(hdlg);
			}
			break;
		}
		break;
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

// input dialog procedure

BOOL CALLBACK CX88EnvSetDlg::InputDlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		{ // dummy block
			m_ahPageHandles[2] = hdlg;
			SetChildrenInput(hdlg);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			bResult = FALSE;
			break;
		case IDC_DIRECT_INPUT_USE:
		case IDC_DIRECT_INPUT_NOUSE:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnDirectInputUseChange(hdlg);
			}
			break;
		case IDC_IME_ENABLE:
		case IDC_IME_DISABLE:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnIMEDisableChange(hdlg);
			}
			break;
		}
		break;
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

// sound dialog procedure

BOOL CALLBACK CX88EnvSetDlg::SoundDlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		{ // dummy block
			m_ahPageHandles[3] = hdlg;
			SendDlgItemMessage(
				hdlg, IDC_BEEP_VOLUME,
				TBM_SETTICFREQ, 10, 0);
			SendDlgItemMessage(
				hdlg, IDC_BEEP_VOLUME,
				TBM_SETRANGE, (WPARAM)TRUE, MAKELPARAM(0, 100));
			SendDlgItemMessage(
				hdlg, IDC_BEEP_VOLUME,
				TBM_SETPAGESIZE, 0, 10);
			SendDlgItemMessage(
				hdlg, IDC_PCG_VOLUME,
				TBM_SETTICFREQ, 10, 0);
			SendDlgItemMessage(
				hdlg, IDC_PCG_VOLUME,
				TBM_SETRANGE, (WPARAM)TRUE, MAKELPARAM(0, 100));
			SendDlgItemMessage(
				hdlg, IDC_PCG_VOLUME,
				TBM_SETPAGESIZE, 0, 10);
			SetChildrenSound(hdlg);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			bResult = FALSE;
			break;
		case IDC_DIRECT_SOUND_USE:
		case IDC_DIRECT_SOUND_NOUSE:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnDirectSoundUseChange(hdlg);
			}
			break;
		case IDC_DSOUND_FREQ:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				OnDirectSoundFrequencyChange(hdlg);
			}
			break;
		case IDC_DSOUND_BIT:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				OnDirectSoundBitsChange(hdlg);
			}
			break;
		case IDC_DSOUND_CH:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				OnDirectSoundChannelsChange(hdlg);
			}
			break;
		case IDC_BEEP_MUTE:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnBeepPlayerMuteChange(hdlg);
			}
			break;
		case IDC_PCG_MUTE:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnPcgPlayerMuteChange(hdlg);
			}
			break;
		}
		break;
	case WM_HSCROLL:
		if ((HWND)lParam != NULL) {
			switch (GetDlgCtrlID((HWND)lParam)) {
			case IDC_BEEP_VOLUME:
				OnBeepPlayerVolumeChange(hdlg);
				break;
			case IDC_PCG_VOLUME:
				OnPcgPlayerVolumeChange(hdlg);
				break;
			}
		}
		break;
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

#elif defined(X88_GUI_GTK)

// dialog procedure(initialize & dispose)

bool CX88EnvSetDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		GtkWidget* pGeneral = GetDlgItem(
			pDialog, NX88Resource::IDC_ENV_GENERAL);
		if (pGeneral != NULL) {
			m_ahPageHandles[0] = pGeneral;
			if (!BuildDialog(pGeneral, IDD_ENV_GENERAL)) {
				return false;
			}
			SetChildrenGeneral(pGeneral);
			GtkWidget* pcomboUILanguage = GetDlgItem(
				pGeneral, IDC_UI_LANGUAGE);
			if (pcomboUILanguage != NULL) {
				g_signal_connect(
					pcomboUILanguage, "changed",
					G_CALLBACK(OnChangedSignalUILanguage),
					pGeneral);
			}
			GtkWidget* pcomboBoostLimiter = GetDlgItem(
				pGeneral, IDC_BOOST_LIMITER);
			if (pcomboBoostLimiter != NULL) {
				g_signal_connect(
					pcomboBoostLimiter, "changed",
					G_CALLBACK(OnChangedSignalBoostLimiter),
					pGeneral);
			}
			int i;
			for (i = 0; i < 4; i++) {
				int nID = 0;
				switch (i) {
				case 0:
					nID = IDC_DISK_DRIVE1;
					break;
				case 1:
					nID = IDC_DISK_DRIVE2;
					break;
				case 2:
					nID = IDC_DISK_DRIVE3;
					break;
				case 3:
					nID = IDC_DISK_DRIVE4;
					break;
				}
				g_signal_connect(
					GetDlgItem(pGeneral, nID), "toggled",
					G_CALLBACK(OnToggledSignalDriveCount),
					pGeneral);
			}
			for (i = 0; i < 2; i++) {
				int nID = 0;
				switch (i) {
				case 0:
					nID = IDC_VSYNC_24KHZ;
					break;
				case 1:
					nID = IDC_VSYNC_15KHZ;
					break;
				}
				g_signal_connect(
					GetDlgItem(pGeneral, nID), "toggled",
					G_CALLBACK(OnToggledSignalHiresolution),
					pGeneral);
			}
			g_signal_connect(
				GetDlgItem(pGeneral, IDC_WAIT_EMULATION), "toggled",
				G_CALLBACK(OnToggledSignalWaitEmulation),
				pGeneral);
			g_signal_connect(
				GetDlgItem(pGeneral, IDC_OLD_COMPATIBLE), "toggled",
				G_CALLBACK(OnToggledSignalOldCompatible),
				pGeneral);
			g_signal_connect(
				GetDlgItem(pGeneral, IDC_PCG_ENABLE), "toggled",
				G_CALLBACK(OnToggledSignalPcgEnable),
				pGeneral);
			g_signal_connect(
				GetDlgItem(pGeneral, IDC_OPTION_FONT), "toggled",
				G_CALLBACK(OnToggledSignalOptionFont),
				pGeneral);
		}
		GtkWidget* pDisplay = GetDlgItem(
			pDialog, NX88Resource::IDC_ENV_DISPLAY);
		if (pDisplay != NULL) {
			m_ahPageHandles[1] = pDisplay;
			if (!BuildDialog(pDisplay, IDD_ENV_DISPLAY)) {
				return false;
			}
			SetChildrenDisplay(pDisplay);
			GtkWidget* pcomboFrameRate = GetDlgItem(
				pDisplay, IDC_FRAME_RATE);
			if (pcomboFrameRate != NULL) {
				g_signal_connect(
					pcomboFrameRate, "changed",
					G_CALLBACK(OnChangedSignalScreenFrameRate),
					pDisplay);
			}
			for (int i = 0; i < 2; i++) {
				int nID = 0;
				switch (i) {
				case 0:
					nID = IDC_INTERLACE_DO;
					break;
				case 1:
					nID = IDC_INTERLACE_NODO;
					break;
				}
				g_signal_connect(
					GetDlgItem(pDisplay, nID), "toggled",
					G_CALLBACK(OnToggledSignalInterlace),
					pDisplay);
			}
		}
		GtkWidget* pInput = GetDlgItem(
			pDialog, NX88Resource::IDC_ENV_INPUT);
		if (pInput != NULL) {
			m_ahPageHandles[2] = pInput;
			if (!BuildDialog(pInput, IDD_ENV_INPUT)) {
				return false;
			}
			SetChildrenInput(pInput);
			int i;
			for (i = 0; i < 2; i++) {
				int nID = 0;
				switch (i) {
				case 0:
					nID = IDC_DIRECT_INPUT_USE;
					break;
				case 1:
					nID = IDC_DIRECT_INPUT_NOUSE;
					break;
				}
				g_signal_connect(
					GetDlgItem(pInput, nID), "toggled",
					G_CALLBACK(OnToggledSignalDirectInput),
					pInput);
			}
			for (i = 0; i < 2; i++) {
				int nID = 0;
				switch (i) {
				case 0:
					nID = IDC_IME_ENABLE;
					break;
				case 1:
					nID = IDC_IME_DISABLE;
					break;
				}
				g_signal_connect(
					GetDlgItem(pInput, nID), "toggled",
					G_CALLBACK(OnToggledSignalIME),
					pInput);
			}
		}
	} else {
		GtkWidget* pGeneral = GetDlgItem(
			pDialog, NX88Resource::IDC_ENV_GENERAL);
		if (pGeneral != NULL) {
			CleanupDialog(pGeneral);
		}
		GtkWidget* pDisplay = GetDlgItem(
			pDialog, NX88Resource::IDC_ENV_DISPLAY);
		if (pDisplay != NULL) {
			CleanupDialog(pDisplay);
		}
		GtkWidget* pInput = GetDlgItem(
			pDialog, NX88Resource::IDC_ENV_INPUT);
		if (pInput != NULL) {
			CleanupDialog(pInput);
		}
	}
	return true;
}

// UI language changed signal

void CX88EnvSetDlg::OnChangedSignalUILanguage(
	GtkWidget* /*pWidget*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnUILanguageChange(pDialog);
}

// boost limiter changed signal

void CX88EnvSetDlg::OnChangedSignalBoostLimiter(
	GtkWidget* /*pWidget*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnBoostLimiterChange(pDialog);
}

// floppy disk drive count toggled signal

void CX88EnvSetDlg::OnToggledSignalDriveCount(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	if (gtk_toggle_button_get_active(pToggleButton) != FALSE) {
		GtkWidget* pDialog = GTK_WIDGET(pData);
		OnDriveCountChange(pDialog);
	}
}

// hi-resolution mode toggled signal

void CX88EnvSetDlg::OnToggledSignalHiresolution(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	if (gtk_toggle_button_get_active(pToggleButton) != FALSE) {
		GtkWidget* pDialog = GTK_WIDGET(pData);
		OnHiresolutionChange(pDialog);
	}
}

// wait emulation mode toggled signal

void CX88EnvSetDlg::OnToggledSignalWaitEmulation(
	GtkToggleButton* /*pToggleButton*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnWaitEmulationChange(pDialog);
}

// old-machine compatible mode signal

void CX88EnvSetDlg::OnToggledSignalOldCompatible(
	GtkToggleButton* /*pToggleButton*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnOldCompatibleChange(pDialog);
}

// PCG enable signal

void CX88EnvSetDlg::OnToggledSignalPcgEnable(
	GtkToggleButton* /*pToggleButton*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnPcgChange(pDialog);
}

// international display signal

void CX88EnvSetDlg::OnToggledSignalOptionFont(
	GtkToggleButton* /*pToggleButton*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnOptFontChange(pDialog);
}

// frame-rate changed signal

void CX88EnvSetDlg::OnChangedSignalScreenFrameRate(
	GtkWidget* /*pWidget*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnScreenFrameRateChange(pDialog);
}

// interlace mode toggled signal

void CX88EnvSetDlg::OnToggledSignalInterlace(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	if (gtk_toggle_button_get_active(pToggleButton) != FALSE) {
		GtkWidget* pDialog = GTK_WIDGET(pData);
		OnInterlaceChange(pDialog);
	}
}

// DirectInput toggled signal

void CX88EnvSetDlg::OnToggledSignalDirectInput(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	if (gtk_toggle_button_get_active(pToggleButton) != FALSE) {
		GtkWidget* pDialog = GTK_WIDGET(pData);
		OnDirectInputUseChange(pDialog);
	}
}

// IME toggled signal

void CX88EnvSetDlg::OnToggledSignalIME(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	if (gtk_toggle_button_get_active(pToggleButton) != FALSE) {
		GtkWidget* pDialog = GTK_WIDGET(pData);
		OnIMEDisableChange(pDialog);
	}
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// general

// set general children

void CX88EnvSetDlg::SetChildrenGeneral(CX88WndHandle hdlg) {
	CX88WndHandle hcomboUILanguage = GetDlgItem(
		hdlg, IDC_UI_LANGUAGE);
	if (hcomboUILanguage != NULL) {
		AddComboBoxItem(
			hcomboUILanguage,
			RSTR("Auto"));
		AddComboBoxItem(
			hcomboUILanguage,
			RSTR("Japanese"));
		AddComboBoxItem(
			hcomboUILanguage,
			RSTR("English"));
		SetComboBoxSel(
			hcomboUILanguage, GetUILanguage());
	}
	CX88WndHandle hcomboBoostLimiter = GetDlgItem(
		hdlg, IDC_BOOST_LIMITER);
	if (hcomboBoostLimiter != NULL) {
		AddComboBoxItem(
			hcomboBoostLimiter,
			RSTR("V-MAX"));
		AddComboBoxItem(
			hcomboBoostLimiter,
			RSTR("25%"));
		AddComboBoxItem(
			hcomboBoostLimiter,
			RSTR("50%"));
		AddComboBoxItem(
			hcomboBoostLimiter,
			RSTR("75%"));
		AddComboBoxItem(
			hcomboBoostLimiter,
			RSTR("150%"));
		AddComboBoxItem(
			hcomboBoostLimiter,
			RSTR("200%"));
		AddComboBoxItem(
			hcomboBoostLimiter,
			RSTR("300%"));
		AddComboBoxItem(
			hcomboBoostLimiter,
			RSTR("400%"));
		int nIndex = 0;
		switch (GetBoostLimiter()) {
		case 25:
			nIndex = 1;
			break;
		case 50:
			nIndex = 2;
			break;
		case 75:
			nIndex = 3;
			break;
		case 150:
			nIndex = 4;
			break;
		case 200:
			nIndex = 5;
			break;
		case 300:
			nIndex = 6;
			break;
		case 400:
			nIndex = 7;
			break;
		}
		SetComboBoxSel(
			hcomboBoostLimiter, nIndex);
	}
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_DISK_DRIVE1),
		GetDriveCount()-1);
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_VSYNC_24KHZ),
		IsHiresolution()? 0: 1);
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WAIT_EMULATION),
		IsWaitEmulation());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_OLD_COMPATIBLE),
		IsOldCompatible());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_PCG_ENABLE),
		IsPcgEnable());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_OPTION_FONT),
		IsOptionFont());
}

// on changed UI language

void CX88EnvSetDlg::OnUILanguageChange(CX88WndHandle hdlg) {
	int nSel = GetComboBoxSel(
		GetDlgItem(hdlg, IDC_UI_LANGUAGE));
	if ((nSel >= 0) && (nSel <= 2)) {
		SetUILanguage(nSel);
	}
}

// on changed boost limiter

void CX88EnvSetDlg::OnBoostLimiterChange(CX88WndHandle hdlg) {
	int nSel = GetComboBoxSel(
		GetDlgItem(hdlg, IDC_BOOST_LIMITER));
	switch (nSel) {
	case 0:
		SetBoostLimiter(0);
		break;
	case 1:
		SetBoostLimiter(25);
		break;
	case 2:
		SetBoostLimiter(50);
		break;
	case 3:
		SetBoostLimiter(75);
		break;
	case 4:
		SetBoostLimiter(150);
		break;
	case 5:
		SetBoostLimiter(200);
		break;
	case 6:
		SetBoostLimiter(300);
		break;
	case 7:
		SetBoostLimiter(400);
		break;
	}
}

// on changed floppy disk drive count

void CX88EnvSetDlg::OnDriveCountChange(CX88WndHandle hdlg) {
	int nSel = GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_DISK_DRIVE1));
	if ((nSel >= 0) && (nSel <= 3)) {
		SetDriveCount(nSel+1);
	}
}

// on changed hi-resolution mode

void CX88EnvSetDlg::OnHiresolutionChange(CX88WndHandle hdlg) {
	int nSel = GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_VSYNC_24KHZ));
	switch (nSel) {
	case 0:
		SetHiresolution(true);
		break;
	case 1:
		SetHiresolution(false);
		break;
	}
}

// on changed wait emulation mode

void CX88EnvSetDlg::OnWaitEmulationChange(CX88WndHandle hdlg) {
	SetWaitEmulation(
		IsCheckButtonChecked(
			GetDlgItem(hdlg, IDC_WAIT_EMULATION)));
}

// on changed old-machine compatible mode

void CX88EnvSetDlg::OnOldCompatibleChange(CX88WndHandle hdlg) {
	SetOldCompatible(
		IsCheckButtonChecked(
			GetDlgItem(hdlg, IDC_OLD_COMPATIBLE)));
}

// on changed PCG enable

void CX88EnvSetDlg::OnPcgChange(CX88WndHandle hdlg) {
	SetPcgEnable(
		IsCheckButtonChecked(
			GetDlgItem(hdlg, IDC_PCG_ENABLE)));
}

// on changed international display mode

void CX88EnvSetDlg::OnOptFontChange(CX88WndHandle hdlg) {
	SetOptionFont(
		IsCheckButtonChecked(
			GetDlgItem(hdlg, IDC_OPTION_FONT)));
}

// display

// set display children

void CX88EnvSetDlg::SetChildrenDisplay(CX88WndHandle hdlg) {
	CX88WndHandle hcomboFrameRate = GetDlgItem(
		hdlg, IDC_FRAME_RATE);
	if (hcomboFrameRate != NULL) {
		AddComboBoxItem(
			hcomboFrameRate,
			RSTR("20 FPS"));
		AddComboBoxItem(
			hcomboFrameRate,
			RSTR("30 FPS"));
		AddComboBoxItem(
			hcomboFrameRate,
			RSTR("40 FPS"));
		AddComboBoxItem(
			hcomboFrameRate,
			RSTR("60 FPS"));
		int nIndex = 0;
		switch (GetScreenFrameRate()) {
		case 20:
			nIndex = 0;
			break;
		case 30:
			nIndex = 1;
			break;
		case 40:
			nIndex = 2;
			break;
		case 60:
			nIndex = 3;
			break;
		}
		SetComboBoxSel(
			hcomboFrameRate, nIndex);
	}
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_INTERLACE_DO),
		IsInterlace()? 0: 1);
}

// on changed frame-rate

void CX88EnvSetDlg::OnScreenFrameRateChange(CX88WndHandle hdlg) {
	int nSel = GetComboBoxSel(
		GetDlgItem(hdlg, IDC_FRAME_RATE));
	switch (nSel) {
	case 0:
		SetScreenFrameRate(20);
		break;
	case 1:
		SetScreenFrameRate(30);
		break;
	case 2:
		SetScreenFrameRate(40);
		break;
	case 3:
		SetScreenFrameRate(60);
		break;
	}
}

// on changed interlace mode

void CX88EnvSetDlg::OnInterlaceChange(CX88WndHandle hdlg) {
	int nSel = GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_INTERLACE_DO));
	switch (nSel) {
	case 0:
		SetInterlace(true);
		break;
	case 1:
		SetInterlace(false);
		break;
	}
}

// input

// set input children

void CX88EnvSetDlg::SetChildrenInput(CX88WndHandle hdlg) {
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_DIRECT_INPUT_USE),
		IsDirectInputUse()? 0: 1);
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_IME_ENABLE),
		IsIMEDisable()? 1: 0);
}

// on changed using DirectInput

void CX88EnvSetDlg::OnDirectInputUseChange(CX88WndHandle hdlg) {
	int nSel = GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_DIRECT_INPUT_USE));
	switch (nSel) {
	case 0:
		SetDirectInputUse(true);
		break;
	case 1:
		SetDirectInputUse(false);
		break;
	}
}

// on changed IME disable

void CX88EnvSetDlg::OnIMEDisableChange(CX88WndHandle hdlg) {
	int nSel = GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_IME_ENABLE));
	switch (nSel) {
	case 0:
		SetIMEDisable(false);
		break;
	case 1:
		SetIMEDisable(true);
		break;
	}
}

// sound

// set sound children

void CX88EnvSetDlg::SetChildrenSound(CX88WndHandle hdlg) {
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_DIRECT_SOUND_USE),
		IsDirectSoundUse()? 0: 1);
	CX88WndHandle hcomboDSoundFreq = GetDlgItem(
		hdlg, IDC_DSOUND_FREQ);
	if (hcomboDSoundFreq != NULL) {
		AddComboBoxItem(
			hcomboDSoundFreq,
			RSTR("11025"));
		AddComboBoxItem(
			hcomboDSoundFreq,
			RSTR("22050"));
		AddComboBoxItem(
			hcomboDSoundFreq,
			RSTR("44100"));
		AddComboBoxItem(
			hcomboDSoundFreq,
			RSTR("48000"));
		int nIndex = 0;
		switch (GetDirectSoundFrequency()) {
		case 11025:
			nIndex = 0;
			break;
		case 22050:
			nIndex = 1;
			break;
		case 44100:
			nIndex = 2;
			break;
		case 48000:
			nIndex = 3;
			break;
		}
		SetComboBoxSel(
			hcomboDSoundFreq, nIndex);
	}
	CX88WndHandle hcomboDSoundBit = GetDlgItem(
		hdlg, IDC_DSOUND_BIT);
	if (hcomboDSoundBit != NULL) {
		AddComboBoxItem(
			hcomboDSoundBit,
			RSTR("8 bit"));
		AddComboBoxItem(
			hcomboDSoundBit,
			RSTR("16 bit"));
		int nIndex = 0;
		switch (GetDirectSoundBits()) {
		case 8:
			nIndex = 0;
			break;
		case 16:
			nIndex = 1;
			break;
		}
		SetComboBoxSel(
			hcomboDSoundBit, nIndex);
	}
	CX88WndHandle hcomboDSoundCh = GetDlgItem(
		hdlg, IDC_DSOUND_CH);
	if (hcomboDSoundCh != NULL) {
		AddComboBoxItem(
			hcomboDSoundCh,
			RSTR("Mono"));
		AddComboBoxItem(
			hcomboDSoundCh,
			RSTR("Stereo"));
		int nIndex = 0;
		switch (GetDirectSoundChannels()) {
		case 1:
			nIndex = 0;
			break;
		case 2:
			nIndex = 1;
			break;
		}
		SetComboBoxSel(
			hcomboDSoundCh, nIndex);
	}
	SetSliderValue(
		GetDlgItem(hdlg, IDC_BEEP_VOLUME),
		GetBeepPlayerVolume());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_BEEP_MUTE),
		IsBeepPlayerMute());
	EnableControl(
		GetDlgItem(hdlg, IDC_PCG_GROUP),
		m_bPcgEnable);
	SetSliderValue(
		GetDlgItem(hdlg, IDC_PCG_VOLUME),
		GetPcgPlayerVolume());
	EnableControl(
		GetDlgItem(hdlg, IDC_PCG_VOLUME),
		m_bPcgEnable);
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_PCG_MUTE),
		IsPcgPlayerMute());
	EnableControl(
		GetDlgItem(hdlg, IDC_PCG_MUTE),
		m_bPcgEnable);
}

// on changed using DirectSound

void CX88EnvSetDlg::OnDirectSoundUseChange(CX88WndHandle hdlg) {
	int nSel = GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_DIRECT_SOUND_USE));
	switch (nSel) {
	case 0:
		SetDirectSoundUse(true);
		break;
	case 1:
		SetDirectSoundUse(false);
		break;
	}
}

// on changed DirectSound frequency

void CX88EnvSetDlg::OnDirectSoundFrequencyChange(CX88WndHandle hdlg) {
	int nSel = GetComboBoxSel(
		GetDlgItem(hdlg, IDC_DSOUND_FREQ));
	switch (nSel) {
	case 0:
		SetDirectSoundFrequency(11025);
		break;
	case 1:
		SetDirectSoundFrequency(22050);
		break;
	case 2:
		SetDirectSoundFrequency(44100);
		break;
	case 3:
		SetDirectSoundFrequency(48000);
		break;
	}
}

// on changed DirectSound bit count

void CX88EnvSetDlg::OnDirectSoundBitsChange(CX88WndHandle hdlg) {
	int nSel = GetComboBoxSel(
		GetDlgItem(hdlg, IDC_DSOUND_BIT));
	switch (nSel) {
	case 0:
		SetDirectSoundBits(8);
		break;
	case 1:
		SetDirectSoundBits(16);
		break;
	}
}

// on changed DirectSound channel count

void CX88EnvSetDlg::OnDirectSoundChannelsChange(CX88WndHandle hdlg) {
	int nSel = GetComboBoxSel(
		GetDlgItem(hdlg, IDC_DSOUND_CH));
	switch (nSel) {
	case 0:
		SetDirectSoundChannels(1);
		break;
	case 1:
		SetDirectSoundChannels(2);
		break;
	}
}

// on changed beep player volume

void CX88EnvSetDlg::OnBeepPlayerVolumeChange(CX88WndHandle hdlg) {
	int nValue = GetSliderValue(
		GetDlgItem(hdlg, IDC_BEEP_VOLUME));
	if ((nValue >= 0) && (nValue <= 100)) {
		SetBeepPlayerVolume(nValue);
	}
}

// on changed beep player mute state

void CX88EnvSetDlg::OnBeepPlayerMuteChange(CX88WndHandle hdlg) {
	bool bCheck = IsCheckButtonChecked(
		GetDlgItem(hdlg, IDC_BEEP_MUTE));
	SetBeepPlayerMute(bCheck);
}

// on changed PCG sound player volume

void CX88EnvSetDlg::OnPcgPlayerVolumeChange(CX88WndHandle hdlg) {
	int nValue = GetSliderValue(
		GetDlgItem(hdlg, IDC_PCG_VOLUME));
	if ((nValue >= 0) && (nValue <= 100)) {
		SetPcgPlayerVolume(nValue);
	}
}

// on changed PCG sound player mute state

void CX88EnvSetDlg::OnPcgPlayerMuteChange(CX88WndHandle hdlg) {
	bool bCheck = IsCheckButtonChecked(
		GetDlgItem(hdlg, IDC_PCG_MUTE));
	SetPcgPlayerMute(bCheck);
}

// create modal dialog

int CX88EnvSetDlg::DoModal() {

#ifdef X88_GUI_WINDOWS

	PROPSHEETPAGE apsp[4];
	memset(apsp, 0x00, sizeof(apsp));
	// general
	apsp[0].dwSize = sizeof(PROPSHEETPAGE);
	apsp[0].dwFlags = PSP_USETITLE;
	apsp[0].hInstance = GetResourceHandle();
	apsp[0].pszTemplate = MAKEINTRESOURCE(
		NX88Resource::IsResJapanese()?
			IDD_ENV_GENERAL:
			IDD_ENV_GENERAL_E);
	apsp[0].pfnDlgProc = GeneralDlgProc;
	apsp[0].pszTitle = RSTR("General");
	// display
	apsp[1].dwSize = sizeof(PROPSHEETPAGE);
	apsp[1].dwFlags = PSP_USETITLE;
	apsp[1].hInstance = GetResourceHandle();
	apsp[1].pszTemplate = MAKEINTRESOURCE(
		NX88Resource::IsResJapanese()?
			IDD_ENV_DISPLAY:
			IDD_ENV_DISPLAY_E);
	apsp[1].pfnDlgProc = DisplayDlgProc;
	apsp[1].pszTitle = RSTR("Display");
	// input
	apsp[2].dwSize = sizeof(PROPSHEETPAGE);
	apsp[2].dwFlags = PSP_USETITLE;
	apsp[2].hInstance = GetResourceHandle();
	apsp[2].pszTemplate = MAKEINTRESOURCE(
		NX88Resource::IsResJapanese()?
			IDD_ENV_INPUT:
			IDD_ENV_INPUT_E);
	apsp[2].pfnDlgProc = InputDlgProc;
	apsp[2].pszTitle = RSTR("Input");
	// sound
	apsp[3].dwSize = sizeof(PROPSHEETPAGE);
	apsp[3].dwFlags = PSP_USETITLE;
	apsp[3].hInstance = GetResourceHandle();
	apsp[3].pszTemplate = MAKEINTRESOURCE(
		NX88Resource::IsResJapanese()?
			IDD_ENV_SOUND:
			IDD_ENV_SOUND_E);
	apsp[3].pfnDlgProc = SoundDlgProc;
	apsp[3].pszTitle = RSTR("Sound");
	PROPSHEETHEADER psh;
	memset(&psh, 0x00, sizeof(psh));
	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
	psh.hwndParent = GetParentWndHandle();
	psh.hInstance = GetResourceHandle();
	psh.pszCaption = RSTR("Environment Settings");
	psh.nPages = sizeof(apsp)/sizeof(apsp[0]);
	psh.ppsp = apsp;
	return (int)PropertySheet(&psh);


#elif defined(X88_GUI_GTK)

	return ExecuteDialog(
		NX88Resource::IDD_ENV_SET,
		GetParentWndHandle(),
		DlgProc);

#endif // X88_GUI

}
