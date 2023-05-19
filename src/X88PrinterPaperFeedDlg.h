////////////////////////////////////////////////////////////
// X88000 Printer PaperFeed Dialog
//
// Written by Manuke

#ifndef X88PrinterPaperFeedDlg_DEFINED
#define X88PrinterPaperFeedDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88PrinterPaperFeedDlg;

////////////////////////////////////////////////////////////
// include

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88PrinterPaperFeedDlg

class CX88PrinterPaperFeedDlg :
	public CX88Dialog
{
// enum
public:
	// paper feed unit
	enum {
		UNIT_PAGE  = 0,
		UNIT_LINE  = 1,
		UNIT_POINT = 2
	};

// attribute
protected:
	// paper feed unit
	static int m_nPaperFeedUnit;
	// line feed unit
	static int m_nLineFeedCount;
	// point feed unit
	static int m_nPointFeedCount;

public:
	// get paper feed unit
	static int GetPaperFeedUnit() {
		return m_nPaperFeedUnit;
	}
	// set paper feed unit
	static void SetPaperFeedUnit(int nPaperFeedUnitNew) {
		m_nPaperFeedUnit = nPaperFeedUnitNew;
	}
	// get line feed unit
	static int GetLineFeedCount() {
		return m_nLineFeedCount;
	}
	// set line feed unit
	static void SetLineFeedCount(int nLineFeedCountNew) {
		m_nLineFeedCount = nLineFeedCountNew;
	}
	// get point feed unit
	static int GetPointFeedCount() {
		return m_nPointFeedCount;
	}
	// set point feed unit
	static void SetPointFeedCount(int nPointFeedCountNew) {
		m_nPointFeedCount = nPointFeedCountNew;
	}

// create & destroy
public:
	// standard constructor
	CX88PrinterPaperFeedDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88PrinterPaperFeedDlg();

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
	// feed unit toggled signal
	static void OnToggledSignalFeedUnit(
		GtkToggleButton* pToggleButton, gpointer pData);
	// line feed unit changed signal
	static void OnValueChangedSignalLineFeedCount(
		GtkAdjustment* pAdjustment, gpointer pData);
	// point feed unit changed signal
	static void OnValueChangedSignalPointFeedCount(
		GtkAdjustment* pAdjustment, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// set children
	static void SetChildren(CX88WndHandle hdlg);
	// on changed paper feed unit
	static void OnFeedUnitChange(CX88WndHandle hdlg);
	// on changed line feed unit
	static void OnLineFeedCountChange(CX88WndHandle hdlg);
	// on changed point feed unit
	static void OnPointFeedCountChange(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88PrinterPaperFeedDlg_DEFINED
