////////////////////////////////////////////////////////////
// X88000 Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88Dialog.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

#ifdef X88_PLATFORM_WINDOWS

////////////////////////////////////////////////////////////
// implementation of CX88MessageProcessor

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88MessageProcessor::CX88MessageProcessor() :
	m_pNextProcessor(NULL)
{
}

// destructor
CX88MessageProcessor::~CX88MessageProcessor() {
}

////////////////////////////////////////////////////////////
// operation

// set next processor

CX88MessageProcessor* CX88MessageProcessor::SetNextProcessor(
	CX88MessageProcessor* pProcessor)
{
	CX88MessageProcessor* pResult = m_pNextProcessor;
	m_pNextProcessor = pProcessor;
	return pResult;
}

// call next processor

BOOL CX88MessageProcessor::CallNext(MSG* pMessage) {
	return m_pNextProcessor->ProcessMessage(pMessage);
}

// add processor to chain

CX88MessageProcessor* CX88MessageProcessor::Add(
	CX88MessageProcessor* pProcessor)
{
	pProcessor->SetNextProcessor(this);
	return pProcessor;
}

// remove processor from chain

CX88MessageProcessor* CX88MessageProcessor::Remove(
	CX88MessageProcessor* pProcessor)
{
	CX88MessageProcessor* pResult = this;
	if (pProcessor == this) {
		pResult = SetNextProcessor(NULL);
	} else if (m_pNextProcessor != NULL) {
		m_pNextProcessor = m_pNextProcessor->Remove(pProcessor);
	}
	return pResult;
}

// process message

BOOL CX88MessageProcessor::ProcessMessage(MSG* /*pMessage*/) {
	return FALSE;
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// implementation of CX88Dialog

////////////////////////////////////////////////////////////
// attribute

#ifdef X88_PLATFORM_WINDOWS

// resource handle

HINSTANCE CX88Dialog::m_hinstResource;

#endif // X88_PLATFORM

#ifdef X88_GUI_GTK

// dialog map

CX88Dialog::CDialogMap CX88Dialog::m_mapDialog;

// modal counter

int CX88Dialog::m_nModalCounter = 0;

// signal lock counter

std::map<GtkWidget*, int> CX88Dialog::m_mapSignalLockCounters;

// message-box type

unsigned CX88Dialog::m_nMessageBoxType = 0;

// message-box text(source encoding)

std::string CX88Dialog::m_gstrMessageBoxText;

// message-box caption(source encoding)

std::string CX88Dialog::m_gstrMessageBoxCaption;

#endif // X88_GUI

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88Dialog::CX88Dialog(CX88WndHandle hwndParent) :
	m_hwndParent(hwndParent)
{
}

// destructor

CX88Dialog::~CX88Dialog() {
}

////////////////////////////////////////////////////////////
// implementation


#ifdef X88_GUI_GTK

// modeless-dialog destroy signal

void CX88Dialog::OnModelessDestroySignal(
	GtkWidget* pWidget, gpointer pData)
{
	DLGPROC pDlgProc = (DLGPROC)pData;
	pDlgProc(pWidget, false, 0);
	DisposeSignalLockCounter(pWidget);
	g_object_unref(pWidget);
}

// finish-button clicked signal default

void CX88Dialog::OnClickedSignalFinishButton(
	GtkButton* pButton, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	EndDialog(pDialog, GetDlgItemID(pDialog, GTK_WIDGET(pButton)));
}

// dialog procedure for message-box

bool CX88Dialog::MessageBoxDlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(
				pDialog,
				nID,
				m_nMessageBoxType))
		{
			return false;
		}
		gtk_window_set_title(
			GTK_WINDOW(pDialog),
			m_gstrMessageBoxCaption.c_str());
		gtk_label_set_text(
			GTK_LABEL(GetDlgItem(pDialog, NX88Resource::IDC_MESSAGE)),
			m_gstrMessageBoxText.c_str());
	}
	return true;
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// execute dialog

int CX88Dialog::ExecuteDialog(
	int nID,
	CX88WndHandle hwndParent,
	DLGPROC pDlgProc,
	bool bModal)
{

#ifdef X88_GUI_WINDOWS

	if (!NX88Resource::IsResJapanese()) {
		switch (nID) {
		case IDD_PRINTER:
			nID = IDD_PRINTER_E;
			break;
		case IDD_PRINTER_COPY:
			nID = IDD_PRINTER_COPY_E;
			break;
		case IDD_PRINTER_PAPERDEL:
			nID = IDD_PRINTER_PAPERDEL_E;
			break;
		case IDD_PRINTER_PAPERFEED:
			nID = IDD_PRINTER_PAPERFEED_E;
			break;
		case IDD_DISK_IMAGE:
			nID = IDD_DISK_IMAGE_E;
			break;
		case IDD_TAPE_IMAGE:
			nID = IDD_TAPE_IMAGE_E;
			break;
		case IDD_DEBUG_WRITE_RAM:
			nID = IDD_DEBUG_WRITE_RAM_E;
			break;
		case IDD_DEBUG_DIS_ASSEMBLE:
			nID = IDD_DEBUG_DIS_ASSEMBLE_E;
			break;
		case IDD_DEBUG_DUMP:
			nID = IDD_DEBUG_DUMP_E;
			break;
		case IDD_DEBUG_BREAK_POINT:
			nID = IDD_DEBUG_BREAK_POINT_E;
			break;
		case IDD_VERSION:
			nID = IDD_VERSION_E;
			break;
		}
	}
	int nResult;
	if (bModal) {
		nResult = (int)DialogBox(
			GetResourceHandle(), MAKEINTRESOURCE(nID),
			hwndParent,
			pDlgProc);
	} else {
		nResult = 0;
		HWND hdlg = CreateDialog(
			GetResourceHandle(), MAKEINTRESOURCE(nID),
			hwndParent,
			pDlgProc);
		if (hdlg != NULL) {
			ShowWindow(hdlg, SW_SHOWNORMAL);
			nResult = 1;
		}
	}
	return nResult;

#elif defined(X88_GUI_GTK)

	int nResult = -1;
	GtkWidget* pDialog = gtk_dialog_new();
	if (pDialog != NULL) {
		g_object_ref(pDialog);
		if (bModal) {
			InitSignalLockCounter(pDialog);
			if (pDlgProc(pDialog, true, nID)) {
				nResult = ExecuteModalLoop(pDialog, hwndParent);
			}
			pDlgProc(pDialog, false, nID);
			gtk_widget_destroy(pDialog);
			DisposeSignalLockCounter(pDialog);
			g_object_unref(pDialog);
		} else {
			nResult = 0;
			InitSignalLockCounter(pDialog);
			if (!pDlgProc(pDialog, true, nID)) {
				gtk_widget_destroy(pDialog);
				DisposeSignalLockCounter(pDialog);
				g_object_unref(pDialog);
			} else {
				nResult = 1;
				g_signal_connect(
					pDialog, "destroy",
					G_CALLBACK(OnModelessDestroySignal), (gpointer)pDlgProc);
				gtk_widget_show(pDialog);
			}
		}
	}
	return nResult;

#endif // X88_GUI

}

// destroy dialog(modeless)

