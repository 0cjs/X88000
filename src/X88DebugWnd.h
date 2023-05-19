////////////////////////////////////////////////////////////
// X88000 Debug Window
//
// Written by Manuke

#ifndef X88DebugWnd_DEFINED
#define X88DebugWnd_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88DebugWnd;

////////////////////////////////////////////////////////////
// include

#include "X88000.h"

////////////////////////////////////////////////////////////
// declaration of CX88DebugWnd

class CX88DebugWnd {
// enum
protected:
	// timer
	enum {
		TIMER_ID     = 1,
		TIMER_ELAPSE = 250
	};

public:
	// window parameter
	enum {
		CTRL_ID         = 2,
		CXWINDOWDEFAULT = 640,
		CYWINDOWDEFAULT = 80,
		CXBUTTON        = 60,
		CYBUTTON        = 20,
		CXDEBUGFONT     = 8,
		CYDEBUGFONT     = 16
	};

// attribute
protected:
	// X88000 object
	static CX88000* m_pX88k;

	// window handle
	static CX88WndHandle m_hWnd;

	// step execution button handle
	static CX88WndHandle m_hbtnStep;
	// trace execution button handle
	static CX88WndHandle m_hbtnTrace;
	// step execution2 button handle
	static CX88WndHandle m_hbtnStep2;

#ifdef X88_GUI_WINDOWS

	// debug display font
	static HFONT m_hfontDebug;
	// timer id
	static UINT_PTR m_nTimerID;

#elif defined(X88_GUI_GTK)

	// horizontal packing-box
	static CX88WndHandle m_hwndHBox;
	// mnemonic label
	static CX88WndHandle m_hstcMnemonic;
	// register label
	static CX88WndHandle m_hstcRegister;
	// timeout id
	static gint m_nTimeOutID;

#endif // X88_GUI

public:
	// X88000 object
	static CX88000& X88k() {
		return *m_pX88k;
	}
	// get window handle
	static CX88WndHandle GetWndHandle() {
		return m_hWnd;
	}
	// get step execution button handle
	static CX88WndHandle GetStepButtonHandle() {
		return m_hbtnStep;
	}
	// get trace execution button handle
	static CX88WndHandle GetTraceButtonHandle() {
		return m_hbtnTrace;
	}
	// get step execution2 button handle
	static CX88WndHandle GetStep2ButtonHandle() {
		return m_hbtnStep2;
	}

// create & destroy
public:
	// default constructor
	CX88DebugWnd();
	// destructor
	~CX88DebugWnd();

	// create
	static bool Create(CX88WndHandle hwndFrame, CX88000& x88k);
	// destroy
	static bool Destroy();

// implementation
protected:

#ifdef X88_GUI_WINDOWS

	// window procedure
	static LRESULT CALLBACK WndProc(
		HWND hwnd, UINT nMessage, WPARAM wParam, LPARAM lParam);
	// draw debug info
	static void DrawDebugInfo(HDC hdc);

#elif defined(X88_GUI_GTK)

	// destroy signal
	static void OnDestroySignal(
		GtkWidget* pWidget, gpointer pData);
	// update mnemonic label text
	static void UpdateMnemonic(
		GtkWidget* pWidget);
	// update register label text
	static void UpdateRegister(
		GtkWidget* pWidget);
	// time out callback
	static gboolean TimeOutCallback(gpointer pData);

#endif // X88_GUI

public:
	// update debug window
	static void UpdateDebugWnd();
};

#endif // X88DebugWnd_DEFINED
