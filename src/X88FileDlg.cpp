////////////////////////////////////////////////////////////
// X88000 File Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88FileDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88FileDlg

////////////////////////////////////////////////////////////
// attribute

#ifdef X88_GUI_GTK

#if 1 // GtkFileSelectionDialog

// file name(filesystem encoding)

std::string CX88FileDlg::m_fstrFileName;

#endif // GtkFile???Dialog

#endif // X88_GUI

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88FileDlg::CX88FileDlg(
		CX88WndHandle hwndParent,
		bool bOpenDialog,
		const std::string& gstrTitle,
		const std::string& gstrFilter,
		const std::string& fstrDefExt,
		bool bMultiSelect,
		bool bEnableReadOnly) :
	CX88Dialog(hwndParent),
	m_bOpenDialog(bOpenDialog),
	m_bMultiSelect(bOpenDialog && bMultiSelect),
	m_bEnableReadOnly(bEnableReadOnly),
	m_bReadOnly(false),
	m_gstrTitle(gstrTitle),
	m_gstrFilter(gstrFilter),
	m_fstrDefExt(fstrDefExt)
{

#ifdef X88_GUI_WINDOWS

	size_t nFind = 0;
	while (
		(nFind = m_gstrFilter.find('|', nFind)) !=
			std::string::npos)
	{
		m_gstrFilter[nFind] = '\0';
	}
	memset(&m_ofn, 0x00, sizeof(m_ofn));
	m_ofn.lStructSize = sizeof(OPENFILENAME);
	m_ofn.hInstance = GetResourceHandle();
	m_ofn.lpstrFilter = m_gstrFilter.c_str();
	m_ofn.lpstrInitialDir = ".";
	m_ofn.lpstrTitle = m_gstrTitle.c_str();
	m_ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING |
		(IsOpenDialog()? OFN_FILEMUSTEXIST: OFN_OVERWRITEPROMPT) |
		(IsMultiSelect()? OFN_ALLOWMULTISELECT: 0) |
		(IsEnableReadOnly()? 0: OFN_HIDEREADONLY);
	m_ofn.lpstrDefExt = m_fstrDefExt.c_str();
	m_ofn.lpfnHook = HookProc;
	m_ofn.nMaxFile = IsMultiSelect()? (_MAX_PATH*16): _MAX_PATH;
	m_ofn.lpstrFile = new char[m_ofn.nMaxFile];

#elif defined(X88_GUI_GTK)

#if 1 // GtkFileSelectionDialog

	m_fstrFileName = "";

	m_bMultiSelect = false;

#endif // GtkFile???Dialog

#endif // X88_GUI

}

// destructor

CX88FileDlg::~CX88FileDlg() {

#ifdef X88_GUI_WINDOWS

	delete [] m_ofn.lpstrFile;

#elif defined(X88_GUI_GTK)

#if 1 // GtkFileSelectionDialog

	m_fstrFileName = "";

#endif // GtkFile???Dialog

#endif // X88_GUI

}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// hook procedure

UINT CALLBACK CX88FileDlg::HookProc(
	HWND hdlg, UINT nMessage, WPARAM /*wParam*/, LPARAM lParam)
{
	UINT nResult = TRUE;
	switch (nMessage) {
	case WM_NOTIFY:
		{ // dummy block
			NMHDR* pnmh = (NMHDR*)lParam;
			if (pnmh->code == CDN_INITDONE) {
				HWND hdlgFIle = GetParent(hdlg);
				if (hdlgFIle != NULL) {
					Centering(hdlgFIle);
				}
			} else {
				nResult = FALSE;
			}
		}
		break;
	default:
		nResult = FALSE;
		break;
	}
	return nResult;
}

#elif defined(X88_GUI_GTK)

#if 1 // GtkFileSelectionDialog

// OK clicked signal

void CX88FileDlg::OnClickedSignalOK(
	GtkButton* /*pButton*/, gpointer pData)
{
	GtkWidget* pFileSel = GTK_WIDGET(pData);
	m_fstrFileName = gtk_file_selection_get_filename(
		GTK_FILE_SELECTION(pFileSel));
	EndDialog(pFileSel, IDOK);
}

// cancel clicked signal

void CX88FileDlg::OnClickedSignalCancel(
	GtkButton* /*pButton*/, gpointer pData)
{
	GtkWidget* pFileSel = GTK_WIDGET(pData);
	EndDialog(pFileSel, IDCANCEL);
}

#endif // GtkFile???Dialog

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// create file name list

void CX88FileDlg::CreateFileNameList(const char* pfszFileName) {
	int nLength = (int)strlen(pfszFileName);
	if (nLength > 0) {
		const char* pfszFileName2 = pfszFileName+nLength+1;
		if (IsMultiSelect() && (*pfszFileName2 != '\0')) {
			for (
				;
				*pfszFileName2 != '\0';
				pfszFileName2 += strlen(pfszFileName2)+1)
			{
				char* pfszFileName3 = new char[_MAX_PATH];

#ifdef X88_PLATFORM_WINDOWS

				_makepath(
					pfszFileName3,
					NULL, pfszFileName, pfszFileName2, NULL);

#elif defined(X88_PLATFORM_UNIX)

				strcpy(pfszFileName3, pfszFileName);
				strcat(pfszFileName3, pfszFileName2);

#endif // X88_PLATFORM

				m_fcntFileNames.push_back(pfszFileName3);
				delete [] pfszFileName3;
			}
		} else {
			m_fcntFileNames.push_back(pfszFileName);
		}
	}
}