bool CX88Dialog::DestroyDialog(
	CX88WndHandle hdlg)
{

#ifdef X88_GUI_WINDOWS

	if (hdlg == NULL) {
		return FALSE;
	}
	return DestroyWindow(hdlg) != FALSE;

#elif defined(X88_GUI_GTK)

	if (hdlg == NULL) {
		return FALSE;
	}
	gtk_widget_destroy(hdlg);
	return TRUE;

#endif // X88_GUI

}

// get child window id in dialog

int CX88Dialog::GetDlgItemID(
	CX88WndHandle hdlg, CX88WndHandle hwndChild)
{

#ifdef X88_GUI_WINDOWS

	hdlg;
	return GetDlgCtrlID(hwndChild);

#elif defined(X88_GUI_GTK)

	int nID = 0;
	CDialogMap::iterator itDialog = m_mapDialog.find(hdlg);
	if (itDialog != m_mapDialog.end()) {
		CWidget2IDMap::iterator itID =
			(*itDialog).second->mapWidget2ID.find(hwndChild);
		if (itID != (*itDialog).second->mapWidget2ID.end()) {
			nID = (*itID).second;
		}
	}
	return nID;

#endif // X88_GUI

}

#ifdef X88_GUI_WINDOWS

// centering dialog

void CX88Dialog::Centering(HWND hdlg) {
	int cxScreen = GetSystemMetrics(SM_CXSCREEN),
		cyScreen = GetSystemMetrics(SM_CYSCREEN);
	HWND hwndOwner = GetWindow(hdlg, GW_OWNER);
	RECT rectDlg, rectOwner;
	if (hwndOwner != NULL) {
		GetWindowRect(hwndOwner, &rectOwner);
	} else {
		SetRect(&rectOwner, 0, 0, cxScreen, cyScreen);
	}
	int xCenter = (rectOwner.left+rectOwner.right)/2,
		yCenter = (rectOwner.top+rectOwner.bottom)/2;
	GetWindowRect(hdlg, &rectDlg);
	int cxDlg = rectDlg.right-rectDlg.left,
		cyDlg = rectDlg.bottom-rectDlg.top;
	int xDlg = xCenter-cxDlg/2,
		yDlg = yCenter-cyDlg/2;
	if ((xDlg < 0) || (xDlg+cxDlg > cxScreen)) {
		if (cxDlg > cxScreen) {
			xDlg = cxScreen/2-cxDlg/2;
		} else if (xDlg < 0) {
			xDlg = 0;
		} else {
			xDlg = cxScreen-cxDlg;
		}
	}
	if ((yDlg < 0) || (yDlg+cyDlg > cyScreen)) {
		if (cyDlg > cyScreen) {
			yDlg = cyScreen/2-cyDlg/2;
		} else if (yDlg < 0) {
			yDlg = 0;
		} else {
			yDlg = cyScreen-cyDlg;
		}
	}
	SetWindowPos(
		hdlg, NULL,
		xDlg, yDlg, 0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

#elif defined(X88_GUI_GTK)

// build dialog

bool CX88Dialog::BuildDialog(
	GtkWidget* pDialog,
	int nDialogID,
	unsigned nState)
{
	if (pDialog == NULL) {
		return false;
	}
	CDialogMap::iterator itDialog = m_mapDialog.find(pDialog);
	if (itDialog == m_mapDialog.end()) {
		itDialog = m_mapDialog.insert(
			std::make_pair(pDialog, new SDialogInfo)).first;
	}
	if (nDialogID == NX88Resource::IDD_NONE) {
		return true;
	}
	int nItemCount = 0;
	NX88Resource::SDialogEntry* pdeTemplate =
		NX88Resource::GetDialogEntries(nDialogID, nItemCount);
	if (pdeTemplate == NULL) {
		return false;
	}
	std::vector<GtkWidget*> vectContainer;
	int nNowLevel = 0;
	if (GTK_IS_DIALOG(pDialog)) {
		vectContainer.push_back(GTK_DIALOG(pDialog)->vbox);
		nNowLevel++;
		vectContainer.push_back(GTK_DIALOG(pDialog)->action_area);
		nNowLevel++;
	} else {
		vectContainer.push_back(pDialog);
		nNowLevel++;
	}
	GtkWidget* pPrevControl = NULL;
	GtkWidget* pRadioLead = NULL;
	const char* pgszNoteTag = NULL;
	GtkAdjustment* pAdjustment = NULL;
	int nDialogUnit = 16;
	PangoLayout* pLayout = gtk_widget_create_pango_layout(
		pDialog, "A");
	if (pLayout != NULL) {
		pango_layout_get_pixel_size(
			pLayout, NULL, &nDialogUnit);
		g_object_unref(pLayout);
	}
	for (int nItem = 0; nItem < nItemCount; nItem++) {
		NX88Resource::SDialogEntry deControl = pdeTemplate[nItem];
		if ((deControl.nRule & nState) != deControl.nRule) {
			continue;
		}
		const char* pszItemType = deControl.pszItemType;
		if (strcasecmp(pszItemType, "Caption") == 0) {
			if (GTK_IS_WINDOW(pDialog)) {
				gtk_window_set_title(
					GTK_WINDOW(pDialog),
					RSTR(deControl.psszText));
			}
			continue;
		} else if (strcasecmp(pszItemType, "NoteTag") == 0) {
			pgszNoteTag = RSTR(deControl.psszText);
			continue;
		}
		int nNewLevel = 1;
		while (*pszItemType == ' ') {
			pszItemType++;
			nNewLevel++;
		}
		bool bEndPack = false,
			bExpand = false,
			bFill = false;
		int nPadding = 0;
		while (true) {
			switch (*pszItemType) {
			case '-':
				pszItemType++;
				bEndPack = true;
				continue;
			case '+':
				pszItemType++;
				bExpand = true;
				continue;
			case '*':
				pszItemType++;
				bFill = true;
				continue;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				nPadding = *(pszItemType++)-'0';
				continue;
			}
			break;
		}
		while (nNowLevel < nNewLevel) {
			vectContainer.push_back(pPrevControl);
			nNowLevel++;
		}
		while (nNowLevel > nNewLevel) {
			vectContainer.pop_back();
			nNowLevel--;
		}
		GtkWidget* pControl = NULL;
		bool bDefaultButton = false;
		if (strcasecmp(pszItemType, "VBox") == 0) {
			int nHomogeneous = FALSE,
				nSpacing = 1;
			if (deControl.pszLocalOption != NULL) {
				int n;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'h':
						sscanf(
							pszOption+1,
							"%d%n",
							&nHomogeneous,
							&n);
						pszOption += n;
						break;
					case 's':
						sscanf(
							pszOption+1,
							"%d%n",
							&nSpacing,
							&n);
						pszOption += n;
						break;
					}
				}
			}
			pControl = gtk_vbox_new(
				nHomogeneous,
				(nSpacing*nDialogUnit)/4);
		} else if (strcasecmp(pszItemType, "HBox") == 0) {
			int nHomogeneous = FALSE,
				nSpacing = 1;
			if (deControl.pszLocalOption != NULL) {
				int n;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'h':
						sscanf(
							pszOption+1,
							"%d%n",
							&nHomogeneous,
							&n);
						pszOption += n;
						break;
					case 's':
						sscanf(
							pszOption+1,
							"%d%n",
							&nSpacing,
							&n);
						pszOption += n;
						break;
					}
				}
			}
			pControl = gtk_hbox_new(
				nHomogeneous,
				(nSpacing*nDialogUnit)/4);
		} else if (strcasecmp(pszItemType, "Frame") == 0) {
			pControl = gtk_frame_new(
				RSTR(deControl.psszText));
		} else if (strcasecmp(pszItemType, "ScrolledWindow") == 0) {
			pControl = gtk_scrolled_window_new(NULL, NULL);
			if (pControl == NULL) {
				continue;
			}
			if (deControl.pszLocalOption != NULL) {
				int nHScrollBarPolicy = 1,
					nVScrollBarPolicy = 1;
				int n;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'p':
						sscanf(
							pszOption+1,
							"%d,%d%n",
							&nHScrollBarPolicy,
							&nVScrollBarPolicy,
							&n);
						pszOption += n;
						break;
					}
				}
				gtk_scrolled_window_set_policy(
					GTK_SCROLLED_WINDOW(pControl),
					(nHScrollBarPolicy != 0)?
						GTK_POLICY_ALWAYS: GTK_POLICY_AUTOMATIC,
					(nVScrollBarPolicy != 0)?
						GTK_POLICY_ALWAYS: GTK_POLICY_AUTOMATIC);
			}
		} else if (strcasecmp(pszItemType, "NoteBook") == 0) {
			pControl = gtk_notebook_new();
		} else if (strcasecmp(pszItemType, "Label") == 0) {
			pControl = gtk_label_new(
				RSTR(deControl.psszText));
		} else if (strcasecmp(pszItemType, "Button") == 0) {
			pControl = gtk_button_new_with_label(
				RSTR(deControl.psszText));
			if (pControl == NULL) {
				continue;
			}
			if (deControl.pszLocalOption != NULL) {
				bool bFinishButton = false;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'd':
						bDefaultButton = true;
						break;
					case 'f':
						bFinishButton = true;
						break;
					}
				}
				if (bFinishButton) {
					g_signal_connect(
						pControl, "clicked",
						G_CALLBACK(OnClickedSignalFinishButton),
						pDialog);
				}
			}
		} else if (strcasecmp(pszItemType, "CheckButton") == 0) {
			pControl = gtk_check_button_new_with_label(
				RSTR(deControl.psszText));
		} else if (strcasecmp(pszItemType, "RadioButton") == 0) {
			bool bLead = false, bTrail = false;
			if (deControl.pszLocalOption != NULL) {
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'l':
						bLead = true;
						break;
					case 't':
						bTrail = true;
						break;
					}
				}
			}
			GtkWidget* pPrevRadio = NULL;
			GSList* pGroup = NULL;
			if (bLead) {
			} else if (bTrail) {
				pPrevRadio = pRadioLead;
			} else {
				pPrevRadio = pPrevControl;
			}
			if (GTK_IS_RADIO_BUTTON(pPrevRadio)) {
				pGroup = gtk_radio_button_get_group(
					GTK_RADIO_BUTTON(pPrevRadio));
			}
			pControl = gtk_radio_button_new_with_label(
				pGroup,
				RSTR(deControl.psszText));
			if (bLead) {
				pRadioLead = pControl;
			}
		} else if (strcasecmp(pszItemType, "Entry") == 0) {
			pControl = gtk_entry_new();
		} else if (strcasecmp(pszItemType, "Text") == 0) {
			pControl = gtk_text_view_new();
			if (pControl == NULL) {
				continue;
			}
			gtk_text_view_set_wrap_mode(
				GTK_TEXT_VIEW(pControl),
				GTK_WRAP_CHAR);
			if (deControl.pszLocalOption != NULL) {
				int nEditable = 1, n;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'v':
						pAdjustment = (GtkAdjustment*)gtk_adjustment_new(
							0,
							0, 100,
							1, 10,
							1);
						gtk_widget_set_scroll_adjustments(
							GTK_WIDGET(pControl),
							NULL,
							pAdjustment);
						break;
					case 'e':
						sscanf(
							pszOption+1,
							"%d%n",
							&nEditable,
							&n);
						gtk_text_view_set_editable(
							GTK_TEXT_VIEW(pControl),
							(nEditable != 0)? TRUE: FALSE);
						pszOption += n;
						break;
					}
				}
			}
		} else if (strcasecmp(pszItemType, "List") == 0) {
			GtkListStore* pListStore = gtk_list_store_new(
				2, G_TYPE_STRING, G_TYPE_INT);
			if (pListStore == NULL) {
				continue;
			}
			pControl = gtk_tree_view_new_with_model(
				GTK_TREE_MODEL(pListStore));
			g_object_unref(pListStore);
			if (pControl == NULL) {
				continue;
			}
			gtk_tree_view_set_headers_visible(
				GTK_TREE_VIEW(pControl),
				FALSE);
			gtk_tree_view_append_column(
				GTK_TREE_VIEW(pControl),
				gtk_tree_view_column_new_with_attributes(
					NULL,
					gtk_cell_renderer_text_new(),
					"text", 0,
					NULL));
			gtk_tree_view_set_enable_search(
				GTK_TREE_VIEW(pControl),
				FALSE);
			if (deControl.pszLocalOption != NULL) {
				GtkSelectionMode nSelectionMode = GTK_SELECTION_SINGLE;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 's':
						nSelectionMode = GTK_SELECTION_SINGLE;
						break;
					case 'm':
						nSelectionMode = GTK_SELECTION_MULTIPLE;
						break;
					case 'b':
						nSelectionMode = GTK_SELECTION_BROWSE;
						break;
					case 'e':
						nSelectionMode = GTK_SELECTION_EXTENDED;
						break;
					}
				}
				gtk_tree_selection_set_mode(
					gtk_tree_view_get_selection(
						GTK_TREE_VIEW(pControl)),
					nSelectionMode);
			}
		} else if (strcasecmp(pszItemType, "Combo") == 0) {
			int nEditable = 1;
			if (deControl.pszLocalOption != NULL) {
				int n;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'e':
						sscanf(
							pszOption+1,
							"%d%n",
							&nEditable,
							&n);
						pszOption += n;
						break;
					}
				}
			}
			if (nEditable == 0) {
				pControl = gtk_combo_box_new_text();
			} else {
				pControl = gtk_combo_box_entry_new_text();
			}
		} else if (strcasecmp(pszItemType, "HScrollBar") == 0) {
			GtkAdjustment* pAdjustment2 = NULL;
			if (deControl.pszLocalOption != NULL) {
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'a':
						pAdjustment2 = pAdjustment;
						break;
					}
				}
			}
			pControl = gtk_hscrollbar_new(pAdjustment2);
		} else if (strcasecmp(pszItemType, "VScrollBar") == 0) {
			GtkAdjustment* pAdjustment2 = NULL;
			if (deControl.pszLocalOption != NULL) {
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'a':
						pAdjustment2 = pAdjustment;
						break;
					}
				}
			}
			pControl = gtk_vscrollbar_new(pAdjustment2);
		} else if (strcasecmp(pszItemType, "HScale") == 0) {
			GtkAdjustment* pAdjustment2 = NULL;
			if (deControl.pszLocalOption != NULL) {
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'a':
						pAdjustment2 = pAdjustment;
						break;
					}
				}
			}
			pControl = gtk_hscale_new(pAdjustment2);
		} else if (strcasecmp(pszItemType, "VScale") == 0) {
			GtkAdjustment* pAdjustment2 = NULL;
			if (deControl.pszLocalOption != NULL) {
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'a':
						pAdjustment2 = pAdjustment;
						break;
					}
				}
			}
			pControl = gtk_hscale_new(pAdjustment2);
		} else if (strcasecmp(pszItemType, "SpinButton") == 0) {
			double fValue = 0,
				fLower = 0, fUpper = 100,
				fStepIncrement = 1,
				fPageIncrement = 10,
				fPageSize = 0,
				fClimbRate = 0;
			int nDigits = 0;
			if (deControl.pszLocalOption != NULL) {
				int n;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'v':
						sscanf(
							pszOption+1,
							"%lf%n",
							&fValue,
							&n);
						pszOption += n;
						break;
					case 'l':
						sscanf(
							pszOption+1,
							"%lf%n",
							&fLower,
							&n);
						pszOption += n;
						break;
					case 'u':
						sscanf(
							pszOption+1,
							"%lf%n",
							&fUpper,
							&n);
						pszOption += n;
						break;
					case 's':
						sscanf(
							pszOption+1,
							"%lf%n",
							&fStepIncrement,
							&n);
						pszOption += n;
						break;
					case 'p':
						sscanf(
							pszOption+1,
							"%lf%n",
							&fPageIncrement,
							&n);
						pszOption += n;
						break;
					case 'z':
						sscanf(
							pszOption+1,
							"%lf%n",
							&fPageSize,
							&n);
						pszOption += n;
						break;
					case 'c':
						sscanf(
							pszOption+1,
							"%lf%n",
							&fClimbRate,
							&n);
						pszOption += n;
						break;
					case 'd':
						sscanf(
							pszOption+1,
							"%d%n",
							&nDigits,
							&n);
						pszOption += n;
						break;
					}
				}
			}
			GtkAdjustment* pAdjustment2 =
				(GtkAdjustment*)gtk_adjustment_new(
					(gfloat)fValue,
					(gfloat)fLower, (gfloat)fUpper,
					(gfloat)fStepIncrement, (gfloat)fPageIncrement,
					(gfloat)fPageSize);
			pControl = gtk_spin_button_new(
				pAdjustment2,
				(gfloat)fClimbRate,
				nDigits);
		} else if (strcasecmp(pszItemType, "Alignment") == 0) {
			double xAlign = 0.5, yAlign = 0.5,
				xScale = 0, yScale = 0;
			if (deControl.pszLocalOption != NULL) {
				int n;
				for (
					const char* pszOption = deControl.pszLocalOption;
					*pszOption != '\0';
					pszOption++)
				{
					switch (tolower(*pszOption)) {
					case 'x':
						sscanf(
							pszOption+1,
							"%lf%n",
							&xAlign,
							&n);
						pszOption += n;
						break;
					case 'y':
						sscanf(
							pszOption+1,
							"%lf%n",
							&yAlign,
							&n);
						pszOption += n;
						break;
					case 'w':
						sscanf(
							pszOption+1,
							"%lf%n",
							&xScale,
							&n);
						pszOption += n;
						break;
					case 'h':
						sscanf(
							pszOption+1,
							"%lf%n",
							&yScale,
							&n);
						pszOption += n;
						break;
					}
				}
			}
			pControl = gtk_alignment_new(
				(gfloat)xAlign, (gfloat)yAlign,
				(gfloat)xScale, (gfloat)yScale);
		}
		if (pControl == NULL) {
			continue;
		}
		if (deControl.pszGlobalOption != NULL) {
			int nWidth = -1, nHeight = -1;
			int nBorderWidth = 0;
			int n;
			for (
				const char* pszOption = deControl.pszGlobalOption;
				*pszOption != '\0';
				pszOption++)
			{
				switch (tolower(*pszOption)) {
				case 's':
					sscanf(
						pszOption+1,
						"%d,%d%n",
						&nWidth,
						&nHeight,
						&n);
					pszOption += n;
					break;
				case 'b':
					sscanf(
						pszOption+1,
						"%d%n",
						&nBorderWidth,
						&n);
					pszOption += n;
					break;
				}
			}
			if ((nWidth >= 0) || (nHeight >= 0)) {
				gtk_widget_set_size_request(
					pControl,
					(nWidth*nDialogUnit)/4,
					(nHeight*nDialogUnit)/4);
			}
			if ((nBorderWidth > 0) &&
				GTK_IS_CONTAINER(pControl))
			{
				gtk_container_set_border_width(
					GTK_CONTAINER(pControl),
					(nBorderWidth*nDialogUnit)/4);
			}
		}
		if (GTK_IS_BOX(vectContainer.back())) {
			if (!bEndPack) {
				gtk_box_pack_start(
					GTK_BOX(vectContainer.back()),
					pControl,
					bExpand? TRUE: FALSE,
					bFill? TRUE: FALSE,
					nPadding);
			} else {
				gtk_box_pack_end(
					GTK_BOX(vectContainer.back()),
					pControl,
					bExpand? TRUE: FALSE,
					bFill? TRUE: FALSE,
					nPadding);
			}
		} else if (GTK_IS_SCROLLED_WINDOW(vectContainer.back())) {
			gtk_scrolled_window_add_with_viewport(
				GTK_SCROLLED_WINDOW(vectContainer.back()),
				pControl);
		} else if (GTK_IS_NOTEBOOK(vectContainer.back())) {
			GtkWidget* pLabel = gtk_label_new(
				pgszNoteTag);
			gtk_notebook_append_page(
				GTK_NOTEBOOK(vectContainer.back()),
				pControl,
				pLabel);
		} else {
			gtk_container_add(
				GTK_CONTAINER(vectContainer.back()),
				pControl);
		}
		if (deControl.nID != 0) {
			RegisterDlgItem(pDialog, pControl, deControl.nID);
		}
		if (bDefaultButton) {
			GTK_WIDGET_SET_FLAGS(
				pControl,
				GTK_CAN_DEFAULT);
			gtk_widget_grab_default(pControl);
		}
		gtk_widget_show(pControl);
		pPrevControl = pControl;
	}
	return true;
}

