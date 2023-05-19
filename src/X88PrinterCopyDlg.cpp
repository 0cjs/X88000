////////////////////////////////////////////////////////////
// X88000 Printer Copy Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88PrinterCopyDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88PrinterCopyDlg

////////////////////////////////////////////////////////////
// attribute

// copy page number

int CX88PrinterCopyDlg::m_nCopyPageNo;

// page count

int CX88PrinterCopyDlg::m_nPageCount;

// copy metafile

bool CX88PrinterCopyDlg::m_bCopyMetafile;

// zoom-out rate

int CX88PrinterCopyDlg::m_nZoomOut;

// draw paper-border

bool CX88PrinterCopyDlg::m_bDrawBorder;

// drawing sprocket-hole

bool CX88PrinterCopyDlg::m_bDrawSprocketHole;

// exist sheet feeder

bool CX88PrinterCopyDlg::m_bExistSheetFeeder;

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88PrinterCopyDlg::CX88PrinterCopyDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	m_nCopyPageNo = 0;
	m_nPageCount = 1;
	m_bCopyMetafile = true;
	m_nZoomOut = 0;
	m_bDrawBorder = true;
	m_bDrawSprocketHole = true;
	m_bExistSheetFeeder = false;
}

// destructor

CX88PrinterCopyDlg::~CX88PrinterCopyDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88PrinterCopyDlg::DlgProc(
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
		case IDC_PRINTER_COPY_PAGENO:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				OnPageNoChange(hdlg);
			}
			break;
		case IDC_PRINTER_COPY_METAFILE:
		case IDC_PRINTER_COPY_TEXT:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnDataTypeChange(hdlg);
			}
			break;
		case IDC_PRINTER_COPY_ZOOM:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				OnZoomOutChange(hdlg);
			}
			break;
		case IDC_PRINTER_COPY_BORDER:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnBorderChange(hdlg);
			}
			break;
		case IDC_PRINTER_COPY_HOLE:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnSprocketHoleChange(hdlg);
			}
			break;
		}
		break;
	case WM_VSCROLL:
		switch (GetDlgCtrlID((HWND)lParam)) {
		case IDC_PRINTER_COPY_PAGENO_SPIN:
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

bool CX88PrinterCopyDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		SetChildren(pDialog);
		GtkWidget* pspinPageNo = GetDlgItem(
			pDialog, IDC_PRINTER_COPY_PAGENO_SPIN);
		if (pspinPageNo != NULL) {
			GtkAdjustment* pAdjustment =
				gtk_spin_button_get_adjustment(
					GTK_SPIN_BUTTON(pspinPageNo));
			g_signal_connect(
				pAdjustment, "value_changed",
				G_CALLBACK(OnValueChangedSignalPageNo),
				pDialog);
		}
		for (int i = 0; i < 2; i++) {
			int nID = 0;
			switch (i) {
			case 0:
				nID = IDC_PRINTER_COPY_METAFILE;
				break;
			case 1:
				nID = IDC_PRINTER_COPY_TEXT;
				break;
			}
			g_signal_connect(
				GetDlgItem(pDialog, nID), "toggled",
				G_CALLBACK(OnToggledSignalDataType),
				pDialog);
		}
		GtkWidget* pcomboZoomOut = GetDlgItem(pDialog, IDC_PRINTER_COPY_ZOOM);
		if (pcomboZoomOut != NULL) {
			g_signal_connect(
				pcomboZoomOut, "changed",
				G_CALLBACK(OnChangedSignalZoomOutCombo),
				pDialog);
		}
		g_signal_connect(
			GetDlgItem(pDialog, IDC_PRINTER_COPY_BORDER), "toggled",
			G_CALLBACK(OnToggledSignalBorder),
			pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_PRINTER_COPY_HOLE), "toggled",
			G_CALLBACK(OnToggledSignalSprocketHole),
			pDialog);
	}
	return true;
}

// page number changed signal

