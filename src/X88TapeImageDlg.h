////////////////////////////////////////////////////////////
// X88000 TapeImage Dialog
//
// Written by Manuke

#ifndef X88TapeImageDlg_DEFINED
#define X88TapeImageDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88TapeImageDlg;

////////////////////////////////////////////////////////////
// include

#include "TapeImage.h"

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88TapeImageDlg

class CX88TapeImageDlg :
	public CX88Dialog
{
// attribute
protected:
	// tape image for loading
	static CTapeImage* m_ptimageLoad;
	// tape image for saving
	static CTapeImage* m_ptimageSave;

public:
	// set tape image for loading
	static void SetLoadTapeImage(CTapeImage& ptimageLoad) {
		m_ptimageLoad = &ptimageLoad;
	}
	// set tape image for saving
	static void SetSaveTapeImage(CTapeImage& ptimageSave) {
		m_ptimageSave = &ptimageSave;
	}

// create & destroy
public:
	// standard constructor
	CX88TapeImageDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88TapeImageDlg();

// implementation
protected:

#ifdef X88_GUI_WINDOWS

	// dialog procedure
	static BOOL CALLBACK DlgProc(
		HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM lParam);

#elif defined(X88_GUI_GTK)

	// dialog procedure(initialize & dispose)
	static bool DlgProc(
		GtkWidget* pDialog,
		bool bInitialize,
		int nID);
	// image operation clicked signal
	static void OnClickedSignalImageOperate(
		GtkButton* pButton, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// is file ext equals CMT
	static bool IsFileExtCMT(const std::string& fstrFileName);
	// set children
	static void SetChildren(CX88WndHandle hdlg);
	// on open image for loading
	static void OnLoadImageOpen(CX88WndHandle hdlg);
	// on forward image for loading
	static void OnLoadImageFwd(CX88WndHandle hdlg);
	// on rewind image for loading
	static void OnLoadImageRew(CX88WndHandle hdlg);
	// on save image for saving
	static void OnSaveImageSave(CX88WndHandle hdlg);
	// on erase image for saving
	static void OnSaveImageErase(CX88WndHandle hdlg);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88TapeImageDlg_DEFINED
