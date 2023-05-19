////////////////////////////////////////////////////////////
// X88000 Printer PaperDelete Dialog
//
// Written by Manuke

#ifndef X88PrinterPaperDelDlg_DEFINED
#define X88PrinterPaperDelDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88PrinterPaperDelDlg;

////////////////////////////////////////////////////////////
// include

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88PrinterPaperDelDlg

class CX88PrinterPaperDelDlg :
	public CX88Dialog
{
// attribute
protected:
	// delete all pages
	static bool m_bDelAllPages;
	// delete page number
	static int m_nDelPageNo;
	// page count
	static int m_nPageCount;

public:
	// is delete all pages
	static bool IsDelAllPages() {
		return m_bDelAllPages;
	}
	// set delete all pages
	static void SetDelAllPages(bool bDelAllPagesNew) {
		m_bDelAllPages = bDelAllPagesNew;
	}
	// get delete page number
	static int GetDelPageNo() {
		return m_nDelPageNo;
	}
	// set delete page number
	static void SetDelPageNo(int nDelPageNoNew) {
		m_nDelPageNo = nDelPageNoNew;
	}
	// get page count
	static int GetPageCount() {
		return m_nPageCount;
	}
	// set page count
	static void SetPageCount(int nPageCountNew) {
		m_nPageCount = nPageCountNew;
	}

// create & destroy
public:
	// standard constructor
	CX88PrinterPaperDelDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88PrinterPaperDelDlg();

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
	// target type toggled signal
	static void OnToggledSignalTarget(
		GtkToggleButton* pToggleButton, gpointer pData);
	// page number changed signal
	static void OnValueChangedSignalPageNo(
		GtkAdjustment* pAdjustment, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// set children
	static void SetChildren(CX88WndHandle hdlg);
	// on changed target type
	static void OnTargetChange(CX88WndHandle hdlg);
	// on changed target page number
	static void OnPageNoChange(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88PrinterPaperDelDlg_DEFINED
