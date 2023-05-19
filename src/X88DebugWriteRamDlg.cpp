////////////////////////////////////////////////////////////
// X88000 Debug Write RAM Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88DebugWriteRamDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88DebugWriteRamDlg

////////////////////////////////////////////////////////////
// attribute

// main RAM0 writing

bool CX88DebugWriteRamDlg::m_bWriteMainRam0;

// main RAM1 writing

bool CX88DebugWriteRamDlg::m_bWriteMainRam1;

// fast text VRAM writing

bool CX88DebugWriteRamDlg::m_bWriteFastTVRam;

// slow text VRAM writing

bool CX88DebugWriteRamDlg::m_bWriteSlowTVRam;

// graphic VRAM0 writing

bool CX88DebugWriteRamDlg::m_bWriteGVRam0;

// graphic VRAM1 writing

bool CX88DebugWriteRamDlg::m_bWriteGVRam1;

// graphic VRAM2 writing

bool CX88DebugWriteRamDlg::m_bWriteGVRam2;

// subsystem RAM writing

bool CX88DebugWriteRamDlg::m_bWriteSubRam;

// expansion RAM0 writing

bool CX88DebugWriteRamDlg::m_bWriteExRam0;

// expansion RAM1 writing

bool CX88DebugWriteRamDlg::m_bWriteExRam1;

// fast text VRAM using mode

bool CX88DebugWriteRamDlg::m_bFastTVRamUse;

// subsystem disable

bool CX88DebugWriteRamDlg::m_bSubSystemDisable;

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88DebugWriteRamDlg::CX88DebugWriteRamDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	m_bWriteMainRam0 = true;
	m_bWriteMainRam1 = true;
	m_bWriteFastTVRam = false;
	m_bWriteSlowTVRam = false;
	m_bWriteGVRam0 = true;
	m_bWriteGVRam1 = true;
	m_bWriteGVRam2 = true;
	m_bWriteSubRam = true;
	m_bWriteExRam0 = false;
	m_bWriteExRam1 = false;
	m_bFastTVRamUse = true;
	m_bSubSystemDisable = false;
}

// destructor

CX88DebugWriteRamDlg::~CX88DebugWriteRamDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88DebugWriteRamDlg::DlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		ShowControl(
			GetDlgItem(hdlg, IDC_WRITE_FAST_TVRAM),
			!IsFastTVRamUse());
		ShowControl(
			GetDlgItem(hdlg, IDC_WRITE_SLOW_TVRAM),
			IsFastTVRamUse());
		Centering(hdlg);
		SetChildren(hdlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, LOWORD(wParam));
			break;
		case IDC_WRITE_MAIN_RAM0:
		case IDC_WRITE_MAIN_RAM1:
		case IDC_WRITE_FAST_TVRAM:
		case IDC_WRITE_SLOW_TVRAM:
		case IDC_WRITE_GVRAM0:
		case IDC_WRITE_GVRAM1:
		case IDC_WRITE_GVRAM2:
		case IDC_WRITE_SUB_RAM:
		case IDC_WRITE_EX_RAM0:
		case IDC_WRITE_EX_RAM1:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnWriteRamChange(hdlg, LOWORD(wParam));
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

#elif defined(X88_GUI_GTK)

// dialog procedure(initialize & dispose)

bool CX88DebugWriteRamDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		unsigned nState = IsFastTVRamUse()? 2: 1;
		if (!BuildDialog(pDialog, nID, nState)) {
			return false;
		}
		SetChildren(pDialog);
		for (int i = 0; i < 9; i++) {
			int nID = 0;
			switch (i) {
			case 0:
				nID = IDC_WRITE_MAIN_RAM0;
				break;
			case 1:
				nID = IDC_WRITE_MAIN_RAM1;
				break;
			case 2:
				nID = !IsFastTVRamUse()?
					IDC_WRITE_FAST_TVRAM:
					IDC_WRITE_SLOW_TVRAM;
				break;
			case 3:
				nID = IDC_WRITE_GVRAM0;
				break;
			case 4:
				nID = IDC_WRITE_GVRAM1;
				break;
			case 5:
				nID = IDC_WRITE_GVRAM2;
				break;
			case 6:
				nID = IDC_WRITE_SUB_RAM;
				break;
			case 7:
				nID = IDC_WRITE_EX_RAM0;
				break;
			case 8:
				nID = IDC_WRITE_EX_RAM1;
				break;
			}
			g_signal_connect(
				GetDlgItem(pDialog, nID),
				"toggled",
				G_CALLBACK(OnToggledSignalWriteRam),
				pDialog);
		}
	}
	return true;
}

// write RAM toggled signal

void CX88DebugWriteRamDlg::OnToggledSignalWriteRam(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnWriteRamChange(
		pDialog,
		GetDlgItemID(pDialog, GTK_WIDGET(pToggleButton)));
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// set children

void CX88DebugWriteRamDlg::SetChildren(CX88WndHandle hdlg) {
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WRITE_MAIN_RAM0),
		IsWriteMainRam0());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WRITE_MAIN_RAM1),
		IsWriteMainRam1());
	if (!IsFastTVRamUse()) {
		SetCheckButtonChecked(
			GetDlgItem(hdlg, IDC_WRITE_FAST_TVRAM),
			IsWriteFastTVRam());
	} else {
		SetCheckButtonChecked(
			GetDlgItem(hdlg, IDC_WRITE_SLOW_TVRAM),
			IsWriteSlowTVRam());
	}
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WRITE_GVRAM0),
		IsWriteGVRam0());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WRITE_GVRAM1),
		IsWriteGVRam1());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WRITE_GVRAM2),
		IsWriteGVRam2());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WRITE_SUB_RAM),
		IsWriteSubRam());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WRITE_EX_RAM0),
		IsWriteExRam0());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_WRITE_EX_RAM1),
		IsWriteExRam1());
	EnableControl(
		GetDlgItem(hdlg, IDC_WRITE_SUB_RAM),
		!IsSubSystemDisable());
}

// on changed RAM writing

void CX88DebugWriteRamDlg::OnWriteRamChange(
	CX88WndHandle hdlg, int nID)
{
	bool bCheck = IsCheckButtonChecked(
		GetDlgItem(hdlg, nID));
	switch (nID) {
	case IDC_WRITE_MAIN_RAM0:
		SetWriteMainRam0(bCheck);
		break;
	case IDC_WRITE_MAIN_RAM1:
		SetWriteMainRam1(bCheck);
		break;
	case IDC_WRITE_FAST_TVRAM:
		SetWriteFastTVRam(bCheck);
		break;
	case IDC_WRITE_SLOW_TVRAM:
		SetWriteSlowTVRam(bCheck);
		break;
	case IDC_WRITE_GVRAM0:
		SetWriteGVRam0(bCheck);
		break;
	case IDC_WRITE_GVRAM1:
		SetWriteGVRam1(bCheck);
		break;
	case IDC_WRITE_GVRAM2:
		SetWriteGVRam2(bCheck);
		break;
	case IDC_WRITE_SUB_RAM:
		SetWriteSubRam(bCheck);
		break;
	case IDC_WRITE_EX_RAM0:
		SetWriteExRam0(bCheck);
		break;
	case IDC_WRITE_EX_RAM1:
		SetWriteExRam1(bCheck);
		break;
	}
}

// create modal dialog

int CX88DebugWriteRamDlg::DoModal() {
	return ExecuteDialog(
		IDD_DEBUG_WRITE_RAM,
		GetParentWndHandle(),
		DlgProc);
}
