////////////////////////////////////////////////////////////
// X88000 Frame Window
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88Frame.h"

#include "X88Dialog.h"
#include "X88FileDlg.h"
#include "X88EnvSetDlg.h"
#include "X88DiskImageDlg.h"
#include "X88TapeImageDlg.h"
#include "X88DebugWriteRamDlg.h"
#include "X88DebugDisAssembleDlg.h"
#include "X88DebugDumpDlg.h"
#include "X88DebugBreakPointDlg.h"
#include "X88VersionDlg.h"
#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

#ifdef X88_PLATFORM_WINDOWS

////////////////////////////////////////////////////////////
// implementation of CX88FrameMessageProcessor

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88FrameMessageProcessor::CX88FrameMessageProcessor(
	CX88Frame* pFrame) :
		CX88MessageProcessor(),
		m_pFrame(pFrame)
{
}

// destructor

CX88FrameMessageProcessor::~CX88FrameMessageProcessor() {
}

////////////////////////////////////////////////////////////
// operation

// process message

BOOL CX88FrameMessageProcessor::ProcessMessage(MSG* pMessage) {
	// root, not call next processor
	return m_pFrame->PreTranslateMessage(pMessage);
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// implementation of CX88Frame

////////////////////////////////////////////////////////////
// attribute

// X88000 object

CX88000* CX88Frame::m_pX88k;

// window handle

CX88WndHandle CX88Frame::m_hWnd;

// view

CX88View CX88Frame::m_view;

// debug window

CX88DebugWnd CX88Frame::m_debugwnd;

// full-screen mode

bool CX88Frame::m_bFullScreen;

// activated GDI surface

bool CX88Frame::m_bActivatedGDI;

// mouse hovering on menu bar

bool CX88Frame::m_bMouseHoveringOnMenuBar;

// have focus

bool CX88Frame::m_bHaveFocus;

// active process

bool CX88Frame::m_bActiveProcess;

#ifdef X88_GUI_WINDOWS

// keyboard accelerator

HACCEL CX88Frame::m_haccel;

// IME convert window opened

bool CX88Frame::m_bIMEOpen;

// backup of IMM input context

HIMC CX88Frame::m_hIMCBackup;

// keyboard state table

uint8_t CX88Frame::m_abtKeyboardState[256];

// message processor

CX88MessageProcessor* CX88Frame::m_pMessageProcessor = NULL;

#elif defined(X88_GUI_GTK)

// vertical packing-box

CX88WndHandle CX88Frame::m_hwndVBox;

// menu box

CX88WndHandle CX88Frame::m_hwndMenuBox;

// menu bar

CX88WndHandle CX88Frame::m_hwndMenuBar;

// keyboard accelerator group

GtkAccelGroup* CX88Frame::m_pAccelGroup;

// menu showing

bool CX88Frame::m_bMenuShowing;

// keyboard state table

uint8_t CX88Frame::m_abtKeyboardState[256*2];

// modifier key state

guint CX88Frame::m_nModKeyState;

// timeout id

gint CX88Frame::m_nTimeOutID;

#ifdef X88_GUI_GTK_USEKEYMAP

// key code table initialized

bool CX88Frame::m_bInitializedKeyCodes;

// key code table

int CX88Frame::m_anKeyCodes[256];

// keyboard state updated

bool CX88Frame::m_bDirtyKeyboardState;

#endif // X88_GUI_GTK_USEKEYMAP

#ifdef X88_GUI_GTK_NOKEYRELEASE

// key code of pressed last

int CX88Frame::m_nLastPressKeyCode;

// key released counter

int CX88Frame::m_nKeyReleaseCounter;

#endif // X88_GUI_GTK_NOKEYRELEASE

#ifdef X88_GUI_GTK_USEIM

// input method context

GtkIMContext* CX88Frame::m_pIMC;

// input method pre-editing

bool CX88Frame::m_bPreEditing;

// input method pre-edit window

CX88WndHandle CX88Frame::m_hwndPreEditWindow;

// input method pre-edit drawing-area

CX88WndHandle CX88Frame::m_hwndPreEditDArea;

// input method pre-edit string(GUI encoding)

gchar* CX88Frame::m_pgszIMPreEdit;

// input method pre-edit attribute

PangoAttrList* CX88Frame::m_pattrIMPreEdit;

// input method pre-edit cursor position

gint CX88Frame::m_nIMPreEditCursorPos;

#endif // X88_GUI_GTK_USEIM

#endif // X88_GUI

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88Frame::CX88Frame() {
	m_pX88k = NULL;
	m_hWnd = NULL;
	m_bFullScreen = false;
	m_bHaveFocus = false;
	m_bActiveProcess = true;

#ifdef X88_GUI_WINDOWS

	m_haccel = NULL;
	m_bIMEOpen = false;
	m_hIMCBackup = NULL;
	m_pMessageProcessor = new CX88FrameMessageProcessor(this);

#elif defined(X88_GUI_GTK)

	m_hwndVBox = NULL;
	m_hwndMenuBox = NULL;
	m_hwndMenuBar = NULL;
	m_pAccelGroup = NULL;

#ifdef X88_GUI_GTK_USEIM

	m_pIMC = NULL;
	m_bPreEditing = false;
	m_hwndPreEditWindow = NULL;
	m_hwndPreEditDArea = NULL;
	m_pgszIMPreEdit = NULL;
	m_pattrIMPreEdit = NULL;
	m_nIMPreEditCursorPos = 0;

#endif // X88_GUI_GTK_USEIM

#endif // X88_GUI

}

// destructor

CX88Frame::~CX88Frame() {
	if (GetWndHandle() != NULL) {
		Destroy();
	}
}

// create

bool CX88Frame::Create(CX88000& x88k) {
	if (GetWndHandle() != NULL) {
		return false;
	}
	m_pX88k = &x88k;

#ifdef X88_GUI_WINDOWS

	CX88Dialog::SetResourceHandle(X88k().GetInstanceHandle());
	m_haccel = LoadAccelerators(
		X88k().GetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));
	if (m_haccel == NULL) {
		return false;
	}
	const char* pszFrameName = "MainWindow";
	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = X88k().GetInstanceHandle();
	wc.hIcon = LoadIcon(
		X88k().GetInstanceHandle(), MAKEINTRESOURCE(IDI_X88000));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = pszFrameName;
	RegisterClass(&wc);
	uint32_t dwWindowStyle = WS_OVERLAPPED |
		WS_THICKFRAME | WS_CAPTION | WS_SYSMENU |
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
		WS_CLIPCHILDREN;
	RECT rectFrame;
	SetRect(&rectFrame, 0, 0, 640, 400);
	AdjustWindowRect(&rectFrame, dwWindowStyle, TRUE);
	CreateWindow(
		pszFrameName, "X88000",
		dwWindowStyle | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rectFrame.right-rectFrame.left, rectFrame.bottom-rectFrame.top,
		NULL,
		LoadMenu(
			X88k().GetInstanceHandle(),
			MAKEINTRESOURCE(
				NX88Resource::IsResJapanese()?
				IDR_MAINMENU:
				IDR_MAINMENU_E)),
		X88k().GetInstanceHandle(), NULL);
	if (GetWndHandle() != NULL) {
		if (X88k().IsFullScreenBoot()) {
			PostMessage(
				GetWndHandle(),
				WM_COMMAND, MAKEWPARAM(IDM_FULL_SCREEN_MODE, 0), 0);
		}
	}

#elif defined(X88_GUI_GTK)

	m_hWnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	if (m_hWnd != NULL) {
		g_object_ref(GetWndHandle());
		CX88Dialog::InitSignalLockCounter(GetWndHandle());
		gtk_window_set_title(GTK_WINDOW(GetWndHandle()), "X88000");
		g_signal_connect(
			GetWndHandle(), "destroy",
			G_CALLBACK(OnDestroySignal), NULL);
		g_signal_connect(
			GetWndHandle(), "realize",
			G_CALLBACK(OnRealizeSignal), NULL);
		g_signal_connect(
			GetWndHandle(), "unrealize",
			G_CALLBACK(OnUnrealizeSignal), NULL);
		g_signal_connect(
			GetWndHandle(), "delete_event",
			G_CALLBACK(OnDeleteEvent), NULL);
		g_signal_connect(
			GetWndHandle(), "key_press_event",
			G_CALLBACK(OnKeyPressEvent), NULL);
		g_signal_connect(
			GetWndHandle(), "key_release_event",
			G_CALLBACK(OnKeyReleaseEvent), NULL);
		g_signal_connect(
			GetWndHandle(), "focus_in_event",
			G_CALLBACK(OnFocusInEvent), NULL);
		g_signal_connect(
			GetWndHandle(), "focus_out_event",
			G_CALLBACK(OnFocusOutEvent), NULL);
		gtk_widget_set_events(
			GetWndHandle(),
			gtk_widget_get_events(GetWndHandle()) |
				GDK_KEY_PRESS_MASK |
				GDK_KEY_RELEASE_MASK);
		memset(m_abtKeyboardState, 0, sizeof(m_abtKeyboardState));
		m_nModKeyState = 0;

#ifdef X88_GUI_GTK_USEKEYMAP

		m_bInitializedKeyCodes = false;
		m_bDirtyKeyboardState = true;

#endif // X88_GUI_GTK_USEKEYMAP

#ifdef X88_GUI_GTK_NOKEYRELEASE

		m_nLastPressKeyCode = -1;
		m_nKeyReleaseCounter = 0;

#endif // X88_GUI_GTK_NOKEYRELEASE

		m_nTimeOutID = g_timeout_add(
			10,
			TimeOutCallback, NULL);

		m_hwndVBox = gtk_vbox_new(FALSE, 0);

		m_hwndMenuBox = gtk_vbox_new(FALSE, 0);
		m_hwndMenuBar = NULL;
		m_pAccelGroup = NULL;
		BuildMenu(m_hwndMenuBar, m_pAccelGroup);
		gtk_container_add(
			GTK_CONTAINER(m_hwndMenuBox), m_hwndMenuBar);
		gtk_widget_show(m_hwndMenuBar);
		gtk_window_add_accel_group(
			GTK_WINDOW(GetWndHandle()), m_pAccelGroup);
		SetAllMenuItemState();
		gtk_box_pack_start(
			GTK_BOX(m_hwndVBox), m_hwndMenuBox, FALSE, FALSE, 0);
		gtk_widget_show(m_hwndMenuBox);

		m_view.Create(GetWndHandle(), X88k());
		g_object_ref(m_view.GetWndHandle());
		gtk_box_pack_start(
			GTK_BOX(m_hwndVBox), m_view.GetWndHandle(), TRUE, TRUE, 0);
		gtk_widget_show(m_view.GetWndHandle());

		gtk_container_add(GTK_CONTAINER(GetWndHandle()), m_hwndVBox);
		gtk_widget_show(m_hwndVBox);

		gtk_widget_show(GetWndHandle());

		m_bMenuShowing = false;
		m_bHaveFocus = true;
	}

#endif // X88_GUI

	return GetWndHandle() != NULL;
}

// destroy

bool CX88Frame::Destroy() {
	if (GetWndHandle() == NULL) {
		return false;
	}

#ifdef X88_GUI_WINDOWS

	DestroyWindow(GetWndHandle());
	delete m_pMessageProcessor;
	m_pMessageProcessor = NULL;

#elif defined(X88_GUI_GTK)

	g_object_unref(GetWndHandle());
	m_hWnd = NULL;

#endif // X88_GUI

	return GetWndHandle() == NULL;
}

////////////////////////////////////////////////////////////
// implementation

// reload menu

void CX88Frame::ReloadMenu() {

#ifdef X88_GUI_WINDOWS

	HMENU hmenuMainOld = GetMenu(GetWndHandle()),
		hmenuMainNew = LoadMenu(
			X88k().GetInstanceHandle(),
			MAKEINTRESOURCE(
				NX88Resource::IsResJapanese()?
				IDR_MAINMENU:
				IDR_MAINMENU_E));
	SetMenu(GetWndHandle(), hmenuMainNew);
	DestroyMenu(hmenuMainOld);
	if (IsFullScreen() && !IsMouseHoveringOnMenuBar()) {
		ActivateGDI(false);
		m_view.SetFrameClear(true);
	}

#elif defined(X88_GUI_GTK)

	gtk_container_remove(
		GTK_CONTAINER(m_hwndMenuBox), m_hwndMenuBar);
	gtk_window_remove_accel_group(
		GTK_WINDOW(GetWndHandle()), m_pAccelGroup);
	m_hwndMenuBar = NULL;
	m_pAccelGroup = NULL;
	BuildMenu(m_hwndMenuBar, m_pAccelGroup);
	gtk_container_add(
		GTK_CONTAINER(m_hwndMenuBox), m_hwndMenuBar);
	gtk_widget_show(m_hwndMenuBar);
	gtk_window_add_accel_group(
		GTK_WINDOW(GetWndHandle()), m_pAccelGroup);
	SetAllMenuItemState();

#endif // X88_GUI

}

#ifdef X88_GUI_WINDOWS

// window procedure

