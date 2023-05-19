////////////////////////////////////////////////////////////
// X88000 Debug Window
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88DebugWnd.h"

#include "X88Dialog.h"
#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88DebugWnd

////////////////////////////////////////////////////////////
// attribute

// X88000 object

CX88000* CX88DebugWnd::m_pX88k;

// window handle

CX88WndHandle CX88DebugWnd::m_hWnd;

// step execution button handle

CX88WndHandle CX88DebugWnd::m_hbtnStep;

// trace execution button handle

CX88WndHandle CX88DebugWnd::m_hbtnTrace;

// step execution2 button handle

CX88WndHandle CX88DebugWnd::m_hbtnStep2;

#ifdef X88_GUI_WINDOWS

// debug display font

HFONT CX88DebugWnd::m_hfontDebug;

// timer id

UINT_PTR CX88DebugWnd::m_nTimerID;

#elif defined(X88_GUI_GTK)

// horizontal packing-box

CX88WndHandle CX88DebugWnd::m_hwndHBox;

// mnemonic label

CX88WndHandle CX88DebugWnd::m_hstcMnemonic;
// register label

CX88WndHandle CX88DebugWnd::m_hstcRegister;

// timeout id

gint CX88DebugWnd::m_nTimeOutID;

#endif // X88_GUI

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88DebugWnd::CX88DebugWnd() {
	m_pX88k = NULL;
	m_hWnd = NULL;
	m_hbtnStep = m_hbtnTrace = m_hbtnStep2 = NULL;

#ifdef X88_GUI_WINDOWS

	m_hfontDebug = NULL;
	m_nTimerID = 0;

#elif defined(X88_GUI_GTK)

	m_hwndHBox = NULL;
	m_hstcMnemonic = NULL;
	m_hstcRegister = NULL;
	m_nTimeOutID = 0;

#endif // X88_GUI

}

// destructor

CX88DebugWnd::~CX88DebugWnd() {
	if (GetWndHandle() != NULL) {
		Destroy();
	}
}

// create

bool CX88DebugWnd::Create(CX88WndHandle hwndFrame, CX88000& x88k) {
	if (GetWndHandle() != NULL) {
		return false;
	}
	m_pX88k = &x88k;

#ifdef X88_GUI_WINDOWS

	const char* pszDebugWndName = "X88Debug";
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = X88k().GetInstanceHandle();
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = pszDebugWndName;
	RegisterClass(&wc);
	CreateWindow(
		pszDebugWndName, pszDebugWndName,
		WS_CHILD | WS_VISIBLE,
		0, 400, CXWINDOWDEFAULT, CYWINDOWDEFAULT,
		hwndFrame, (HMENU)CTRL_ID, X88k().GetInstanceHandle(),
		NULL);

#elif defined(X88_GUI_GTK)

	(void)hwndFrame;

	m_hWnd = gtk_vbox_new(FALSE, 0);
	if (m_hWnd != NULL) {
		g_signal_connect(
			GetWndHandle(), "destroy",
			G_CALLBACK(OnDestroySignal), NULL);

		m_hwndHBox = gtk_hbox_new(FALSE, 0);
		std::string strFontFamilyName = "Monospace";
		int nFontSize = -(CYDEBUGFONT-1);

		m_hstcMnemonic = gtk_label_new(NULL);
		CX88Dialog::SetControlFont(
			m_hstcMnemonic,
			strFontFamilyName, nFontSize);
		UpdateMnemonic(m_hstcMnemonic);
		gtk_widget_show(m_hstcMnemonic);
		gtk_misc_set_alignment(GTK_MISC(m_hstcMnemonic), 0, 0.5);
		gtk_widget_set_size_request(m_hstcMnemonic, 0, -1);
		gtk_box_pack_start(
			GTK_BOX(m_hwndHBox), m_hstcMnemonic, TRUE, TRUE, 0);

		m_hbtnStep2 = gtk_button_new_with_label("Step2");
		CX88Dialog::SetControlFont(
			gtk_bin_get_child(GTK_BIN(m_hbtnStep2)),
			strFontFamilyName, nFontSize);
		gtk_widget_show(m_hbtnStep2);
		gtk_box_pack_end(
			GTK_BOX(m_hwndHBox), m_hbtnStep2, FALSE, FALSE, 0);

		m_hbtnTrace = gtk_button_new_with_label("Trace");
		CX88Dialog::SetControlFont(
			gtk_bin_get_child(GTK_BIN(m_hbtnTrace)),
			strFontFamilyName, nFontSize);
		gtk_widget_show(m_hbtnTrace);
		gtk_box_pack_end(
			GTK_BOX(m_hwndHBox), m_hbtnTrace, FALSE, FALSE, 0);

		m_hbtnStep = gtk_button_new_with_label("Step");
		CX88Dialog::SetControlFont(
			gtk_bin_get_child(GTK_BIN(m_hbtnStep)),
			strFontFamilyName, nFontSize);
		gtk_widget_show(m_hbtnStep);
		gtk_box_pack_end(
			GTK_BOX(m_hwndHBox), m_hbtnStep, FALSE, FALSE, 0);

		gtk_widget_show(m_hwndHBox);
		gtk_box_pack_start(
			GTK_BOX(GetWndHandle()), m_hwndHBox, FALSE, FALSE, 0);

		m_hstcRegister = gtk_label_new(NULL);
		CX88Dialog::SetControlFont(
			m_hstcRegister,
			strFontFamilyName, nFontSize);
		UpdateRegister(m_hstcRegister);
		gtk_misc_set_alignment(GTK_MISC(m_hstcRegister), 0, 0.5);
		gtk_widget_set_size_request(m_hstcRegister, 0, -1);
		gtk_widget_show(m_hstcRegister);
		gtk_box_pack_start(
			GTK_BOX(GetWndHandle()), m_hstcRegister, FALSE, FALSE, 0);

		m_nTimeOutID = g_timeout_add(
			TIMER_ELAPSE,
			TimeOutCallback, NULL);
	}

#endif // X88_GUI

	return GetWndHandle() != NULL;
}