void CX88PrinterCopyDlg::OnValueChangedSignalPageNo(
	GtkAdjustment* /*pAdjustment*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnPageNoChange(pDialog);
}

// data type toggled signal

void CX88PrinterCopyDlg::OnToggledSignalDataType(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	if (gtk_toggle_button_get_active(pToggleButton) != FALSE) {
		GtkWidget* pDialog = GTK_WIDGET(pData);
		OnDataTypeChange(pDialog);
	}
}

// zoom-out rate combobox changed signal

void CX88PrinterCopyDlg::OnChangedSignalZoomOutCombo(
	GtkWidget* /*pWidget*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnZoomOutChange(pDialog);
}

// border drawing toggled signal

void CX88PrinterCopyDlg::OnToggledSignalBorder(
	GtkToggleButton* /*pToggleButton*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnBorderChange(pDialog);
}

// sprocket-hole drawing toggled signal

void CX88PrinterCopyDlg::OnToggledSignalSprocketHole(
	GtkToggleButton* /*pToggleButton*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	OnSprocketHoleChange(pDialog);
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// set children

void CX88PrinterCopyDlg::SetChildren(CX88WndHandle hdlg) {
	SetSpinButtonRange(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_PAGENO_SPIN),
		1, GetPageCount());
	SetSpinButtonValue(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_PAGENO),
		GetDlgItem(hdlg, IDC_PRINTER_COPY_PAGENO_SPIN),
		GetCopyPageNo()+1);
	SetRadioButtonSel(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_METAFILE),
		IsCopyMetafile()? 0: 1);
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_ZOOM),
		IsCopyMetafile());
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_BORDER),
		IsCopyMetafile());
	EnableControl(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_HOLE),
		IsCopyMetafile() && !IsExistSheetFeeder());
	bool bComboSet = (GetComboBoxItemCount(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_ZOOM)) <= 0);
	if (bComboSet) {
		ClearComboBox(
			GetDlgItem(hdlg, IDC_PRINTER_COPY_ZOOM));
		AddComboBoxItem(
			GetDlgItem(hdlg, IDC_PRINTER_COPY_ZOOM),
			RSTR("Same"));
		AddComboBoxItem(
			GetDlgItem(hdlg, IDC_PRINTER_COPY_ZOOM),
			RSTR("1/2"));
		AddComboBoxItem(
			GetDlgItem(hdlg, IDC_PRINTER_COPY_ZOOM),
			RSTR("1/4"));
	}
	SetComboBoxSel(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_ZOOM),
		GetZoomOut());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_BORDER),
		IsDrawBorder());
	SetCheckButtonChecked(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_HOLE),
		IsDrawSprocketHole());
}

// on changed page number

void CX88PrinterCopyDlg::OnPageNoChange(CX88WndHandle hdlg) {
	int nPage = GetSpinButtonValue(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_PAGENO),
		GetDlgItem(hdlg, IDC_PRINTER_COPY_PAGENO_SPIN))-1;
	if (nPage < 0) {
		nPage = 0;
	} else if (nPage >= GetPageCount()) {
		nPage = GetPageCount()-1;
	}
	SetCopyPageNo(nPage);
	SetChildren(hdlg);
}

// on changed data type

void CX88PrinterCopyDlg::OnDataTypeChange(CX88WndHandle hdlg) {
	bool bCopyMetafile = (GetRadioButtonSel(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_METAFILE)) == 0);
	SetCopyMetafile(bCopyMetafile);
	SetChildren(hdlg);
}

// on changed zoom-out rate

void CX88PrinterCopyDlg::OnZoomOutChange(CX88WndHandle hdlg) {
	int nZoomOut = GetComboBoxSel(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_ZOOM));
	if (nZoomOut < 0) {
		nZoomOut = 0;
	} else if (nZoomOut > 2) {
		nZoomOut = 2;
	}
	SetZoomOut(nZoomOut);
	SetChildren(hdlg);
}

// on changed border drawing

void CX88PrinterCopyDlg::OnBorderChange(CX88WndHandle hdlg) {
	bool bDrawBorder = IsCheckButtonChecked(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_BORDER));
	SetDrawBorder(bDrawBorder);
	SetChildren(hdlg);
}

// on changed sprocket-hole drawing

void CX88PrinterCopyDlg::OnSprocketHoleChange(CX88WndHandle hdlg) {
	bool bDrawSprocketHole = IsCheckButtonChecked(
		GetDlgItem(hdlg, IDC_PRINTER_COPY_HOLE));
	SetDrawSprocketHole(bDrawSprocketHole);
	SetChildren(hdlg);
}

// create modal dialog

int CX88PrinterCopyDlg::DoModal() {
	return ExecuteDialog(
		IDD_PRINTER_COPY,
		GetParentWndHandle(),
		DlgProc);
}
