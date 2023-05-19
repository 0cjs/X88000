////////////////////////////////////////////////////////////
// X88000 Printer Preview Window
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88PrinterPreviewWnd.h"

#include "X88Dialog.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88PrinterPreviewWnd

////////////////////////////////////////////////////////////
// attribute

// window handle

CX88WndHandle CX88PrinterPreviewWnd::m_hWnd = NULL;

// limit of window left

int CX88PrinterPreviewWnd::m_nLimitLeft;

// limit of window top

int CX88PrinterPreviewWnd::m_nLimitTop;

// limit of window right

int CX88PrinterPreviewWnd::m_nLimitRight;

// limit of window bottom

int CX88PrinterPreviewWnd::m_nLimitBottom;

// preview width

int CX88PrinterPreviewWnd::m_cxPreview;

// preview height

int CX88PrinterPreviewWnd::m_cyPreview;

// default width

int CX88PrinterPreviewWnd::m_cxDefault;

// default height

int CX88PrinterPreviewWnd::m_cyDefault;

// zoom rate

int CX88PrinterPreviewWnd::m_nZoom = 0;

// target printer

CParallelPrinter* CX88PrinterPreviewWnd::m_pTargetPrinter = NULL;

// target page

int CX88PrinterPreviewWnd::m_nTargetPage = 0;


#ifdef X88_GUI_GTK

// scroll window

GtkScrolledWindow* CX88PrinterPreviewWnd::m_pScrolledWindow;

// current page width

int CX88PrinterPreviewWnd::m_nCurPageWidth;

// current page height

int CX88PrinterPreviewWnd::m_nCurPageHeight;

// current page pixmap

GdkPixmap* CX88PrinterPreviewWnd::m_ppmpCurPage;

#endif // X88_GUI

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88PrinterPreviewWnd::CX88PrinterPreviewWnd() {
}

// destructor

CX88PrinterPreviewWnd::~CX88PrinterPreviewWnd() {
}

// create

