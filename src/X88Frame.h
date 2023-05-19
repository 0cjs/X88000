////////////////////////////////////////////////////////////
// X88000 Frame Window
//
// Written by Manuke

#ifndef X88Frame_DEFINED
#define X88Frame_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88FrameMessageProcessor;
class CX88Frame;

////////////////////////////////////////////////////////////
// include

#include "X88000.h"
#include "X88View.h"
#include "X88Dialog.h"
#include "X88DebugWnd.h"

#ifdef X88_PLATFORM_WINDOWS

////////////////////////////////////////////////////////////
// declaration of CX88FrameMessageProcessor

class CX88FrameMessageProcessor :
	public CX88MessageProcessor
{
// attribute
protected:
	// frame
	CX88Frame* m_pFrame;

// create & destroy
public:
	// default constructor
	CX88FrameMessageProcessor(
		CX88Frame* pFrame);
	// destructor
	virtual ~CX88FrameMessageProcessor();

// operation
public:
	// process message
	virtual BOOL ProcessMessage(MSG* pMessage);
};

#endif // X88_GUI

////////////////////////////////////////////////////////////
// declaration of CX88Frame

class CX88Frame {
// attribute
protected:
	// X88000 object
	static CX88000* m_pX88k;

	// window handle
	static CX88WndHandle m_hWnd;

	// view
	static CX88View m_view;
	// debug window
	static CX88DebugWnd m_debugwnd;

	// full-screen mode
	static bool m_bFullScreen;
	// activated GDI surface
	static bool m_bActivatedGDI;
	// mouse hovering on menu bar
	static bool m_bMouseHoveringOnMenuBar;

	// have focus
	static bool m_bHaveFocus;
	// active process
	static bool m_bActiveProcess;

#ifdef X88_GUI_WINDOWS

	// keyboard accelerator
	static HACCEL m_haccel;

	// IME convert window opened
	static bool m_bIMEOpen;
	// backup of IMM input context
	static HIMC m_hIMCBackup;

	// keyboard state table
	static uint8_t m_abtKeyboardState[256];

	// message processor
	static CX88MessageProcessor* m_pMessageProcessor;

#elif defined(X88_GUI_GTK)

	// vertical packing-box
	static CX88WndHandle m_hwndVBox;
	// menu box
	static CX88WndHandle m_hwndMenuBox;
	// menu bar
	static CX88WndHandle m_hwndMenuBar;
	// keyboard accelerator group
	static GtkAccelGroup* m_pAccelGroup;
	// menu showing
	static bool m_bMenuShowing;

	// keyboard state table
	static uint8_t m_abtKeyboardState[256*2];
	// modifier key state
	static guint m_nModKeyState;

	// timeout id
	static gint m_nTimeOutID;

#ifdef X88_GUI_GTK_USEKEYMAP

	// key code table initialized
	static bool m_bInitializedKeyCodes;
	// key code table
	static int m_anKeyCodes[256];
	// keyboard state updated
	static bool m_bDirtyKeyboardState;

#endif // X88_GUI_GTK_USEKEYMAP

#ifdef X88_GUI_GTK_NOKEYRELEASE

	// key code of pressed last
	static int m_nLastPressKeyCode;
	// key released counter
	static int m_nKeyReleaseCounter;

#endif // X88_GUI_GTK_NOKEYRELEASE

#ifdef X88_GUI_GTK_USEIM

	// input method context
	static GtkIMContext* m_pIMC;
	// input method pre-editing
	static bool m_bPreEditing;
	// input method pre-edit window
	static CX88WndHandle m_hwndPreEditWindow;
	// input method pre-edit drawing-area
	static CX88WndHandle m_hwndPreEditDArea;
	// input method pre-edit string(GUI encoding)
	static gchar* m_pgszIMPreEdit;
	// input method pre-edit attribute
	static PangoAttrList* m_pattrIMPreEdit;
	// input method pre-edit cursor position
	static gint m_nIMPreEditCursorPos;

#endif // X88_GUI_GTK_USEIM

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
	// view
	static CX88View& View() {
		return m_view;
	}
	// is full-screen mode
	static bool IsFullScreen() {
		return m_bFullScreen;
	}
	// is activated GDI surface
	static bool IsActivatedGDI() {
		return m_bActivatedGDI;
	}
	// is mouse hovering on menu bar
	static bool IsMouseHoveringOnMenuBar() {
		return m_bMouseHoveringOnMenuBar;
	}

#ifdef X88_GUI_WINDOWS

	// get message processor
	static CX88MessageProcessor* MessageProcessor() {
		return m_pMessageProcessor;
	}

#endif // X88_GUI

// create & destroy
public:
	// default constructor
	CX88Frame();
	// destructor
	~CX88Frame();

	// create
	static bool Create(CX88000& x88k);
	// destroy
	static bool Destroy();

// implementation
protected:
	// reload menu
	static void ReloadMenu();

#ifdef X88_GUI_WINDOWS

	// window procedure
	static LRESULT CALLBACK WndProc(
		HWND hwnd, UINT nMessage, WPARAM wParam, LPARAM lParam);

#elif defined(X88_GUI_GTK)