LRESULT CALLBACK CX88Frame::WndProc(
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
		m_view.Create(hwnd, X88k());
		m_hIMCBackup = NULL;
		if (X88k().IsIMEDisable()) {
			m_hIMCBackup = ImmAssociateContext(hwnd, NULL);
		}
		if (!X88k().GetDX().IsDirectSoundActive() &&
			X88k().IsDirectSoundUse())
		{
			if (!X88k().GetDX().StartDirectSound(
					GetWndHandle(),
					X88k().GetDirectSoundFrequency(),
					X88k().GetDirectSoundBits(),
					X88k().GetDirectSoundChannels()))
			{
				X88k().GetDX().EndDirectSound();
			}
		}
		if (!X88k().GetDX().IsDirectInputActive() &&
			X88k().IsDirectInputUse())
		{
			if (!X88k().GetDX().StartDirectInput(GetWndHandle())) {
				X88k().GetDX().EndDirectInput();
			}
		}
		if (X88k().GetDX().IsDirectSoundActive() &&
			X88k().IsDirectSoundUse())
		{
			X88k().CreateBeepPlayer();
			X88k().CreatePcgPlayer();
		}
		break;
	case WM_DESTROY:
		X88k().DestroyBeepPlayer();
		X88k().DestroyPcgPlayer();
		if (X88k().GetDX().IsDirectDrawActive()) {
			X88k().GetDX().EndDirectDraw();
		}
		if (X88k().GetDX().IsDirectSoundActive()) {
			X88k().GetDX().EndDirectSound();
		}
		if (X88k().GetDX().IsDirectInputActive()) {
			X88k().GetDX().EndDirectInput();
		}
		if (m_debugwnd.GetWndHandle() != NULL) {
			m_debugwnd.Destroy();
		}
		CloseParallalelModeless();
		m_view.Destroy();
		if (X88k().IsDebugLogging()) {
			X88k().EndDebugLog();
		}
		if (m_hIMCBackup != NULL) {
			ImmDestroyContext(m_hIMCBackup);
			m_hIMCBackup = NULL;
		}
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_EXIT:
		case IDM_RESET:
		case IDM_BASICMODE_N:
		case IDM_BASICMODE_V1_S:
		case IDM_BASICMODE_V1_H:
		case IDM_BASICMODE_V2:
		case IDM_BASICMODE_N80_V1:
		case IDM_BASICMODE_N80_V2:
		case IDM_BASE_CLOCK_4:
		case IDM_BASE_CLOCK_8:
		case IDM_BOOST_MODE:
		case IDM_FULL_SCREEN_MODE:
		case IDM_CLIPBOARD_COPY_BITMAP:
		case IDM_CLIPBOARD_COPY_TEXT:
		case IDM_CLIPBOARD_PASTE_TEXT:
		case IDM_ENV_SET:
		case IDM_PARALLEL_DEVICE_SETTING:
		case IDM_PARALLEL_DEVICE0:
		case IDM_PARALLEL_DEVICE1:
		case IDM_PARALLEL_DEVICE2:
		case IDM_PARALLEL_DEVICE3:
		case IDM_PARALLEL_DEVICE4:
		case IDM_PARALLEL_DEVICE5:
		case IDM_PARALLEL_DEVICE6:
		case IDM_PARALLEL_DEVICE7:
		case IDM_PARALLEL_DEVICE8:
		case IDM_PARALLEL_DEVICE9:
			OnSystemMenuCommand(LOWORD(wParam));
			break;
		case IDM_DISK_IMAGE:
		case IDM_TAPE_IMAGE:
		case IDM_MEMORY_IMAGE:
			OnImageMenuCommand(LOWORD(wParam));
			break;
		case IDM_DEBUG_MAIN:
		case IDM_DEBUG_SUB:
		case IDM_DEBUG_EXECUTE:
		case IDM_DEBUG_EXECUTE_STEP:
		case IDM_DEBUG_EXECUTE_TRACE:
		case IDM_DEBUG_EXECUTE_STEP2:
		case IDM_DEBUG_DIS_ASSEMBLE:
		case IDM_DEBUG_DUMP:
		case IDM_DEBUG_BREAK_POINT:
		case IDM_DEBUG_LOG:
		case IDM_DEBUG_WRITE_RAM:
			OnDebugMenuCommand(LOWORD(wParam));
			break;
		case IDM_VERSION:
			OnHelpMenuCommand(LOWORD(wParam));
			break;
		}
		break;
	case WM_INITMENUPOPUP:
		if (wParam != 0) {
			ModifyFrameMenu((HMENU)wParam);
		}
		break;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			int cx = LOWORD(lParam), cy = HIWORD(lParam);
			if (m_debugwnd.GetWndHandle() != NULL) {
				if (cy > CX88DebugWnd::CYWINDOWDEFAULT) {
					cy -= CX88DebugWnd::CYWINDOWDEFAULT;
				}
				MoveWindow(
					m_debugwnd.GetWndHandle(),
					0, cy, cx, CX88DebugWnd::CYWINDOWDEFAULT,
					TRUE);
			}
			if (m_view.GetWndHandle() != NULL) {
				int cx2 = max_value(cx, (int)CX88View::CXWINDOWMIN),
					cy2 = max_value(cy, (int)CX88View::CYWINDOWMIN),
					cx3 = (cy2*CX88View::CXWINDOWDEFAULT)/
						CX88View::CYWINDOWDEFAULT,
					cy3 = (cx2*CX88View::CYWINDOWDEFAULT)/
						CX88View::CXWINDOWDEFAULT;
				if (cx2 > cx3) {
					cx2 = cx3;
				} else {
					cy2 = cy3;
				}
				MoveWindow(
					m_view.GetWndHandle(),
					(cx-cx2)/2, (cy-cy2)/2, cx2, cy2,
					TRUE);
			}
		}
		break;
	case WM_ENTERMENULOOP:
		if (IsFullScreen()) {
			ActivateGDI(true);
		}
		X88k().PauseSound();
		break;
	case WM_EXITMENULOOP:
		if (IsFullScreen()) {
			ActivateGDI(false);
		}
		X88k().RestartSound();
		break;
	case WM_SETFOCUS:
		m_bHaveFocus = true;
		if (X88k().GetDX().IsDirectInputActive()) {
			X88k().GetDX().GetDIDKeyboard()->Acquire();
		}
		if (IsFullScreen()) {
			ActivateGDI(false);
		}
		break;
	case WM_KILLFOCUS:
		m_bHaveFocus = false;
		if (X88k().GetDX().IsDirectInputActive()) {
			X88k().GetDX().GetDIDKeyboard()->Unacquire();
		}
		if (IsFullScreen()) {
			ActivateGDI(true);
		}
		break;
	case WM_ACTIVATEAPP:
		m_bActiveProcess = (wParam != 0);
		break;
	case WM_SETCURSOR:
		if (IsFullScreen()) {
			if (LOWORD(lParam) == HTMENU) {
				if (!IsActivatedGDI()) {
					ActivateGDI(true);
					m_bMouseHoveringOnMenuBar = true;
				}
			} else {
				if (IsMouseHoveringOnMenuBar()) {
					ActivateGDI(false);
				}
			}
		}
		bDWP = true;
		break;
	case WM_IME_STARTCOMPOSITION:
		if (IsFullScreen()) {
			ActivateGDI(true);
		}
		{ // dummy block
			HIMC hIMC;
			if ((hIMC = ImmGetContext(hwnd)) == NULL) {
				break;
			}
			LOGFONT lf;
			GetObject(GetStockObject(SYSTEM_FONT), sizeof(lf), &lf);
			ImmSetCompositionFont(hIMC, &lf);
			COMPOSITIONFORM cf;
			memset(&cf, 0x00, sizeof(COMPOSITIONFORM));
			cf.dwStyle = CFS_POINT;
			if (!IsFullScreen()) {
				cf.ptCurrentPos.y = 400-lf.lfHeight;
			} else {
				POINT ptIME;
				ptIME.x = 0;
				ptIME.y = 480-lf.lfHeight;
				ScreenToClient(hwnd, &ptIME);
				cf.ptCurrentPos.y = ptIME.y;
			}
			ImmSetCompositionWindow(hIMC, &cf);
			ImmReleaseContext(hwnd, hIMC);
			m_bIMEOpen = true;
			bDWP = true;
		}
		break;
	case WM_IME_ENDCOMPOSITION:
		m_bIMEOpen = false;
		if (IsFullScreen()) {
			ActivateGDI(false);
		}
		bDWP = true;
		break;
	case WM_IME_COMPOSITION:
		if (IsFullScreen()) {
			ActivateGDI(true);
		}
		if ((lParam & GCS_RESULTSTR) != 0) {
			HIMC hIMC;
			if ((hIMC = ImmGetContext(hwnd)) == NULL) {
				break;
			}
			uint32_t dwSize = ImmGetCompositionString(
				hIMC, GCS_RESULTSTR, NULL, 0);
			HGLOBAL hgResult;
			char* pszResult;
			if ((hgResult = GlobalAlloc(GPTR, dwSize+1)) != NULL) {
				if ((pszResult = (char*)GlobalLock(hgResult)) != NULL) {
					ImmGetCompositionString(
						hIMC, GCS_RESULTSTR, pszResult, dwSize);
					X88k().AddIMEChar(pszResult);
					GlobalUnlock(hgResult);
				}
				GlobalFree(hgResult);
			}
			ImmReleaseContext(hwnd, hIMC);
		} else {
			bDWP = true;
		}
		break;
	case WM_PALETTECHANGED:
	case WM_QUERYNEWPALETTE:
		lResult = SendMessage(
			m_view.GetWndHandle(), nMessage, wParam, lParam);
		break;
	case WM_PAINT:
		{ // dummy block
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			RECT rectClient;
			GetClientRect(hwnd, &rectClient);
			FillRect(
				hdc, &rectClient,
				(HBRUSH)GetStockObject(
					IsFullScreen()? BLACK_BRUSH: GRAY_BRUSH));
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		bDWP = true;
		if ((wParam == VK_MENU) &&
			(IsFullScreen() && !m_bActivatedGDI))
		{
			// for menu underline drawing at Vista
			m_view.SetFrameClear(true);
		}
		break;
	default:
		bDWP = true;
		break;
	}
	return bDWP? DefWindowProc(hwnd, nMessage, wParam, lParam): lResult;
}

#elif defined(X88_GUI_GTK)

// build menu

void CX88Frame::BuildMenu(
	GtkWidget*& pMenuBar,
	GtkAccelGroup*& pAccelGroup)
{
	pAccelGroup = gtk_accel_group_new();
	GtkItemFactory* pItemFactory = gtk_item_factory_new(
		GTK_TYPE_MENU_BAR, "<main>", pAccelGroup);
	int nItemCount = 0;
	GtkItemFactoryEntry* pifeMenus =
		NX88Resource::GetMenuEntries(nItemCount);
	std::string gstrPath, gstrRadioPath;
	for (int nItem = 0; nItem < nItemCount; nItem++) {
		GtkItemFactoryEntry ifeMenu = pifeMenus[nItem];
		ifeMenu.path = (char*)RSTR(ifeMenu.path);
		bool bCreate = true;
		guint nAction = ifeMenu.callback_action;
		if (nAction != 0) {
			ifeMenu.callback =
				(GtkItemFactoryCallback)OnMenuSelectCallback;
		}
		switch (nAction) {
		case IDM_PARALLEL_DEVICE0:
		case IDM_PARALLEL_DEVICE1:
		case IDM_PARALLEL_DEVICE2:
		case IDM_PARALLEL_DEVICE3:
		case IDM_PARALLEL_DEVICE4:
		case IDM_PARALLEL_DEVICE5:
		case IDM_PARALLEL_DEVICE6:
		case IDM_PARALLEL_DEVICE7:
		case IDM_PARALLEL_DEVICE8:
		case IDM_PARALLEL_DEVICE9:
			{ // dummy block
				int nDeviceNo = nAction-IDM_PARALLEL_DEVICE0;
				if (nDeviceNo >= (int)X88k().ParallelManager().size()) {
					bCreate = false;
				} else {
					std::string gstrDevName =
						NX88Resource::GetParallelDeviceNameFromID(
							X88k().ParallelManager()[nDeviceNo].
								Device()->GetDeviceID());
					gstrPath = FormatStr(
						ifeMenu.path,
						gstrDevName.c_str());
					ifeMenu.path = (char*)gstrPath.c_str();
				}
			}
			break;
		}
		if (bCreate) {
			if (ifeMenu.item_type != NULL) {
				if (strcasecmp(
						ifeMenu.item_type, "<RadioItem>") == 0)
				{
					gstrRadioPath = ifeMenu.path;
					std::string::size_type nUnder =
						gstrRadioPath.find('_');
					if (nUnder != std::string::npos) {
						gstrRadioPath.erase(nUnder, 1);
					}
				} else if (
					strcasecmp(
						ifeMenu.item_type, "<RadioItem2>") == 0)
				{
					ifeMenu.item_type = (char*)gstrRadioPath.c_str();
				}
			}
			gtk_item_factory_create_item(
				pItemFactory,
				&ifeMenu, NULL, 1);
			if (strchr(ifeMenu.path+1, '/') == NULL) {
				gstrPath = ifeMenu.path;
				std::string::size_type nUnder =
					gstrPath.find('_');
				if (nUnder != std::string::npos) {
					gstrPath.erase(nUnder, 1);
				}
				GtkWidget* pWidget =
					gtk_item_factory_get_widget(
							pItemFactory, gstrPath.c_str());
				if (GTK_IS_MENU(pWidget)) {
					g_signal_connect(
						pWidget, "map",
						G_CALLBACK(OnMenuMapSignal), NULL);
					g_signal_connect(
						pWidget, "unmap",
						G_CALLBACK(OnMenuUnmapSignal), NULL);
					g_signal_connect(
						pWidget, "show",
						G_CALLBACK(OnMenuShowSignal), NULL);
				}
			}
		}
	}
	pMenuBar = gtk_item_factory_get_widget(pItemFactory, "<main>");
	if (GTK_IS_MENU_BAR(pMenuBar)) {
		g_signal_connect(
			pMenuBar, "deactivate",
			G_CALLBACK(OnMenuBarDeactivateSignal), NULL);
	}
}

// destroy signal

void CX88Frame::OnDestroySignal(
	GtkWidget* /*pWidget*/, gpointer /*pData*/)
{
	g_source_remove(m_nTimeOutID);
	m_nTimeOutID = 0;
	m_view.Destroy();
	m_hwndMenuBar = NULL;
	m_pAccelGroup = NULL;
	m_hwndVBox = NULL;
	CX88Dialog::DisposeSignalLockCounter(GetWndHandle());

	gtk_main_quit();
}

// realize signal

void CX88Frame::OnRealizeSignal(
	GtkWidget* /*pWidget*/, gpointer /*pData*/)
{
	if (!X88k().IsIMEDisable()) {

#ifdef X88_GUI_GTK_USEIM

		StartIM();

#endif // X88_GUI_GTK_USEIM

	}
}

// unrealize signal

void CX88Frame::OnUnrealizeSignal(
	GtkWidget* /*pWidget*/, gpointer /*pData*/)
{
	if (!X88k().IsIMEDisable()) {

#ifdef X88_GUI_GTK_USEIM

		StopIM();

#endif // X88_GUI_GTK_USEIM

	}
}

// menu map signal

void CX88Frame::OnMenuMapSignal(
	GtkWidget* pWidget, gpointer pData)
{
	m_bMenuShowing = true;
	if (!X88k().IsDirectInputUse()) {
		memset(m_abtKeyboardState, 0, sizeof(m_abtKeyboardState));
		m_nModKeyState = 0;
	}
}

// menu unmap signal

void CX88Frame::OnMenuUnmapSignal(
	GtkWidget* pWidget, gpointer pData)
{
	m_bMenuShowing = false;

#ifdef X88_GUI_GTK_USEKEYMAP

	m_bDirtyKeyboardState = true;

#endif // X88_GUI_GTK_USEKEYMAP

}

// menu show signal

void CX88Frame::OnMenuShowSignal(
	GtkWidget* pWidget, gpointer pData)
{
	CX88Dialog::CSignalBlocker blocker(GetWndHandle());
	if (blocker.IsBlocked()) {
		return;
	}
	SetAllMenuItemState();
}

// menubar deactivate signal

void CX88Frame::OnMenuBarDeactivateSignal(
	GtkWidget* pWidget, gpointer pData)
{
	m_bMenuShowing = false;

#ifdef X88_GUI_GTK_USEKEYMAP

	m_bDirtyKeyboardState = true;

#endif // X88_GUI_GTK_USEKEYMAP

}

// debug button clicked signal

void CX88Frame::OnClickedSignalDebugButton(
	GtkButton* /*pButton*/, gpointer pData)
{
	int nAction = GPOINTER_TO_INT(pData);
	OnDebugMenuCommand(nAction);
}

// delete event

gboolean CX88Frame::OnDeleteEvent(
	GtkWidget* /*pWidget*/, GdkEvent* /*pEvent*/, gpointer /*pData*/)
{
	return FALSE;
}

// key-press event

gboolean CX88Frame::OnKeyPressEvent(
	GtkWidget* /*pWidget*/, GdkEventKey *pEvent, gpointer* /*pData*/)
{
	m_bHaveFocus = true;

#ifdef X88_GUI_GTK_USEIM

	if (m_pIMC != NULL) {
		if (gtk_im_context_filter_keypress(m_pIMC, pEvent)) {

#ifdef X88_GUI_GTK_USEKEYMAP

			m_bDirtyKeyboardState = true;

#endif // X88_GUI_GTK_USEKEYMAP

			return TRUE;
		}
	}

#endif // X88_GUI_GTK_USEIM

#ifdef X88_GUI_GTK_USEKEYMAP

	if (X88k().IsDirectInputUse()) {
		if ((pEvent->keyval == GDK_VoidSymbol) &&
			(pEvent->string != NULL))
		{
			X88k().AddIMEChar(pEvent->string);
		}
		m_bDirtyKeyboardState = true;
	} else

#else // !X88_GUI_GTK_USEKEYMAP

	if (true)

#endif // X88_GUI_GTK_USEKEYMAP

	{
		m_nModKeyState = pEvent->state;
		guint nKeyVal = KeyValueConvert(pEvent->keyval);
		int nKeyCode = -1;
		switch (nKeyVal >> 8) {
		case 0x00:
		case 0xFF:
			nKeyCode = nKeyVal & 0x01FF;
			break;
		}
		if (nKeyCode >= 0) {
			m_abtKeyboardState[nKeyCode] = 1;
		} else if (
			(nKeyVal == GDK_VoidSymbol) &&
			(pEvent->string != NULL))
		{
			X88k().AddIMEChar(pEvent->string);
		} else if ((nKeyVal & 0xFFFF0000) == 0x01000000) {
			char szUTF16[3];
			szUTF16[0] = (char)(nKeyVal >> 8);
			szUTF16[1] = (char)(nKeyVal & 0xFF);
			szUTF16[2] = 0;
			gsize nWritten = 0;
			char* pszIME = g_convert(
				szUTF16, 2,
				"UTF-8", "UTF-16BE",
				NULL, &nWritten, NULL);
			if (pszIME != NULL) {
				X88k().AddIMEChar(std::string(pszIME, pszIME+nWritten));
				g_free(pszIME);
			}
		}

#ifdef X88_GUI_GTK_NOKEYRELEASE

	if (pEvent->string != NULL) {
		if (m_nLastPressKeyCode >= 0) {
			m_abtKeyboardState[m_nLastPressKeyCode] = 0;
		}
		m_nLastPressKeyCode = nKeyCode;
		m_nKeyReleaseCounter = 10;
	}

#endif // X88_GUI_GTK_NOKEYRELEASE

	}
	return FALSE;
}

// KeyRelease event

gboolean CX88Frame::OnKeyReleaseEvent(
	GtkWidget* /*pWidget*/, GdkEventKey *pEvent, gpointer* /*pData*/)
{
	m_bHaveFocus = true;

#ifdef X88_GUI_GTK_USEIM

	if (m_pIMC != NULL) {
		if (gtk_im_context_filter_keypress(m_pIMC, pEvent)) {

#ifdef X88_GUI_GTK_USEKEYMAP

			m_bDirtyKeyboardState = true;

#endif // X88_GUI_GTK_USEKEYMAP

			return TRUE;
		}
	}

#endif // X88_GUI_GTK_USEIM

#ifdef X88_GUI_GTK_USEKEYMAP

	if (X88k().IsDirectInputUse()) {
		m_bDirtyKeyboardState = true;
	} else

#else // !X88_GUI_GTK_USEKEYMAP

	if (true)

#endif // X88_GUI_GTK_USEKEYMAP

	{
		m_nModKeyState = pEvent->state;
		guint nKeyVal = KeyValueConvert(pEvent->keyval);
		int nKeyCode = -1;
		switch (nKeyVal >> 8) {
		case 0x00:
		case 0xFF:
			nKeyCode = nKeyVal & 0x01FF;
			break;
		}
		if (nKeyCode >= 0) {

#ifdef X88_GUI_GTK_IGNORECHARRELEASE

			if ((nKeyCode >= 0x20) && (nKeyCode <= 0x7E)) {
				return FALSE;
			}

#endif // X88_GUI_GTK_IGNORECHARRELEASE

			m_abtKeyboardState[nKeyCode] = 0;
		}

#ifdef X88_GUI_GTK_NOKEYRELEASE

	if (m_nLastPressKeyCode >= 0) {
		m_abtKeyboardState[m_nLastPressKeyCode] = 0;
		m_nLastPressKeyCode = -1;
		m_nKeyReleaseCounter = 0;
	}

#endif // X88_GUI_GTK_NOKEYRELEASE

	}
	return FALSE;
}

// focus-in event

gboolean CX88Frame::OnFocusInEvent(
	GtkWidget* /*pWidget*/, GdkEventFocus* /*pEvent*/, gpointer* /*pData*/)
{
	m_bHaveFocus = true;
	if (!X88k().IsDirectInputUse()) {
		memset(m_abtKeyboardState, 0, sizeof(m_abtKeyboardState));
		m_nModKeyState = 0;
	}

#ifdef X88_GUI_GTK_USEKEYMAP

	m_bDirtyKeyboardState = true;

#endif // X88_GUI_GTK_USEKEYMAP

	if (!X88k().IsIMEDisable()) {

#ifdef X88_GUI_GTK_USEIM

		if (m_pIMC != NULL) {
			gtk_im_context_focus_in(m_pIMC);
		}

#endif // X88_GUI_GTK_USEIM

	}

	return TRUE;
}