// cleanup dialog

void CX88Dialog::CleanupDialog(GtkWidget* pDialog) {
	CDialogMap::iterator itDialog = m_mapDialog.find(pDialog);
	if (itDialog != m_mapDialog.end()) {
		delete (*itDialog).second;
		m_mapDialog.erase(itDialog);
	}
}

// execute modal loop of dialog

int CX88Dialog::ExecuteModalLoop(
	GtkWidget* pDialog,
	GtkWidget* /*pParent*/)
{
	m_nModalCounter++;
	g_object_ref(pDialog);
	int nResult = gtk_dialog_run(GTK_DIALOG(pDialog));
	gtk_widget_hide(pDialog);
	g_object_unref(pDialog);
	m_nModalCounter--;
	return nResult;
}

// end dialog

void CX88Dialog::EndDialog(
	GtkWidget* pDialog,
	int nResult)
{
	gtk_dialog_response(GTK_DIALOG(pDialog), nResult);
}

// register child widget to dialog

void CX88Dialog::RegisterDlgItem(
	GtkWidget* pDialog, GtkWidget* pControl, int nID)
{
	CDialogMap::iterator itDialog = m_mapDialog.find(pDialog);
	if (itDialog != m_mapDialog.end()) {
		(*itDialog).second->mapID2Widget[nID] = pControl;
		(*itDialog).second->mapWidget2ID[pControl] = nID;
	}
}

