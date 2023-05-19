////////////////////////////////////////////////////////////
// X88000 Debug DisAssemble Dialog
//
// Written by Manuke

#ifndef X88DebugDisAssembleDlg_DEFINED
#define X88DebugDisAssembleDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88DebugDisAssembleDlg;

////////////////////////////////////////////////////////////
// include

#include "Z80Adapter.h"

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88DebugDisAssembleDlg

class CX88DebugDisAssembleDlg :
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
	CX88DebugDisAssembleDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88DebugDisAssembleDlg();

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
	// dis-assemble clicked signal
	static void OnClickedSignalDisAssemble(
		GtkButton* pButton, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// set children
	static void SetChildren(CX88WndHandle hdlg);
	// execute dis-assemble
	static void DoDisAssemble(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88DebugDisAssembleDlg_DEFINED
