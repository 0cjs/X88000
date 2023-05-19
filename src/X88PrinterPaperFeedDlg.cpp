////////////////////////////////////////////////////////////
// X88000 Printer PaperFeed Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88PrinterPaperFeedDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88PrinterPaperFeedDlg

////////////////////////////////////////////////////////////
// attribute

// paper feed unit

int CX88PrinterPaperFeedDlg::m_nPaperFeedUnit;

// line feed unit

int CX88PrinterPaperFeedDlg::m_nLineFeedCount;

// point feed unit

int CX88PrinterPaperFeedDlg::m_nPointFeedCount;

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88PrinterPaperFeedDlg::CX88PrinterPaperFeedDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	m_nPaperFeedUnit = UNIT_PAGE;
	m_nLineFeedCount = 1;
	m_nPointFeedCount = 12;
}

// destructor

CX88PrinterPaperFeedDlg::~CX88PrinterPaperFeedDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88PrinterPaperFeedDlg::DlgProc(
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
		case IDC_PRINTER_PAPERFEED_PAGE:
		case IDC_PRINTER_PAPERFEED_LINE:
		case IDC_PRINTER_PAPERFEED_POINT:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnFeedUnitChange(hdlg);
			}
			break;
		case IDC_PRINTER_PAPERFEED_LINE_COUNT:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				OnLineFeedCountChange(hdlg);
			}
			break;
		case IDC_PRINTER_PAPERFEED_POINT_COUNT:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				OnPointFeedCountChange(hdlg);
			}
			break;
		}
		break;
	case WM_VSCROLL:
		switch (GetDlgCtrlID((HWND)lParam)) {
		case IDC_PRINTER_PAPERFEED_LINE_COUNT_SPIN:
			OnLineFeedCountChange(hdlg);
			break;
		case IDC_PRINTER_PAPERFEED_POINT_COUNT_SPIN:
			OnPointFeedCountChange(hdlg);
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

bool CX88PrinterPaperFeedDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		SetChildren(pDialog);
		for (int i = 0; i < 3; i++) {
			int nID = 0;
			switch (i) {
			case 0:
				nID = IDC_PRINTER_PAPERFEED_PAGE;
				break;
			case 1:
				nID = IDC_PRINTER_PAPERFEED_LINE;
				break;
			case 2:
				nID = IDC_PRINTER_PAPERFEED_POINT;
				break;
			}
			g_signal_connect(
				GetDlgItem(pDialog, nID), "toggled",
				G_CALLBACK(OnToggledSignalFeedUnit),
				pDialog);
		}
		GtkWidget* pspinLineCount = GetDlgItem(
			pDialog, IDC_PRINTER_PAPERFEED_LINE_COUNT_SPIN);
		if (pspinLineCount != NULL) {
			GtkAdjustment* pAdjustment =
				gtk_spin_button_get_adjustment(
					GTK_SPIN_BUTTON(pspinLineCount));
			g_signal_connect(
				pAdjustment, "value_changed",
				G_CALLBACK(OnValueChangedSignalLineFeedCount),
				pDialog);
		}
		GtkWidget* pspinPointCount = GetDlgItem(
			pDialog, IDC_PRINTER_PAPERFEED_POINT_COUNT_SPIN);
		if (pspinPointCount != NULL) {
			GtkAdjustment* pAdjustment =
				gtk_spin_button_get_adjustment(
					GTK_SPIN_BUTTON(pspinPointCount));
			g_signal_connect(
				pAdjustment, "value_changed",
				G_CALLBACK(OnValueChangedSignalPointFeedCount),
				pDialog);
		}
	}
	return true;
}

// feed unit toggled signal

void CX88PrinterPaperFeedDlg::OnToggledSignalFeedUnit(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	if (gtk_toggle_button_get_active(pToggleButton) != FALSE) {
		GtkWidget* pDialog = GTK_WIDGET(pData);
		OnFeedUnitChange(pDialog);
	}
}

// line feed unit changed signal

void CX88PrinterPaperFeedDlg::OnValueChangedSignalLineFeedCount(
	GtkAdjustment* /*pAdjustment*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnLineFeedCountChange(pDialog);
}

// point feed unit changed signal

void CX88PrinterPaperFeedDlg::OnValueChangedSignalPointFeedCount(
	GtkAdjustment* /*pAdjustment*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnPointFeedCountChange(pDialog);
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// set children

void CX88PrinterPaperFeedDlg::SetChildren(CX88WndHandle hdlg) {
	int nSel = 0;
	switch (GetPaperFeedUnit()) {
	case UNIT_PAGE:
		nSel = 0;
		break;
	case UNIT_LINE:
		nSel = 1;
		break;
	case UNIT_POINT:
		nSel = 2;
		break;
	}
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_PAGE),
		nSel);
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_LINE_COUNT),
		GetPaperFeedUnit() == UNIT_LINE);
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_LINE_COUNT_SPIN),
		GetPaperFeedUnit() == UNIT_LINE);
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_POINT_COUNT),
		GetPaperFeedUnit() == UNIT_POINT);
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_POINT_COUNT_SPIN),
		GetPaperFeedUnit() == UNIT_POINT);
	SetSpinButtonRange(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_LINE_COUNT_SPIN),
		1, 100);
	SetSpinButtonValue(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_LINE_COUNT),
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_LINE_COUNT_SPIN),
		GetLineFeedCount());
	SetSpinButtonRange(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_POINT_COUNT_SPIN),
		1, 999);
	SetSpinButtonValue(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_POINT_COUNT),
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_POINT_COUNT_SPIN),
		GetPointFeedCount());
}

// on changed paper feed unit

void CX88PrinterPaperFeedDlg::OnFeedUnitChange(CX88WndHandle hdlg) {
	int nSel = GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_PAGE));
	switch (nSel) {
	case 0:
		SetPaperFeedUnit(UNIT_PAGE);
		break;
	case 1:
		SetPaperFeedUnit(UNIT_LINE);
		break;
	case 2:
		SetPaperFeedUnit(UNIT_POINT);
		break;
	}
	SetChildren(hdlg);
}

// on changed line feed unit

void CX88PrinterPaperFeedDlg::OnLineFeedCountChange(CX88WndHandle hdlg) {
	int nLineFeedCount = GetSpinButtonValue(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_LINE_COUNT),
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_LINE_COUNT_SPIN));
	if (nLineFeedCount < 1) {
		nLineFeedCount = 1;
	} else if (nLineFeedCount >= 100) {
		nLineFeedCount = 100;
	}
	SetLineFeedCount(nLineFeedCount);
	SetChildren(hdlg);
}

// on changed point feed unit

void CX88PrinterPaperFeedDlg::OnPointFeedCountChange(CX88WndHandle hdlg) {
	int nPointFeedCount = GetSpinButtonValue(
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_POINT_COUNT),
		GetDlgItem(hdlg, IDC_PRINTER_PAPERFEED_POINT_COUNT_SPIN));
	if (nPointFeedCount < 1) {
		nPointFeedCount = 1;
	} else if (nPointFeedCount >= 999) {
		nPointFeedCount = 999;
	}
	SetPointFeedCount(nPointFeedCount);
	SetChildren(hdlg);
}

// create modal dialog

int CX88PrinterPaperFeedDlg::DoModal() {
	return ExecuteDialog(
		IDD_PRINTER_PAPERFEED,
		GetParentWndHandle(),
		DlgProc);
}