// get child widget from dialog

GtkWidget* CX88Dialog::GetDlgItem(GtkWidget* pDialog, int nID) {
	GtkWidget* pControl = NULL;
	CDialogMap::iterator itDialog = m_mapDialog.find(pDialog);
	if (itDialog != m_mapDialog.end()) {
		CID2WidgetMap::iterator itID =
			(*itDialog).second->mapID2Widget.find(nID);
		if (itID != (*itDialog).second->mapID2Widget.end()) {
			pControl = (*itID).second;
		}
	}
	return pControl;
}

#endif // X88_GUI

// is modeless dialog

bool CX88Dialog::IsModeless() const {
	return false;
}

// is created modeless dialog

bool CX88Dialog::IsCreatedModeless() const {
	return false;
}

// create modeless dialog

bool CX88Dialog::CreateModeless() {
	return false;
}

// close modeless dialog

bool CX88Dialog::CloseModeless() {
	return false;
}

////////////////////////////////////////////////////////////
// message box

// show message-box

int CX88Dialog::ShowMessageBox(
	CX88WndHandle hwndParent,
	int nType, int nButtons,
	const std::string& gstrCaption,
	const std::string& gstrText)
{

#ifdef X88_GUI_WINDOWS

	return MessageBox(
		hwndParent,
		gstrText.c_str(),
		gstrCaption.c_str(),
		nType | nButtons);

#elif defined(X88_GUI_GTK)

	m_nMessageBoxType = nType | nButtons;
	m_gstrMessageBoxCaption = gstrCaption;
	m_gstrMessageBoxText = gstrText;
	return ExecuteDialog(
		NX88Resource::IDD_MESSAGEBOX,
		hwndParent,
		MessageBoxDlgProc);

#endif // X88_GUI

}

