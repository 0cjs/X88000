////////////////////////////////////////////////////////////
// X88000 Debug BreakPoint Dialog
//
// Written by Manuke

#ifndef X88DebugBreakPointDlg_DEFINED
#define X88DebugBreakPointDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88DebugBreakPointDlg;

////////////////////////////////////////////////////////////
// include

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88DebugBreakPointDlg

class CX88DebugBreakPointDlg :
	public CX88Dialog
{
// attribute
protected:
	// program-counter
	static uint16_t m_wPC;
	// break points
	static std::set<uint16_t>* m_psetBreakPoint;
	// break point list
	static std::list<uint16_t> m_listBreakPoint;

public:
	// set program-counter
	static void SetPC(uint16_t wPC) {
		m_wPC = wPC;
	}
	// set break points
	static void SetBreakPoint(std::set<uint16_t>* psetBreakPoint) {
		m_psetBreakPoint = psetBreakPoint;
	}

// create & destroy
public:
	// standard constructor
	CX88DebugBreakPointDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88DebugBreakPointDlg();

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
	// break point operation clicked signal
	static void OnClickedSignalBreakPointOperate(
		GtkButton* pButton, gpointer pData);
	// break point selection changed signal
	static void OnSelectionChangedSignalBreakPoint(
		GtkTreeSelection* pTreeSelection, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// initialize break point
	static void InitializeBreakPoint(CX88WndHandle hdlg);
	// finalize break point
	static void FinalizeBreakPoint(CX88WndHandle hdlg);
	// set remove erase button
	static void SetRemoveEraseButton(CX88WndHandle hdlg);
	// on add address
	static void OnAddressAdd(CX88WndHandle hdlg);
	// on remove break point
	static void OnBreakPointRemove(CX88WndHandle hdlg);
	// on erase break point
	static void OnBreakPointErase(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88DebugBreakPointDlg_DEFINED
