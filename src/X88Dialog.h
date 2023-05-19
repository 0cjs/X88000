////////////////////////////////////////////////////////////
// X88000 Dialog
//
// Written by Manuke

#ifndef X88Dialog_DEFINED
#define X88Dialog_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88Dialog;

////////////////////////////////////////////////////////////
// constant value

#ifdef X88_GUI_GTK

// message-box option
enum {
	MB_OK              = 0x0001,
	MB_OKCANCEL        = 0x0003,
	MB_YESNO           = 0x000C,
	MB_YESNOCANCEL     = 0x000E,
	MB_ICONEXCLAMATION = 0,
	MB_ICONQUESTION    = 0
};

#endif // X88_GUI

#ifdef X88_PLATFORM_WINDOWS

////////////////////////////////////////////////////////////
// declaration of CX88MessageProcessor

class CX88MessageProcessor {
// attribute
protected:
	// next processor
	CX88MessageProcessor* m_pNextProcessor;

// create & destroy
public:
	// default constructor
	CX88MessageProcessor();
	// destructor
	virtual ~CX88MessageProcessor();

// operation
protected:
	// set next processor
	virtual CX88MessageProcessor* SetNextProcessor(
		CX88MessageProcessor* pProcessor);
	// call next processor
	virtual BOOL CallNext(MSG* pMessage);
public:
	// add processor to chain
	virtual CX88MessageProcessor* Add(
		CX88MessageProcessor* pProcessor);
	// remove processor from chain
	virtual CX88MessageProcessor* Remove(
		CX88MessageProcessor* pProcessor);
	// process message
	virtual BOOL ProcessMessage(MSG* pMessage);
};

#endif // X88_GUI

////////////////////////////////////////////////////////////
// declaration of CX88Dialog

class CX88Dialog {
// typedef

#ifdef X88_GUI_GTK

protected:
	struct SDialogInfo;
	// type of id-widget map
	typedef std::map<int, GtkWidget*> CID2WidgetMap;
	// type of widget-id map
	typedef std::map<GtkWidget*, int> CWidget2IDMap;
	// type of dialog map
	typedef std::map<GtkWidget*, SDialogInfo*> CDialogMap;
	// type of dialog procedure
	typedef bool (*DLGPROC)(
		GtkWidget* pDialog,
		bool bInitialize,
		int nID);

#endif // X88_GUI

// struct / class

#ifdef X88_GUI_GTK

protected:
	// dialog information struct
	struct SDialogInfo {
		// id-widget map
		CID2WidgetMap mapID2Widget;
		// widget-id map
		CWidget2IDMap mapWidget2ID;
	};

public:
	// signal blocker
	class CSignalBlocker {
	// attribute
	protected:
		// target widget
		GtkWidget* m_pTarget;
	// create & destroy
	public:
		// standard constructor
		CSignalBlocker(GtkWidget* pTarget) :
			m_pTarget(pTarget)
		{
			IncSignalLockCounter(m_pTarget);
		}
		// destructor
		~CSignalBlocker() {
			DecSignalLockCounter(m_pTarget);
		}
	// operation
	public:
		bool IsBlocked() const {
			return GetSignalLockCounter(m_pTarget) > 1;
		}
	};

#endif // X88_GUI

// attribute
protected:
	// parent window handle
	CX88WndHandle m_hwndParent;

#ifdef X88_PLATFORM_WINDOWS

	// resource handle
	static HINSTANCE m_hinstResource;

#endif // X88_PLATFORM

#ifdef X88_GUI_GTK

	// dialog map
	static CDialogMap m_mapDialog;
	// modal counter
	static int m_nModalCounter;
	// signal lock counter
	static std::map<GtkWidget*, int> m_mapSignalLockCounters;
	// message-box type
	static unsigned m_nMessageBoxType;
	// message-box text(GUI encoding)
	static std::string m_gstrMessageBoxText;
	// message-box caption(GUI encoding)
	static std::string m_gstrMessageBoxCaption;

#endif // X88_GUI

public:
	// get parent window handle
	CX88WndHandle GetParentWndHandle() const {
		return m_hwndParent;
	}
	// set parent window handle
	void SetParentWndHandle(CX88WndHandle hwndParentNew) {
		m_hwndParent = hwndParentNew;
	}

#ifdef X88_PLATFORM_WINDOWS

