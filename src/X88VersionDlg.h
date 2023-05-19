////////////////////////////////////////////////////////////
// X88000 Version Info Dialog
//
// Written by Manuke

#ifndef X88VersionDlg_DEFINED
#define X88VersionDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88VersionDlg;

////////////////////////////////////////////////////////////
// include

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88VersionDlg

class CX88VersionDlg :
	public CX88Dialog
{
// create & destroy
public:
	// standard constructor
	CX88VersionDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88VersionDlg();

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

#endif // X88_GUI

// operation
protected:
	// set version
	static void SetVersion(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88VersionDlg_DEFINED
