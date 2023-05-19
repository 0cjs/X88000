////////////////////////////////////////////////////////////
// X88000 Debug Write RAM Dialog
//
// Written by Manuke

#ifndef X88DebugWriteRamDlg_DEFINED
#define X88DebugWriteRamDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88DebugWriteRamDlg;

////////////////////////////////////////////////////////////
// include

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88DebugWriteRamDlg

class CX88DebugWriteRamDlg :
	public CX88Dialog
{
// attribute
protected:
	// main RAM0 writing
	static bool m_bWriteMainRam0;
	// main RAM1 writing
	static bool m_bWriteMainRam1;
	// fast text VRAM writing
	static bool m_bWriteFastTVRam;
	// slow text VRAM writing
	static bool m_bWriteSlowTVRam;
	// graphic VRAM0 writing
	static bool m_bWriteGVRam0;
	// graphic VRAM1 writing
	static bool m_bWriteGVRam1;
	// graphic VRAM2 writing
	static bool m_bWriteGVRam2;
	// subsystem RAM writing
	static bool m_bWriteSubRam;
	// expansion RAM0 writing
	static bool m_bWriteExRam0;
	// expansion RAM1 writing
	static bool m_bWriteExRam1;
	// fast text VRAM using mode
	static bool m_bFastTVRamUse;
	// subsystem disable
	static bool m_bSubSystemDisable;

public:
	// is main RAM0 writing
	static bool IsWriteMainRam0() {
		return m_bWriteMainRam0;
	}
	// set main RAM0 writing
	static void SetWriteMainRam0(bool bWriteMainRam0New) {
		m_bWriteMainRam0 = bWriteMainRam0New;
	}
	// is main RAM1 writing
	static bool IsWriteMainRam1() {
		return m_bWriteMainRam1;
	}
	// set main RAM1 writing
	static void SetWriteMainRam1(bool bWriteMainRam1New) {
		m_bWriteMainRam1 = bWriteMainRam1New;
	}
	// is fast text VRAM writing
	static bool IsWriteFastTVRam() {
		return m_bWriteFastTVRam;
	}
	// set fast text VRAM writing
	static void SetWriteFastTVRam(bool bWriteFastTVRamNew) {
		m_bWriteFastTVRam = bWriteFastTVRamNew;
	}
	// is slow text VRAM writing
	static bool IsWriteSlowTVRam() {
		return m_bWriteSlowTVRam;
	}
	// set slow text VRAM writing
	static void SetWriteSlowTVRam(bool bWriteSlowTVRamNew) {
		m_bWriteSlowTVRam = bWriteSlowTVRamNew;
	}
	// is graphic VRAM0 writing
	static bool IsWriteGVRam0() {
		return m_bWriteGVRam0;
	}
	// set graphic VRAM0 writing
	static void SetWriteGVRam0(bool bWriteGVRam0New) {
		m_bWriteGVRam0 = bWriteGVRam0New;
	}
	// is graphic VRAM1 writing
	static bool IsWriteGVRam1() {
		return m_bWriteGVRam1;
	}
	// set graphic VRAM1 writing
	static void SetWriteGVRam1(bool bWriteGVRam1New) {
		m_bWriteGVRam1 = bWriteGVRam1New;
	}
	// is graphic VRAM2 writing
	static bool IsWriteGVRam2() {
		return m_bWriteGVRam2;
	}
	// set graphic VRAM2 writing
	static void SetWriteGVRam2(bool bWriteGVRam2New) {
		m_bWriteGVRam2 = bWriteGVRam2New;
	}
	// is subsystem RAM writing
	static bool IsWriteSubRam() {
		return m_bWriteSubRam;
	}
	// set subsystem RAM writing
	static void SetWriteSubRam(bool bWriteSubRamNew) {
		m_bWriteSubRam = bWriteSubRamNew;
	}
	// is expansion RAM0 writing
	static bool IsWriteExRam0() {
		return m_bWriteExRam0;
	}
	// set expansion RAM0 writing
	static void SetWriteExRam0(bool bWriteExRam0New) {
		m_bWriteExRam0 = bWriteExRam0New;
	}
	// is expansion RAM1 writing
	static bool IsWriteExRam1() {
		return m_bWriteExRam1;
	}
	// set expansion RAM1 writing
	static void SetWriteExRam1(bool bWriteExRam1New) {
		m_bWriteExRam1 = bWriteExRam1New;
	}
	// is fast text VRAM using mode
	static bool IsFastTVRamUse() {
		return m_bFastTVRamUse;
	}
	// set fast text VRAM using mode
	static void SetFastTVRamUse(bool bFastTVRamUseNew) {
		m_bFastTVRamUse = bFastTVRamUseNew;
	}
	// is subsystem disable
	static bool IsSubSystemDisable() {
		return m_bSubSystemDisable;
	}
	// set subsystem disable
	static void SetSubSystemDisable(bool bSubSystemDisableNew) {
		m_bSubSystemDisable = bSubSystemDisableNew;
	}

// create & destroy
public:
	// standard constructor
	CX88DebugWriteRamDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88DebugWriteRamDlg();

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
	// write RAM toggled signal
	static void OnToggledSignalWriteRam(
		GtkToggleButton* pToggleButton, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// set children
	static void SetChildren(CX88WndHandle hdlg);
	// on changed RAM writing
	static void OnWriteRamChange(
		CX88WndHandle hdlg, int nID);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88DebugWriteRamDlg_DEFINED