////////////////////////////////////////////////////////////
// control

// show/hide control

void CX88Dialog::ShowControl(
	CX88WndHandle hwndControl,
	bool bShow)
{

#ifdef X88_GUI_WINDOWS

	if (hwndControl == NULL) {
		return;
	}
	ShowWindow(hwndControl, bShow? SW_SHOWNOACTIVATE: SW_HIDE);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_WIDGET(hwndControl)) {
		return;
	}
	if (bShow) {
		gtk_widget_show(hwndControl);
	} else {
		gtk_widget_hide(hwndControl);
	}

#endif // X88_GUI

}

// enable/disable control

void CX88Dialog::EnableControl(
	CX88WndHandle hwndControl,
	bool bEnable)
{

#ifdef X88_GUI_WINDOWS

	if (hwndControl == NULL) {
		return;
	}
	EnableWindow(hwndControl, bEnable? TRUE: FALSE);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_WIDGET(hwndControl)) {
		return;
	}
	gtk_widget_set_sensitive(hwndControl, bEnable? TRUE: FALSE);

#endif // X88_GUI

}

// set focus to control

void CX88Dialog::SetControlFocus(
	CX88WndHandle hwndControl)
{

#ifdef X88_GUI_WINDOWS

	if (hwndControl == NULL) {
		return;
	}
	SetFocus(hwndControl);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_WIDGET(hwndControl)) {
		return;
	}
	gtk_widget_grab_focus(hwndControl);

#endif // X88_GUI

}

// is check-button checked

bool CX88Dialog::IsCheckButtonChecked(
	CX88WndHandle hwndCheckButton)
{

#ifdef X88_GUI_WINDOWS

	if (hwndCheckButton == NULL) {
		return false;
	}
	return SendMessage(
		hwndCheckButton,
		BM_GETCHECK, 0, 0) == BST_CHECKED;

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_CHECK_BUTTON(hwndCheckButton)) {
		return false;
	}
	return gtk_toggle_button_get_active(
		GTK_TOGGLE_BUTTON(hwndCheckButton))!= FALSE;

#endif // X88_GUI

}

// set check-button checked

void CX88Dialog::SetCheckButtonChecked(
	CX88WndHandle hwndCheckButton,
	bool bCheck)
{

#ifdef X88_GUI_WINDOWS

	if (hwndCheckButton == NULL) {
		return;
	}
	SendMessage(
		hwndCheckButton,
		BM_SETCHECK, bCheck? BST_CHECKED: BST_UNCHECKED, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_CHECK_BUTTON(hwndCheckButton)) {
		return;
	}
	gtk_toggle_button_set_active(
		GTK_TOGGLE_BUTTON(hwndCheckButton),
		bCheck? TRUE: FALSE);
	gtk_widget_queue_draw(hwndCheckButton);

#endif // X88_GUI

}

// get selection number of radio-button

int CX88Dialog::GetRadioButtonSel(
	CX88WndHandle hwndRadioButton)
{

#ifdef X88_GUI_WINDOWS

	if (hwndRadioButton == NULL) {
		return -1;
	}
	for (
		;
		hwndRadioButton != NULL;
		hwndRadioButton = GetWindow(hwndRadioButton, GW_HWNDPREV))
	{
		if ((SendMessage(
					hwndRadioButton, WM_GETDLGCODE, 0, 0) &
				DLGC_RADIOBUTTON) == 0)
		{
			hwndRadioButton = NULL;
			break;
		} else if (
			(GetWindowLong(
				hwndRadioButton, GWL_STYLE) & WS_GROUP) != 0)
		{
			break;
		}
	}
	int nIndex = 0;
	for (
		;
		hwndRadioButton != NULL;
		hwndRadioButton = GetWindow(hwndRadioButton, GW_HWNDNEXT),
			nIndex++)
	{
		if (nIndex > 0) {
			if (((SendMessage(
						hwndRadioButton, WM_GETDLGCODE, 0, 0) &
					DLGC_RADIOBUTTON) == 0) ||
				((GetWindowLong(
					hwndRadioButton, GWL_STYLE) & WS_GROUP) != 0))
			{
				hwndRadioButton = NULL;
				break;
			}
		}
		if (SendMessage(
				hwndRadioButton,
				BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			break;
		}
	}
	if (hwndRadioButton == NULL) {
		nIndex = -1;
	}
	return nIndex;

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_RADIO_BUTTON(hwndRadioButton)) {
		return -1;
	}
	// radio-button group is reversed-order?
	GSList* pGroup = gtk_radio_button_get_group(
		GTK_RADIO_BUTTON(hwndRadioButton));
	int nIndex = g_slist_length(pGroup)-1;
	for (
		;
		pGroup != NULL;
		pGroup = pGroup->next,
			nIndex--)
	{
		if (!GTK_IS_RADIO_BUTTON(pGroup->data)) {
			pGroup = NULL;
			break;
		}
		if (gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(pGroup->data)) != FALSE)
		{
			break;
		}
	}
	if (pGroup == NULL) {
		nIndex = -1;
	}
	return nIndex;

#endif // X88_GUI

}

// set selection number of radio-button

