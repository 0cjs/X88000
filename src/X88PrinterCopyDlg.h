////////////////////////////////////////////////////////////
// X88000 Printer Copy Dialog
//
// Written by Manuke

#ifndef X88PrinterCopyDlg_DEFINED
#define X88PrinterCopyDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88PrinterCopyDlg;

////////////////////////////////////////////////////////////
// include

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88PrinterCopyDlg

class CX88PrinterCopyDlg :
	public CX88Dialog
{
// attribute
protected:
	// copy page number
	static int m_nCopyPageNo;
	// page count
	static int m_nPageCount;
	// copy metafile
	static bool m_bCopyMetafile;
	// zoom-out rate
	static int m_nZoomOut;
	// draw paper-border
	static bool m_bDrawBorder;
	// drawing sprocket-hole
	static bool m_bDrawSprocketHole;
	// exist sheet feeder
	static bool m_bExistSheetFeeder;

public:
	// get copy page number
	static int GetCopyPageNo() {
		return m_nCopyPageNo;
	}
	// set copy page number
	static void SetCopyPageNo(int nCopyPageNoNew) {
		m_nCopyPageNo = nCopyPageNoNew;
	}
	// get page count
	static int GetPageCount() {
		return m_nPageCount;
	}
	// set page count
	static void SetPageCount(int nPageCountNew) {
		m_nPageCount = nPageCountNew;
	}
	// is copy metafile
	static bool IsCopyMetafile() {
		return m_bCopyMetafile;
	}
	// set copy metafile
	static void SetCopyMetafile(bool bCopyMetafileNew) {
		m_bCopyMetafile = bCopyMetafileNew;
	}
	// get zoom-out rate
	static int GetZoomOut() {
		return m_nZoomOut;
	}
	// set zoom-out rate
	static void SetZoomOut(int nZoomOutNew) {
		m_nZoomOut = nZoomOutNew;
	}
	// is draw paper-border
	static bool IsDrawBorder() {
		return m_bDrawBorder;
	}
	// set draw paper-border
	static void SetDrawBorder(bool bDrawBorderNew) {
		m_bDrawBorder = bDrawBorderNew;
	}
	// is drawing sprocket-hole
	static bool IsDrawSprocketHole() {
		return m_bDrawSprocketHole;
	}
	// set drawing sprocket-hole
	static void SetDrawSprocketHole(bool bDrawSprocketHoleNew) {
		m_bDrawSprocketHole = bDrawSprocketHoleNew;
	}
	// is exist sheet feeder
	static bool IsExistSheetFeeder() {
		return m_bExistSheetFeeder;
	}
	// set exist sheet feeder
	static void SetExistSheetFeeder(bool bExistSheetFeederNew) {
		m_bExistSheetFeeder = bExistSheetFeederNew;
	}

// create & destroy
public:
	// standard constructor
	CX88PrinterCopyDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88PrinterCopyDlg();

// implementation
protected:

#ifdef X88_GUI_WINDOWS

	// dialog procedure
	static BOOL CALLBACK DlgProc(
		HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam);

#elif defined(X88_GUI_GTK)

	// dialog procedure(initialize & dispose)
	static bool DlgProc(
		GtkWidget* pDialog,
		bool bInitialize,
		int nID);
	// page number changed signal
	static void OnValueChangedSignalPageNo(
		GtkAdjustment* pAdjustment, gpointer pData);
	// data type toggled signal
	static void OnToggledSignalDataType(
		GtkToggleButton* pToggleButton, gpointer pData);
	// zoom-out rate combobox changed signal
	static void OnChangedSignalZoomOutCombo(
		GtkWidget* pWidget, gpointer pData);
	// border drawing toggled signal
	static void OnToggledSignalBorder(
		GtkToggleButton* pToggleButton, gpointer pData);
	// sprocket-hole drawing toggled signal
	static void OnToggledSignalSprocketHole(
		GtkToggleButton* pToggleButton, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// set children
	static void SetChildren(CX88WndHandle hdlg);
	// on changed page number
	static void OnPageNoChange(CX88WndHandle hdlg);
	// on changed data type
	static void OnDataTypeChange(CX88WndHandle hdlg);
	// on changed zoom-out rate
	static void OnZoomOutChange(CX88WndHandle hdlg);
	// on changed border drawing
	static void OnBorderChange(CX88WndHandle hdlg);
	// on changed sprocket-hole drawing
	static void OnSprocketHoleChange(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88PrinterCopyDlg_DEFINED