// create modal dialog

int CX88FileDlg::DoModal() {
	int nResult;

#ifdef X88_GUI_WINDOWS

	if (m_ofn.lpstrFile == NULL) {
		return IDCANCEL;
	}
	m_ofn.lpstrFile[0] = '\0';
	m_ofn.hwndOwner = GetParentWndHandle();
	if (IsOpenDialog()) {
		nResult = (GetOpenFileName(&m_ofn) != FALSE)? IDOK: IDCANCEL;
		m_bReadOnly = ((m_ofn.Flags & OFN_READONLY) != 0);
	} else {
		nResult = (GetSaveFileName(&m_ofn) != FALSE)? IDOK: IDCANCEL;
	}
	m_fcntFileNames.clear();
	if (nResult == IDOK) {
		CreateFileNameList(m_ofn.lpstrFile);
	}

#elif defined(X88_GUI_GTK)

#if 1 // GtkFileSelectionDialog

	nResult = IDCANCEL;
	m_fcntFileNames.clear();
	GtkWidget* pFileSel = gtk_file_selection_new(m_gstrTitle.c_str());
	if (pFileSel != NULL) {
		g_object_ref(pFileSel);
		BuildDialog(pFileSel, NX88Resource::IDD_NONE);
		g_signal_connect(
			GTK_FILE_SELECTION(pFileSel)->ok_button, "clicked",
			G_CALLBACK(OnClickedSignalOK),
			pFileSel);
		g_signal_connect(
			GTK_FILE_SELECTION(pFileSel)->cancel_button, "clicked",
			G_CALLBACK(OnClickedSignalCancel),
			pFileSel);
		nResult = ExecuteModalLoop(pFileSel, GetParentWndHandle());
		if ((nResult == IDOK) && !m_fstrFileName.empty()) {
			CreateFileNameList(m_fstrFileName.c_str());
			m_fstrFileName = "";
		}
		gtk_widget_destroy(pFileSel);
		g_object_unref(pFileSel);
	}

#else // GtkFileChooserDialog

	nResult = IDCANCEL;
	m_fcntFileNames.clear();
	GtkWidget* pDialog = gtk_file_chooser_dialog_new(
		m_gstrTitle.c_str(),
		GTK_WINDOW(GetParentWndHandle()),
		m_bOpenDialog?
			GTK_FILE_CHOOSER_ACTION_OPEN:
			GTK_FILE_CHOOSER_ACTION_SAVE,
		GTK_STOCK_CANCEL, IDCANCEL,
		m_bOpenDialog? GTK_STOCK_OPEN: GTK_STOCK_SAVE, IDOK,
		NULL);
	if (pDialog != NULL) {
		g_object_ref(pDialog);
		if (m_bOpenDialog) {
			gtk_file_chooser_set_select_multiple(
				GTK_FILE_CHOOSER(pDialog),
				m_bMultiSelect? TRUE: FALSE);
		} else {
			gtk_file_chooser_set_do_overwrite_confirmation(
				GTK_FILE_CHOOSER(pDialog),
				TRUE);
		}
		nResult = ExecuteModalLoop(pDialog, GetParentWndHandle());
		if (nResult == IDOK) {
			if (!m_bMultiSelect) {
				char* pfszFileName = gtk_file_chooser_get_filename(
					GTK_FILE_CHOOSER(pDialog));
				m_fcntFileNames.push_back(pfszFileName);
				g_free(pfszFileName);
			} else {
				GSList* pFileNames = gtk_file_chooser_get_filenames(
					GTK_FILE_CHOOSER(pDialog));
				GSList* pFileNames2 = pFileNames;
				while (pFileNames2 != NULL) {
					char* pfszFileName = (char*)pFileNames2->data;
					m_fcntFileNames.push_back(pfszFileName);
					g_free(pfszFileName);
					pFileNames2 = pFileNames2->next;
				}
				g_slist_free(pFileNames);
			}
		}
		gtk_widget_destroy(pDialog);
		g_object_unref(pDialog);
	}

#endif // GtkFile???Dialog

#endif // X88_GUI

	if (m_fcntFileNames.size() <= 0) {
		nResult = IDCANCEL;
	}
	return nResult;
}

// get file name(filesystem encoding)

std::string CX88FileDlg::GetFileName(int nIndex) const {
	std::string fstrFileNames;
	if ((nIndex >= 0) && (nIndex < (int)m_fcntFileNames.size())) {
		CFileNamesContainer::const_iterator itFileName =
			m_fcntFileNames.begin();
		std::advance(itFileName, nIndex);
		fstrFileNames = *itFileName;
	}
	return fstrFileNames;
}

// get file name count

int CX88FileDlg::GetFileNameCount() {
	return (int)m_fcntFileNames.size();
}
