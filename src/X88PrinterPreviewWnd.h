////////////////////////////////////////////////////////////
// X88000 Printer Preview Window
//
// Written by Manuke

#ifndef X88PrinterPreviewWnd_DEFINED
#define X88PrinterPreviewWnd_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88PrinterPreviewWnd;

////////////////////////////////////////////////////////////
// include

#include "ParallelPrinter.h"

#include "X88PrinterDrawer.h"

////////////////////////////////////////////////////////////
// declaration of CX88PrinterPreviewWnd

class CX88PrinterPreviewWnd {
// enum
protected:

#ifdef X88_GUI_WINDOWS

	// zoom
	enum {
		ZOOM_MAX = 5
	};

#elif defined(X88_GUI_GTK)

	// zoom
	enum {
		ZOOM_MAX = 4
	};
	// base DPI
	enum {
		BASE_DPI = 18
	};

#endif // X88_GUI

// attribute
protected:
	// window handle
	static CX88WndHandle m_hWnd;
	// limit of window left
	static int m_nLimitLeft;
	// limit of window top
	static int m_nLimitTop;
	// limit of window right
	static int m_nLimitRight;
	// limit of window bottom
	static int m_nLimitBottom;
	// default width
	static int m_cxDefault;
	// default height
	static int m_cyDefault;
	// preview width
	static int m_cxPreview;
	// preview height
	static int m_cyPreview;
	// zoom rate
	static int m_nZoom;

	// target printer
	static CParallelPrinter* m_pTargetPrinter;
	// target page
	static int m_nTargetPage;

#ifdef X88_GUI_GTK

	// scroll window
	static GtkScrolledWindow* m_pScrolledWindow;
	// current page width
	static int m_nCurPageWidth;
	// current page height
	static int m_nCurPageHeight;
	// current page pixmap
	static GdkPixmap* m_ppmpCurPage;

#endif // X88_GUI

public:
	// get window handle
	static CX88WndHandle GetWndHandle() {
		return m_hWnd;
	}
	// set target printer
	static void SetTargetPrinter(CParallelPrinter* pTargetPrinterNew) {
		m_pTargetPrinter = pTargetPrinterNew;
	}
	// get target page
	static int GetTargetPage() {
		return m_nTargetPage;
	}

// create & destroy
public:
	// default constructor
	CX88PrinterPreviewWnd();
	// destructor
	virtual ~CX88PrinterPreviewWnd();

	// create
	static bool Create(
		int nLimitLeft, int nLimitTop, int nLimitRight, int nLimitBottom,
		CX88WndHandle hwndParent, CX88WndHandle hwndHint,
		int nID);
	// destroy
	static bool Destroy();

// implementation
protected:

#ifdef X88_GUI_WINDOWS

	// window procedure
	static LRESULT CALLBACK WndProc(
		HWND hwnd, UINT nMessage, WPARAM wParam, LPARAM lParam);

#elif defined(X88_GUI_GTK)

	// expose event
	static gboolean OnExposeEvent(
		GtkWidget* pWidget,
		GdkEventExpose* pEvent,
		gpointer pData);
	// button-press event
	static gboolean OnButtonPressEvent(
		GtkWidget* pWidget, GdkEventButton* pEvent, gpointer* pData);
	// key-press event
	static gboolean OnKeyPressEvent(
		GtkWidget* pWidget, GdkEventKey* pEvent, gpointer* pData);
	// focus-in event
	static gboolean OnFocusInEvent(
		GtkWidget* pWidget, GdkEventFocus* pEvent, gpointer* pData);
	// focus-out event
	static gboolean OnFocusOutEvent(
		GtkWidget* pWidget, GdkEventFocus* pEvent, gpointer* pData);

#endif // X88_GUI

// operation
protected:
	// convert from preview-point to print-point
	static void Preview2Print(int& xConv, int& yConv);
	// convert from -printpoint to preview-point
	static void Print2Preview(int& xConv, int& yConv);
	// adjust preview size
	static void AdjustPreviewSize(int xCenter, int yCenter);

#ifdef X88_GUI_GTK

	// create current page pixmap
	static GdkPixmap* CreateCurPagePixmap(
		int& nWidth, int& nHeight);

#endif // X88_GUI

public:
	// change paper
	static void ChangePaper();
	// set target page
	static void SetTargetPage(int nTargetPage);
	// rebuild page
	static void RebuildPage();
	// resize
	static void Resize(int nLimitRightNew, int nLimitBottomNew);
};

#endif // X88PrinterPreviewWnd_DEFINED