// destroy

bool CX88DebugWnd::Destroy() {
	if (GetWndHandle() == NULL) {
		return false;
	}

#ifdef X88_GUI_WINDOWS

	DestroyWindow(GetWndHandle());

#elif defined(X88_GUI_GTK)

	g_object_unref(GetWndHandle());
	m_hWnd = NULL;

#endif // X88_GUI

	return GetWndHandle() == NULL;
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// window procedure

LRESULT CALLBACK CX88DebugWnd::WndProc(
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
	case WM_CREATE:
		if (m_hfontDebug == NULL) {
			LOGFONT lf;
			memset(&lf, 0x00, sizeof(LOGFONT));
			lf.lfHeight = CYDEBUGFONT;
			lf.lfWeight = FW_BOLD;
			lf.lfCharSet = DEFAULT_CHARSET;
			lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
			strcpy(lf.lfFaceName, RSTR("Courier New"));
			m_hfontDebug = CreateFontIndirect(&lf);
		}
		if (m_hbtnStep == NULL) {
			m_hbtnStep = CreateWindow(
				"button", "Step",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				CXWINDOWDEFAULT-CXBUTTON*3-1, 1,
				CXBUTTON, CYBUTTON,
				hwnd, (HMENU)IDM_DEBUG_EXECUTE_STEP,
				X88k().GetInstanceHandle(), NULL);
		}
		if (m_hbtnTrace == NULL) {
			m_hbtnTrace = CreateWindow(
				"button", "Trace",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				CXWINDOWDEFAULT-CXBUTTON*2-1, 1,
				CXBUTTON, CYBUTTON,
				hwnd, (HMENU)IDM_DEBUG_EXECUTE_TRACE,
				X88k().GetInstanceHandle(), NULL);
		}
		if (m_hbtnStep2 == NULL) {
			m_hbtnStep2 = CreateWindow(
				"button", "Step2",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				CXWINDOWDEFAULT-CXBUTTON-1, 1,
				CXBUTTON, CYBUTTON,
				hwnd, (HMENU)IDM_DEBUG_EXECUTE_STEP2,
				X88k().GetInstanceHandle(), NULL);
		}
		if (m_hfontDebug != NULL) {
			SendMessage(
				m_hbtnStep,
				WM_SETFONT,
				(WPARAM)m_hfontDebug, MAKELPARAM(TRUE, 0));
			SendMessage(
				m_hbtnTrace,
				WM_SETFONT,
				(WPARAM)m_hfontDebug, MAKELPARAM(TRUE, 0));
			SendMessage(
				m_hbtnStep2,
				WM_SETFONT,
				(WPARAM)m_hfontDebug, MAKELPARAM(TRUE, 0));
		}
		m_nTimerID = SetTimer(hwnd, TIMER_ID, TIMER_ELAPSE, NULL);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, m_nTimerID);
		if (m_hbtnStep != NULL) {
			DestroyWindow(m_hbtnStep);
			m_hbtnStep = NULL;
		}
		if (m_hbtnTrace != NULL) {
			DestroyWindow(m_hbtnTrace);
			m_hbtnTrace = NULL;
		}
		if (m_hbtnStep2 != NULL) {
			DestroyWindow(m_hbtnStep2);
			m_hbtnStep2 = NULL;
		}
		if (m_hfontDebug != NULL) {
			DeleteObject(m_hfontDebug);
			m_hfontDebug = NULL;
		}
		break;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			int cx = LOWORD(lParam);
			if (m_hbtnStep != NULL) {
				MoveWindow(
					m_hbtnStep,
					cx-CXBUTTON*3-1, 1, CXBUTTON, CYBUTTON,
					TRUE);
			}
			if (m_hbtnTrace != NULL) {
				MoveWindow(
					m_hbtnTrace,
					cx-CXBUTTON*2-1, 1, CXBUTTON, CYBUTTON,
					TRUE);
			}
			if (m_hbtnStep2 != NULL) {
				MoveWindow(
					m_hbtnStep2,
					cx-CXBUTTON*1-1, 1, CXBUTTON, CYBUTTON,
					TRUE);
			}
		}
		break;
	case WM_COMMAND:
		PostMessage(GetParent(hwnd), WM_COMMAND, wParam, lParam);
		break;
	case WM_TIMER:
		{ // dummy block
			HWND hwndFrame = GetParent(hwnd),
				hwndFocus = GetFocus();
			if ((hwndFocus != hwndFrame) && (hwndFocus != NULL) &&
				(GetCapture() == NULL))
			{
				if ((hwndFocus == hwnd) ||
					(hwndFocus == m_hbtnStep) ||
					(hwndFocus == m_hbtnTrace) ||
					(hwndFocus == m_hbtnStep2))
				{
					SetFocus(hwndFrame);
				}
			}
			if (X88k().PC88().IsMnemonicUpdated()) {
				X88k().PC88().SetMnemonicUpdated(false);
				InvalidateRect(hwnd, NULL, FALSE);
			}
		}
		break;
	case WM_PAINT:
		{ // dummy block
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			DrawDebugInfo(hdc);
			EndPaint(hwnd, &ps);
		}
		break;
	default:
		bDWP = true;
		break;
	}
	return bDWP? DefWindowProc(hwnd, nMessage, wParam, lParam): lResult;
}

