////////////////////////////////////////////////////////////
// X88000 Debug Dump Dialog
//
// Written by Manuke

#ifndef X88DebugDumpDlg_DEFINED
#define X88DebugDumpDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88DebugDumpDlg;

////////////////////////////////////////////////////////////
// include

#include "Z80Adapter.h"

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88DebugDumpDlg

class CX88DebugDumpDlg :
	public CX88Dialog
{
// attribute
protected:
	// Z80 adapter
	static CZ80Adapter* m_pZ80A;

#ifdef X88_GUI_WINDOWS

	// fixed pitch font
	static HFONT m_hfontFix;

#endif // X88_GUI

public:
	// set Z80 adapter
	void SetZ80Adapter(CZ80Adapter* pZ80A) {
		m_pZ80A = pZ80A;
	}

// create & destroy
public:
	// standard constructor
	CX88DebugDumpDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88DebugDumpDlg();

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
	// dump clicked signal
	static void OnClickedSignalDump(
		GtkButton* pButton, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// set children
	static void SetChildren(CX88WndHandle hdlg);
	// execute dump
	static void DoDump(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88DebugDumpDlg_DEFINED