	// get resource handle
	static HINSTANCE GetResourceHandle() {
		return m_hinstResource;
	}
	// set resource handle
	static void SetResourceHandle(HINSTANCE hinstResource) {
		m_hinstResource = hinstResource;
	}

#endif // X88_PLATFORM

#ifdef X88_GUI_GTK

	// get modal counter
	static int GetModalCounter() {
		return m_nModalCounter;
	}
	// get signal lock counter
	static int GetSignalLockCounter(GtkWidget* pWidget) {
		return m_mapSignalLockCounters[pWidget];
	}
	// init signal lock counter
	static void InitSignalLockCounter(GtkWidget* pWidget) {
		m_mapSignalLockCounters[pWidget] = 0;
	}
	// dispose signal lock counter
	static void DisposeSignalLockCounter(GtkWidget* pWidget) {
		m_mapSignalLockCounters.erase(pWidget);
	}
	// increment signal lock counter
	static int IncSignalLockCounter(GtkWidget* pWidget) {
		return ++m_mapSignalLockCounters[pWidget];
	}
	// decrement signal lock counter
	static int DecSignalLockCounter(GtkWidget* pWidget) {
		return --m_mapSignalLockCounters[pWidget];
	}

#endif // X88_GUI

// create & destroy
public:
	// standard constructor
	CX88Dialog(CX88WndHandle hwndParent = NULL);
	// destructor
	virtual ~CX88Dialog();

// implementation
protected:

#ifdef X88_GUI_GTK

	// modeless-dialog destroy signal
	static void OnModelessDestroySignal(
		GtkWidget* pWidget, gpointer pData);
	// finish-button clicked signal default
	static void OnClickedSignalFinishButton(
		GtkButton* pButton, gpointer pData);
	// dialog procedure for message-box
	static bool MessageBoxDlgProc(
		GtkWidget* pDialog,
		bool bInitialize,
		int nID);

#endif // X88_GUI

// operation
public:
	// execute dialog
	static int ExecuteDialog(
		int nID,
		CX88WndHandle hwndParent,
		DLGPROC pDlgProc,
		bool bModal = true);
	// destroy dialog(modeless)
	static bool DestroyDialog(
		CX88WndHandle hdlg);
	// get child window id in dialog
	static int GetDlgItemID(
		CX88WndHandle hdlg, CX88WndHandle hwndChild);

#ifdef X88_GUI_WINDOWS

	// centering dialog
	static void Centering(HWND hdlg);

#elif defined(X88_GUI_GTK)

	// build dialog
	static bool BuildDialog(
		GtkWidget* pDialog,
		int nDialogID,
		unsigned nState = 0);
	// cleanup dialog
	static void CleanupDialog(GtkWidget* pDialog);
	// execute modal loop of dialog
	static int ExecuteModalLoop(
		GtkWidget* pDialog,
		GtkWidget* pParent);
	// end dialog
	static void EndDialog(
		GtkWidget* pDialog,
		int nResult);
	// register child widget to dialog
	static void RegisterDlgItem(
		GtkWidget* pDialog, GtkWidget* pControl, int nID);
	// get child widget from dialog
	static GtkWidget* GetDlgItem(
		GtkWidget* pDialog, int nID);

#endif // X88_GUI