// focus-out event

gboolean CX88Frame::OnFocusOutEvent(
	GtkWidget* /*pWidget*/, GdkEventFocus* /*pEvent*/, gpointer* /*pData*/)
{
	m_bHaveFocus = false;
	if (!X88k().IsDirectInputUse()) {
		memset(m_abtKeyboardState, 0, sizeof(m_abtKeyboardState));
		m_nModKeyState = 0;
	}

	if (!X88k().IsIMEDisable()) {

#ifdef X88_GUI_GTK_USEIM

		if (m_pIMC != NULL) {
			gtk_im_context_focus_out(m_pIMC);
		}

#endif // X88_GUI_GTK_USEIM

	}

	return TRUE;
}

// menu select callback

void CX88Frame::OnMenuSelectCallback(
	gpointer /*pData*/, guint nAction, GtkWidget* pWidget)
{
	if (!GTK_IS_MENU_ITEM(pWidget)) {
		return;
	}
	if (GTK_IS_RADIO_MENU_ITEM(pWidget) &&
		(gtk_check_menu_item_get_active(
			GTK_CHECK_MENU_ITEM(pWidget)) == FALSE))
	{
		return;
	}
	CX88Dialog::CSignalBlocker blocker(GetWndHandle());
	if (blocker.IsBlocked()) {
		return;
	}
	switch (nAction) {
	case IDM_EXIT:
	case IDM_RESET:
	case IDM_BASICMODE_N:
	case IDM_BASICMODE_V1_S:
	case IDM_BASICMODE_V1_H:
	case IDM_BASICMODE_V2:
	case IDM_BASICMODE_N80_V1:
	case IDM_BASICMODE_N80_V2:
	case IDM_BASE_CLOCK_4:
	case IDM_BASE_CLOCK_8:
	case IDM_BOOST_MODE:
	case IDM_FULL_SCREEN_MODE:
	case IDM_CLIPBOARD_COPY_BITMAP:
	case IDM_CLIPBOARD_COPY_TEXT:
	case IDM_CLIPBOARD_PASTE_TEXT:
	case IDM_ENV_SET:
	case IDM_PARALLEL_DEVICE_SETTING:
	case IDM_PARALLEL_DEVICE0:
	case IDM_PARALLEL_DEVICE1:
	case IDM_PARALLEL_DEVICE2:
	case IDM_PARALLEL_DEVICE3:
	case IDM_PARALLEL_DEVICE4:
	case IDM_PARALLEL_DEVICE5:
	case IDM_PARALLEL_DEVICE6:
	case IDM_PARALLEL_DEVICE7:
	case IDM_PARALLEL_DEVICE8:
	case IDM_PARALLEL_DEVICE9:
		OnSystemMenuCommand(nAction);
		break;
	case IDM_DISK_IMAGE:
	case IDM_TAPE_IMAGE:
	case IDM_MEMORY_IMAGE:
		OnImageMenuCommand(nAction);
		break;
	case IDM_DEBUG_MAIN:
	case IDM_DEBUG_SUB:
	case IDM_DEBUG_EXECUTE:
	case IDM_DEBUG_EXECUTE_STEP:
	case IDM_DEBUG_EXECUTE_TRACE:
	case IDM_DEBUG_EXECUTE_STEP2:
	case IDM_DEBUG_DIS_ASSEMBLE:
	case IDM_DEBUG_DUMP:
	case IDM_DEBUG_BREAK_POINT:
	case IDM_DEBUG_LOG:
	case IDM_DEBUG_WRITE_RAM:
		OnDebugMenuCommand(nAction);
		break;
	case IDM_VERSION:
		OnHelpMenuCommand(nAction);
		break;
	}
}

// time out callback

gboolean CX88Frame::TimeOutCallback(gpointer /*pData*/) {

#ifdef X88_GUI_GTK_USEKEYMAP

	if (X88k().IsDirectInputUse() &&
		GTK_WIDGET_DRAWABLE(GetWndHandle()) &&
		m_bDirtyKeyboardState)
	{
		m_bDirtyKeyboardState = false;
		if (!m_bInitializedKeyCodes) {
			for (gint nKey = 0; nKey < 256; nKey++) {
				guint nKeyVal = XkbKeycodeToKeysym(
						GDK_WINDOW_XDISPLAY(GetWndHandle()->window),
						nKey, 0, 0),
					nKeyValShifted = XkbKeycodeToKeysym(
						GDK_WINDOW_XDISPLAY(GetWndHandle()->window),
						nKey, 1, 0);
				if ((nKeyVal != nKeyValShifted) &&
					(nKeyValShifted != GDK_VoidSymbol) &&
					((nKeyVal == GDK_VoidSymbol) ||
						(nKeyVal == GDK_backslash)))
				{
					nKeyVal = nKeyValShifted;
				}
				nKeyVal = KeyValueConvert(nKeyVal);
				int nKeyCode = -1;
				switch (nKeyVal >> 8) {
				case 0x00:
				case 0xFF:
					nKeyCode = nKeyVal & 0x01FF;
					break;
				}
				m_anKeyCodes[nKey] = nKeyCode;
			}
			m_bInitializedKeyCodes = true;
		}
		char achKeymap[32];
		XQueryKeymap(GDK_WINDOW_XDISPLAY(GetWndHandle()->window), achKeymap);
		memset(m_abtKeyboardState, 0, sizeof(m_abtKeyboardState));
		for (gint nKey = 0; nKey < 256; nKey++) {
			if ((achKeymap[nKey/8] & (1 << (nKey%8))) != 0) {
				int nKeyCode = m_anKeyCodes[nKey];
				if (nKeyCode >= 0) {
					m_abtKeyboardState[nKeyCode] = 1;
				}
			}
		}
		gint x, y;
		GdkModifierType state;
		gdk_window_get_pointer(GetWndHandle()->window, &x, &y, &state);
		m_nModKeyState = state;
	}

#endif // X88_GUI_GTK_USEKEYMAP

#ifdef X88_GUI_GTK_NOKEYRELEASE

	if (m_nKeyReleaseCounter > 0) {
		if (--m_nKeyReleaseCounter <= 0) {
			m_abtKeyboardState[m_nLastPressKeyCode] = 0;
			m_nLastPressKeyCode = -1;
		}
	}

#endif // X88_GUI_GTK_NOKEYRELEASE

	return TRUE;
}

#ifdef X88_GUI_GTK_USEIM

// input method commit signal

void CX88Frame::OnIMCommitSignal(
	GtkIMContext* /*pIMC*/, gchar* pgszText, gpointer* /*pData*/)
{
	if (m_bPreEditing ||
		IsContainedNonASCII(pgszText))
	{
		X88k().AddIMEChar(pgszText);
	}
}

// input method pre-edit string start signal

void CX88Frame::OnIMPreEditStartSignal(
	GtkIMContext* /*pIMC*/, gpointer* /*pData*/)
{
	m_bPreEditing = true;
}

// input method pre-edit string end signal

void CX88Frame::OnIMPreEditEndSignal(
	GtkIMContext* /*pIMC*/, gpointer* /*pData*/)
{
	m_bPreEditing = false;
}

// input method pre-edit string changed signal

void CX88Frame::OnIMPreEditChangedSignal(
	GtkIMContext* pIMC, gpointer* /*pData*/)
{
	DeleteIMPreEdit();
	gtk_im_context_get_preedit_string(
		pIMC,
		&m_pgszIMPreEdit,
		&m_pattrIMPreEdit,
		&m_nIMPreEditCursorPos);
	if ((m_pgszIMPreEdit == NULL) || (m_pgszIMPreEdit[0] == '\0')) {
		DeleteIMPreEdit();
	}
	UpdateIMPreEditWindow();
}

// input method pre-edit expose event

gboolean CX88Frame::OnIMPreEditExposeEvent(
	GtkWidget* pWidget, GdkEventExpose* pEvent, gpointer /*pData*/)
{
	GdkGC* pGC = gdk_gc_new(pWidget->window);
	GdkColor colWhite, colBlack;
	colWhite.red = colWhite.green = colWhite.blue = 0xFFFF;
	colBlack.red = colBlack.green = colBlack.blue = 0x0000;
	gdk_colormap_alloc_color(
		gdk_colormap_get_system(), &colWhite, FALSE, TRUE);
	gdk_colormap_alloc_color(
		gdk_colormap_get_system(), &colBlack, FALSE, TRUE);
	gdk_gc_set_foreground(pGC, &colWhite);
	gdk_draw_rectangle(
		pWidget->window, pGC,
		TRUE,
		pEvent->area.x, pEvent->area.y,
		pEvent->area.width, pEvent->area.height);
	gdk_gc_set_foreground(pGC, &colBlack);
	gdk_gc_set_background(pGC, &colWhite);
	PangoLayout* pLayout = gtk_widget_create_pango_layout(
		GetWndHandle(),
		m_pgszIMPreEdit);
	if (pLayout != NULL) {
		pango_layout_set_attributes(
			pLayout,
			m_pattrIMPreEdit);
		gdk_draw_layout(
			pWidget->window, pGC,
			0, 0,
			pLayout);
		g_object_unref(pLayout);
	}
	g_object_unref(pGC);
	return TRUE;
}

#endif // X88_GUI_GTK_USEIM

#endif // X88_GUI

#ifdef X88_GUI_WINDOWS

// pre-process of TrenslateMessage