	// build menu
	static void BuildMenu(
		GtkWidget*& pMenuBar,
		GtkAccelGroup*& pAccelGroup);
	// destroy signal
	static void OnDestroySignal(
		GtkWidget* pWidget, gpointer pData);
	// realize signal
	static void OnRealizeSignal(
		GtkWidget* pWidget, gpointer pData);
	// unrealize signal
	static void OnUnrealizeSignal(
		GtkWidget* pWidget, gpointer pData);
	// menu map signal
	static void OnMenuMapSignal(
		GtkWidget* pWidget, gpointer pData);
	// menu unmap signal
	static void OnMenuUnmapSignal(
		GtkWidget* pWidget, gpointer pData);
	// menu show signal
	static void OnMenuShowSignal(
		GtkWidget* pWidget, gpointer pData);
	// menubar deactivate signal
	static void OnMenuBarDeactivateSignal(
		GtkWidget* pWidget, gpointer pData);
	// debug button clicked signal
	static void OnClickedSignalDebugButton(
		GtkButton* pButton, gpointer pData);
	// delete event
	static gboolean OnDeleteEvent(
		GtkWidget* pWidget, GdkEvent* pEvent, gpointer pData);
	// key-press event
	static gboolean OnKeyPressEvent(
		GtkWidget* pWidget, GdkEventKey* pEvent, gpointer* pData);
	// KeyRelease event
	static gboolean OnKeyReleaseEvent(
		GtkWidget* pWidget, GdkEventKey* pEvent, gpointer* pData);
	// focus-in event
	static gboolean OnFocusInEvent(
		GtkWidget* pWidget, GdkEventFocus* pEvent, gpointer* pData);
	// focus-out event
	static gboolean OnFocusOutEvent(
		GtkWidget* pWidget, GdkEventFocus* pEvent, gpointer* pData);
	// menu select callback
	static void OnMenuSelectCallback(
		gpointer pData, guint nAction, GtkWidget* pWidget);
	// time out callback
	static gboolean TimeOutCallback(gpointer pData);

#ifdef X88_GUI_GTK_USEIM

	// input method commit signal
	static void OnIMCommitSignal(
		GtkIMContext* pIMC, gchar* pgszText, gpointer* pData);
	// input method pre-edit string start signal
	static void OnIMPreEditStartSignal(
		GtkIMContext* pIMC, gpointer* pData);
	// input method pre-edit string end signal
	static void OnIMPreEditEndSignal(
		GtkIMContext* pIMC, gpointer* pData);
	// input method pre-edit string changed signal
	static void OnIMPreEditChangedSignal(
		GtkIMContext* pIMC, gpointer* pData);
	// input method pre-edit expose event
	static gboolean OnIMPreEditExposeEvent(
		GtkWidget* pWidget,
		GdkEventExpose* pEvent,
		gpointer pData);

#endif // X88_GUI_GTK_USEIM

#endif // X88_GUI

public:

#ifdef X88_GUI_WINDOWS

	// pre-process of TrenslateMessage
	static BOOL PreTranslateMessage(MSG* pMessage);

#endif // X88_GUI

// command
protected:
	// system menu command
	static void OnSystemMenuCommand(unsigned nCommand);
	// image menu command
	static void OnImageMenuCommand(unsigned nCommand);
	// debug menu command
	static void OnDebugMenuCommand(unsigned nCommand);
	// help menu command
	static void OnHelpMenuCommand(unsigned nCommand);

// operation
protected:
	// check menu command state
	static void CheckMenuCommandState(
		unsigned nCommand,
		int& nCheck, int& nGray);

#ifdef X88_GUI_WINDOWS

	// is key pressed
	static bool IsKeyPressed(UINT nVKey) {
		return (m_abtKeyboardState[nVKey] & 0x80) != 0;
	}
	// is key pressed(one of 2 keys)
	static bool IsKeyPressed(UINT nVKey1, UINT nVKey2) {
		return ((m_abtKeyboardState[nVKey1] & 0x80) != 0) ||
			((m_abtKeyboardState[nVKey2] & 0x80) != 0);
	}
	// is key pressed(one of 3 keys)
	static bool IsKeyPressed(UINT nVKey1, UINT nVKey2, UINT nVKey3) {
		return ((m_abtKeyboardState[nVKey1] & 0x80) != 0) ||
			((m_abtKeyboardState[nVKey2] & 0x80) != 0) ||
			((m_abtKeyboardState[nVKey3] & 0x80) != 0);
	}
	// is key toggled
	static bool IsKeyToggled(UINT nVirtualKey) {
		return (m_abtKeyboardState[nVirtualKey] & 0x01) != 0;
	}

	// modify frame menu
	static void ModifyFrameMenu(HMENU hMenu);

#elif defined(X88_GUI_GTK)

	// is key pressed
	static bool IsKeyPressed(guint nKeyVal) {
		return m_abtKeyboardState[nKeyVal & 0x01FF] != 0;
	}

	// convert key value
	static guint KeyValueConvert(guint nKeyVal);
	// set menu item state
	static void SetMenuItemState(guint nAction);
	// set all menu items state
	static void SetAllMenuItemState();

#ifdef X88_GUI_GTK_USEIM

	// start IM
	static bool StartIM();
	// stop IM
	static bool StopIM();
	// delete IM pre-edit
	static void DeleteIMPreEdit();
	// update IM pre-edit window
	static void UpdateIMPreEditWindow();

#endif // X88_GUI_GTK_USEIM

#endif // X88_GUI

public:
	// update debug window
	static void UpdateDebugWnd();
	// close parallel-device modeless dialog
	static void CloseParallalelModeless();
	// show executing frequency
	static void ShowFrequency(double fMHz);
	// set full-screen mode
	static bool SetFullScreen(bool bFullScreenNew);
	// activate gui
	static bool ActivateGDI(bool bActivate);
	// update key matrics
	static void UpdateAllKeyMatrics();
	// is pressed esc key
	static bool IsPressedESC();
};

#endif // X88Frame_DEFINED