// draw debug info

void CX88DebugWnd::DrawDebugInfo(HDC hdc) {
	RECT rectDraw;
	GetClientRect(GetWndHandle(), &rectDraw);
	HPEN hpenOrg = (HPEN)SelectObject(
		hdc, GetStockObject(WHITE_PEN));
	MoveToEx(hdc, rectDraw.right-1, rectDraw.top, NULL);
	LineTo(hdc, rectDraw.left, rectDraw.top);
	LineTo(hdc, rectDraw.left, rectDraw.bottom-1);
	HPEN hpenGray = CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
	if (hpenGray == NULL) {
		SelectObject(hdc, hpenOrg);
		return;
	}
	SelectObject(hdc, hpenGray);
	LineTo(hdc, rectDraw.right-1, rectDraw.bottom-1);
	LineTo(hdc, rectDraw.right-1, rectDraw.top);
	SelectObject(hdc, hpenOrg);
	DeleteObject(hpenGray);
	InflateRect(&rectDraw, -1, -1);
	FillRect(hdc, &rectDraw, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
	HFONT hfontOrg = NULL;
	if (m_hfontDebug != NULL) {
		hfontOrg = (HFONT)SelectObject(hdc, m_hfontDebug);
	}
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	std::string strDebugInfo;
	CZ80Adapter* pZ80A = X88k().PC88().GetDebugAdapter();
	int xPos = tm.tmAveCharWidth, yPos = tm.tmHeight/4;
	if (X88k().PC88().IsDebugStopped()) {
		strDebugInfo.reserve(80);
		strDebugInfo = FormatStr(
			"0%04XH  ",
			pZ80A->RegPC().Get());
		int nTab = 8;
		char ch;
		for (
			const char* pszMnemonic = pZ80A->GetMnemonic();
			(ch = *pszMnemonic) != '\0';
			pszMnemonic++)
		{
			if (ch == '\t') {
				do {
					strDebugInfo += ' ';
				} while (--nTab > 0);
				nTab = 8;
			} else {
				strDebugInfo += ch;
				if (--nTab <= 0) {
					nTab = 8;
				}
			}
		}
		TextOut(
			hdc,
			xPos, yPos,
			strDebugInfo.c_str(), (int)strDebugInfo.size());
		yPos += tm.tmHeight+4;
		const char* pszText =
			"F :        A :    BC :      DE :      HL :      IX :      IY :";
		SetTextColor(hdc, RGB(0, 0, 128));
		TextOut(
			hdc,
			xPos, yPos,
			pszText, (int)strlen(pszText));
		strDebugInfo = FormatStr(
			"   %c%c%c%c%c%c     %02X      %04X      %04X      %04X  "
				"    %04X      %04X",
			pZ80A->TestRegF(CZ80Adapter::C_FLAG)? 'C': '-',
			pZ80A->TestRegF(CZ80Adapter::Z_FLAG)? 'Z': '-',
			pZ80A->TestRegF(CZ80Adapter::P_V_FLAG)? 'E': 'O',
			pZ80A->TestRegF(CZ80Adapter::S_FLAG)? 'M': 'P',
			pZ80A->TestRegF(CZ80Adapter::N_FLAG)? 'N': '-',
			pZ80A->TestRegF(CZ80Adapter::H_FLAG)? 'H': '-',
			pZ80A->RegA().Get(),
			pZ80A->RegBC().Get(),
			pZ80A->RegDE().Get(),
			pZ80A->RegHL().Get(),
			pZ80A->RegIX().Get(),
			pZ80A->RegIY().Get());
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(
			hdc,
			xPos, yPos,
			strDebugInfo.c_str(), (int)strDebugInfo.size());
		yPos += tm.tmHeight+2;
		pszText = "F':        A':    BC':      DE':      HL':      SP :";
		SetTextColor(hdc, RGB(0, 0, 128));
		TextOut(
			hdc,
			xPos, yPos,
			pszText, (int)strlen(pszText));
		strDebugInfo = FormatStr(
			"   %c%c%c%c%c%c     %02X      %04X      %04X      %04X  "
				"    %04X",
			pZ80A->TestRegF2(CZ80Adapter::C_FLAG)? 'C': '-',
			pZ80A->TestRegF2(CZ80Adapter::Z_FLAG)? 'Z': '-',
			pZ80A->TestRegF2(CZ80Adapter::P_V_FLAG)? 'E': 'O',
			pZ80A->TestRegF2(CZ80Adapter::S_FLAG)? 'M': 'P',
			pZ80A->TestRegF2(CZ80Adapter::N_FLAG)? 'N': '-',
			pZ80A->TestRegF2(CZ80Adapter::H_FLAG)? 'H': '-',
			pZ80A->RegAF2().GetHi(),
			pZ80A->RegBC2().Get(),
			pZ80A->RegDE2().Get(),
			pZ80A->RegHL2().Get(),
			pZ80A->RegSP().Get());
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(
			hdc,
			xPos, yPos,
			strDebugInfo.c_str(), (int)strDebugInfo.size());
		yPos += tm.tmHeight+2;
		pszText = "    I :    R :   (BC):     (DE):     (HL):     (SP):      <  >";
		SetTextColor(hdc, RGB(0, 0, 128));
		TextOut(
			hdc,
			xPos, yPos,
			pszText, (int)strlen(pszText));
		strDebugInfo = FormatStr(
			"       %02X     %02X      %04X      %04X      %04X      %04X   %cI",
			pZ80A->GetRegI(),
			pZ80A->GetRegR(),
			pZ80A->ReadMemoryW(pZ80A->RegBC().Get()),
			pZ80A->ReadMemoryW(pZ80A->RegDE().Get()),
			pZ80A->ReadMemoryW(pZ80A->RegHL().Get()),
			pZ80A->ReadMemoryW(pZ80A->RegSP().Get()),
			pZ80A->IsEnableInterrupt()? 'E': 'D');
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(
			hdc,
			xPos, yPos,
			strDebugInfo.c_str(), (int)strDebugInfo.size());
	} else {
		strDebugInfo = FormatStr(
			"0%04XH  Running...",
			pZ80A->RegPC().Get());
		TextOut(
			hdc,
			xPos, yPos,
			strDebugInfo.c_str(), (int)strDebugInfo.size());
	}
	if (hfontOrg != NULL) {
		SelectObject(hdc, hfontOrg);
	}
}

#elif defined(X88_GUI_GTK)

// destroy signal

void CX88DebugWnd::OnDestroySignal(
	GtkWidget* /*pWidget*/, gpointer /*pData*/)
{
	g_source_remove(m_nTimeOutID);
	m_nTimeOutID = 0;
	m_hbtnStep = NULL;
	m_hbtnTrace = NULL;
	m_hbtnStep2 = NULL;
	m_hwndHBox = NULL;
	m_hstcMnemonic = NULL;
	m_hstcRegister = NULL;
}

// update mnemonic label text

void CX88DebugWnd::UpdateMnemonic(
	GtkWidget* pWidget)
{
	std::string strDebugInfo;
	CZ80Adapter* pZ80A = X88k().PC88().GetDebugAdapter();
	if (X88k().PC88().IsDebugStopped()) {
		strDebugInfo.reserve(80);
		strDebugInfo = FormatStr(
			" 0%04XH  ",
			pZ80A->RegPC().Get());
		int nTab = 8;
		char ch;
		for (
			const char* pszMnemonic = pZ80A->GetMnemonic();
			(ch = *pszMnemonic) != '\0';
			pszMnemonic++)
		{
			if (ch == '\t') {
				do {
					strDebugInfo += ' ';
				} while (--nTab > 0);
				nTab = 8;
			} else {
				strDebugInfo += ch;
				if (--nTab <= 0) {
					nTab = 8;
				}
			}
		}
	} else {
		strDebugInfo = FormatStr(
			" 0%04XH  Running...",
			pZ80A->RegPC().Get());
	}
	gtk_label_set_markup(GTK_LABEL(pWidget), strDebugInfo.c_str());
}

// update register label text

void CX88DebugWnd::UpdateRegister(
	GtkWidget* pWidget)
{
	std::string strDebugInfo;
	CZ80Adapter* pZ80A = X88k().PC88().GetDebugAdapter();
	if (X88k().PC88().IsDebugStopped()) {
		strDebugInfo = FormatStr(
			"<span color='#000080'> F :</span>%c%c%c%c%c%c "
			"<span color='#000080'> A :</span>%02X "
			"<span color='#000080'> BC :</span>%04X "
			"<span color='#000080'> DE :</span>%04X "
			"<span color='#000080'> HL :</span>%04X "
			"<span color='#000080'> IX :</span>%04X "
			"<span color='#000080'> IY :</span>%04X \n",
			pZ80A->TestRegF(CZ80Adapter::C_FLAG)? 'C': '-',
			pZ80A->TestRegF(CZ80Adapter::Z_FLAG)? 'Z': '-',
			pZ80A->TestRegF(CZ80Adapter::P_V_FLAG)? 'E': 'O',
			pZ80A->TestRegF(CZ80Adapter::S_FLAG)? 'M': 'P',
			pZ80A->TestRegF(CZ80Adapter::N_FLAG)? 'N': '-',
			pZ80A->TestRegF(CZ80Adapter::H_FLAG)? 'H': '-',
			pZ80A->RegA().Get(),
			pZ80A->RegBC().Get(),
			pZ80A->RegDE().Get(),
			pZ80A->RegHL().Get(),
			pZ80A->RegIX().Get(),
			pZ80A->RegIY().Get());
		strDebugInfo += FormatStr(
			"<span color='#000080'> F':</span>%c%c%c%c%c%c "
			"<span color='#000080'> A':</span>%02X "
			"<span color='#000080'> BC':</span>%04X "
			"<span color='#000080'> DE':</span>%04X "
			"<span color='#000080'> HL':</span>%04X "
			"<span color='#000080'> SP :</span>%04X \n",
			pZ80A->TestRegF2(CZ80Adapter::C_FLAG)? 'C': '-',
			pZ80A->TestRegF2(CZ80Adapter::Z_FLAG)? 'Z': '-',
			pZ80A->TestRegF2(CZ80Adapter::P_V_FLAG)? 'E': 'O',
			pZ80A->TestRegF2(CZ80Adapter::S_FLAG)? 'M': 'P',
			pZ80A->TestRegF2(CZ80Adapter::N_FLAG)? 'N': '-',
			pZ80A->TestRegF2(CZ80Adapter::H_FLAG)? 'H': '-',
			pZ80A->RegAF2().GetHi(),
			pZ80A->RegBC2().Get(),
			pZ80A->RegDE2().Get(),
			pZ80A->RegHL2().Get(),
			pZ80A->RegSP().Get());
		strDebugInfo += FormatStr(
			"    <span color='#000080'>I :</span> %02X  "
			"<span color='#000080'>R :</span>%02X "
			"<span color='#000080'>(BC):</span>%04X "
			"<span color='#000080'>(DE):</span>%04X "
			"<span color='#000080'>(HL):</span>%04X "
			"<span color='#000080'>(SP):</span>%04X "
			" <span color='#000080'>&lt;</span>"
				"%cI"
				"<span color='#000080'>&gt;</span>",
			pZ80A->GetRegI(),
			pZ80A->GetRegR(),
			pZ80A->ReadMemoryW(pZ80A->RegBC().Get()),
			pZ80A->ReadMemoryW(pZ80A->RegDE().Get()),
			pZ80A->ReadMemoryW(pZ80A->RegHL().Get()),
			pZ80A->ReadMemoryW(pZ80A->RegSP().Get()),
			pZ80A->IsEnableInterrupt()? 'E': 'D');
	}
	gtk_label_set_markup(GTK_LABEL(pWidget), strDebugInfo.c_str());
}

// time out callback

gboolean CX88DebugWnd::TimeOutCallback(gpointer /*pData*/) {
	if (X88k().PC88().IsMnemonicUpdated()) {
		X88k().PC88().SetMnemonicUpdated(false);
		if (m_hstcMnemonic != NULL) {
			UpdateMnemonic(m_hstcMnemonic);
		}
		if (m_hstcRegister != NULL) {
			UpdateRegister(m_hstcRegister);
		}
	}
	return TRUE;
}

#endif // X88_GUI

// update debug window

void CX88DebugWnd::UpdateDebugWnd() {

#ifdef X88_GUI_WINDOWS

	if (GetWndHandle() != NULL) {
		InvalidateRect(GetWndHandle(), NULL, FALSE);
		bool bEnable = X88k().PC88().IsDebugStopped();
		HWND hwndFocus = GetFocus(),
			hwndFrame = GetParent(GetWndHandle());
		if (m_hbtnStep != NULL) {
			if (!bEnable && (hwndFocus == m_hbtnStep)) {
				SetFocus(hwndFrame);
			}
			CX88Dialog::EnableControl(m_hbtnStep, bEnable);
		}
		if (m_hbtnTrace != NULL) {
			if (!bEnable && (hwndFocus == m_hbtnTrace)) {
				SetFocus(hwndFrame);
			}
			CX88Dialog::EnableControl(m_hbtnTrace, bEnable);
		}
		if (m_hbtnStep2 != NULL) {
			if (!bEnable && (hwndFocus == m_hbtnStep2)) {
				SetFocus(hwndFrame);
			}
			CX88Dialog::EnableControl(m_hbtnStep2, bEnable);
		}
	}

#elif defined(X88_GUI_GTK)

	if (GetWndHandle() != NULL) {
		if (m_hstcMnemonic != NULL) {
			UpdateMnemonic(m_hstcMnemonic);
		}
		if (m_hstcRegister != NULL) {
			UpdateRegister(m_hstcRegister);
		}
		bool bEnable = X88k().PC88().IsDebugStopped();
		if (m_hbtnStep != NULL) {
			CX88Dialog::EnableControl(m_hbtnStep, bEnable);
		}
		if (m_hbtnTrace != NULL) {
			CX88Dialog::EnableControl(m_hbtnTrace, bEnable);
		}
		if (m_hbtnStep2 != NULL) {
			CX88Dialog::EnableControl(m_hbtnStep2, bEnable);
		}
	}

#endif // X88_GUI

}