void CX88Dialog::SetRadioButtonSel(
	CX88WndHandle hwndRadioButton,
	int nSel)
{

#ifdef X88_GUI_WINDOWS

	if (hwndRadioButton == NULL) {
		return;
	}
	for (
		;
		hwndRadioButton != NULL;
		hwndRadioButton = GetWindow(hwndRadioButton, GW_HWNDPREV))
	{
		if ((SendMessage(
					hwndRadioButton, WM_GETDLGCODE, 0, 0) &
				DLGC_RADIOBUTTON) == 0)
		{
			hwndRadioButton = NULL;
			break;
		} else if (
			(GetWindowLong(
				hwndRadioButton, GWL_STYLE) & WS_GROUP) != 0)
		{
			break;
		}
	}
	for (
		int nIndex = 0;
		hwndRadioButton != NULL;
		hwndRadioButton = GetWindow(hwndRadioButton, GW_HWNDNEXT),
			nIndex++)
	{
		if (nIndex > 0) {
			if (((SendMessage(
						hwndRadioButton, WM_GETDLGCODE, 0, 0) &
					DLGC_RADIOBUTTON) == 0) ||
				((GetWindowLong(
					hwndRadioButton, GWL_STYLE) & WS_GROUP) != 0))
			{
				break;
			}
		}
		SendMessage(
			hwndRadioButton,
			BM_SETCHECK, (nIndex == nSel)? BST_CHECKED: BST_UNCHECKED, 0);
	}

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_RADIO_BUTTON(hwndRadioButton)) {
		return;
	}
	// radio-button group is reversed-order?
	GSList* pGroup = gtk_radio_button_get_group(
		GTK_RADIO_BUTTON(hwndRadioButton));
	for (
		int nIndex = g_slist_length(pGroup)-1;
		pGroup != NULL;
		pGroup = pGroup->next,
			nIndex--)
	{
		if (!GTK_IS_RADIO_BUTTON(pGroup->data)) {
			break;
		}
		int nNowState =
				(gtk_toggle_button_get_active(
					GTK_TOGGLE_BUTTON(pGroup->data)) != FALSE)?
						TRUE: FALSE,
			nNewState = (nIndex == nSel)? TRUE: FALSE;
		if (nNowState != nNewState) {
			gtk_toggle_button_set_active(
				GTK_TOGGLE_BUTTON(pGroup->data), nNewState);
		}
	}

#endif // X88_GUI

}

// get static text

void CX88Dialog::GetStaticText(
	CX88WndHandle hwndStatic,
	std::string& gstrText)
{

#ifdef X88_GUI_WINDOWS

	gstrText = "";
	if (hwndStatic == NULL) {
		return;
	}
	int nLength = GetWindowTextLength(hwndStatic);
	if (nLength > 0) {
		std::vector<char> vectText(nLength+1);
		GetWindowText(hwndStatic, &vectText[0], nLength+1);
		vectText[nLength] = '\0';
		gstrText = &vectText[0];
	}

#elif defined(X88_GUI_GTK)

	gstrText = "";
	if (!GTK_IS_LABEL(hwndStatic)) {
		return;
	}
	gstrText = gtk_label_get_text(GTK_LABEL(hwndStatic));

#endif // X88_GUI

}

// set static text

void CX88Dialog::SetStaticText(
	CX88WndHandle hwndStatic,
	const std::string& gstrText)
{

#ifdef X88_GUI_WINDOWS

	if (hwndStatic == NULL) {
		return;
	}
	SetWindowText(hwndStatic, gstrText.c_str());

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_LABEL(hwndStatic)) {
		return;
	}
	gtk_label_set_text(GTK_LABEL(hwndStatic), gstrText.c_str());

#endif // X88_GUI

}

// get edit-box text

void CX88Dialog::GetEditBoxText(
	CX88WndHandle hwndEdit,
	std::string& gstrText)
{

#ifdef X88_GUI_WINDOWS

	gstrText = "";
	if (hwndEdit == NULL) {
		return;
	}
	int nLength = GetWindowTextLength(hwndEdit);
	if (nLength > 0) {
		std::vector<char> vectText(nLength+1);
		GetWindowText(hwndEdit, &vectText[0], nLength+1);
		vectText[nLength] = '\0';
		gstrText = &vectText[0];
	}

#elif defined(X88_GUI_GTK)

	gstrText = "";
	if (!GTK_IS_ENTRY(hwndEdit)) {
		return;
	}
	const gchar* pgszEntry = gtk_entry_get_text(
		GTK_ENTRY(hwndEdit));
	if (pgszEntry != NULL) {
		gstrText = pgszEntry;
	}

#endif // X88_GUI

}

// set edit-box text

void CX88Dialog::SetEditBoxText(
	CX88WndHandle hwndEdit,
	const std::string& gstrText)
{

#ifdef X88_GUI_WINDOWS

	if (hwndEdit == NULL) {
		return;
	}
	SetWindowText(hwndEdit, gstrText.c_str());

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_ENTRY(hwndEdit)) {
		return;
	}
	gtk_entry_set_text(
		GTK_ENTRY(hwndEdit), gstrText.c_str());

#endif // X88_GUI

}

// get listbox selection number

int CX88Dialog::GetListBoxSel(
	CX88WndHandle hwndListBox,
	std::vector<int>* pvectSel)
{

#ifdef X88_GUI_WINDOWS

	if (hwndListBox == NULL) {
		if (pvectSel != NULL) {
			pvectSel->clear();
		}
		return 0;
	}
	int nSelCount = (int)SendMessage(hwndListBox, LB_GETSELCOUNT, 0, 0);
	if (nSelCount == LB_ERR) {
		nSelCount = 0;
	}
	if (pvectSel != NULL) {
		pvectSel->clear();
		pvectSel->reserve(nSelCount);
		for (
			int nIndex = 0,
				nCount = (int)SendMessage(hwndListBox, LB_GETCOUNT, 0, 0);
			nIndex < nCount;
			nIndex++)
		{
			if (SendMessage(hwndListBox, LB_GETSEL, nIndex, 0) > 0) {
				pvectSel->push_back(nIndex);
			}
		}
	}
	return nSelCount;

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_TREE_VIEW(hwndListBox)) {
		if (pvectSel != NULL) {
			pvectSel->clear();
		}
		return 0;
	}
	GtkTreeModel* pTreeModel = gtk_tree_view_get_model(
		GTK_TREE_VIEW(hwndListBox));
	GtkTreeSelection* pTreeSelection = gtk_tree_view_get_selection(
		GTK_TREE_VIEW(hwndListBox));
	int nSelCount = 0;
	if (pvectSel != NULL) {
		pvectSel->clear();
	}
	GtkTreeIter iter;
	for (
		int nIndex = 0;
		gtk_tree_model_iter_nth_child(
			pTreeModel, &iter, NULL, nIndex);
		nIndex++)
	{
		if (gtk_tree_selection_iter_is_selected(
				pTreeSelection, &iter))
		{
			nSelCount++;
			if (pvectSel != NULL) {
				pvectSel->push_back(nIndex);
			}
		}
	}
	return nSelCount;

#endif // X88_GUI

}

// set listbox selection number

