////////////////////////////////////////////////////////////
// X88000 Version Info Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88VersionDlg.h"

#include "X88Resource.h"

////////////////////////////////////////////////////////////
// implementation of CX88VersionDlg

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88VersionDlg::CX88VersionDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
}

// destructor

CX88VersionDlg::~CX88VersionDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88VersionDlg::DlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		Centering(hdlg);
		SetVersion(hdlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, LOWORD(wParam));
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

bool CX88VersionDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		SetVersion(pDialog);
	}
	return true;
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// set version

void CX88VersionDlg::SetVersion(CX88WndHandle hdlg) {
	CX88WndHandle hstcVersion = GetDlgItem(hdlg, IDC_VERSION);
	std::string gstrVersion;
	GetStaticText(hstcVersion, gstrVersion);
	gstrVersion += NX88Resource::GetVersion();
	SetStaticText(hstcVersion, gstrVersion);
}

// create modal dialog

int CX88VersionDlg::DoModal() {
	return ExecuteDialog(
		IDD_VERSION,
		GetParentWndHandle(),
		DlgProc);
}
