////////////////////////////////////////////////////////////
// X88000 Printer PaperDelete Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88PrinterPaperDelDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88PrinterPaperDelDlg

////////////////////////////////////////////////////////////
// attribute

// delete all pages

bool CX88PrinterPaperDelDlg::m_bDelAllPages;

// delete page number

int CX88PrinterPaperDelDlg::m_nDelPageNo;

// page count

int CX88PrinterPaperDelDlg::m_nPageCount;

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88PrinterPaperDelDlg::CX88PrinterPaperDelDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	m_bDelAllPages = false;
	m_nDelPageNo = 0;
	m_nPageCount = 1;
}

// destructor

CX88PrinterPaperDelDlg::~CX88PrinterPaperDelDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88PrinterPaperDelDlg::DlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		SetChildren(hdlg);
		Centering(hdlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, LOWORD(wParam));
			break;
		case IDC_PRINTER_PAPERDEL_ONEPAGE:
		case IDC_PRINTER_PAPERDEL_ALLPAGES:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnTargetChange(hdlg);
			}
			break;
		case IDC_PRINTER_PAPERDEL_PAGENO:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				OnPageNoChange(hdlg);
			}
			break;
		}
		break;
	case WM_VSCROLL:
		switch (GetDlgCtrlID((HWND)lParam)) {
		case IDC_PRINTER_PAPERDEL_PAGENO_SPIN:
			OnPageNoChange(hdlg);
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

bool CX88PrinterPaperDelDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		SetChildren(pDialog);
		for (int i = 0; i < 2; i++) {
			int nID = 0;
			switch (i) {
			case 0:
				nID = IDC_PRINTER_PAPERDEL_ONEPAGE;
				break;
			case 1:
				nID = IDC_PRINTER_PAPERDEL_ALLPAGES;
				break;
			}
			g_signal_connect(
				GetDlgItem(pDialog, nID), "toggled",
				G_CALLBACK(OnToggledSignalTarget),
				pDialog);
		}
		GtkWidget* pspinPageNo = GetDlgItem(
			pDialog, IDC_PRINTER_PAPERDEL_PAGENO_SPIN);
		if (pspinPageNo != NULL) {
			GtkAdjustment* pAdjustment =
				gtk_spin_button_get_adjustment(
					GTK_SPIN_BUTTON(pspinPageNo));
			g_signal_connect(
				pAdjustment, "value_changed",
				G_CALLBACK(OnValueChangedSignalPageNo),
				pDialog);
		}
	}
	return true;
}

// target type toggled signal

void CX88PrinterPaperDelDlg::OnToggledSignalTarget(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	if (gtk_toggle_button_get_active(pToggleButton) != FALSE) {
		GtkWidget* pDialog = GTK_WIDGET(pData);
		OnTargetChange(pDialog);
	}
}

// page number changed signal

void CX88PrinterPaperDelDlg::OnValueChangedSignalPageNo(
	GtkAdjustment* /*pAdjustment*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnPageNoChange(pDialog);
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// set children

void CX88PrinterPaperDelDlg::SetChildren(CX88WndHandle hdlg) {
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_ONEPAGE),
		IsDelAllPages()? 1: 0);
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_PAGENO),
		!IsDelAllPages());
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_PAGENO_SPIN),
		!IsDelAllPages());
	SetSpinButtonRange(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_PAGENO_SPIN),
		1, GetPageCount());
	SetSpinButtonValue(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_PAGENO),
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_PAGENO_SPIN),
		GetDelPageNo()+1);
}

// on changed target type

void CX88PrinterPaperDelDlg::OnTargetChange(CX88WndHandle hdlg) {
	int nSel = GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_ONEPAGE));
	if ((nSel >= 0) && (nSel <= 1)) {
		SetDelAllPages(nSel != 0);
		SetChildren(hdlg);
	}
}

// on changed target page number

void CX88PrinterPaperDelDlg::OnPageNoChange(CX88WndHandle hdlg) {
	int nPage = GetSpinButtonValue(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_PAGENO),
		GetDlgItem(hdlg, IDC_PRINTER_PAPERDEL_PAGENO_SPIN))-1;
	if (nPage < 0) {
		nPage = 0;
	} else if (nPage >= GetPageCount()) {
		nPage = GetPageCount()-1;
	}
	SetDelPageNo(nPage);
	SetChildren(hdlg);
}

// create modal dialog

int CX88PrinterPaperDelDlg::DoModal() {
	return ExecuteDialog(
		IDD_PRINTER_PAPERDEL,
		GetParentWndHandle(),
		DlgProc);
}