	// create modal dialog
	virtual int DoModal() = 0;
	// is modeless dialog
	virtual bool IsModeless() const;
	// is created modeless dialog
	virtual bool IsCreatedModeless() const;
	// create modeless dialog
	virtual bool CreateModeless();
	// close modeless dialog
	virtual bool CloseModeless();

// message box
public:
	// show message-box
	static int ShowMessageBox(
		CX88WndHandle hwndParent,
		int nType, int nButtons,
		const std::string& gstrCaption,
		const std::string& gstrText);

// control
public:
	// show/hide control
	static void ShowControl(
		CX88WndHandle hwndControl,
		bool bShow);
	// enable/disable control
	static void EnableControl(
		CX88WndHandle hwndControl,
		bool bEnable);
	// set focus to control
	static void SetControlFocus(
		CX88WndHandle hwndControl);
	// is check-button checked
	static bool IsCheckButtonChecked(
		CX88WndHandle hwndCheckButton);
	// set check-button checked
	static void SetCheckButtonChecked(
		CX88WndHandle hwndCheckButton,
		bool bCheck);
	// get selection number of radio-button
	static int GetRadioButtonSel(
		CX88WndHandle hwndRadioButton);
	// set selection number of radio-button
	static void SetRadioButtonSel(
		CX88WndHandle hwndRadioButton,
		int nSel);
	// get static text
	static void GetStaticText(
		CX88WndHandle hwndStatic,
		std::string& gstrText);
	// set static text
	static void SetStaticText(
		CX88WndHandle hwndStatic,
		const std::string& gstrText);
	// get edit-box text
	static void GetEditBoxText(
		CX88WndHandle hwndEdit,
		std::string& gstrText);
	// set edit-box text
	static void SetEditBoxText(
		CX88WndHandle hwndEdit,
		const std::string& gstrText);
	// get listbox selection number
	static int GetListBoxSel(
		CX88WndHandle hwndListBox,
		std::vector<int>* pvectSel);
	// set listbox selection number
	static void SetListBoxSel(
		CX88WndHandle hwndListBox,
		const std::vector<int>* pvectSel);
	// clear listbox
	static void ClearListBox(
		CX88WndHandle hwndListBox);
	// add item to listbox
	static void AddListBoxItem(
		CX88WndHandle hwndListBox,
		const std::string& gstrItem);
	// insert item to listbox
	static void InsertListBoxItem(
		CX88WndHandle hwndListBox,
		int nIndex, const std::string& gstrItem);
	// delete item from listbox
	static void DeleteListBoxItem(
		CX88WndHandle hwndListBox,
		int nIndex);
	// get listbox item count
	static int GetListBoxItemCount(
		CX88WndHandle hwndListBox);
	// get combobox selection number
	static int GetComboBoxSel(
		CX88WndHandle hwndComboBox);
	// set combobox selection number
	static void SetComboBoxSel(
		CX88WndHandle hwndComboBox,
		int nSel);
	// clear combobox
	static void ClearComboBox(
		CX88WndHandle hwndComboBox);
	// add item to combobox
	static void AddComboBoxItem(
		CX88WndHandle hwndComboBox,
		const std::string& gstrItem);
	// insert item to combobox
	static void InsertComboBoxItem(
		CX88WndHandle hwndComboBox,
		int nIndex, const std::string& gstrItem);
	// delete item from combobox
	static void DeleteComboBoxItem(
		CX88WndHandle hwndComboBox,
		int nIndex);
	// get combobox item count
	static int GetComboBoxItemCount(
		CX88WndHandle hwndComboBox);
	// get slider value
	static int GetSliderValue(
		CX88WndHandle hwndSlider);
	// set slider value
	static void SetSliderValue(
		CX88WndHandle hwndSlider,
		int nValue);
	// get spin-button value
	static int GetSpinButtonValue(
		CX88WndHandle hwndEdit, CX88WndHandle hwndSpin);
	// set spin-button value
	static void SetSpinButtonValue(
		CX88WndHandle hwndEdit, CX88WndHandle hwndSpin,
		int nValue);
	// set spin-button range
	static void SetSpinButtonRange(
		CX88WndHandle hwndSpin,
		int nLower, int nUpper);

// utility
public:

#ifdef X88_GUI_GTK

	// set control font
	static void SetControlFont(
		CX88WndHandle hwndTarget,
		const std::string& gstrFontFamilyName,
		int nFontSize = 0);

#endif // X88_GUI

};

#endif // X88Dialog_DEFINED