bool CX88PrinterPreviewWnd::Create(
	int nLimitLeft, int nLimitTop, int nLimitRight, int nLimitBottom,
	CX88WndHandle hwndParent, CX88WndHandle hwndHint,
	int nID)
{

#ifdef X88_GUI_WINDOWS

	m_hWnd = NULL;
	m_nTargetPage = 0;
	m_nLimitLeft = nLimitLeft;
	m_nLimitTop = nLimitTop;
	m_nLimitRight = nLimitRight;
	m_nLimitBottom = nLimitBottom;
	m_cxDefault = m_nLimitRight-m_nLimitLeft;
	m_cyDefault = m_nLimitBottom-m_nLimitTop;
	m_nZoom = 0;
	m_cxPreview = m_cyPreview = 1;
	const char* pszPreviewName = "X88PrinterPreviewWnd";
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = CX88Dialog::GetResourceHandle();
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = pszPreviewName;
	RegisterClass(&wc);
	CreateWindow(
		pszPreviewName, pszPreviewName,
		WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP,
		m_nLimitLeft, m_nLimitTop,
		m_nLimitRight-m_nLimitLeft, m_nLimitBottom-m_nLimitTop,
		hwndParent, (HMENU)(INT_PTR)nID, wc.hInstance,
		NULL);
	if (GetWndHandle() != NULL) {
		SetWindowPos(
			GetWndHandle(),
			hwndHint,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		int xCenter = 0,
			yCenter = 0;
		AdjustPreviewSize(xCenter, yCenter);
		ShowWindow(GetWndHandle(), SW_SHOWNOACTIVATE);
	}

#elif defined(X88_GUI_GTK)

	(void)nLimitLeft;
	(void)nLimitTop;
	(void)nLimitRight;
	(void)nLimitBottom;
	(void)nID;

	m_nTargetPage = 0;
	m_nZoom = 0;
	m_nCurPageWidth = m_nCurPageHeight = 0;
	m_ppmpCurPage = NULL;
	m_pScrolledWindow = GTK_SCROLLED_WINDOW(hwndHint);
	m_hWnd = NULL;
	if (m_pScrolledWindow != NULL) {
		g_object_ref(m_pScrolledWindow);
		m_hWnd = gtk_drawing_area_new();
	}
	if (GetWndHandle() != NULL) {
		g_object_ref(GetWndHandle());
		GTK_WIDGET_SET_FLAGS(GetWndHandle(), GTK_CAN_FOCUS);
		RebuildPage();
		g_signal_connect(
			GetWndHandle(), "expose_event",
			G_CALLBACK(OnExposeEvent), NULL);
		g_signal_connect(
			GetWndHandle(), "button_press_event",
			G_CALLBACK(OnButtonPressEvent), NULL);
		g_signal_connect_after(
			GetWndHandle(), "key_press_event",
			G_CALLBACK(OnKeyPressEvent), NULL);
		g_signal_connect(
			GetWndHandle(), "focus_in_event",
			G_CALLBACK(OnFocusInEvent), NULL);
		g_signal_connect(
			GetWndHandle(), "focus_out_event",
			G_CALLBACK(OnFocusOutEvent), NULL);
		gtk_widget_set_events(
			GetWndHandle(),
			gtk_widget_get_events(GetWndHandle()) |
				GDK_BUTTON_PRESS_MASK |
				GDK_KEY_PRESS_MASK |
				GDK_EXPOSURE_MASK);
		gtk_container_add(
			GTK_CONTAINER(hwndParent),
			GetWndHandle());
		int xCenter = 0,
			yCenter = 0;
		AdjustPreviewSize(xCenter, yCenter);
		gtk_widget_show(GetWndHandle());
	}

#endif // X88_GUI

	return GetWndHandle() != NULL;
}

// destroy

bool CX88PrinterPreviewWnd::Destroy() {

#ifdef X88_GUI_WINDOWS

	if (GetWndHandle() == NULL) {
		return false;
	}
	DestroyWindow(GetWndHandle());

#elif defined(X88_GUI_GTK)

	if (GetWndHandle() == NULL) {
		return false;
	}
	g_object_unref(GetWndHandle());
	m_hWnd = NULL;
	g_object_unref(m_pScrolledWindow);
	m_pScrolledWindow = NULL;
	if (m_ppmpCurPage != NULL) {
		g_object_unref(m_ppmpCurPage);
		m_ppmpCurPage = NULL;
	}

#endif // X88_GUI

	return GetWndHandle() == NULL;
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// window procedure

LRESULT CALLBACK CX88PrinterPreviewWnd::WndProc(
	HWND hwnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;
	bool bDWP = false;
	switch (nMessage) {
	case WM_NCCREATE:
		m_hWnd = hwnd;
		bDWP = TRUE;
		break;
	case WM_NCDESTROY:
		m_hWnd = NULL;
		bDWP = TRUE;
		break;
	case WM_GETDLGCODE:
		lResult = DLGC_WANTARROWS;
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (((nMessage == WM_LBUTTONDOWN) && (m_nZoom < ZOOM_MAX)) ||
			((nMessage == WM_RBUTTONDOWN) && (m_nZoom > 0)))
		{
			int xCenter = (short)LOWORD(lParam),
				yCenter = (short)HIWORD(lParam);
			SCROLLINFO si;
			memset(&si, 0, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;
			GetScrollInfo(hwnd, SB_HORZ, &si);
			xCenter += si.nPos;
			GetScrollInfo(hwnd, SB_VERT, &si);
			yCenter += si.nPos;
			Preview2Print(xCenter, yCenter);
			m_nZoom += (nMessage == WM_LBUTTONDOWN)? 1: -1;
			AdjustPreviewSize(xCenter, yCenter);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_NEXT:
		case VK_PRIOR:
			if (((wParam == VK_NEXT) && (m_nZoom < ZOOM_MAX)) ||
				((wParam == VK_PRIOR) && (m_nZoom > 0)))
			{
				RECT rectClient;
				GetClientRect(hwnd, &rectClient);
				int xCenter = (rectClient.left+rectClient.right)/2,
					yCenter = (rectClient.top+rectClient.bottom)/2;
				SCROLLINFO si;
				memset(&si, 0, sizeof(si));
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS;
				GetScrollInfo(hwnd, SB_HORZ, &si);
				xCenter += si.nPos;
				GetScrollInfo(hwnd, SB_VERT, &si);
				yCenter += si.nPos;
				Preview2Print(xCenter, yCenter);
				m_nZoom += (wParam == VK_NEXT)? 1: -1;
				AdjustPreviewSize(xCenter, yCenter);
			}
			break;
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
			{ // dummy block
				UINT nMessage2;
				if ((wParam == VK_LEFT) || (wParam == VK_RIGHT)) {
					nMessage2 = WM_HSCROLL;
				} else {
					nMessage2 = WM_VSCROLL;
				}
				bool bControl = (GetKeyState(VK_CONTROL) < 0);
				WPARAM wParam2;
				if ((wParam == VK_LEFT) || (wParam == VK_UP)) {
					if (!bControl) {
						wParam2 = MAKEWPARAM(SB_LINELEFT, 0);
					} else {
						wParam2 = MAKEWPARAM(SB_LEFT, 0);
					}
				} else {
					if (!bControl) {
						wParam2 = MAKEWPARAM(SB_LINERIGHT, 0);
					} else {
						wParam2 = MAKEWPARAM(SB_RIGHT, 0);
					}
				}
				PostMessage(hwnd, nMessage2, wParam2, 0);
			}
			break;
		}
		break;
	case WM_HSCROLL:
	case WM_VSCROLL:
		{ // dummy block
			int nKind = (nMessage == WM_HSCROLL)? SB_HORZ: SB_VERT;
			SCROLLINFO si;
			memset(&si, 0, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			GetScrollInfo(hwnd, nKind, &si);
			int nNewPos = si.nPos;
			switch (LOWORD(wParam)) {
			case SB_LEFT:
			// case SB_TOP:
				nNewPos = 0;
				break;
			case SB_RIGHT:
			// case SB_BOTTOM:
				nNewPos = si.nMax-si.nPage;
				break;
			case SB_LINELEFT:
			// case SB_LINETOP:
				nNewPos -= si.nPage/16;
				break;
			case SB_LINERIGHT:
			// case SB_LINEBOTTOM:
				nNewPos += si.nPage/16;
				break;
			case SB_PAGELEFT:
			// case SB_PAGETOP:
				nNewPos -= si.nPage/2;
				break;
			case SB_PAGERIGHT:
			// case SB_PAGEBOTTOM:
				nNewPos += si.nPage/2;
				break;
			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				nNewPos = si.nTrackPos;
				break;
			}
			if (nNewPos < 0) {
				nNewPos = 0;
			} else if (nNewPos > (int)(si.nMax-si.nPage)) {
				nNewPos = si.nMax-si.nPage;
			}
			if (nNewPos != si.nPos) {
				si.nPos = nNewPos;
				SetScrollInfo(hwnd, nKind, &si, TRUE);
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
		break;
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		{ // dummy block
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			SaveDC(hdc);
			POINT ptScroll;
			SCROLLINFO si;
			memset(&si, 0, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;
			GetScrollInfo(hwnd, SB_HORZ, &si);
			ptScroll.x = si.nPos;
			GetScrollInfo(hwnd, SB_VERT, &si);
			ptScroll.y = si.nPos;
			int nPaperWidth = m_pTargetPrinter->GetPaperWidth(),
				nPaperHeight = m_pTargetPrinter->GetPaperHeight();
			SetMapMode(hdc, MM_ANISOTROPIC);
			SetViewportOrgEx(
				hdc,
				-ptScroll.x, -ptScroll.y,
				NULL);
			SetViewportExtEx(
				hdc,
				m_cxPreview, m_cyPreview,
				NULL);
			SetWindowOrgEx(
				hdc,
				0, 0,
				NULL);
			SetWindowExtEx(
				hdc,
				nPaperWidth, nPaperHeight,
				NULL);
			RECT rectClip;
			GetClientRect(hwnd, &rectClip);
			DPtoLP(hdc, (POINT*)&rectClip, 2);
			CX88PrinterDrawer prtdrawer;
			prtdrawer.DrawSprocketHole(
				hdc,
				m_pTargetPrinter, m_nTargetPage,
				&rectClip);
			prtdrawer.DrawPrinterHead(
				hdc,
				m_pTargetPrinter, m_nTargetPage,
				&rectClip);
			prtdrawer.DrawPrinterObject(
				hdc,
				m_pTargetPrinter, m_nTargetPage,
				&rectClip);
			RestoreDC(hdc, -1);
			if (GetFocus() == hwnd) {
				RECT rectFocus;
				GetClientRect(hwnd, &rectFocus);
				InflateRect(&rectFocus, -1, -1);
				DrawFocusRect(hdc, &rectFocus);
			}
			EndPaint(hwnd, &ps);
		}
		break;
	default:
		bDWP = true;
		break;
	}
	return bDWP? DefWindowProc(hwnd, nMessage, wParam, lParam): lResult;
}

#elif defined(X88_GUI_GTK)

// expose event

gboolean CX88PrinterPreviewWnd::OnExposeEvent(
	GtkWidget* pWidget,
	GdkEventExpose* /*pEvent*/,
	gpointer /*pData*/)
{
	if (GTK_WIDGET_DRAWABLE(pWidget) &&
		(m_ppmpCurPage != NULL))
	{
		GdkGC* pGC = gdk_gc_new(pWidget->window);
		GdkColor colWhite, colBlack;
		colWhite.red = colWhite.green = colWhite.blue = 0xFFFF;
		colBlack.red = colBlack.green = colBlack.blue = 0x0000;
		gdk_colormap_alloc_color(
			gdk_colormap_get_system(), &colWhite, FALSE, TRUE);
		gdk_colormap_alloc_color(
			gdk_colormap_get_system(), &colBlack, FALSE, TRUE);
		gdk_gc_set_foreground(pGC, &colBlack);
		gdk_draw_rectangle(
			pWidget->window, pGC,
			FALSE,
			0, 0,
			m_cxPreview+1, m_cyPreview+1);
		gdk_gc_set_foreground(pGC, &colWhite);
		gdk_draw_rectangle(
			pWidget->window, pGC,
			TRUE,
			1, 1,
			m_cxPreview, m_cyPreview);
		gdk_draw_drawable(
			pWidget->window, pGC,
			m_ppmpCurPage,
			0, 0,
			1, 1,
			min_value(m_cxPreview, m_nCurPageWidth),
			min_value(m_cyPreview, m_nCurPageHeight));
		if (GTK_WIDGET_HAS_FOCUS(pWidget)) {
			gdk_gc_set_line_attributes(
				pGC,
				1,
				GDK_LINE_ON_OFF_DASH,
				GDK_CAP_ROUND,
				GDK_JOIN_ROUND);
			gint8 anDashes[] = {
				2, 2
			};
			gdk_gc_set_dashes(
				pGC,
				0, anDashes, 1);
			gdk_gc_set_foreground(pGC, &colBlack);
			gdk_draw_rectangle(
				pWidget->window, pGC,
				FALSE,
				2, 2,
				m_cxPreview-3, m_cyPreview-3);
		}
		g_object_unref(pGC);
	}
	return FALSE;
}

// button-press event

gboolean CX88PrinterPreviewWnd::OnButtonPressEvent(
	GtkWidget* /*pWidget*/, GdkEventButton* pEvent, gpointer* /*pData*/)
{
	switch (pEvent->button) {
	case 1:
	case 3:
		if (((pEvent->button == 1) && (m_nZoom < ZOOM_MAX)) ||
			((pEvent->button == 3) && (m_nZoom > 0)))
		{
			int xCenter = (int)pEvent->x-1,
				yCenter = (int)pEvent->y-1;
			Preview2Print(xCenter, yCenter);
			m_nZoom += (pEvent->button == 1)? 1: -1;
			AdjustPreviewSize(xCenter, yCenter);
			RebuildPage();
		}
		break;
	}
	return TRUE;
}

// key-press event

gboolean CX88PrinterPreviewWnd::OnKeyPressEvent(
	GtkWidget* /*pWidget*/, GdkEventKey* pEvent, gpointer* /*pData*/)
{
	gboolean bResult = TRUE;
	switch (pEvent->keyval) {
	case GDK_Next:
	case GDK_Prior:
		if (((pEvent->keyval == GDK_Next) && (m_nZoom < ZOOM_MAX)) ||
			((pEvent->keyval == GDK_Prior) && (m_nZoom > 0)))
		{
			int xCenter = 0, yCenter = 0;
			if (m_pScrolledWindow != NULL) {
				GtkAdjustment* pAdjustmentH =
					gtk_scrolled_window_get_hadjustment(m_pScrolledWindow);
				if (pAdjustmentH != NULL) {
					if (pAdjustmentH->page_size > m_cxPreview+2) {
						xCenter = m_cxPreview/2;
					} else {
						xCenter += (int)(
							pAdjustmentH->value+pAdjustmentH->page_size/2);
					}
				}
				GtkAdjustment* pAdjustmentV =
					gtk_scrolled_window_get_vadjustment(m_pScrolledWindow);
				if (pAdjustmentV != NULL) {
					if (pAdjustmentV->page_size > m_cyPreview+2) {
						yCenter = m_cyPreview/2;
					} else {
						yCenter += (int)(
							pAdjustmentV->value+pAdjustmentV->page_size/2);
					}
				}
			}
			Preview2Print(xCenter, yCenter);
			m_nZoom += (pEvent->keyval == GDK_Next)? 1: -1;
			AdjustPreviewSize(xCenter, yCenter);
			RebuildPage();
		}
		break;
	case GDK_Up:
	case GDK_Down:
	case GDK_Left:
	case GDK_Right:
		if (m_pScrolledWindow != NULL) {
			GtkAdjustment* pAdjustment = NULL;
			switch (pEvent->keyval) {
			case GDK_Up:
			case GDK_Down:
				pAdjustment = gtk_scrolled_window_get_vadjustment(
					m_pScrolledWindow);
				break;
			case GDK_Left:
			case GDK_Right:
				pAdjustment = gtk_scrolled_window_get_hadjustment(
					m_pScrolledWindow);
				break;
			}
			if (pAdjustment != NULL) {
				double fValue = pAdjustment->value;
				switch (pEvent->keyval) {
				case GDK_Up:
				case GDK_Left:
					if ((pEvent->state & GDK_CONTROL_MASK) == 0) {
						fValue -= pAdjustment->step_increment;
					} else {
						fValue = pAdjustment->lower;
					}
					break;
				case GDK_Down:
				case GDK_Right:
					if ((pEvent->state & GDK_CONTROL_MASK) == 0) {
						fValue += pAdjustment->step_increment;
					} else {
						fValue = pAdjustment->upper-pAdjustment->page_size;
					}
					break;
				}
				if (fValue < pAdjustment->lower) {
					fValue = pAdjustment->lower;
				} else if (
					fValue > pAdjustment->upper-pAdjustment->page_size)
				{
					fValue = pAdjustment->upper-pAdjustment->page_size;
				}
				gtk_adjustment_set_value(
					pAdjustment,
					fValue);
			}
		}
		break;
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

// focus-in event

gboolean CX88PrinterPreviewWnd::OnFocusInEvent(
	GtkWidget* pWidget, GdkEventFocus* /*pEvent*/, gpointer* /*pData*/)
{
	GTK_WIDGET_SET_FLAGS(pWidget, GTK_HAS_FOCUS);
	OnExposeEvent(pWidget, NULL, NULL);
	return TRUE;
}

// focus-out event

gboolean CX88PrinterPreviewWnd::OnFocusOutEvent(
	GtkWidget* pWidget, GdkEventFocus* /*pEvent*/, gpointer* /*pData*/)
{
	GTK_WIDGET_UNSET_FLAGS(pWidget, GTK_HAS_FOCUS);
	OnExposeEvent(pWidget, NULL, NULL);
	return TRUE;
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// convert from preview-point to print-point

void CX88PrinterPreviewWnd::Preview2Print(int& xConv, int& yConv) {
	int nPaperWidth = m_pTargetPrinter->GetPaperWidth(),
		nPaperHeight = m_pTargetPrinter->GetPaperHeight();
	if (xConv < 0) {
		xConv = 0;
	} else if (xConv >= m_cxPreview) {
		xConv = nPaperWidth;
	} else {
		xConv = (xConv*nPaperWidth+m_cxPreview/2)/
			m_cxPreview;
	}
	if (yConv < 0) {
		yConv = 0;
	} else if (yConv >= m_cyPreview) {
		yConv = nPaperHeight;
	} else {
		yConv = (yConv*nPaperHeight+m_cyPreview/2)/
			m_cyPreview;
	}
}

// convert from -printpoint to preview-point

void CX88PrinterPreviewWnd::Print2Preview(int& xConv, int& yConv) {
	int nPaperWidth = m_pTargetPrinter->GetPaperWidth(),
		nPaperHeight = m_pTargetPrinter->GetPaperHeight();
	if (xConv < 0) {
		xConv = 0;
	} else if (xConv >= nPaperWidth) {
		xConv = m_cxPreview;
	} else {
		xConv = (xConv*m_cxPreview+nPaperWidth/2)/
			nPaperWidth;
	}
	if (yConv < 0) {
		yConv = 0;
	} else if (yConv >= nPaperHeight) {
		yConv = m_cyPreview;
	} else {
		yConv = (yConv*m_cyPreview+nPaperHeight/2)/
			nPaperHeight;
	}
}

// adjust preview size

void CX88PrinterPreviewWnd::AdjustPreviewSize(int xCenter, int yCenter) {

#ifdef X88_GUI_WINDOWS

	int cxBorder = GetSystemMetrics(SM_CXBORDER),
		cyBorder = GetSystemMetrics(SM_CYBORDER),
		cxVScroll = GetSystemMetrics(SM_CXVSCROLL),
		cyHScroll = GetSystemMetrics(SM_CYHSCROLL),
		cxMax = m_nLimitRight-m_nLimitLeft-cxBorder*2,
		cyMax = m_nLimitBottom-m_nLimitTop-cyBorder*2,
		nPaperWidth = m_pTargetPrinter->GetPaperWidth(),
		nPaperHeight = m_pTargetPrinter->GetPaperHeight();
	int cxPreview = (m_cxDefault-cxBorder*2) << m_nZoom,
		cyPreview = (m_cyDefault-cxBorder*2) << m_nZoom;
	m_cxPreview = (cyPreview*nPaperWidth+nPaperHeight/2)/nPaperHeight;
	m_cyPreview = (cxPreview*nPaperHeight+nPaperWidth/2)/nPaperWidth;
	if (m_cxPreview > cxPreview) {
		m_cxPreview = cxPreview;
	} else {
		m_cyPreview = cyPreview;
	}
	if (m_cxPreview < 1) {
		m_cxPreview = 1;
	}
	if (m_cyPreview < 1) {
		m_cyPreview = 1;
	}
	int cxClient = m_cxPreview,
		cyClient = m_cyPreview;
	bool bHScroll = false, bVScroll = false;
	if (cxClient > cxMax) {
		bHScroll = true;
		cyMax -= cyHScroll;
		cxClient = cxMax;
	}
	if (cyClient > cyMax) {
		bVScroll = true;
		cxMax -= cxVScroll;
		if (cxClient > cxMax) {
			if (!bHScroll) {
				bHScroll = true;
				cyMax -= cyHScroll;
			}
			cxClient = cxMax;
		}
		cyClient = cyMax;
	}
	SetWindowPos(
		GetWndHandle(), NULL,
		m_nLimitLeft+(cxMax-cxClient)/2,
		m_nLimitTop+(cyMax-cyClient)/2,
		cxClient+cxBorder*2+(bVScroll? cxVScroll: 0),
		cyClient+cyBorder*2+(bHScroll? cyHScroll: 0),
		SWP_NOZORDER | SWP_NOACTIVATE);
	Print2Preview(xCenter, yCenter);
	SCROLLINFO si;
	memset(&si, 0, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	si.nMax = m_cxPreview-1;
	si.nPage = cxClient;
	si.nPos = xCenter-cxClient/2;
	SetScrollInfo(GetWndHandle(), SB_HORZ, &si, TRUE);
	si.nMax = m_cyPreview-1;
	si.nPage = cyClient;
	si.nPos = yCenter-cyClient/2;
	SetScrollInfo(GetWndHandle(), SB_VERT, &si, TRUE);
	InvalidateRect(GetWndHandle(), NULL, TRUE);

#elif defined(X88_GUI_GTK)

	int nDrawDPI = BASE_DPI << m_nZoom,
		nPrinterDPI = m_pTargetPrinter->GetDPI(),
		nPaperWidth = m_pTargetPrinter->GetPaperWidth(),
		nPaperHeight = m_pTargetPrinter->GetPaperHeight();
	m_cxPreview = (nPaperWidth*nDrawDPI+nPrinterDPI-1)/nPrinterDPI;
	m_cyPreview = (nPaperHeight*nDrawDPI+nPrinterDPI-1)/nPrinterDPI;
	if (m_cxPreview < 1) {
		m_cxPreview = 1;
	}
	if (m_cyPreview < 1) {
		m_cyPreview = 1;
	}
	if (GetWndHandle() != NULL) {
		gtk_widget_set_size_request(
			GetWndHandle(),
			m_cxPreview+2,
			m_cyPreview+2);
		gtk_widget_queue_resize(GetWndHandle());
	}
	if (m_pScrolledWindow != NULL) {
		GtkWidget* pParent = gtk_widget_get_parent(GetWndHandle());
		if (GTK_IS_CONTAINER(pParent)) {
			gtk_container_check_resize(
				GTK_CONTAINER(pParent));
			gtk_widget_queue_resize(
				pParent);
		}
		gtk_container_check_resize(GTK_CONTAINER(m_pScrolledWindow));
		GtkAdjustment* pAdjustmentH =
			gtk_scrolled_window_get_hadjustment(m_pScrolledWindow);
		if (pAdjustmentH != NULL) {
			double fValue =
				(xCenter*m_cxPreview)/nPaperWidth-
					pAdjustmentH->page_size/2;
			if (fValue < pAdjustmentH->lower) {
				fValue = pAdjustmentH->lower;
			} else if (
				fValue > pAdjustmentH->upper-pAdjustmentH->page_size)
			{
				fValue = pAdjustmentH->upper-pAdjustmentH->page_size;
			}
			gtk_adjustment_set_value(
				pAdjustmentH,
				fValue);
		}
		GtkAdjustment* pAdjustmentV =
			gtk_scrolled_window_get_vadjustment(m_pScrolledWindow);
		if (pAdjustmentV != NULL) {
			double fValue =
				(yCenter*m_cyPreview)/nPaperHeight-
					pAdjustmentV->page_size/2;
			if (fValue < pAdjustmentV->lower) {
				fValue = pAdjustmentV->lower;
			} else if (
				fValue > pAdjustmentV->upper-pAdjustmentV->page_size)
			{
				fValue = pAdjustmentV->upper-pAdjustmentV->page_size;
			}
			gtk_adjustment_set_value(
				pAdjustmentV,
				fValue);
		}
	}

#endif // X88_GUI

}

#ifdef X88_GUI_GTK

// create current page pixmap

GdkPixmap* CX88PrinterPreviewWnd::CreateCurPagePixmap(
	int& nWidth, int& nHeight)
{

#ifdef X88_PRINTER_DRAW_POSTSCRIPT

	char fszPSFileName[_MAX_PATH], fszPBMFileName[_MAX_PATH];
	fszPSFileName[0] = fszPBMFileName[0] = '\0';
	GtkWidget* pWindow = NULL;
	GdkPixmap* ppmpCurPage = NULL;
	char* pchData = NULL;
	FILE* pFile = NULL;
	char szTmp[_MAX_PATH*4];
	bool bResult = false;
	nWidth = nHeight = 0;
	try {

#ifdef X88_PLATFORM_WINDOWS

		strcpy(fszPSFileName, "x88psXXXXXX");
		if (_mktemp(fszPSFileName) == NULL) {
			fszPSFileName[0] = '\0';
			throw 1;
		}
		strcpy(fszPBMFileName, "x88pbmXXXXXX");
		if (_mktemp(fszPBMFileName) == NULL) {
			fszPBMFileName[0] = '\0';
			throw 1;
		}

#elif defined(X88_PLATFORM_UNIX)

		const char* pszTmpDir = getenv("TMPDIR");

#ifdef P_tmpdir

		if (pszTmpDir == NULL) {
			pszTmpDir = P_tmpdir;
		}

#endif // P_tmpdir

		if (pszTmpDir == NULL) {
			pszTmpDir = "/tmp";
		}
		int nTmpHandle;
		sprintf(fszPSFileName, "%s/x88psXXXXXX", pszTmpDir);
		nTmpHandle = mkstemp(fszPSFileName);
		if (nTmpHandle == -1) {
			fszPSFileName[0] = fszPBMFileName[0] = '\0';
			throw 1;
		}
		close(nTmpHandle);
		sprintf(fszPBMFileName, "%s/x88pbmXXXXXX", pszTmpDir);
		nTmpHandle = mkstemp(fszPBMFileName);
		if (nTmpHandle == -1) {
			fszPBMFileName[0] = '\0';
			throw 1;
		}
		close(nTmpHandle);

#endif // X88_PLATFORM

		FILE* pFile = fopen(fszPSFileName, "w");
		if (pFile == NULL) {
			throw 2;
		}
		// Create PostScript File
		CX88PrinterDrawer prtdrawer;
		if (!prtdrawer.InitializePostScript(
				pFile, m_pTargetPrinter, 0) ||
			!prtdrawer.DrawPrinterHeadToPostScript(
				pFile, m_pTargetPrinter, m_nTargetPage) ||
			!prtdrawer.DrawSprocketHoleToPostScript(
				pFile, m_pTargetPrinter, m_nTargetPage) ||
			!prtdrawer.DrawPrinterObjectToPostScript(
				pFile, m_pTargetPrinter, m_nTargetPage) ||
			!prtdrawer.FinalizePostScript(
				pFile, m_pTargetPrinter) ||
			(fflush(pFile) != 0))
		{
			throw 2;
		}
		fclose(pFile);
		pFile = NULL;
		// Call GhostScript
		int nDrawDPI = BASE_DPI << m_nZoom;

#ifdef X88_PLATFORM_WINDOWS

		sprintf(
			szTmp,
			"gswin32c.exe "
				"-sDEVICE=pbmraw -sOutputFile=\"%s\" -r%d -q "
				"-dNOPAUSE \"%s\" quit.ps",
			fszPBMFileName,
			nDrawDPI,
			fszPSFileName);
		STARTUPINFO si;
		memset(&si, 0, sizeof(si));
		si.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(pi));
		bResult = (CreateProcess(
			NULL,
			szTmp,
			NULL, NULL, FALSE,
			CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS |
				DETACHED_PROCESS,
			NULL, NULL,
			&si, &pi) != FALSE);
		if (bResult) {
			CloseHandle(pi.hThread);
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
		}

#elif defined(X88_PLATFORM_UNIX)

		sprintf(
			szTmp,
			"ghostscript "
				"-sDEVICE=pbmraw -sOutputFile=\"%s\" -r%d -q "
				"-dNOPAUSE \"%s\" quit.ps",
			fszPBMFileName,
			nDrawDPI,
			fszPSFileName);
		int nSystemResult = system(szTmp);
		bResult = (nSystemResult != 127) &&
			(nSystemResult != -1);

#endif // X88_PLATFORM

		if (!bResult) {
			throw 3;
		}
		// Read PBM-Raw File
		pFile = fopen(fszPBMFileName, "rb");
		if (pFile == NULL) {
			throw 4;
		}
		nWidth = nHeight = 0;
		int nStep = 0;
		char szTmp2[256];
		szTmp2[255] = '\0';
		while (fgets(szTmp, sizeof(szTmp), pFile) != NULL) {
			if (szTmp[0] == '#') {
				continue;
			}
			char* p = szTmp;
			int nRead, nSkip;
			while ((p != NULL) && (*p != '\0') && (*p != '\n')) {
				switch (nStep) {
				case 0:
					nRead = sscanf(p, "%255s%n", szTmp2, &nSkip);
					if (nRead != 1) {
						p = NULL;
					} else if (strcmp(szTmp2, "P4") != 0) {
						throw 4;
					}
					p += nSkip;
					nStep++;
					break;
				case 1:
					nRead = sscanf(p, "%d%n", &nWidth, &nSkip);
					if (nRead != 1) {
						p = NULL;
					} else if (nWidth <= 0) {
						throw 4;
					}
					p += nSkip;
					nStep++;
					break;
				case 2:
					nRead = sscanf(p, "%d%n", &nHeight, &nSkip);
					if (nRead != 1) {
						p = NULL;
					} else if (nHeight <= 0) {
						throw 4;
					}
					p = NULL;
					nStep++;
					break;
				}
			}
			if (nStep == 3) {
				int nTotalLength = ((nWidth+7)/8)*nHeight;
				pchData = new char[nTotalLength];
				if (pchData == NULL) {
					throw 4;
				}
				if (fread(pchData, 1, nTotalLength, pFile) !=
					(size_t)nTotalLength)
				{
					throw 4;
				}
				char achTrans[256];
				int i;
				for (i = 0; i < 256; i++) {
					achTrans[i] = (char)(~(
						((i & 0x01) << 7) |
						((i & 0x02) << 5) |
						((i & 0x04) << 3) |
						((i & 0x08) << 1) |
						((i & 0x10) >> 1) |
						((i & 0x20) >> 3) |
						((i & 0x40) >> 5) |
						((i & 0x80) >> 7)));
				}
				for (i = 0; i < nTotalLength; i++) {
					pchData[i] = achTrans[(uint8_t)pchData[i]];
				}
				break;
			}
		}
		fclose(pFile);
		pFile = NULL;
		// Create Pixmap
		pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		if (pWindow == NULL) {
			throw 5;
		}
		gtk_widget_realize(pWindow);
		GdkColor colWhite, colBlack;
		colWhite.red = colWhite.green = colWhite.blue = 0xFFFF;
		colBlack.red = colBlack.green = colBlack.blue = 0x0000;
		gdk_colormap_alloc_color(
			gdk_colormap_get_system(), &colWhite, FALSE, TRUE);
		gdk_colormap_alloc_color(
			gdk_colormap_get_system(), &colBlack, FALSE, TRUE);
		ppmpCurPage = gdk_pixmap_create_from_data(
			pWindow->window,
			(const gchar*)pchData,
			nWidth, nHeight, -1,
			&colWhite, &colBlack);
		if (ppmpCurPage == NULL) {
			throw 5;
		}
		// Complete
	} catch (int /*nError*/) {
		nWidth = nHeight = 0;
	}
	if (pFile != NULL) {
		fclose(pFile);
	}
	if (pchData != NULL) {
		delete [] pchData;
	}
	if (pWindow != NULL) {
		gtk_widget_destroy(pWindow);
	}
	if (fszPSFileName[0] != '\0') {
		unlink(fszPSFileName);
	}
	if (fszPBMFileName[0] != '\0') {
		unlink(fszPBMFileName);
	}
	return ppmpCurPage;

#elif defined(X88_PRINTER_DRAW_CAIRO)

	GtkWidget* pWindow = NULL;
	GdkPixmap* ppmpCurPage = NULL;
	cairo_t* pCairo = NULL;
	nWidth = nHeight = 0;
	try {
		// Initialize
		int nDrawDPI = BASE_DPI << m_nZoom,
			nPrinterDPI = m_pTargetPrinter->GetDPI(),
			nPaperWidth = m_pTargetPrinter->GetPaperWidth(),
			nPaperHeight = m_pTargetPrinter->GetPaperHeight();
		nWidth = (nPaperWidth*nDrawDPI+nPrinterDPI-1)/nPrinterDPI;
		nHeight = (nPaperHeight*nDrawDPI+nPrinterDPI-1)/nPrinterDPI;
		// Create Pixmap
		pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		if (pWindow == NULL) {
			throw 1;
		}
		gtk_widget_realize(pWindow);
		ppmpCurPage = gdk_pixmap_new(
			pWindow->window,
			nWidth, nHeight, -1);
		if (ppmpCurPage == NULL) {
			throw 1;
		}
		// Cairo Setup
		pCairo = gdk_cairo_create(ppmpCurPage);
		cairo_set_source_rgb(pCairo, 1, 1, 1);
		cairo_paint(pCairo);
		cairo_scale(
			pCairo,
			(double)nWidth/nPaperWidth, (double)nHeight/nPaperHeight);
		// Draw
		CX88PrinterDrawer prtdrawer;
		cairo_set_source_rgb(pCairo,1, 0.75, 0.75);
		prtdrawer.DrawPrinterHeadToCairo(
			pCairo,
			m_pTargetPrinter, m_nTargetPage);
		cairo_set_source_rgb(pCairo, 0, 0, 0);
		prtdrawer.DrawSprocketHoleToCairo(
			pCairo,
			m_pTargetPrinter, m_nTargetPage);
		prtdrawer.DrawPrinterObjectToCairo(
			pCairo,
			m_pTargetPrinter, m_nTargetPage);
		// Complete
	} catch (int /*nError*/) {
		nWidth = nHeight = 0;
	}
	if (pCairo != NULL) {
		cairo_destroy(pCairo);
	}
	if (pWindow != NULL) {
		gtk_widget_destroy(pWindow);
	}
	return ppmpCurPage;

#endif // X88_PRINTER_DRAW

}

#endif // X88_GUI

// change paper

void CX88PrinterPreviewWnd::ChangePaper() {
	m_nZoom = 0;
	if (GetWndHandle() != NULL) {
		int xCenter = 0,
			yCenter = 0;
		AdjustPreviewSize(xCenter, yCenter);
	}
}

// set target page

void CX88PrinterPreviewWnd::SetTargetPage(int nTargetPage) {
	m_nTargetPage = nTargetPage;
}

// rebuild page

void CX88PrinterPreviewWnd::RebuildPage() {
	if (GetWndHandle() != NULL) {

#ifdef X88_GUI_WINDOWS

		InvalidateRect(GetWndHandle(), NULL, TRUE);

#elif defined(X88_GUI_GTK)

		if (m_ppmpCurPage != NULL) {
			g_object_unref(m_ppmpCurPage);
		}
		m_ppmpCurPage = CreateCurPagePixmap(
			m_nCurPageWidth, m_nCurPageHeight);
		if (GetWndHandle() != NULL) {
			gtk_widget_queue_draw(GetWndHandle());
		}

#endif // X88_GUI

	}
}

// resize

void CX88PrinterPreviewWnd::Resize(int nLimitRightNew, int nLimitBottomNew) {

#ifdef X88_GUI_WINDOWS

	if ((GetWndHandle() != NULL) &&
		((nLimitRightNew != m_nLimitRight) ||
			(nLimitBottomNew != m_nLimitBottom)))
	{
		int xCenter = 0,
			yCenter = 0;
		SCROLLINFO si;
		memset(&si, 0, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS | SIF_PAGE;
		GetScrollInfo(GetWndHandle(), SB_HORZ, &si);
		xCenter = si.nPos+si.nPage/2;
		GetScrollInfo(GetWndHandle(), SB_VERT, &si);
		yCenter = si.nPos+si.nPage/2;
		Preview2Print(xCenter, yCenter);
		m_nLimitRight = max_value(nLimitRightNew, m_nLimitLeft+m_cxDefault/4);
		m_nLimitBottom = max_value(nLimitBottomNew, m_nLimitTop+m_cyDefault/4);
		AdjustPreviewSize(xCenter, yCenter);
	}

#elif defined(X88_GUI_GTK)

	(void)nLimitRightNew;
	(void)nLimitBottomNew;

#endif // X88_GUI

}