void CX88Dialog::SetListBoxSel(
	CX88WndHandle hwndListBox,
	const std::vector<int>* pvectSel)
{

#ifdef X88_GUI_WINDOWS

	if (hwndListBox == NULL) {
		return;
	}
	SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
	if (pvectSel != NULL) {
		for (
			int nSelIndex = 0;
			nSelIndex < (int)pvectSel->size();
			nSelIndex++)
		{
			SendMessage(
				hwndListBox, LB_GETSEL, TRUE, (*pvectSel)[nSelIndex]);
		}
	}

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_TREE_VIEW(hwndListBox)) {
		return;
	}
	if (pvectSel != NULL) {
		GtkTreeModel* pTreeModel = gtk_tree_view_get_model(
			GTK_TREE_VIEW(hwndListBox));
		GtkTreeSelection* pTreeSelection = gtk_tree_view_get_selection(
			GTK_TREE_VIEW(hwndListBox));
		gtk_tree_selection_unselect_all(
			pTreeSelection);
		int nSelCount = (int)pvectSel->size();
		GtkTreeIter iter;
		for (int i = 0; i < nSelCount; i++) {
			int nIndex = (*pvectSel)[i];
			if (gtk_tree_model_iter_nth_child(
					pTreeModel, &iter, NULL, nIndex))
			{
				gtk_tree_selection_select_iter(
					pTreeSelection, &iter);
			}
		}
	}

#endif // X88_GUI

}

// clear listbox

void CX88Dialog::ClearListBox(
	CX88WndHandle hwndListBox)
{

#ifdef X88_GUI_WINDOWS

	if (hwndListBox == NULL) {
		return;
	}
	SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_TREE_VIEW(hwndListBox)) {
		return;
	}
	GtkListStore* pListStore = GTK_LIST_STORE(
		gtk_tree_view_get_model(
			GTK_TREE_VIEW(hwndListBox)));
	if (pListStore == NULL) {
		return;
	}
	gtk_list_store_clear(pListStore);

#endif // X88_GUI

}

// add item to listbox

void CX88Dialog::AddListBoxItem(
	CX88WndHandle hwndListBox,
	const std::string& gstrItem)
{

#ifdef X88_GUI_WINDOWS

	if (hwndListBox == NULL) {
		return;
	}
	SendMessage(
		hwndListBox,
		LB_ADDSTRING, 0, (LPARAM)gstrItem.c_str());

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_TREE_VIEW(hwndListBox)) {
		return;
	}
	GtkListStore* pListStore = GTK_LIST_STORE(
		gtk_tree_view_get_model(
			GTK_TREE_VIEW(hwndListBox)));
	if (pListStore == NULL) {
		return;
	}
	GtkTreeIter iter;
	gtk_list_store_append(
		pListStore, &iter);
	gtk_list_store_set(
		pListStore, &iter,
		0, gstrItem.c_str(),
		1, 0,
		-1);

#endif // X88_GUI

}

// insert item to listbox

void CX88Dialog::InsertListBoxItem(
	CX88WndHandle hwndListBox,
	int nIndex, const std::string& gstrItem)
{

#ifdef X88_GUI_WINDOWS

	if (hwndListBox == NULL) {
		return;
	}
	SendMessage(
		hwndListBox,
		LB_INSERTSTRING, nIndex, (LPARAM)gstrItem.c_str());

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_TREE_VIEW(hwndListBox)) {
		return;
	}
	int nItemCount = GetListBoxItemCount(hwndListBox);
	if ((nIndex < 0) || (nIndex > nItemCount)) {
		nIndex = nItemCount;
	}
	GtkListStore* pListStore = GTK_LIST_STORE(
		gtk_tree_view_get_model(
			GTK_TREE_VIEW(hwndListBox)));
	if (pListStore == NULL) {
		return;
	}
	GtkTreeIter iter;
	gtk_list_store_insert(
		pListStore, &iter, nIndex);
	gtk_list_store_set(
		pListStore, &iter,
		0, gstrItem.c_str(),
		1, 0,
		-1);

#endif // X88_GUI

}

// delete item from listbox

void CX88Dialog::DeleteListBoxItem(
	CX88WndHandle hwndListBox, int nIndex)
{

#ifdef X88_GUI_WINDOWS

	if (hwndListBox == NULL) {
		return;
	}
	SendMessage(
		hwndListBox,
		LB_DELETESTRING, nIndex, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_TREE_VIEW(hwndListBox)) {
		return;
	}
	GtkListStore* pListStore = GTK_LIST_STORE(
		gtk_tree_view_get_model(
			GTK_TREE_VIEW(hwndListBox)));
	if (pListStore == NULL) {
		return;
	}
	GtkTreeModel* pTreeModel = gtk_tree_view_get_model(
		GTK_TREE_VIEW(hwndListBox));
	GtkTreeIter iter;
	if (gtk_tree_model_iter_nth_child(
			pTreeModel, &iter, NULL, nIndex))
	{
		gtk_list_store_remove(
			pListStore, &iter);
	}

#endif // X88_GUI

}

// get listbox item count

int CX88Dialog::GetListBoxItemCount(
	CX88WndHandle hwndListBox)
{

#ifdef X88_GUI_WINDOWS

	if (hwndListBox == NULL) {
		return 0;
	}
	return (int)SendMessage(hwndListBox, LB_GETCOUNT, 0, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_TREE_VIEW(hwndListBox)) {
		return 0;
	}
	GtkTreeModel* pTreeModel = gtk_tree_view_get_model(
		GTK_TREE_VIEW(hwndListBox));
	return gtk_tree_model_iter_n_children(
		pTreeModel,
		NULL);

#endif // X88_GUI

}

// get combobox selection number

int CX88Dialog::GetComboBoxSel(
	CX88WndHandle hwndComboBox)
{

#ifdef X88_GUI_WINDOWS

	if (hwndComboBox == NULL) {
		return -1;
	}
	return (int)SendMessage(
		hwndComboBox, CB_GETCURSEL, 0, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_COMBO_BOX(hwndComboBox)) {
		return -1;
	}
	return gtk_combo_box_get_active(
		GTK_COMBO_BOX(hwndComboBox));

#endif // X88_GUI

}

// clear combobox selection number

void CX88Dialog::SetComboBoxSel(
	CX88WndHandle hwndComboBox, int nSel)
{

#ifdef X88_GUI_WINDOWS

	if (hwndComboBox == NULL) {
		return;
	}
	SendMessage(
		hwndComboBox, CB_SETCURSEL, nSel, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_COMBO_BOX(hwndComboBox)) {
		return;
	}
	gtk_combo_box_set_active(
		GTK_COMBO_BOX(hwndComboBox),
		nSel);

#endif // X88_GUI

}

// clear combobox

void CX88Dialog::ClearComboBox(
	CX88WndHandle hwndComboBox)
{

#ifdef X88_GUI_WINDOWS

	if (hwndComboBox == NULL) {
		return;
	}
	SendMessage(hwndComboBox, CB_RESETCONTENT, 0, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_COMBO_BOX(hwndComboBox)) {
		return;
	}
	int nCount = GetComboBoxItemCount(hwndComboBox);
	while (nCount > 0) {
		DeleteComboBoxItem(
			hwndComboBox,
			--nCount);
	}

#endif // X88_GUI

}

// add item to combobox

void CX88Dialog::AddComboBoxItem(
	CX88WndHandle hwndComboBox,
	const std::string& gstrItem)
{

#ifdef X88_GUI_WINDOWS

	if (hwndComboBox == NULL) {
		return;
	}
	SendMessage(
		hwndComboBox,
		CB_ADDSTRING, 0, (LPARAM)gstrItem.c_str());

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_COMBO_BOX(hwndComboBox)) {
		return;
	}
	gtk_combo_box_append_text(
		GTK_COMBO_BOX(hwndComboBox),
		gstrItem.c_str());