BOOL CX88Frame::PreTranslateMessage(MSG* pMessage) {
	if (((pMessage->message == WM_SYSKEYDOWN) ||
			(pMessage->message == WM_SYSKEYUP)) &&
		(pMessage->wParam == VK_F10) &&
		(GetKeyState(VK_MENU) >= 0))
	{
		return TRUE;
	}
	return TranslateAccelerator(
			GetWndHandle(), m_haccel, pMessage) != FALSE;
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// command

// system menu command

void CX88Frame::OnSystemMenuCommand(unsigned nCommand) {
	switch (nCommand) {
	case IDM_EXIT:

#ifdef X88_GUI_WINDOWS

		PostMessage(GetWndHandle(), WM_CLOSE, 0, 0);

#elif defined(X88_GUI_GTK)

		gtk_widget_destroy(GetWndHandle());

#endif // X88_GUI

		break;
	case IDM_RESET:
		X88k().DoReset();
		break;
	case IDM_BASICMODE_N:
	case IDM_BASICMODE_V1_S:
	case IDM_BASICMODE_V1_H:
	case IDM_BASICMODE_V2:
	case IDM_BASICMODE_N80_V1:
	case IDM_BASICMODE_N80_V2:
		{ // dummy block
			int nBasicMode = CPC88Z80Main::BASICMODE_N88V2;
			bool bHighSpeedMode = true;
			switch (nCommand) {
			case IDM_BASICMODE_N:
				nBasicMode = CPC88Z80Main::BASICMODE_N;
				bHighSpeedMode = false;
				break;
			case IDM_BASICMODE_V1_S:
				nBasicMode = CPC88Z80Main::BASICMODE_N88V1;
				bHighSpeedMode = false;
				break;
			case IDM_BASICMODE_V1_H:
				nBasicMode = CPC88Z80Main::BASICMODE_N88V1;
				bHighSpeedMode = true;
				break;
			case IDM_BASICMODE_V2:
				nBasicMode = CPC88Z80Main::BASICMODE_N88V2;
				bHighSpeedMode = true;
				break;
			case IDM_BASICMODE_N80_V1:
				nBasicMode = CPC88Z80Main::BASICMODE_N80V1;
				bHighSpeedMode = false;
				break;
			case IDM_BASICMODE_N80_V2:
				nBasicMode = CPC88Z80Main::BASICMODE_N80V2;
				bHighSpeedMode = true;
				break;
			}
			X88k().SetBasicMode(nBasicMode, bHighSpeedMode);
			X88k().DoReset();
			X88k().SaveOption(CX88Option::OPT_BASIC);
		}
		break;
	case IDM_BASE_CLOCK_4:
	case IDM_BASE_CLOCK_8:
		{ // dummy block
			int nClock = 4;
			switch (nCommand) {
			case IDM_BASE_CLOCK_4:
				nClock = 4;
				break;
			case IDM_BASE_CLOCK_8:
				nClock = 8;
				break;
			}
			X88k().SetBaseClock(nClock);
			X88k().DoReset();
			X88k().SaveOption(CX88Option::OPT_CLOCK);
		}
		break;
	case IDM_BOOST_MODE:
		X88k().SetBoostMode(!X88k().IsBoostMode());
		X88k().ResetExecuteCounter();
		X88k().SaveOption(CX88Option::OPT_BOOST);
		break;
	case IDM_FULL_SCREEN_MODE:
		if (X88k().PC88().IsDebugMode()) {
			X88k().PC88().SetDebugExecMode(CPC88::DEBUGEXEC_NONE);
			X88k().OutputDebugLog(CPC88::DEBUGLOG_END);
			X88k().ResetExecuteCounter();
		}
		SetFullScreen(!IsFullScreen());
		X88k().SetFullScreenBoot(IsFullScreen());
		X88k().SaveOption(CX88Option::OPT_FULLSCREEN);
		break;
	case IDM_CLIPBOARD_COPY_BITMAP:

#ifdef X88_GUI_GTK

		{ // dummy block
			CX88FileDlg dlgFile(
				GetWndHandle(),
				false,
				RSTR("Export Screenshot(XWD)"),
				RSTR("Screenshot(XWD)|*.xwd||"),
				"xwd");
			if (dlgFile.DoModal() != IDOK) {
				break;
			}
			X88k().SetWriteFileName(dlgFile.GetFileName());
		}

#endif // X88_GUI

		if (!X88k().DoClipboardCopyBitmap()) {

#ifdef X88_GUI_GTK

			CX88Dialog::ShowMessageBox(
				GetWndHandle(),
				MB_ICONEXCLAMATION, MB_OK,
				RSTR("Export Screenshot"),
				RSTR("An error occurred while writing."));

#endif // X88_GUI

		}
		break;
	case IDM_CLIPBOARD_COPY_TEXT:
		if (!X88k().DoClipboardCopyText()) {
		}
		break;
	case IDM_CLIPBOARD_PASTE_TEXT:
		if (!X88k().DoClipboardPasteText()) {
		}
		break;
	case IDM_ENV_SET:
		{ // dummy block
			X88k().PauseSound();
			CX88EnvSetDlg dlgEnvSet(GetWndHandle());
			// general
			dlgEnvSet.SetUILanguage(
				NX88Resource::GetUILanguage());
			dlgEnvSet.SetBoostLimiter(
				X88k().GetBoostLimiter());
			dlgEnvSet.SetDriveCount(
				X88k().GetDriveCount());
			dlgEnvSet.SetHiresolution(
				X88k().IsHiresolution());
			dlgEnvSet.SetWaitEmulation(
				X88k().IsWaitEmulation());
			dlgEnvSet.SetOldCompatible(
				X88k().IsOldCompatible());
			// display
			dlgEnvSet.SetScreenFrameRate(
				X88k().GetScreenFrameRate());
			dlgEnvSet.SetInterlace(
				X88k().IsInterlace());
			dlgEnvSet.SetPcgEnable(
				X88k().IsPcgEnable());
			dlgEnvSet.SetOptionFont(
				X88k().IsOptionFont());
			// input
			dlgEnvSet.SetDirectInputUse(
				X88k().IsDirectInputUse());
			dlgEnvSet.SetIMEDisable(
				X88k().IsIMEDisable());
			// sound
			dlgEnvSet.SetDirectSoundUse(
				X88k().IsDirectSoundUse());
			dlgEnvSet.SetDirectSoundFrequency(
				X88k().GetDirectSoundFrequency());
			dlgEnvSet.SetDirectSoundBits(
				X88k().GetDirectSoundBits());
			dlgEnvSet.SetDirectSoundChannels(
				X88k().GetDirectSoundChannels());
			dlgEnvSet.SetBeepPlayerVolume(
				X88k().GetBeepPlayerVolume());
			dlgEnvSet.SetBeepPlayerMute(
				X88k().IsBeepPlayerMute());
			dlgEnvSet.SetPcgPlayerVolume(
				X88k().GetPcgPlayerVolume());
			dlgEnvSet.SetPcgPlayerMute(
				X88k().IsPcgPlayerMute());
			if (dlgEnvSet.DoModal() == IDOK) {
				// general
				if (dlgEnvSet.GetUILanguage() !=
						NX88Resource::GetUILanguage())
				{
					NX88Resource::SetUILanguage(
						dlgEnvSet.GetUILanguage());
					ReloadMenu();
				}
				X88k().SetBoostLimiter(
					dlgEnvSet.GetBoostLimiter());
				X88k().SetDriveCount(
					dlgEnvSet.GetDriveCount());
				X88k().SetHiresolution(
					dlgEnvSet.IsHiresolution());
				X88k().SetWaitEmulation(
					dlgEnvSet.IsWaitEmulation());
				X88k().SetOldCompatible(
					dlgEnvSet.IsOldCompatible());
				// display
				if (dlgEnvSet.GetScreenFrameRate() !=
						X88k().GetScreenFrameRate())
				{
					X88k().SetScreenFrameRate(
						dlgEnvSet.GetScreenFrameRate());
					m_view.RestartTimer();
				}
				if (dlgEnvSet.IsInterlace() != X88k().IsInterlace()) {
					X88k().SetInterlace(
						dlgEnvSet.IsInterlace());
					X88k().GetScreenDrawer().SetInterlace(
						dlgEnvSet.IsInterlace());
					X88k().PC88().Z80Main().SetGVRamUpdate(TRUE);
				}
				X88k().SetPcgEnable(
					dlgEnvSet.IsPcgEnable());
				if (dlgEnvSet.IsOptionFont() !=
						X88k().IsOptionFont())
				{
					X88k().SetOptionFont(
						dlgEnvSet.IsOptionFont());
					X88k().PC88().Z80Main().SetOptionFont(
						dlgEnvSet.IsOptionFont());
					X88k().PC88().Z80Main().SetGVRamUpdate(TRUE);
				}
				// input
				if (dlgEnvSet.IsDirectInputUse() !=
						X88k().IsDirectInputUse())
				{
					X88k().SetDirectInputUse(
						dlgEnvSet.IsDirectInputUse());

#ifdef X88_GUI_WINDOWS

					if (X88k().IsDirectInputUse()) {
						if (!X88k().GetDX().StartDirectInput(GetWndHandle())) {
							X88k().GetDX().EndDirectInput();
						}
					} else {
						X88k().GetDX().EndDirectInput();
					}

#elif defined(X88_GUI_GTK)

					if (X88k().IsDirectInputUse()) {

#ifdef X88_GUI_GTK_USEKEYMAP

						m_bDirtyKeyboardState = true;

#endif // X88_GUI_GTK_USEKEYMAP

					} else {
						memset(
							m_abtKeyboardState, 0,
							sizeof(m_abtKeyboardState));
						m_nModKeyState = 0;
					}

#endif // X88_GUI

				}
				if (dlgEnvSet.IsIMEDisable() !=
						X88k().IsIMEDisable())
				{
					X88k().SetIMEDisable(
						dlgEnvSet.IsIMEDisable());

#ifdef X88_GUI_WINDOWS

					m_hIMCBackup = ImmAssociateContext(
						GetWndHandle(), m_hIMCBackup);

#elif defined(X88_GUI_GTK)

#ifdef X88_GUI_GTK_USEIM

					if (!dlgEnvSet.IsIMEDisable()) {
						StartIM();
					} else {
						StopIM();
					}

#endif // X88_GUI_GTK_USEIM

#endif // X88_GUI

				}
				// sound

#ifdef X88_GUI_WINDOWS

				bool bDirectSoundON = false,
					bDirectSoundOFF = false,
					bBeepPlayerChange = false;
				if ((dlgEnvSet.IsDirectSoundUse() !=
						X88k().IsDirectSoundUse()) ||
					(dlgEnvSet.GetDirectSoundFrequency() !=
						X88k().GetDirectSoundFrequency()) ||
					(dlgEnvSet.GetDirectSoundBits() !=
						X88k().GetDirectSoundBits()) ||
					(dlgEnvSet.GetDirectSoundChannels() !=
						X88k().GetDirectSoundChannels()))
				{
					X88k().SetDirectSoundUse(
						dlgEnvSet.IsDirectSoundUse());
					X88k().SetDirectSoundFrequency(
						dlgEnvSet.GetDirectSoundFrequency());
					X88k().SetDirectSoundBits(
						dlgEnvSet.GetDirectSoundBits());
					X88k().SetDirectSoundChannels(
						dlgEnvSet.GetDirectSoundChannels());
					bDirectSoundON = dlgEnvSet.IsDirectSoundUse();
					bDirectSoundOFF = !dlgEnvSet.IsDirectSoundUse() ||
						(bDirectSoundON &&
							X88k().GetDX().IsDirectSoundActive());
					bBeepPlayerChange = true;
				}
				if (bBeepPlayerChange) {
					X88k().DestroyBeepPlayer();
					X88k().DestroyPcgPlayer();
				}
				if (bDirectSoundOFF) {
					if (X88k().GetDX().IsDirectSoundActive()) {
						X88k().GetDX().EndDirectSound();
					}
				}
				if (bDirectSoundON) {
					if (!X88k().GetDX().StartDirectSound(
							GetWndHandle(),
							X88k().GetDirectSoundFrequency(),
							X88k().GetDirectSoundBits(),
							X88k().GetDirectSoundChannels()))
					{
						X88k().GetDX().EndDirectSound();
						bBeepPlayerChange = false;
					}
				}
				if (bBeepPlayerChange) {
					if (X88k().IsDirectSoundUse()) {
						X88k().CreateBeepPlayer();
						X88k().CreatePcgPlayer();
					}
				}

#elif defined(X88_GUI_GTK)

				//!!!!!!!!!!!!!!!!!!!

#endif // X88_GUI

				if ((dlgEnvSet.GetBeepPlayerVolume() !=
						X88k().GetBeepPlayerVolume()) ||
					(dlgEnvSet.IsBeepPlayerMute() !=
						X88k().IsBeepPlayerMute()))
				{
					X88k().SetBeepPlayerVolume(
						dlgEnvSet.GetBeepPlayerVolume());
					X88k().SetBeepPlayerMute(
						dlgEnvSet.IsBeepPlayerMute());
					X88k().UpdateBeepPlayerVolumeMute();
				}
				if ((dlgEnvSet.GetPcgPlayerVolume() !=
						X88k().GetPcgPlayerVolume()) ||
					(dlgEnvSet.IsPcgPlayerMute() !=
						X88k().IsPcgPlayerMute()))
				{
					X88k().SetPcgPlayerVolume(
						dlgEnvSet.GetPcgPlayerVolume());
					X88k().SetPcgPlayerMute(
						dlgEnvSet.IsPcgPlayerMute());
					X88k().UpdatePcgPlayerVolumeMute();
				}
				// save option
				std::vector<std::string> vectOptions;
				vectOptions.push_back(CX88Option::OPT_UILANG);
				vectOptions.push_back(CX88Option::OPT_BOOSTLIM);
				vectOptions.push_back(CX88Option::OPT_DRIVES);
				vectOptions.push_back(CX88Option::OPT_CRTVSYNC);
				vectOptions.push_back(CX88Option::OPT_WAITEMU);
				vectOptions.push_back(CX88Option::OPT_OLDCOMPAT);
				vectOptions.push_back(CX88Option::OPT_PCGENABLE);
				vectOptions.push_back(CX88Option::OPT_OPTIONFONT);
				vectOptions.push_back(CX88Option::OPT_FRAMERATE);
				vectOptions.push_back(CX88Option::OPT_INTERLACE);
				vectOptions.push_back(CX88Option::OPT_BEEPVOLUME);
				vectOptions.push_back(CX88Option::OPT_BEEPMUTE);
				vectOptions.push_back(CX88Option::OPT_PCGVOLUME);
				vectOptions.push_back(CX88Option::OPT_PCGMUTE);
				vectOptions.push_back(CX88Option::OPT_DIRECTINPUT);
				vectOptions.push_back(CX88Option::OPT_IMEDISABLE);

#ifdef X88_GUI_WINDOWS

				vectOptions.push_back(CX88Option::OPT_DIRECTSOUND);
				vectOptions.push_back(CX88Option::OPT_DSOUNDFREQ);
				vectOptions.push_back(CX88Option::OPT_DSOUNDBIT);
				vectOptions.push_back(CX88Option::OPT_DSOUNDCH);

#elif defined(X88_GUI_GTK)

				//!!!!!!!!!!!!!!!!!!!

#endif // X88_GUI

				X88k().SaveOptions(&vectOptions);
			}
			X88k().RestartSound();
		}
		break;
	case IDM_PARALLEL_DEVICE_SETTING:
		{ // dummy block
			int nDeviceNo = X88k().GetConnectedParallelDeviceNo();
			CX88Dialog* pdlgPara = X88k().ParallelManager()[nDeviceNo].Dialog();
			if (pdlgPara != NULL) {
				pdlgPara->SetParentWndHandle(GetWndHandle());
				if (!pdlgPara->IsModeless()) {
					pdlgPara->DoModal();
				} else if (!pdlgPara->IsCreatedModeless()) {
					pdlgPara->CreateModeless();
				} else {
					pdlgPara->CloseModeless();
				}
			}
		}
		break;
	case IDM_PARALLEL_DEVICE0:
	case IDM_PARALLEL_DEVICE1:
	case IDM_PARALLEL_DEVICE2:
	case IDM_PARALLEL_DEVICE3:
	case IDM_PARALLEL_DEVICE4:
	case IDM_PARALLEL_DEVICE5:
	case IDM_PARALLEL_DEVICE6:
	case IDM_PARALLEL_DEVICE7:
	case IDM_PARALLEL_DEVICE8:
	case IDM_PARALLEL_DEVICE9:
		{ // dummy block
			CloseParallalelModeless();
			X88k().SelectParallelDevice(nCommand-IDM_PARALLEL_DEVICE0);
			X88k().SaveOption(CX88Option::OPT_PARALLEL);
		}
		break;
	}
}

// image menu command

void CX88Frame::OnImageMenuCommand(unsigned nCommand) {
	switch (nCommand) {
	case IDM_DISK_IMAGE:
		{ // dummy block
			X88k().PauseSound();
			CX88DiskImageDlg dlgDiskImage(GetWndHandle());
			dlgDiskImage.SetFdc(X88k().PC88().Fdc());
			dlgDiskImage.SetDiskImageCollection(
				X88k().PC88().GetDiskImageCollection());
			dlgDiskImage.DoModal();
			X88k().RestartSound();
		}
		break;
	case IDM_TAPE_IMAGE:
		{ // dummy block
			X88k().PauseSound();
			CX88TapeImageDlg dlgTapeImage(GetWndHandle());
			dlgTapeImage.SetLoadTapeImage(
				X88k().PC88().Usart().GetLoadTapeImage());
			dlgTapeImage.SetSaveTapeImage(
				X88k().PC88().Usart().GetSaveTapeImage());
			dlgTapeImage.DoModal();
			X88k().RestartSound();
		}
		break;
	case IDM_MEMORY_IMAGE:
		{ // dummy block
			X88k().PauseSound();
			CX88FileDlg dlgFile(
				GetWndHandle(),
				true,
				RSTR("Load Memory Image"),
				RSTR("Memory Image(n80)|*.n80||"),
				"n80");
			if (dlgFile.DoModal() == IDOK) {
				X88k().PC88().LoadMemoryImage(dlgFile.GetFileName());
			}
			if (X88k().PC88().IsDebugMode()) {
				X88k().PC88().UpdateMnemonic();
				UpdateDebugWnd();
			}
			X88k().OutputDebugLog(CPC88::DEBUGLOG_READ_MEMIMAGE);
			X88k().RestartSound();
		}
		break;
	}
}

// debug menu command

void CX88Frame::OnDebugMenuCommand(unsigned nCommand) {
	switch (nCommand) {
	case IDM_DEBUG_MAIN:
	case IDM_DEBUG_SUB:
		if ((nCommand != IDM_DEBUG_SUB) ||
			!X88k().PC88().IsSubSystemDisableNow())
		{
			int nDebugExecMode = (nCommand == IDM_DEBUG_MAIN)?
				CPC88::DEBUGEXEC_MAIN:
				CPC88::DEBUGEXEC_SUB;
			int nLogMode;
			if (!X88k().PC88().IsDebugMode()) {
				nLogMode = CPC88::DEBUGLOG_START;
			} else if (nDebugExecMode != X88k().PC88().GetDebugExecMode()) {
				nLogMode = CPC88::DEBUGLOG_CHANGE_CPU;
			} else {
				nLogMode = CPC88::DEBUGLOG_END;
				nDebugExecMode = CPC88::DEBUGEXEC_NONE;
			}
			X88k().PC88().SetDebugExecMode(nDebugExecMode);
			X88k().OutputDebugLog(nLogMode);
			X88k().ResetExecuteCounter();

#ifdef X88_GUI_GTK

			if ((nLogMode != CPC88::DEBUGLOG_CHANGE_CPU) &&
				!X88k().PC88().IsDebugMode())
			{
				gtk_container_remove(
					GTK_CONTAINER(m_hwndVBox), m_debugwnd.GetWndHandle());
				gtk_widget_hide(m_debugwnd.GetWndHandle());
			}

#endif // X88_GUI

			if (nLogMode != CPC88::DEBUGLOG_CHANGE_CPU) {
				if (X88k().PC88().IsDebugMode()) {
					m_debugwnd.Create(GetWndHandle(), X88k());
				} else {
					m_debugwnd.Destroy();
				}
			}

#ifdef X88_GUI_WINDOWS

			RECT rectFrame;
			SetRect(
				&rectFrame,
				0, 0, 640, X88k().PC88().IsDebugMode()? 480: 400);
			uint32_t dwStyle = GetWindowLong(GetWndHandle(), GWL_STYLE);
			AdjustWindowRect(&rectFrame, dwStyle, TRUE);
			SetWindowPos(
				GetWndHandle(), NULL,
				0, 0,
				rectFrame.right-rectFrame.left,
					rectFrame.bottom-rectFrame.top,
				SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);

#elif defined(X88_GUI_GTK)

			if (nLogMode != CPC88::DEBUGLOG_CHANGE_CPU) {
				if (X88k().PC88().IsDebugMode()) {
					g_signal_connect(
						m_debugwnd.GetStepButtonHandle(), "clicked",
						G_CALLBACK(OnClickedSignalDebugButton),
						GINT_TO_POINTER(IDM_DEBUG_EXECUTE_STEP));
					g_signal_connect(
						m_debugwnd.GetTraceButtonHandle(), "clicked",
						G_CALLBACK(OnClickedSignalDebugButton),
						GINT_TO_POINTER(IDM_DEBUG_EXECUTE_TRACE));
					g_signal_connect(
						m_debugwnd.GetStep2ButtonHandle(), "clicked",
						G_CALLBACK(OnClickedSignalDebugButton),
						GINT_TO_POINTER(IDM_DEBUG_EXECUTE_STEP2));
					g_object_ref(m_debugwnd.GetWndHandle());
					gtk_box_pack_start(
						GTK_BOX(m_hwndVBox), m_debugwnd.GetWndHandle(),
						FALSE, FALSE, 0);
					gtk_widget_show(m_debugwnd.GetWndHandle());
				}
				GtkAllocation allocation;
				allocation.width = 0;
				allocation.height = 0;
				gtk_widget_size_allocate(GetWndHandle(), &allocation);
			}

#endif // X88_GUI

			UpdateDebugWnd();
		}
		break;
	case IDM_DEBUG_EXECUTE:
		if (X88k().PC88().IsDebugMode()) {
			X88k().PC88().SetDebugStop(!X88k().PC88().IsDebugStopped());
			X88k().ResetExecuteCounter();
			UpdateDebugWnd();
		}
		break;
	case IDM_DEBUG_EXECUTE_STEP:
	case IDM_DEBUG_EXECUTE_TRACE:
	case IDM_DEBUG_EXECUTE_STEP2:
		if (X88k().PC88().IsDebugMode() &&
			X88k().PC88().IsDebugStopped())
		{
			int nDebugStep;
			switch (nCommand) {
			case IDM_DEBUG_EXECUTE_STEP:
				nDebugStep = CPC88::DEBUGSTEP_STEP;
				break;
			case IDM_DEBUG_EXECUTE_STEP2:
				nDebugStep = CPC88::DEBUGSTEP_STEP2;
				break;
			default:
				nDebugStep = CPC88::DEBUGSTEP_TRACE;
				break;
			}
			X88k().PC88().DebugExecuteStepTrace(nDebugStep);
			if (!X88k().PC88().IsDebugStopped()) {
				X88k().ResetExecuteCounter();
			}
			UpdateDebugWnd();
		}

#ifdef X88_GUI_WINDOWS

		if (GetFocus() != GetWndHandle()) {
			SetFocus(GetWndHandle());
		}

#endif // X88_GUI

		break;
	case IDM_DEBUG_DIS_ASSEMBLE:
		if (X88k().PC88().IsDebugMode() && X88k().PC88().IsDebugStopped()) {
			X88k().PauseSound();
			CX88DebugDisAssembleDlg dlgDDA(GetWndHandle());
			dlgDDA.SetZ80Adapter(X88k().PC88().GetDebugAdapter());
			dlgDDA.DoModal();
			X88k().PC88().UpdateMnemonic();
			X88k().RestartSound();
		}
		break;
	case IDM_DEBUG_DUMP:
		if (X88k().PC88().IsDebugMode() && X88k().PC88().IsDebugStopped()) {
			X88k().PauseSound();
			CX88DebugDumpDlg dlgDD(GetWndHandle());
			dlgDD.SetZ80Adapter(X88k().PC88().GetDebugAdapter());
			dlgDD.DoModal();
			X88k().RestartSound();
		}
		break;
	case IDM_DEBUG_WRITE_RAM:
		if (X88k().PC88().IsDebugMode() && X88k().PC88().IsDebugStopped()) {
			X88k().PauseSound();
			CX88DebugWriteRamDlg dlgDWR(GetWndHandle());
			if (X88k().PC88().IsDebugMain()) {
				dlgDWR.SetWriteSubRam(false);
			} else {
				dlgDWR.SetWriteMainRam0(false);
				dlgDWR.SetWriteMainRam1(false);
				dlgDWR.SetWriteFastTVRam(false);
				dlgDWR.SetWriteSlowTVRam(false);
				dlgDWR.SetWriteGVRam0(false);
				dlgDWR.SetWriteGVRam1(false);
				dlgDWR.SetWriteGVRam2(false);
			}
			dlgDWR.SetFastTVRamUse(
				X88k().PC88().Z80Main().IsFastTVRamUse());
			dlgDWR.SetSubSystemDisable(
				X88k().PC88().IsSubSystemDisableNow());
			if (dlgDWR.DoModal() == IDOK) {
				X88k().WriteRam(
					dlgDWR.IsWriteMainRam0(),
					dlgDWR.IsWriteMainRam1(),
					dlgDWR.IsWriteFastTVRam(),
					dlgDWR.IsWriteSlowTVRam(),
					dlgDWR.IsWriteGVRam0(),
					dlgDWR.IsWriteGVRam1(),
					dlgDWR.IsWriteGVRam2(),
					dlgDWR.IsWriteSubRam(),
					dlgDWR.IsWriteExRam0(),
					dlgDWR.IsWriteExRam1());
			}
			X88k().RestartSound();
		}
		break;
	case IDM_DEBUG_BREAK_POINT:
		if (X88k().PC88().IsDebugMode()) {
			X88k().PauseSound();
			CX88DebugBreakPointDlg dlgDBP(GetWndHandle());
			X88k().PC88().RemoveTmpBreakPoint();
			dlgDBP.SetPC(X88k().PC88().GetDebugPC());
			dlgDBP.SetBreakPoint(
				X88k().PC88().GetBreakPoint(
					X88k().PC88().IsDebugMain()));
			dlgDBP.DoModal();
			X88k().PC88().RestoreTmpBreakPoint();
			X88k().RestartSound();
		}
		break;
	case IDM_DEBUG_LOG:
		if (!X88k().IsDebugLogging()) {
			if (!X88k().StartDebugLog()) {

#ifdef X88_GUI_WINDOWS

				MessageBeep(MB_ICONEXCLAMATION);

#endif // X88_GUI

			}
		} else {
			X88k().EndDebugLog();
		}
		break;
	}
}

// help menu command

void CX88Frame::OnHelpMenuCommand(unsigned nCommand) {
	switch (nCommand) {
	case IDM_VERSION:
		{ // dummy block
			X88k().PauseSound();
			CX88VersionDlg dlgVersion(GetWndHandle());
			dlgVersion.DoModal();
			X88k().RestartSound();
		}
		break;
	}
}

////////////////////////////////////////////////////////////
// operation

// check menu command state
//
//     nCommand
//         menu command id
//     nCheck
//         -1 : not change, 0 : uncheck, 1 : check
//     nGray
//         -1 : not change, 0 : not gray, 1 : gray

void CX88Frame::CheckMenuCommandState(
	unsigned nCommand,
	int& nCheck, int& nGray)
{
	nCheck = -1;
	nGray = -1;
	switch (nCommand) {
	case IDM_BASICMODE_N:
		nCheck = (X88k().PC88().GetBasicMode() ==
			CPC88Z80Main::BASICMODE_N)? 1: 0;
		break;
	case IDM_BASICMODE_V1_S:
		nCheck = (!X88k().PC88().IsHighSpeedMode() &&
			(X88k().PC88().GetBasicMode() ==
				CPC88Z80Main::BASICMODE_N88V1))? 1: 0;
		break;
	case IDM_BASICMODE_V1_H:
		nCheck = (X88k().PC88().IsHighSpeedMode() &&
			(X88k().PC88().GetBasicMode() ==
				CPC88Z80Main::BASICMODE_N88V1))? 1: 0;
		break;
	case IDM_BASICMODE_V2:
		nCheck = (X88k().PC88().GetBasicMode() ==
			CPC88Z80Main::BASICMODE_N88V2)? 1: 0;
		break;
	case IDM_BASICMODE_N80_V1:
		nCheck = (X88k().PC88().GetBasicMode() ==
			CPC88Z80Main::BASICMODE_N80V1)? 1: 0;
		break;
	case IDM_BASICMODE_N80_V2:
		nCheck = (X88k().PC88().GetBasicMode() ==
			CPC88Z80Main::BASICMODE_N80V2)? 1: 0;
		break;
	case IDM_BASE_CLOCK_4:
		nCheck = (X88k().PC88().GetBaseClock() == 4)? 1: 0;
		break;
	case IDM_BASE_CLOCK_8:
		nCheck = (X88k().PC88().GetBaseClock() == 8)? 1: 0;
		break;
	case IDM_BOOST_MODE:
		nCheck = X88k().IsBoostMode()? 1: 0;
		break;
	case IDM_FULL_SCREEN_MODE:
		nCheck = IsFullScreen()? 1: 0;
		break;
	case IDM_CLIPBOARD_PASTE_TEXT:

#ifdef X88_GUI_WINDOWS

		nGray = IsClipboardFormatAvailable(CF_TEXT)? 0: 1;

#endif // X88_GUI

		break;
	case IDM_DEBUG_MAIN:
		nGray = IsFullScreen()? 1: 0;
		nCheck = ((nGray == 0) && X88k().PC88().IsDebugMain())? 1: 0;
		break;
	case IDM_DEBUG_SUB:
		nGray = (IsFullScreen() ||
			X88k().PC88().IsSubSystemDisableNow())? 1: 0;
		nCheck = ((nGray == 0) && X88k().PC88().IsDebugSub())? 1: 0;
		break;
	case IDM_DEBUG_EXECUTE:
		nGray = (!X88k().PC88().IsDebugMode())? 1: 0;
		nCheck = (X88k().PC88().IsDebugMode() &&
			!X88k().PC88().IsDebugStopped())? 1: 0;
		break;
	case IDM_DEBUG_EXECUTE_STEP:
	case IDM_DEBUG_EXECUTE_TRACE:
	case IDM_DEBUG_EXECUTE_STEP2:
	case IDM_DEBUG_DIS_ASSEMBLE:
	case IDM_DEBUG_DUMP:
	case IDM_DEBUG_WRITE_RAM:
		nGray = (!X88k().PC88().IsDebugMode() ||
			!X88k().PC88().IsDebugStopped())? 1: 0;
		break;
	case IDM_DEBUG_BREAK_POINT:
		nGray = (!X88k().PC88().IsDebugMode())? 1: 0;
		break;
	case IDM_DEBUG_LOG:
		nGray = (!X88k().PC88().IsDebugMode())? 1: 0;
		nCheck = X88k().IsDebugLogging()? 1: 0;
		break;
	case IDM_PARALLEL_DEVICE_SETTING:
		{ // dummy block
			int nDeviceNo = X88k().GetConnectedParallelDeviceNo();
			CX88Dialog* pdlgPara = X88k().ParallelManager()[nDeviceNo].Dialog();
			nGray = (pdlgPara == NULL);
			nCheck = (pdlgPara != NULL) && pdlgPara->IsModeless() && pdlgPara->IsCreatedModeless();
		}
		break;
	case IDM_PARALLEL_DEVICE0:
	case IDM_PARALLEL_DEVICE1:
	case IDM_PARALLEL_DEVICE2:
	case IDM_PARALLEL_DEVICE3:
	case IDM_PARALLEL_DEVICE4:
	case IDM_PARALLEL_DEVICE5:
	case IDM_PARALLEL_DEVICE6:
	case IDM_PARALLEL_DEVICE7:
	case IDM_PARALLEL_DEVICE8:
	case IDM_PARALLEL_DEVICE9:
		{ // dummy block
			int nDeviceNo = nCommand-IDM_PARALLEL_DEVICE0;
			nCheck = (nDeviceNo == X88k().GetConnectedParallelDeviceNo());
		}
		break;
	}
}

#ifdef X88_GUI_WINDOWS

// modify frame menu

void CX88Frame::ModifyFrameMenu(HMENU hMenu) {
	std::string strMenu;
	int nMenuCount = GetMenuItemCount(hMenu);
	for (int nMenuPos = 0; nMenuPos < nMenuCount; nMenuPos++) {
		UINT nID = GetMenuItemID(hMenu, nMenuPos);
		int nCheck, nGray;
		CheckMenuCommandState(nID, nCheck, nGray);
		switch (nID) {
		case IDM_PARALLEL_DEVICE0:
			{ // dummy block
				int nDevCount = (int)X88k().ParallelManager().size(),
					nMenuDevCount = nMenuCount-nMenuPos;
				if (nMenuDevCount < nDevCount) {
					for (; nMenuDevCount < nDevCount; nMenuDevCount++) {
						InsertMenu(
							hMenu,
							nMenuCount++,
							MF_BYPOSITION | MF_STRING,
							IDM_PARALLEL_DEVICE0+nMenuDevCount,
							"");
					}
				} else if (nMenuDevCount > nDevCount) {
					for (; nMenuDevCount > nDevCount; nMenuDevCount--) {
						DeleteMenu(
							hMenu,
							--nMenuCount,
							MF_BYPOSITION);
					}
				}
			}
			// no break
		case IDM_PARALLEL_DEVICE1:
		case IDM_PARALLEL_DEVICE2:
		case IDM_PARALLEL_DEVICE3:
		case IDM_PARALLEL_DEVICE4:
		case IDM_PARALLEL_DEVICE5:
		case IDM_PARALLEL_DEVICE6:
		case IDM_PARALLEL_DEVICE7:
		case IDM_PARALLEL_DEVICE8:
		case IDM_PARALLEL_DEVICE9:
			{ // dummy block
				int nDeviceNo = nID-IDM_PARALLEL_DEVICE0;
				strMenu = FormatStr(
					"%s(&%d)",
					NX88Resource::GetParallelDeviceNameFromID(
						X88k().ParallelManager()[nDeviceNo].Device()->
							GetDeviceID()).c_str(),
					nDeviceNo);
				ModifyMenu(
					hMenu,
					nMenuPos,
					MF_BYPOSITION | MF_STRING,
					nID,
					strMenu.c_str());
			}
			break;
		}
		if (nCheck >= 0) {
			CheckMenuItem(
				hMenu,
				nMenuPos,
				((nCheck != 0)? MF_CHECKED: MF_UNCHECKED) |
					MF_BYPOSITION);
		}
		if (nGray >= 0) {
			EnableMenuItem(
				hMenu,
				nMenuPos,
				((nGray != 0)? MF_GRAYED: MF_ENABLED) |
					MF_BYPOSITION);
		}
	}
}

#elif defined(X88_GUI_GTK)

// convert key value

guint CX88Frame::KeyValueConvert(guint nKeyVal) {
	guint nKeyVal2 = gdk_keyval_to_lower(nKeyVal);
	if ((nKeyVal >> 8) == 0xFF) {
		switch (nKeyVal) {
		case GDK_KP_Insert:
			nKeyVal2 = GDK_KP_0;
			break;
		case GDK_KP_End:
			nKeyVal2 = GDK_KP_1;
			break;
		case GDK_KP_Down:
			nKeyVal2 = GDK_KP_2;
			break;
		case GDK_KP_Next:
			nKeyVal2 = GDK_KP_3;
			break;
		case GDK_KP_Left:
			nKeyVal2 = GDK_KP_4;
			break;
		case GDK_KP_Begin:
			nKeyVal2 = GDK_KP_5;
			break;
		case GDK_KP_Right:
			nKeyVal2 = GDK_KP_6;
			break;
		case GDK_KP_Home:
			nKeyVal2 = GDK_KP_7;
			break;
		case GDK_KP_Up:
			nKeyVal2 = GDK_KP_8;
			break;
		case GDK_KP_Prior:
			nKeyVal2 = GDK_KP_9;
			break;
		case GDK_KP_Delete:
			nKeyVal2 = GDK_KP_Decimal;
			break;
		case GDK_Meta_L:
			nKeyVal2 = GDK_Alt_L;
			break;
		case GDK_Meta_R:
			nKeyVal2 = GDK_Alt_R;
			break;
		case GDK_Eisu_toggle:
			nKeyVal2 = GDK_Caps_Lock;
			break;
		case GDK_Break:
			nKeyVal2 = GDK_Pause;
			break;
		case GDK_Execute:
			nKeyVal2 = GDK_Print;
			break;
		case GDK_Romaji:
			nKeyVal2 = GDK_Hiragana_Katakana;
			break;
		case GDK_Mode_switch:
			nKeyVal2 = GDK_Henkan_Mode;
			break;
		case GDK_Kanji:
			nKeyVal2 = GDK_Zenkaku_Hankaku;
			break;
		}
	} else if ((nKeyVal >> 8) == 0) {
		switch (nKeyVal) {
		case GDK_exclam:
			nKeyVal2 = GDK_1;
			break;
		case GDK_quotedbl:
			nKeyVal2 = GDK_2;
			break;
		case GDK_numbersign:
			nKeyVal2 = GDK_3;
			break;
		case GDK_dollar:
			nKeyVal2 = GDK_4;
			break;
		case GDK_percent:
			nKeyVal2 = GDK_5;
			break;
		case GDK_ampersand:
			nKeyVal2 = GDK_6;
			break;
		case GDK_apostrophe:
			nKeyVal2 = GDK_7;
			break;
		case GDK_parenleft:
			nKeyVal2 = GDK_8;
			break;
		case GDK_parenright:
			nKeyVal2 = GDK_9;
			break;
		case GDK_asciitilde:
			nKeyVal2 = GDK_asciicircum;
			break;
		case GDK_equal:
			nKeyVal2 = GDK_minus;
			break;
		case GDK_grave:
			nKeyVal2 = GDK_at;
			break;
		case GDK_braceleft:
			nKeyVal2 = GDK_bracketleft;
			break;
		case GDK_plus:
			nKeyVal2 = GDK_semicolon;
			break;
		case GDK_asterisk:
			nKeyVal2 = GDK_colon;
			break;
		case GDK_braceright:
			nKeyVal2 = GDK_bracketright;
			break;
		case GDK_less:
			nKeyVal2 = GDK_comma;
			break;
		case GDK_greater:
			nKeyVal2 = GDK_period;
			break;
		case GDK_question:
			nKeyVal2 = GDK_slash;
			break;
		}
	} else if ((nKeyVal >> 8) == 4) {
		switch (nKeyVal) {
		case GDK_overline:
			nKeyVal2 = GDK_asciitilde;
			break;
		case GDK_kana_fullstop:
			nKeyVal2 = GDK_period;
			break;
		case GDK_kana_openingbracket:
			nKeyVal2 = GDK_bracketleft;
			break;
		case GDK_kana_closingbracket:
			nKeyVal2 = GDK_bracketright;
			break;
		case GDK_kana_comma:
			nKeyVal2 = GDK_comma;
			break;
		case GDK_kana_middledot:
			nKeyVal2 = GDK_slash;
			break;
		case GDK_kana_WO:
			nKeyVal2 = GDK_0;
			break;
		case GDK_kana_a:
			nKeyVal2 = GDK_3;
			break;
		case GDK_kana_i:
			nKeyVal2 = GDK_e;
			break;
		case GDK_kana_u:
			nKeyVal2 = GDK_4;
			break;
		case GDK_kana_e:
			nKeyVal2 = GDK_5;
			break;
		case GDK_kana_o:
			nKeyVal2 = GDK_6;
			break;
		case GDK_kana_ya:
			nKeyVal2 = GDK_7;
			break;
		case GDK_kana_yu:
			nKeyVal2 = GDK_8;
			break;
		case GDK_kana_yo:
			nKeyVal2 = GDK_9;
			break;
		case GDK_kana_tsu:
			nKeyVal2 = GDK_z;
			break;
		case GDK_prolongedsound:
			nKeyVal2 = GDK_yen;
			break;
		case GDK_kana_A:
			nKeyVal2 = GDK_3;
			break;
		case GDK_kana_I:
			nKeyVal2 = GDK_e;
			break;
		case GDK_kana_U:
			nKeyVal2 = GDK_4;
			break;
		case GDK_kana_E:
			nKeyVal2 = GDK_5;
			break;
		case GDK_kana_O:
			nKeyVal2 = GDK_6;
			break;
		case GDK_kana_KA:
			nKeyVal2 = GDK_t;
			break;
		case GDK_kana_KI:
			nKeyVal2 = GDK_g;
			break;
		case GDK_kana_KU:
			nKeyVal2 = GDK_h;
			break;
		case GDK_kana_KE:
			nKeyVal2 = GDK_colon;
			break;
		case GDK_kana_KO:
			nKeyVal2 = GDK_b;
			break;
		case GDK_kana_SA:
			nKeyVal2 = GDK_x;
			break;
		case GDK_kana_SHI:
			nKeyVal2 = GDK_d;
			break;
		case GDK_kana_SU:
			nKeyVal2 = GDK_r;
			break;
		case GDK_kana_SE:
			nKeyVal2 = GDK_p;
			break;
		case GDK_kana_SO:
			nKeyVal2 = GDK_c;
			break;
		case GDK_kana_TA:
			nKeyVal2 = GDK_q;
			break;
		case GDK_kana_CHI:
			nKeyVal2 = GDK_a;
			break;
		case GDK_kana_TSU:
			nKeyVal2 = GDK_z;
			break;
		case GDK_kana_TE:
			nKeyVal2 = GDK_w;
			break;
		case GDK_kana_TO:
			nKeyVal2 = GDK_s;
			break;
		case GDK_kana_NA:
			nKeyVal2 = GDK_u;
			break;
		case GDK_kana_NI:
			nKeyVal2 = GDK_i;
			break;
		case GDK_kana_NU:
			nKeyVal2 = GDK_1;
			break;
		case GDK_kana_NE:
			nKeyVal2 = GDK_comma;
			break;
		case GDK_kana_NO:
			nKeyVal2 = GDK_k;
			break;
		case GDK_kana_HA:
			nKeyVal2 = GDK_f;
			break;
		case GDK_kana_HI:
			nKeyVal2 = GDK_v;
			break;
		case GDK_kana_FU:
			nKeyVal2 = GDK_2;
			break;
		case GDK_kana_HE:
			nKeyVal2 = GDK_asciicircum;
			break;
		case GDK_kana_HO:
			nKeyVal2 = GDK_minus;
			break;
		case GDK_kana_MA:
			nKeyVal2 = GDK_j;
			break;
		case GDK_kana_MI:
			nKeyVal2 = GDK_n;
			break;
		case GDK_kana_MU:
			nKeyVal2 = GDK_bracketright;
			break;
		case GDK_kana_ME:
			nKeyVal2 = GDK_slash;
			break;
		case GDK_kana_MO:
			nKeyVal2 = GDK_m;
			break;
		case GDK_kana_YA:
			nKeyVal2 = GDK_7;
			break;
		case GDK_kana_YU:
			nKeyVal2 = GDK_8;
			break;
		case GDK_kana_YO:
			nKeyVal2 = GDK_9;
			break;
		case GDK_kana_RA:
			nKeyVal2 = GDK_o;
			break;
		case GDK_kana_RI:
			nKeyVal2 = GDK_l;
			break;
		case GDK_kana_RU:
			nKeyVal2 = GDK_period;
			break;
		case GDK_kana_RE:
			nKeyVal2 = GDK_semicolon;
			break;
		case GDK_kana_RO:
			nKeyVal2 = GDK_underscore;
			break;
		case GDK_kana_WA:
			nKeyVal2 = GDK_0;
			break;
		case GDK_kana_N:
			nKeyVal2 = GDK_y;
			break;
		case GDK_voicedsound:
			nKeyVal2 = GDK_at;
			break;
		case GDK_semivoicedsound:
			nKeyVal2 = GDK_bracketleft;
			break;
		}
	}
	return nKeyVal2;
}

// set menu item state

void CX88Frame::SetMenuItemState(guint nAction) {
	GtkItemFactory* pFactory = gtk_item_factory_from_widget(m_hwndMenuBar);
	if (pFactory == NULL) {
		return;
	}
	GtkWidget* pWidget = gtk_item_factory_get_widget_by_action(
		pFactory, nAction);
	if (pWidget == NULL) {
		return;
	}
	int nCheck, nGray;
	CheckMenuCommandState(nAction, nCheck, nGray);
	if ((nCheck >= 0) &&
		GTK_IS_CHECK_MENU_ITEM(pWidget) &&
		((nCheck > 0) ||
			!GTK_IS_RADIO_MENU_ITEM(pWidget)))
	{
		gtk_check_menu_item_set_active(
			GTK_CHECK_MENU_ITEM(pWidget), nCheck);
	}
	if (nGray >= 0) {
		gtk_widget_set_sensitive(pWidget, (nGray == 0)? 1: 0);
	}
}

// set all menu items state

void CX88Frame::SetAllMenuItemState() {
	CX88Dialog::CSignalBlocker blocker(GetWndHandle());
	SetMenuItemState(IDM_BASICMODE_N);
	SetMenuItemState(IDM_BASICMODE_V1_S);
	SetMenuItemState(IDM_BASICMODE_V1_H);
	SetMenuItemState(IDM_BASICMODE_V2);
	SetMenuItemState(IDM_BASICMODE_N80_V1);
	SetMenuItemState(IDM_BASICMODE_N80_V2);
	SetMenuItemState(IDM_BASE_CLOCK_4);
	SetMenuItemState(IDM_BASE_CLOCK_8);
	SetMenuItemState(IDM_BOOST_MODE);
	SetMenuItemState(IDM_FULL_SCREEN_MODE);
	SetMenuItemState(IDM_PARALLEL_DEVICE_SETTING);
	SetMenuItemState(IDM_PARALLEL_DEVICE0);
	SetMenuItemState(IDM_PARALLEL_DEVICE1);
	SetMenuItemState(IDM_PARALLEL_DEVICE2);
	SetMenuItemState(IDM_PARALLEL_DEVICE3);
	SetMenuItemState(IDM_PARALLEL_DEVICE4);
	SetMenuItemState(IDM_PARALLEL_DEVICE5);
	SetMenuItemState(IDM_PARALLEL_DEVICE6);
	SetMenuItemState(IDM_PARALLEL_DEVICE7);
	SetMenuItemState(IDM_PARALLEL_DEVICE8);
	SetMenuItemState(IDM_PARALLEL_DEVICE9);
	SetMenuItemState(IDM_DEBUG_MAIN);
	SetMenuItemState(IDM_DEBUG_SUB);
	SetMenuItemState(IDM_DEBUG_EXECUTE);
	SetMenuItemState(IDM_DEBUG_EXECUTE_STEP);
	SetMenuItemState(IDM_DEBUG_EXECUTE_TRACE);
	SetMenuItemState(IDM_DEBUG_EXECUTE_STEP2);
	SetMenuItemState(IDM_DEBUG_DIS_ASSEMBLE);
	SetMenuItemState(IDM_DEBUG_DUMP);
	SetMenuItemState(IDM_DEBUG_BREAK_POINT);
	SetMenuItemState(IDM_DEBUG_LOG);
	SetMenuItemState(IDM_DEBUG_WRITE_RAM);
}

#ifdef X88_GUI_GTK_USEIM

// start IM

bool CX88Frame::StartIM() {
	if ((GetWndHandle() == NULL) ||
		(GetWndHandle()->window == NULL))
	{
		return false;
	}
	if (m_pIMC != NULL) {
		return false;
	}
	m_pIMC = gtk_im_multicontext_new();
	if (m_pIMC == NULL) {
		StopIM();
		return false;
	}
	m_bPreEditing = false;
	gtk_im_context_set_client_window(m_pIMC, GetWndHandle()->window);
	g_signal_connect(
		m_pIMC, "commit",
		G_CALLBACK(OnIMCommitSignal), NULL);
	g_signal_connect(
		m_pIMC, "preedit-start",
		G_CALLBACK(OnIMPreEditStartSignal), NULL);
	g_signal_connect(
		m_pIMC, "preedit-end",
		G_CALLBACK(OnIMPreEditEndSignal), NULL);
	g_signal_connect(
		m_pIMC, "preedit-changed",
		G_CALLBACK(OnIMPreEditChangedSignal), NULL);
	return m_pIMC != NULL;
}

// stop IM

bool CX88Frame::StopIM() {
	m_bPreEditing = false;
	DeleteIMPreEdit();
	UpdateIMPreEditWindow();
	if (m_pIMC != NULL) {
		gtk_im_context_reset(m_pIMC);
		gtk_im_context_set_client_window(m_pIMC, NULL);
		g_object_unref(m_pIMC);
		m_pIMC = NULL;
	}
	return true;
}

// delete IM pre-edit

void CX88Frame::DeleteIMPreEdit() {
	if (m_pgszIMPreEdit != NULL) {
		g_free(m_pgszIMPreEdit);
		m_pgszIMPreEdit = NULL;
	}
	if (m_pattrIMPreEdit != NULL) {
		pango_attr_list_unref(m_pattrIMPreEdit);
		m_pattrIMPreEdit = NULL;
	}
	m_nIMPreEditCursorPos = 0;
}

// update IM pre-edit window

void CX88Frame::UpdateIMPreEditWindow() {
	if (m_pgszIMPreEdit != NULL) {
		bool bShow = false;
		if (m_hwndPreEditWindow == NULL) {
			bShow = true;
			m_hwndPreEditWindow = gtk_window_new(GTK_WINDOW_POPUP);
			if (m_hwndPreEditWindow == NULL) {
				return;
			}
			g_object_ref(m_hwndPreEditWindow);
			m_hwndPreEditDArea = gtk_drawing_area_new();
			if (m_hwndPreEditDArea == NULL) {
				g_object_unref(m_hwndPreEditWindow);
				m_hwndPreEditWindow = NULL;
				return;
			}
			g_object_ref(m_hwndPreEditDArea);
			gtk_window_move(
				GTK_WINDOW(m_hwndPreEditWindow),
				0, 0);
			gtk_container_add(
				GTK_CONTAINER(m_hwndPreEditWindow),
				m_hwndPreEditDArea);
			g_signal_connect(
				m_hwndPreEditDArea, "expose_event",
				G_CALLBACK(OnIMPreEditExposeEvent), NULL);
			gtk_widget_show(m_hwndPreEditDArea);
		}
		int x = 0, y = 0, cx = 1, cy = 1;
		PangoLayout* pLayout = gtk_widget_create_pango_layout(
			GetWndHandle(),
			m_pgszIMPreEdit);
		if (pLayout != NULL) {
			pango_layout_set_attributes(
				pLayout,
				m_pattrIMPreEdit);
			pango_layout_get_pixel_size(
				pLayout,
				&cx, &cy);
			g_object_unref(pLayout);
		}
		if ((m_view.GetWndHandle() != NULL) &&
			(m_view.GetWndHandle()->window != NULL))
		{
			gdk_window_get_origin(
				m_view.GetWndHandle()->window,
				&x, &y);
			y += 400-cy;
		}
		gtk_window_move(
			GTK_WINDOW(m_hwndPreEditWindow),
			x, y);
		gtk_window_resize(
			GTK_WINDOW(m_hwndPreEditWindow),
			cx, cy);
		if (bShow) {
			gtk_widget_show(m_hwndPreEditWindow);
		}
		gtk_widget_queue_draw(m_hwndPreEditWindow);
	} else {
		if (m_hwndPreEditWindow != NULL) {
			gtk_widget_hide(m_hwndPreEditWindow);
			if (m_hwndPreEditDArea != NULL) {
				g_object_unref(m_hwndPreEditDArea);
				m_hwndPreEditDArea = NULL;
			}
			gtk_widget_destroy(m_hwndPreEditWindow);
			g_object_unref(m_hwndPreEditWindow);
			m_hwndPreEditWindow = NULL;
		}
	}
}

#endif // X88_GUI_GTK_USEIM

#endif // X88_GUI

// update debug window

void CX88Frame::UpdateDebugWnd() {
	m_debugwnd.UpdateDebugWnd();
	if ((GetWndHandle() != NULL) &&
		X88k().PC88().IsDebugMode() && X88k().PC88().IsDebugStopped())
	{
		std::string gstrCaption;
		gstrCaption = FormatStr(
			RSTR("X88000 - DEBUG<%s>"),
			RSTR(
				X88k().PC88().IsDebugMain()? "MainCPU": "SubCPU"));

#ifdef X88_GUI_WINDOWS

		SetWindowText(
			GetWndHandle(), gstrCaption.c_str());

#elif defined(X88_GUI_GTK)

		gtk_window_set_title(
			GTK_WINDOW(GetWndHandle()), gstrCaption.c_str());

#endif // X88_GUI

	}
}

// close parallel-device modeless dialog

void CX88Frame::CloseParallalelModeless() {
	int nDeviceNo = X88k().GetConnectedParallelDeviceNo();
	CX88Dialog* pdlgPara = X88k().ParallelManager()[nDeviceNo].Dialog();
	if ((pdlgPara != NULL) &&
		pdlgPara->IsModeless() &&
		pdlgPara->IsCreatedModeless())
	{
		pdlgPara->CloseModeless();
	}
}

// show executing frequency

void CX88Frame::ShowFrequency(double fMHz) {
	if ((GetWndHandle() != NULL) &&
		(!X88k().PC88().IsDebugMode() || !X88k().PC88().IsDebugStopped()))
	{
		std::string gstrCaption;
		gstrCaption = FormatStr(
			RSTR("X88000 - %4.2lfMHz%s"),
			fMHz,
			RSTR(X88k().IsBoostMode()? "<B>": ""));

#ifdef X88_GUI_WINDOWS

		SetWindowText(
			GetWndHandle(), gstrCaption.c_str());

#elif defined(X88_GUI_GTK)

		gtk_window_set_title(
			GTK_WINDOW(GetWndHandle()), gstrCaption.c_str());

#endif // X88_GUI

	}
}

// set full-screen mode

bool CX88Frame::SetFullScreen(bool bFullScreenNew) {
	bool bResult = true;

#ifdef X88_GUI_WINDOWS

	m_bFullScreen = bFullScreenNew;
	if (m_bFullScreen) {
		uint32_t dwStyle = GetWindowLong(GetWndHandle(), GWL_STYLE);
		dwStyle &= ~(WS_THICKFRAME | WS_CAPTION | WS_SYSMENU |
			WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
			WS_CLIPCHILDREN);
		SetWindowLong(GetWndHandle(), GWL_STYLE, dwStyle);
		SetWindowPos(
			GetWndHandle(), NULL,
			0, 0, 640, 480,
			SWP_NOACTIVATE | SWP_NOZORDER);
		if (!X88k().GetDX().StartDirectDraw(
				GetWndHandle()))
		{
			X88k().GetDX().EndDirectDraw();
			SetFullScreen(false);
			bResult = false;
		} else {
			SetWindowPos(
				GetWndHandle(), NULL,
				0, 0, 640, 480,
				SWP_NOACTIVATE | SWP_NOZORDER);
			m_bMouseHoveringOnMenuBar = false;
			m_bActivatedGDI = false;
			UpdateWindow(GetWndHandle());
			m_view.SetFrameClear(true);
		}
	} else {
		X88k().GetDX().EndDirectDraw();
		uint32_t dwStyle = GetWindowLong(GetWndHandle(), GWL_STYLE);
		dwStyle |= WS_THICKFRAME | WS_CAPTION | WS_SYSMENU |
			WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
			WS_CLIPCHILDREN;
		SetWindowLong(GetWndHandle(), GWL_STYLE, dwStyle);
		RECT rect;
		SetRect(&rect, 0, 0, 640, 400);
		AdjustWindowRect(&rect, dwStyle, TRUE);
		SetWindowPos(
			GetWndHandle(), NULL,
			0, 0, rect.right-rect.left, rect.bottom-rect.top,
			SWP_NOMOVE | SWP_NOZORDER);
		SetWindowPos(
			GetWndHandle(), HWND_BOTTOM,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
		SetWindowPos(
			GetWndHandle(), HWND_TOP,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE);
	}

#elif defined(X88_GUI_GTK)

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	(void)bFullScreenNew;

#endif // X88_GUI

	return bResult;
}

// activate gui

bool CX88Frame::ActivateGDI(bool bActivate) {
	bool bOldActivate = m_bActivatedGDI;
	m_bActivatedGDI = bActivate;
	bool bResult = false;

#ifdef X88_GUI_WINDOWS

	if (X88k().GetDX().IsDirectDrawActive()) {
		if (bActivate) {
			m_view.SetFrameClear(false);
			DrawMenuBar(GetWndHandle());
		} else if (bOldActivate) {
			m_view.SetFrameClear(true);
			m_view.DrawSurface();
		}
	}

#elif defined(X88_GUI_GTK)

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	(void)bOldActivate;

#endif // X88_GUI

	m_bMouseHoveringOnMenuBar = false;
	return bResult;
}

// update key matrics

void CX88Frame::UpdateAllKeyMatrics() {
	X88k().ClearKeyMatrics();

#ifdef X88_GUI_WINDOWS

	if (!m_bHaveFocus || m_bIMEOpen) {
		return;
	}
	if (!X88k().GetDX().IsDirectInputActive()) {
		if (!GetKeyboardState(m_abtKeyboardState)) {
			return;
		}
		if (IsKeyPressed(VK_MENU)) {
			if (IsKeyPressed(VK_F1)) {
				X88k().SetKeyMatrics(
					0x0A, 4,
					true);
				X88k().SetKeyMatrics(
					0x08, 4,
					IsKeyPressed(VK_SHIFT));
				X88k().SetKeyMatrics(
					0x08, 7,
					IsKeyPressed(VK_CONTROL));
			}
			return;
		}
		bool bShift;
		if (IsKeyPressed(VK_F6) ||
			IsKeyPressed(VK_F7) ||
			IsKeyPressed(VK_F8) ||
			IsKeyPressed(VK_F9) ||
			IsKeyPressed(VK_F10) ||
			IsKeyPressed(VK_INSERT))
		{
			bShift = true;
		} else if (IsKeyPressed(VK_DELETE)) {
			bShift = false;
		} else {
			bShift = IsKeyPressed(VK_SHIFT);
		}
		bool bUpArrow = IsKeyPressed(VK_UP),
			bRightArrow = IsKeyPressed(VK_RIGHT);
		if (X88k().PC88().Z80Main().GetBasicMode() ==
				CPC88Z80Main::BASICMODE_N80V1)
		{
			if (IsKeyPressed(VK_DOWN)) {
				bShift = bUpArrow = true;
			}
			if (IsKeyPressed(VK_LEFT)) {
				bShift = bRightArrow = true;
			}
		}
		X88k().SetKeyMatrics(0x00, 0, IsKeyPressed(VK_NUMPAD0));
		X88k().SetKeyMatrics(0x00, 1, IsKeyPressed(VK_NUMPAD1));
		X88k().SetKeyMatrics(0x00, 2, IsKeyPressed(VK_NUMPAD2));
		X88k().SetKeyMatrics(0x00, 3, IsKeyPressed(VK_NUMPAD3));
		X88k().SetKeyMatrics(0x00, 4, IsKeyPressed(VK_NUMPAD4));
		X88k().SetKeyMatrics(0x00, 5, IsKeyPressed(VK_NUMPAD5));
		X88k().SetKeyMatrics(0x00, 6, IsKeyPressed(VK_NUMPAD6));
		X88k().SetKeyMatrics(0x00, 7, IsKeyPressed(VK_NUMPAD7));
		X88k().SetKeyMatrics(0x01, 0, IsKeyPressed(VK_NUMPAD8));
		X88k().SetKeyMatrics(0x01, 1, IsKeyPressed(VK_NUMPAD9));
		X88k().SetKeyMatrics(0x01, 2, IsKeyPressed(VK_MULTIPLY));
		X88k().SetKeyMatrics(0x01, 3, IsKeyPressed(VK_ADD));
		X88k().SetKeyMatrics(0x01, 6, IsKeyPressed(VK_DECIMAL));
		X88k().SetKeyMatrics(0x01, 7, IsKeyPressed(VK_RETURN));
		X88k().SetKeyMatrics(0x02, 0, IsKeyPressed(0xC0));
		X88k().SetKeyMatrics(0x02, 1, IsKeyPressed('A'));
		X88k().SetKeyMatrics(0x02, 2, IsKeyPressed('B'));
		X88k().SetKeyMatrics(0x02, 3, IsKeyPressed('C'));
		X88k().SetKeyMatrics(0x02, 4, IsKeyPressed('D'));
		X88k().SetKeyMatrics(0x02, 5, IsKeyPressed('E'));
		X88k().SetKeyMatrics(0x02, 6, IsKeyPressed('F'));
		X88k().SetKeyMatrics(0x02, 7, IsKeyPressed('G'));
		X88k().SetKeyMatrics(0x03, 0, IsKeyPressed('H'));
		X88k().SetKeyMatrics(0x03, 1, IsKeyPressed('I'));
		X88k().SetKeyMatrics(0x03, 2, IsKeyPressed('J'));
		X88k().SetKeyMatrics(0x03, 3, IsKeyPressed('K'));
		X88k().SetKeyMatrics(0x03, 4, IsKeyPressed('L'));
		X88k().SetKeyMatrics(0x03, 5, IsKeyPressed('M'));
		X88k().SetKeyMatrics(0x03, 6, IsKeyPressed('N'));
		X88k().SetKeyMatrics(0x03, 7, IsKeyPressed('O'));
		X88k().SetKeyMatrics(0x04, 0, IsKeyPressed('P'));
		X88k().SetKeyMatrics(0x04, 1, IsKeyPressed('Q'));
		X88k().SetKeyMatrics(0x04, 2, IsKeyPressed('R'));
		X88k().SetKeyMatrics(0x04, 3, IsKeyPressed('S'));
		X88k().SetKeyMatrics(0x04, 4, IsKeyPressed('T'));
		X88k().SetKeyMatrics(0x04, 5, IsKeyPressed('U'));
		X88k().SetKeyMatrics(0x04, 6, IsKeyPressed('V'));
		X88k().SetKeyMatrics(0x04, 7, IsKeyPressed('W'));
		X88k().SetKeyMatrics(0x05, 0, IsKeyPressed('X'));
		X88k().SetKeyMatrics(0x05, 1, IsKeyPressed('Y'));
		X88k().SetKeyMatrics(0x05, 2, IsKeyPressed('Z'));
		X88k().SetKeyMatrics(0x05, 3, IsKeyPressed(0xDB));
		X88k().SetKeyMatrics(0x05, 4, IsKeyPressed(0xDC));
		X88k().SetKeyMatrics(0x05, 5, IsKeyPressed(0xDD));
		X88k().SetKeyMatrics(0x05, 6, IsKeyPressed(0xDE));
		X88k().SetKeyMatrics(0x05, 7, IsKeyPressed(0xBD));
		X88k().SetKeyMatrics(0x06, 0, IsKeyPressed('0'));
		X88k().SetKeyMatrics(0x06, 1, IsKeyPressed('1'));
		X88k().SetKeyMatrics(0x06, 2, IsKeyPressed('2'));
		X88k().SetKeyMatrics(0x06, 3, IsKeyPressed('3'));
		X88k().SetKeyMatrics(0x06, 4, IsKeyPressed('4'));
		X88k().SetKeyMatrics(0x06, 5, IsKeyPressed('5'));
		X88k().SetKeyMatrics(0x06, 6, IsKeyPressed('6'));
		X88k().SetKeyMatrics(0x06, 7, IsKeyPressed('7'));
		X88k().SetKeyMatrics(0x07, 0, IsKeyPressed('8'));
		X88k().SetKeyMatrics(0x07, 1, IsKeyPressed('9'));
		X88k().SetKeyMatrics(0x07, 2, IsKeyPressed(0xBA));
		X88k().SetKeyMatrics(0x07, 3, IsKeyPressed(0xBB));
		X88k().SetKeyMatrics(0x07, 4, IsKeyPressed(0xBC));
		X88k().SetKeyMatrics(0x07, 5, IsKeyPressed(0xBE));
		X88k().SetKeyMatrics(0x07, 6, IsKeyPressed(0xBF));
		X88k().SetKeyMatrics(0x07, 7, IsKeyPressed(0xE2));
		X88k().SetKeyMatrics(0x08, 0, IsKeyPressed(VK_HOME));
		X88k().SetKeyMatrics(0x08, 1, bUpArrow);
		X88k().SetKeyMatrics(0x08, 2, bRightArrow);
		X88k().SetKeyMatrics(
			0x08, 3,
			IsKeyPressed(VK_BACK, VK_INSERT, VK_DELETE));
		X88k().SetKeyMatrics(0x08, 4, IsKeyPressed(VK_F12));
		X88k().SetKeyMatrics(
			0x08, 5,
			IsKeyToggled(VK_KANA) ||
			IsKeyToggled(VK_SCROLL));
		X88k().SetKeyMatrics(0x08, 6, bShift);
		X88k().SetKeyMatrics(0x08, 7, IsKeyPressed(VK_CONTROL));
		X88k().SetKeyMatrics(0x09, 0, IsKeyPressed(VK_F11));
		X88k().SetKeyMatrics(
			0x09, 1,
			IsKeyPressed(VK_F1, VK_F6));
		X88k().SetKeyMatrics(
			0x09, 2,
			IsKeyPressed(VK_F2, VK_F7));
		X88k().SetKeyMatrics(
			0x09, 3,
			IsKeyPressed(VK_F3, VK_F8));
		X88k().SetKeyMatrics(
			0x09, 4,
			IsKeyPressed(VK_F4, VK_F9));
		X88k().SetKeyMatrics(
			0x09, 5,
			IsKeyPressed(VK_F5, VK_F10));
		X88k().SetKeyMatrics(0x09, 6, IsKeyPressed(VK_SPACE));
		X88k().SetKeyMatrics(0x09, 7, IsKeyPressed(VK_ESCAPE));
		X88k().SetKeyMatrics(0x0A, 0, IsKeyPressed(VK_TAB));
		X88k().SetKeyMatrics(0x0A, 1, IsKeyPressed(VK_DOWN));
		X88k().SetKeyMatrics(0x0A, 2, IsKeyPressed(VK_LEFT));
		X88k().SetKeyMatrics(0x0A, 3, IsKeyPressed(VK_END));
		X88k().SetKeyMatrics(0x0A, 4, IsKeyPressed(VK_SNAPSHOT));
		X88k().SetKeyMatrics(0x0A, 5, IsKeyPressed(VK_SUBTRACT));
		X88k().SetKeyMatrics(0x0A, 6, IsKeyPressed(VK_DIVIDE));
		X88k().SetKeyMatrics(0x0A, 7, IsKeyToggled(VK_CAPITAL));
		X88k().SetKeyMatrics(0x0B, 0, IsKeyPressed(VK_NEXT));
		X88k().SetKeyMatrics(0x0B, 1, IsKeyPressed(VK_PRIOR));
		X88k().SetKeyMatrics(0x0C, 0, IsKeyPressed(VK_F6));
		X88k().SetKeyMatrics(0x0C, 1, IsKeyPressed(VK_F7));
		X88k().SetKeyMatrics(0x0C, 2, IsKeyPressed(VK_F8));
		X88k().SetKeyMatrics(0x0C, 3, IsKeyPressed(VK_F9));
		X88k().SetKeyMatrics(0x0C, 4, IsKeyPressed(VK_F10));
		X88k().SetKeyMatrics(0x0C, 5, IsKeyPressed(VK_BACK));
		X88k().SetKeyMatrics(0x0C, 6, IsKeyPressed(VK_INSERT));
		X88k().SetKeyMatrics(0x0C, 7, IsKeyPressed(VK_DELETE));
		X88k().SetKeyMatrics(0x0D, 0, IsKeyPressed(VK_CONVERT));
		X88k().SetKeyMatrics(0x0D, 1, IsKeyPressed(VK_NONCONVERT));
		X88k().SetKeyMatrics(0x0E, 0, IsKeyPressed(VK_RETURN));
		X88k().SetKeyMatrics(0x0E, 2, IsKeyPressed(VK_LSHIFT));
		X88k().SetKeyMatrics(0x0E, 3, IsKeyPressed(VK_RSHIFT));
	} else {
		HRESULT hResult;
		hResult = X88k().GetDX().GetDIDKeyboard()->GetDeviceState(
			sizeof(m_abtKeyboardState),
			m_abtKeyboardState);
		if ((hResult == DIERR_NOTACQUIRED) ||
			(hResult == DIERR_INPUTLOST))
		{
			hResult = X88k().GetDX().GetDIDKeyboard()->Acquire();
			if (hResult == DI_OK) {
				hResult = X88k().GetDX().GetDIDKeyboard()->GetDeviceState(
					sizeof(m_abtKeyboardState),
					m_abtKeyboardState);
			}
		}
		if (hResult != DI_OK) {
			return;
		}
		if (IsKeyPressed(DIK_LMENU) || IsKeyPressed(DIK_RMENU)) {
			if (IsKeyPressed(DIK_F1)) {
				X88k().SetKeyMatrics(
					0x0A, 4,
					true);
				X88k().SetKeyMatrics(
					0x08, 4,
					IsKeyPressed(DIK_LSHIFT, DIK_RSHIFT));
				X88k().SetKeyMatrics(
					0x08, 7,
					IsKeyPressed(DIK_LCONTROL, DIK_RCONTROL));
			}
			return;
		}
		bool bShift;
		if (IsKeyPressed(DIK_F6) ||
			IsKeyPressed(DIK_F7) ||
			IsKeyPressed(DIK_F8) ||
			IsKeyPressed(DIK_F9) ||
			IsKeyPressed(DIK_F10) ||
			IsKeyPressed(DIK_INSERT))
		{
			bShift = true;
		} else if (IsKeyPressed(DIK_DELETE)) {
			bShift = false;
		} else {
			bShift = IsKeyPressed(DIK_LSHIFT, DIK_RSHIFT);
		}
		bool bUpArrow = IsKeyPressed(DIK_UP),
			bRightArrow = IsKeyPressed(DIK_RIGHT);
		if (X88k().PC88().Z80Main().GetBasicMode() ==
				CPC88Z80Main::BASICMODE_N80V1)
		{
			if (IsKeyPressed(DIK_DOWN)) {
				bShift = bUpArrow = true;
			}
			if (IsKeyPressed(DIK_LEFT)) {
				bShift = bRightArrow = true;
			}
		}
		X88k().SetKeyMatrics(0x00, 0, IsKeyPressed(DIK_NUMPAD0));
		X88k().SetKeyMatrics(0x00, 1, IsKeyPressed(DIK_NUMPAD1));
		X88k().SetKeyMatrics(0x00, 2, IsKeyPressed(DIK_NUMPAD2));
		X88k().SetKeyMatrics(0x00, 3, IsKeyPressed(DIK_NUMPAD3));
		X88k().SetKeyMatrics(0x00, 4, IsKeyPressed(DIK_NUMPAD4));
		X88k().SetKeyMatrics(0x00, 5, IsKeyPressed(DIK_NUMPAD5));
		X88k().SetKeyMatrics(0x00, 6, IsKeyPressed(DIK_NUMPAD6));
		X88k().SetKeyMatrics(0x00, 7, IsKeyPressed(DIK_NUMPAD7));
		X88k().SetKeyMatrics(0x01, 0, IsKeyPressed(DIK_NUMPAD8));
		X88k().SetKeyMatrics(0x01, 1, IsKeyPressed(DIK_NUMPAD9));
		X88k().SetKeyMatrics(0x01, 2, IsKeyPressed(DIK_MULTIPLY));
		X88k().SetKeyMatrics(0x01, 3, IsKeyPressed(DIK_ADD));
		X88k().SetKeyMatrics(0x01, 4, IsKeyPressed(DIK_NUMPADEQUALS));
		X88k().SetKeyMatrics(0x01, 5, IsKeyPressed(DIK_NUMPADCOMMA));
		X88k().SetKeyMatrics(0x01, 6, IsKeyPressed(DIK_DECIMAL));
		X88k().SetKeyMatrics(
			0x01, 7,
			IsKeyPressed(DIK_RETURN, DIK_NUMPADENTER));
		X88k().SetKeyMatrics(0x02, 0, IsKeyPressed(DIK_AT));
		X88k().SetKeyMatrics(0x02, 1, IsKeyPressed(DIK_A));
		X88k().SetKeyMatrics(0x02, 2, IsKeyPressed(DIK_B));
		X88k().SetKeyMatrics(0x02, 3, IsKeyPressed(DIK_C));
		X88k().SetKeyMatrics(0x02, 4, IsKeyPressed(DIK_D));
		X88k().SetKeyMatrics(0x02, 5, IsKeyPressed(DIK_E));
		X88k().SetKeyMatrics(0x02, 6, IsKeyPressed(DIK_F));
		X88k().SetKeyMatrics(0x02, 7, IsKeyPressed(DIK_G));
		X88k().SetKeyMatrics(0x03, 0, IsKeyPressed(DIK_H));
		X88k().SetKeyMatrics(0x03, 1, IsKeyPressed(DIK_I));
		X88k().SetKeyMatrics(0x03, 2, IsKeyPressed(DIK_J));
		X88k().SetKeyMatrics(0x03, 3, IsKeyPressed(DIK_K));
		X88k().SetKeyMatrics(0x03, 4, IsKeyPressed(DIK_L));
		X88k().SetKeyMatrics(0x03, 5, IsKeyPressed(DIK_M));
		X88k().SetKeyMatrics(0x03, 6, IsKeyPressed(DIK_N));
		X88k().SetKeyMatrics(0x03, 7, IsKeyPressed(DIK_O));
		X88k().SetKeyMatrics(0x04, 0, IsKeyPressed(DIK_P));
		X88k().SetKeyMatrics(0x04, 1, IsKeyPressed(DIK_Q));
		X88k().SetKeyMatrics(0x04, 2, IsKeyPressed(DIK_R));
		X88k().SetKeyMatrics(0x04, 3, IsKeyPressed(DIK_S));
		X88k().SetKeyMatrics(0x04, 4, IsKeyPressed(DIK_T));
		X88k().SetKeyMatrics(0x04, 5, IsKeyPressed(DIK_U));
		X88k().SetKeyMatrics(0x04, 6, IsKeyPressed(DIK_V));
		X88k().SetKeyMatrics(0x04, 7, IsKeyPressed(DIK_W));
		X88k().SetKeyMatrics(0x05, 0, IsKeyPressed(DIK_X));
		X88k().SetKeyMatrics(0x05, 1, IsKeyPressed(DIK_Y));
		X88k().SetKeyMatrics(0x05, 2, IsKeyPressed(DIK_Z));
		X88k().SetKeyMatrics(0x05, 3, IsKeyPressed(DIK_LBRACKET));
		X88k().SetKeyMatrics(0x05, 4, IsKeyPressed(DIK_YEN));
		X88k().SetKeyMatrics(0x05, 5, IsKeyPressed(DIK_RBRACKET));
		X88k().SetKeyMatrics(0x05, 6, IsKeyPressed(DIK_CIRCUMFLEX));
		X88k().SetKeyMatrics(0x05, 7, IsKeyPressed(DIK_MINUS));
		X88k().SetKeyMatrics(0x06, 0, IsKeyPressed(DIK_0));
		X88k().SetKeyMatrics(0x06, 1, IsKeyPressed(DIK_1));
		X88k().SetKeyMatrics(0x06, 2, IsKeyPressed(DIK_2));
		X88k().SetKeyMatrics(0x06, 3, IsKeyPressed(DIK_3));
		X88k().SetKeyMatrics(0x06, 4, IsKeyPressed(DIK_4));
		X88k().SetKeyMatrics(0x06, 5, IsKeyPressed(DIK_5));
		X88k().SetKeyMatrics(0x06, 6, IsKeyPressed(DIK_6));
		X88k().SetKeyMatrics(0x06, 7, IsKeyPressed(DIK_7));
		X88k().SetKeyMatrics(0x07, 0, IsKeyPressed(DIK_8));
		X88k().SetKeyMatrics(0x07, 1, IsKeyPressed(DIK_9));
		X88k().SetKeyMatrics(0x07, 2, IsKeyPressed(DIK_COLON));
		X88k().SetKeyMatrics(0x07, 3, IsKeyPressed(DIK_SEMICOLON));
		X88k().SetKeyMatrics(0x07, 4, IsKeyPressed(DIK_COMMA));
		X88k().SetKeyMatrics(0x07, 5, IsKeyPressed(DIK_PERIOD));
		X88k().SetKeyMatrics(0x07, 6, IsKeyPressed(DIK_SLASH));
		X88k().SetKeyMatrics(
			0x07, 7,
			IsKeyPressed(DIK_BACKSLASH, DIK_UNDERLINE));
		X88k().SetKeyMatrics(0x08, 0, IsKeyPressed(DIK_HOME));
		X88k().SetKeyMatrics(0x08, 1, bUpArrow);
		X88k().SetKeyMatrics(0x08, 2, bRightArrow);
		X88k().SetKeyMatrics(
			0x08, 3,
			IsKeyPressed(DIK_BACK, DIK_INSERT, DIK_DELETE));
		X88k().SetKeyMatrics(0x08, 4, IsKeyPressed(DIK_F12));
		X88k().SetKeyMatrics(
			0x08, 5,
			((GetKeyState(VK_KANA) & 1) != 0) ||
				((GetKeyState(VK_SCROLL) & 1) != 0));
		X88k().SetKeyMatrics(0x08, 6, bShift);
		X88k().SetKeyMatrics(
			0x08, 7,
			IsKeyPressed(DIK_LCONTROL, DIK_RCONTROL));
		X88k().SetKeyMatrics(0x09, 0, IsKeyPressed(DIK_F11));
		X88k().SetKeyMatrics(
			0x09, 1,
			IsKeyPressed(DIK_F1, DIK_F6));
		X88k().SetKeyMatrics(
			0x09, 2,
			IsKeyPressed(DIK_F2, DIK_F7));
		X88k().SetKeyMatrics(
			0x09, 3,
			IsKeyPressed(DIK_F3, DIK_F8));
		X88k().SetKeyMatrics(
			0x09, 4,
			IsKeyPressed(DIK_F4, DIK_F9));
		X88k().SetKeyMatrics(
			0x09, 5,
			IsKeyPressed(DIK_F5, DIK_F10));
		X88k().SetKeyMatrics(0x09, 6, IsKeyPressed(DIK_SPACE));
		X88k().SetKeyMatrics(0x09, 7, IsKeyPressed(DIK_ESCAPE));
		X88k().SetKeyMatrics(0x0A, 0, IsKeyPressed(DIK_TAB));
		X88k().SetKeyMatrics(0x0A, 1, IsKeyPressed(DIK_DOWN));
		X88k().SetKeyMatrics(0x0A, 2, IsKeyPressed(DIK_LEFT));
		X88k().SetKeyMatrics(0x0A, 3, IsKeyPressed(DIK_END));
		X88k().SetKeyMatrics(0x0A, 4, IsKeyPressed(DIK_SYSRQ));
		X88k().SetKeyMatrics(0x0A, 5, IsKeyPressed(DIK_SUBTRACT));
		X88k().SetKeyMatrics(0x0A, 6, IsKeyPressed(DIK_DIVIDE));
		X88k().SetKeyMatrics(
			0x0A, 7,
			(GetKeyState(VK_CAPITAL) & 1) != 0);
		X88k().SetKeyMatrics(0x0B, 0, IsKeyPressed(DIK_NEXT));
		X88k().SetKeyMatrics(0x0B, 1, IsKeyPressed(DIK_PRIOR));
		X88k().SetKeyMatrics(0x0C, 0, IsKeyPressed(DIK_F6));
		X88k().SetKeyMatrics(0x0C, 1, IsKeyPressed(DIK_F7));
		X88k().SetKeyMatrics(0x0C, 2, IsKeyPressed(DIK_F8));
		X88k().SetKeyMatrics(0x0C, 3, IsKeyPressed(DIK_F9));
		X88k().SetKeyMatrics(0x0C, 4, IsKeyPressed(DIK_F10));
		X88k().SetKeyMatrics(0x0C, 5, IsKeyPressed(DIK_BACK));
		X88k().SetKeyMatrics(0x0C, 6, IsKeyPressed(DIK_INSERT));
		X88k().SetKeyMatrics(0x0C, 7, IsKeyPressed(DIK_DELETE));
		X88k().SetKeyMatrics(0x0D, 0, IsKeyPressed(DIK_CONVERT));
		X88k().SetKeyMatrics(0x0D, 1, IsKeyPressed(DIK_NOCONVERT));
		X88k().SetKeyMatrics(0x0D, 3, IsKeyPressed(DIK_KANJI));
		X88k().SetKeyMatrics(0x0E, 0, IsKeyPressed(DIK_RETURN));
		X88k().SetKeyMatrics(0x0E, 1, IsKeyPressed(DIK_NUMPADENTER));
		X88k().SetKeyMatrics(0x0E, 2, IsKeyPressed(DIK_LSHIFT));
		X88k().SetKeyMatrics(0x0E, 3, IsKeyPressed(DIK_RSHIFT));
	}

#elif defined(X88_GUI_GTK)

	if (!m_bHaveFocus || m_bMenuShowing) {
		return;
	}
	if ((m_nModKeyState & GDK_MOD1_MASK) != 0) {
		return;
	}

#ifdef X88_GUI_GTK_USEIM

	if (m_bPreEditing) {
		return;
	}

#endif // X88_GUI_GTK_USEIM

	bool bShift;
	if (IsKeyPressed(GDK_F6) ||
		IsKeyPressed(GDK_F7) ||
		IsKeyPressed(GDK_F8) ||
		IsKeyPressed(GDK_F9) ||
		IsKeyPressed(GDK_F10) ||
		IsKeyPressed(GDK_Insert))
	{
		bShift = true;
	} else if (IsKeyPressed(GDK_Delete)) {
		bShift = false;
	} else {
		bShift = IsKeyPressed(GDK_Shift_L) | IsKeyPressed(GDK_Shift_R) |
			((m_nModKeyState & GDK_SHIFT_MASK) != 0);
	}
	bool bUpArrow = IsKeyPressed(GDK_Up),
		bRightArrow = IsKeyPressed(GDK_Right);
	if (X88k().PC88().Z80Main().GetBasicMode() ==
			CPC88Z80Main::BASICMODE_N80V1)
	{
		if (IsKeyPressed(GDK_Down)) {
			bShift = bUpArrow = true;
		}
		if (IsKeyPressed(GDK_Left)) {
			bShift = bRightArrow = true;
		}
	}
	X88k().SetKeyMatrics(0x00, 0, IsKeyPressed(GDK_KP_0));
	X88k().SetKeyMatrics(0x00, 1, IsKeyPressed(GDK_KP_1));
	X88k().SetKeyMatrics(0x00, 2, IsKeyPressed(GDK_KP_2));
	X88k().SetKeyMatrics(0x00, 3, IsKeyPressed(GDK_KP_3));
	X88k().SetKeyMatrics(0x00, 4, IsKeyPressed(GDK_KP_4));
	X88k().SetKeyMatrics(0x00, 5, IsKeyPressed(GDK_KP_5));
	X88k().SetKeyMatrics(0x00, 6, IsKeyPressed(GDK_KP_6));
	X88k().SetKeyMatrics(0x00, 7, IsKeyPressed(GDK_KP_7));
	X88k().SetKeyMatrics(0x01, 0, IsKeyPressed(GDK_KP_8));
	X88k().SetKeyMatrics(0x01, 1, IsKeyPressed(GDK_KP_9));
	X88k().SetKeyMatrics(0x01, 2, IsKeyPressed(GDK_KP_Multiply));
	X88k().SetKeyMatrics(0x01, 3, IsKeyPressed(GDK_KP_Add));
	X88k().SetKeyMatrics(0x01, 4, IsKeyPressed(GDK_KP_Equal));
	X88k().SetKeyMatrics(0x01, 5, IsKeyPressed(GDK_KP_Separator));
	X88k().SetKeyMatrics(0x01, 6, IsKeyPressed(GDK_KP_Decimal));
	X88k().SetKeyMatrics(
		0x01, 7,
		IsKeyPressed(GDK_Return) | IsKeyPressed(GDK_KP_Enter));
	X88k().SetKeyMatrics(0x02, 0, IsKeyPressed(GDK_at));
	X88k().SetKeyMatrics(0x02, 1, IsKeyPressed(GDK_a));
	X88k().SetKeyMatrics(0x02, 2, IsKeyPressed(GDK_b));
	X88k().SetKeyMatrics(0x02, 3, IsKeyPressed(GDK_c));
	X88k().SetKeyMatrics(0x02, 4, IsKeyPressed(GDK_d));
	X88k().SetKeyMatrics(0x02, 5, IsKeyPressed(GDK_e));
	X88k().SetKeyMatrics(0x02, 6, IsKeyPressed(GDK_f));
	X88k().SetKeyMatrics(0x02, 7, IsKeyPressed(GDK_g));
	X88k().SetKeyMatrics(0x03, 0, IsKeyPressed(GDK_h));
	X88k().SetKeyMatrics(0x03, 1, IsKeyPressed(GDK_i));
	X88k().SetKeyMatrics(0x03, 2, IsKeyPressed(GDK_j));
	X88k().SetKeyMatrics(0x03, 3, IsKeyPressed(GDK_k));
	X88k().SetKeyMatrics(0x03, 4, IsKeyPressed(GDK_l));
	X88k().SetKeyMatrics(0x03, 5, IsKeyPressed(GDK_m));
	X88k().SetKeyMatrics(0x03, 6, IsKeyPressed(GDK_n));
	X88k().SetKeyMatrics(0x03, 7, IsKeyPressed(GDK_o));
	X88k().SetKeyMatrics(0x04, 0, IsKeyPressed(GDK_p));
	X88k().SetKeyMatrics(0x04, 1, IsKeyPressed(GDK_q));
	X88k().SetKeyMatrics(0x04, 2, IsKeyPressed(GDK_r));
	X88k().SetKeyMatrics(0x04, 3, IsKeyPressed(GDK_s));
	X88k().SetKeyMatrics(0x04, 4, IsKeyPressed(GDK_t));
	X88k().SetKeyMatrics(0x04, 5, IsKeyPressed(GDK_u));
	X88k().SetKeyMatrics(0x04, 6, IsKeyPressed(GDK_v));
	X88k().SetKeyMatrics(0x04, 7, IsKeyPressed(GDK_w));
	X88k().SetKeyMatrics(0x05, 0, IsKeyPressed(GDK_x));
	X88k().SetKeyMatrics(0x05, 1, IsKeyPressed(GDK_y));
	X88k().SetKeyMatrics(0x05, 2, IsKeyPressed(GDK_z));
	X88k().SetKeyMatrics(0x05, 3, IsKeyPressed(GDK_bracketleft));
	X88k().SetKeyMatrics(
		0x05, 4,
		IsKeyPressed(GDK_yen) | IsKeyPressed(GDK_backslash) |
			IsKeyPressed(GDK_bar));
	X88k().SetKeyMatrics(0x05, 5, IsKeyPressed(GDK_bracketright));
	X88k().SetKeyMatrics(0x05, 6, IsKeyPressed(GDK_asciicircum));
	X88k().SetKeyMatrics(0x05, 7, IsKeyPressed(GDK_minus));
	X88k().SetKeyMatrics(0x06, 0, IsKeyPressed(GDK_0));
	X88k().SetKeyMatrics(0x06, 1, IsKeyPressed(GDK_1));
	X88k().SetKeyMatrics(0x06, 2, IsKeyPressed(GDK_2));
	X88k().SetKeyMatrics(0x06, 3, IsKeyPressed(GDK_3));
	X88k().SetKeyMatrics(0x06, 4, IsKeyPressed(GDK_4));
	X88k().SetKeyMatrics(0x06, 5, IsKeyPressed(GDK_5));
	X88k().SetKeyMatrics(0x06, 6, IsKeyPressed(GDK_6));
	X88k().SetKeyMatrics(0x06, 7, IsKeyPressed(GDK_7));
	X88k().SetKeyMatrics(0x07, 0, IsKeyPressed(GDK_8));
	X88k().SetKeyMatrics(0x07, 1, IsKeyPressed(GDK_9));
	X88k().SetKeyMatrics(0x07, 2, IsKeyPressed(GDK_colon));
	X88k().SetKeyMatrics(0x07, 3, IsKeyPressed(GDK_semicolon));
	X88k().SetKeyMatrics(0x07, 4, IsKeyPressed(GDK_comma));
	X88k().SetKeyMatrics(0x07, 5, IsKeyPressed(GDK_period));
	X88k().SetKeyMatrics(0x07, 6, IsKeyPressed(GDK_slash));
	X88k().SetKeyMatrics(0x07, 7, IsKeyPressed(GDK_underscore));
	X88k().SetKeyMatrics(0x08, 0, IsKeyPressed(GDK_Home));
	X88k().SetKeyMatrics(0x08, 1, bUpArrow);
	X88k().SetKeyMatrics(0x08, 2, bRightArrow);
	X88k().SetKeyMatrics(
		0x08, 3,
		IsKeyPressed(GDK_BackSpace) |
			IsKeyPressed(GDK_Insert) | IsKeyPressed(GDK_Delete));
	X88k().SetKeyMatrics(0x08, 4, IsKeyPressed(GDK_F12));
	X88k().SetKeyMatrics(
		0x08, 5,
		(m_nModKeyState & GDK_MOD5_MASK) != 0);
	X88k().SetKeyMatrics(0x08, 6, bShift);
	X88k().SetKeyMatrics(
		0x08, 7,
		IsKeyPressed(GDK_Control_L) | IsKeyPressed(GDK_Control_R));
	X88k().SetKeyMatrics(0x09, 0, IsKeyPressed(GDK_F11));
	X88k().SetKeyMatrics(
		0x09, 1,
		IsKeyPressed(GDK_F1) | IsKeyPressed(GDK_F6));
	X88k().SetKeyMatrics(
		0x09, 2,
		IsKeyPressed(GDK_F2) | IsKeyPressed(GDK_F7));
	X88k().SetKeyMatrics(
		0x09, 3,
		IsKeyPressed(GDK_F3) | IsKeyPressed(GDK_F8));
	X88k().SetKeyMatrics(
		0x09, 4,
		IsKeyPressed(GDK_F4) | IsKeyPressed(GDK_F9));
	X88k().SetKeyMatrics(
		0x09, 5,
		IsKeyPressed(GDK_F5) | IsKeyPressed(GDK_F10));
	X88k().SetKeyMatrics(0x09, 6, IsKeyPressed(GDK_space));
	X88k().SetKeyMatrics(0x09, 7, IsKeyPressed(GDK_Escape));
	X88k().SetKeyMatrics(0x0A, 0, IsKeyPressed(GDK_Tab));
	X88k().SetKeyMatrics(0x0A, 1, IsKeyPressed(GDK_Down));
	X88k().SetKeyMatrics(0x0A, 2, IsKeyPressed(GDK_Left));
	X88k().SetKeyMatrics(0x0A, 3, IsKeyPressed(GDK_End));
	X88k().SetKeyMatrics(0x0A, 4, IsKeyPressed(GDK_Sys_Req));
	X88k().SetKeyMatrics(0x0A, 5, IsKeyPressed(GDK_KP_Subtract));
	X88k().SetKeyMatrics(0x0A, 6, IsKeyPressed(GDK_KP_Divide));
	X88k().SetKeyMatrics(
		0x0A, 7,
		(m_nModKeyState & GDK_LOCK_MASK) != 0);
	X88k().SetKeyMatrics(0x0B, 0, IsKeyPressed(GDK_Next));
	X88k().SetKeyMatrics(0x0B, 1, IsKeyPressed(GDK_Prior));
	X88k().SetKeyMatrics(0x0C, 0, IsKeyPressed(GDK_F6));
	X88k().SetKeyMatrics(0x0C, 1, IsKeyPressed(GDK_F7));
	X88k().SetKeyMatrics(0x0C, 2, IsKeyPressed(GDK_F8));
	X88k().SetKeyMatrics(0x0C, 3, IsKeyPressed(GDK_F9));
	X88k().SetKeyMatrics(0x0C, 4, IsKeyPressed(GDK_F10));
	X88k().SetKeyMatrics(0x0C, 5, IsKeyPressed(GDK_BackSpace));
	X88k().SetKeyMatrics(0x0C, 6, IsKeyPressed(GDK_Insert));
	X88k().SetKeyMatrics(0x0C, 7, IsKeyPressed(GDK_Delete));
	X88k().SetKeyMatrics(0x0D, 0, IsKeyPressed(GDK_Henkan_Mode));
	X88k().SetKeyMatrics(0x0D, 1, IsKeyPressed(GDK_Muhenkan));
	X88k().SetKeyMatrics(0x0D, 3, IsKeyPressed(GDK_Zenkaku_Hankaku));
	X88k().SetKeyMatrics(0x0E, 0, IsKeyPressed(GDK_Return));
	X88k().SetKeyMatrics(0x0E, 1, IsKeyPressed(GDK_KP_Enter));
	X88k().SetKeyMatrics(0x0E, 2, IsKeyPressed(GDK_Shift_L));
	X88k().SetKeyMatrics(0x0E, 3, IsKeyPressed(GDK_Shift_R));

#endif // X88_GUI

}

// is pressed esc key

bool CX88Frame::IsPressedESC() {
	bool bPressed = false;
	if (m_bHaveFocus) {

#ifdef X88_GUI_WINDOWS

		if (!X88k().GetDX().IsDirectInputActive()) {
			if (GetKeyState(VK_MENU) >= 0) {
				bPressed = GetKeyState(VK_ESCAPE) < 0;
			}
		} else {
			HRESULT hResult;
			hResult = X88k().GetDX().GetDIDKeyboard()->GetDeviceState(
				sizeof(m_abtKeyboardState),
				m_abtKeyboardState);
			if ((hResult == DIERR_NOTACQUIRED) ||
				(hResult == DIERR_INPUTLOST))
			{
				hResult = X88k().GetDX().GetDIDKeyboard()->Acquire();
				if (hResult == DI_OK) {
					hResult = X88k().GetDX().GetDIDKeyboard()->GetDeviceState(
						sizeof(m_abtKeyboardState),
						m_abtKeyboardState);
				}
			}
			if ((hResult == DI_OK) &&
				!IsKeyPressed(DIK_LMENU) && !IsKeyPressed(DIK_RMENU))
			{
				bPressed = IsKeyPressed(DIK_ESCAPE);
			}
		}

#elif defined(X88_GUI_GTK)

		if (!IsKeyPressed(GDK_Alt_L) && !IsKeyPressed(GDK_Alt_R) &&
			((m_nModKeyState & GDK_MOD1_MASK) == 0))
		{
			bPressed = IsKeyPressed(GDK_Escape);
		}

#endif // X88_GUI

	}
	return bPressed;
}
