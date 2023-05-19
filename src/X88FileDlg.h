////////////////////////////////////////////////////////////
// X88000 File Dialog
//
// Written by Manuke

#ifndef X88FileDlg_DEFINED
#define X88FileDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88FileDlg;

////////////////////////////////////////////////////////////
// include

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88FileDlg

class CX88FileDlg :
	public CX88Dialog
{
// typedef
protected:
	// type of file name container
	typedef std::vector<std::string> CFileNamesContainer;

// attribute
protected:
	// open dialog
	bool m_bOpenDialog;
	// file name container(filesystem encoding)
	CFileNamesContainer m_fcntFileNames;
	// multi selectable
	bool m_bMultiSelect;
	// enable read-only
	bool m_bEnableReadOnly;
	// read-only
	bool m_bReadOnly;
	// title(GUI encoding)
	std::string m_gstrTitle;
	// filter(GUI encoding)
	std::string m_gstrFilter;
	// default ext(filesystem encoding)
	std::string m_fstrDefExt;

#ifdef X88_GUI_WINDOWS

	// file dialog struct
	OPENFILENAME m_ofn;

#elif defined(X88_GUI_GTK)

#if 1 // GtkFileSelectionDialog

	// file name(filesystem encoding)
	static std::string m_fstrFileName;

#endif // GtkFile???Dialog

#endif // X88_GUI

public:
	// is open dialog
	bool IsOpenDialog() const {
		return m_bOpenDialog;
	}
	// is multi selectable
	bool IsMultiSelect() const {
		return m_bMultiSelect;
	}
	// is enable read-only
	bool IsEnableReadOnly() const {
		return m_bEnableReadOnly;
	}
	// is read-only
	bool IsReadOnly() const {
		return m_bReadOnly;
	}

// create & destroy
public:
	// standard constructor
	CX88FileDlg(
		CX88WndHandle hwndParent,
		bool bOpenDialog,
		const std::string& gstrTitle,
		const std::string& gstrFilter,
		const std::string& fstrDefExt,
		bool bMultiSelect = false,
		bool bEnableReadOnly = false);
	// destructor
	virtual ~CX88FileDlg();

// implementation
protected:

#ifdef X88_GUI_WINDOWS

	// hook procedure
	static UINT CALLBACK HookProc(
		HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam);

#elif defined(X88_GUI_GTK)

#if 1 // GtkFileSelectionDialog

	// OK clicked signal
	static void OnClickedSignalOK(
		GtkButton* pButton, gpointer pData);
	// cancel clicked signal
	static void OnClickedSignalCancel(
		GtkButton* pButton, gpointer pData);

#endif // GtkFile???Dialog

#endif // X88_GUI

// operation
protected:
	// create file name list
	virtual void CreateFileNameList(const char* pfszFileName);

public:
	// create modal dialog
	virtual int DoModal();
	// get file name(filesystem encoding)
	virtual std::string GetFileName(int nIndex = 0) const;
	// get file name count
	virtual int GetFileNameCount();
};

#endif // X88FileDlg_DEFINED