#endif // X88_GUI

}

// insert item to combobox

void CX88Dialog::InsertComboBoxItem(
	CX88WndHandle hwndComboBox,
	int nIndex, const std::string& gstrItem)
{

#ifdef X88_GUI_WINDOWS

	if (hwndComboBox == NULL) {
		return;
	}
	SendMessage(
		hwndComboBox,
		CB_INSERTSTRING, nIndex, (LPARAM)gstrItem.c_str());

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_COMBO_BOX(hwndComboBox)) {
		return;
	}
	gtk_combo_box_insert_text(
		GTK_COMBO_BOX(hwndComboBox),
		nIndex,
		gstrItem.c_str());

#endif // X88_GUI

}

// delete item from combobox

void CX88Dialog::DeleteComboBoxItem(
	CX88WndHandle hwndComboBox, int nIndex)
{

#ifdef X88_GUI_WINDOWS

	if (hwndComboBox == NULL) {
		return;
	}
	SendMessage(hwndComboBox, CB_DELETESTRING, nIndex, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_COMBO_BOX(hwndComboBox)) {
		return;
	}
	gtk_combo_box_remove_text(
		GTK_COMBO_BOX(hwndComboBox),
		nIndex);

#endif // X88_GUI

}

// get combobox item count

int CX88Dialog::GetComboBoxItemCount(
	CX88WndHandle hwndComboBox)
{

#ifdef X88_GUI_WINDOWS

	if (hwndComboBox == NULL) {
		return 0;
	}
	return (int)SendMessage(hwndComboBox, CB_GETCOUNT, 0, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_COMBO_BOX(hwndComboBox)) {
		return 0;
	}
	GtkTreeModel* pTreeModel = gtk_combo_box_get_model(
		GTK_COMBO_BOX(hwndComboBox));
	return gtk_tree_model_iter_n_children(
		pTreeModel,
		NULL);

#endif // X88_GUI

}

// get slider value

int CX88Dialog::GetSliderValue(
	CX88WndHandle hwndSlider)
{

#ifdef X88_GUI_WINDOWS

	if (hwndSlider == NULL) {
		return 0;
	}
	return (int)SendMessage(hwndSlider, TBM_GETPOS, 0, 0);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_RANGE(hwndSlider)) {
		return 0;
	}
	int nValue = 0;
	GtkAdjustment* pAdjust = gtk_range_get_adjustment(
		GTK_RANGE(hwndSlider));
	if (pAdjust != NULL) {
		nValue = (int)(pAdjust->value+((pAdjust->value >= 0)? 0.5: -0.5));
	}
	return nValue;

#endif // X88_GUI

}

// set slider value

void CX88Dialog::SetSliderValue(
	CX88WndHandle hwndSlider, int nValue)
{

#ifdef X88_GUI_WINDOWS

	if (hwndSlider == NULL) {
		return;
	}
	SendMessage(hwndSlider, TBM_SETPOS, (WPARAM)TRUE, nValue);

#elif defined(X88_GUI_GTK)

	if (!GTK_IS_RANGE(hwndSlider)) {
		return;
	}
	GtkAdjustment* pAdjust = gtk_range_get_adjustment(
		GTK_RANGE(hwndSlider));
	if (pAdjust != NULL) {
		pAdjust->value = (gfloat)nValue;
		gtk_widget_queue_draw(hwndSlider);
	}

#endif // X88_GUI

}

// get spin-button value

int CX88Dialog::GetSpinButtonValue(
	CX88WndHandle hwndEdit, CX88WndHandle hwndSpin)
{

#ifdef X88_GUI_WINDOWS

	int nValue = 0;
	if (hwndEdit != NULL) {
		std::string strText;
		GetEditBoxText(hwndEdit, strText);
		nValue = AtoI(strText);
	} else if (hwndSpin != NULL) {
		nValue = (int)SendMessage(
			hwndSpin,
			UDM_GETPOS, 0, 0);
	}
	return nValue;

#elif defined(X88_GUI_GTK)

	(void)hwndEdit;

	int nValue = 0;
	if (GTK_IS_SPIN_BUTTON(hwndSpin)) {
		nValue = gtk_spin_button_get_value_as_int(
			GTK_SPIN_BUTTON(hwndSpin));
	}
	return nValue;

#endif // X88_GUI

}

// set spin-button value

void CX88Dialog::SetSpinButtonValue(
	CX88WndHandle hwndEdit, CX88WndHandle hwndSpin,
	int nValue)
{

#ifdef X88_GUI_WINDOWS

	if (hwndEdit != NULL) {
		std::string strValue = FormatStr("%d", nValue);
		SetWindowText(hwndEdit, strValue.c_str());
	}
	if (hwndSpin != NULL) {
		SendMessage(
			hwndSpin,
			UDM_SETPOS, 0, MAKELPARAM(nValue, 0));
	}

#elif defined(X88_GUI_GTK)

	(void)hwndEdit;

	if (GTK_IS_SPIN_BUTTON(hwndSpin)) {
		gtk_spin_button_set_value(
			GTK_SPIN_BUTTON(hwndSpin),
			(gfloat)nValue);
	}

#endif // X88_GUI

}

// set spin-button range

void CX88Dialog::SetSpinButtonRange(
	CX88WndHandle hwndSpin,
	int nLower, int nUpper)
{

#ifdef X88_GUI_WINDOWS

	if (hwndSpin != NULL) {
		SendMessage(
			hwndSpin,
			UDM_SETRANGE, 0, MAKELPARAM(nUpper, nLower));
	}

#elif defined(X88_GUI_GTK)

	if (GTK_IS_SPIN_BUTTON(hwndSpin)) {
		GtkAdjustment* pAdjustment =
			gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(hwndSpin));
		if (pAdjustment != NULL) {
			pAdjustment->lower = (gfloat)nLower;
			pAdjustment->upper = (gfloat)nUpper;
		}
	}

#endif // X88_GUI

}

////////////////////////////////////////////////////////////
// utility


#ifdef X88_GUI_GTK

// set control font

void CX88Dialog::SetControlFont(
	CX88WndHandle hwndTarget,
	const std::string& gstrFontFamilyName,
	int nFontSize)
{
	GtkRcStyle* pOldStyle = gtk_widget_get_modifier_style(hwndTarget);
	GtkRcStyle* pNewStyle;
	if (pOldStyle == NULL) {
		pNewStyle = gtk_rc_style_new();
	} else {
		pNewStyle = gtk_rc_style_copy(pOldStyle);
	}
	if (pNewStyle->font_desc == NULL) {
		pNewStyle->font_desc =
			pango_font_description_from_string(
				gstrFontFamilyName.c_str());
	} else {
		pango_font_description_set_family(
			pNewStyle->font_desc,
			gstrFontFamilyName.c_str());
	}
	if (nFontSize > 0) {
		pango_font_description_set_size(
			pNewStyle->font_desc,
			nFontSize*PANGO_SCALE);
	} else if (nFontSize < 0) {
		pango_font_description_set_absolute_size(
			pNewStyle->font_desc,
			(0.0-nFontSize)*PANGO_SCALE);
	}
	gtk_widget_modify_style(hwndTarget, pNewStyle);
	g_object_unref(pNewStyle);
}

#endif // X88_GUI
