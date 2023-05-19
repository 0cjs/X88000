////////////////////////////////////////////////////////////
// X88000 DiskImage Dialog
//
// Written by Manuke

#ifndef X88DiskImageDlg_DEFINED
#define X88DiskImageDlg_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88DiskImageDlg;

////////////////////////////////////////////////////////////
// include

#include "PC88Fdc.h"

#include "DiskImageCollection.h"

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88DiskImageDlg

class CX88DiskImageDlg :
	public CX88Dialog
{
// attribute
protected:
	// FDC
	static CPC88Fdc* m_pFdc;
	// disk image collection
	static CDiskImageCollection* m_pDiskImageCollection;
	// changed disk
	static bool m_bChangedDisk;
	// popupped list of combobox
	static bool m_bPopuppedComboList;

public:
	// set FDC
	static void SetFdc(CPC88Fdc& fdc) {
		m_pFdc = &fdc;
	}
	// set disk image collection
	static void SetDiskImageCollection(CDiskImageCollection& dicDisks) {
		m_pDiskImageCollection = &dicDisks;
	}

// create & destroy
public:
	// standard constructor
	CX88DiskImageDlg(CX88WndHandle hwndParent);
	// destructor
	virtual ~CX88DiskImageDlg();

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
	// image selection changed signal
	static void OnSelectionChangedSignalImages(
		GtkTreeSelection* pTreeSelection, gpointer pData);
	// image operation clicked signal
	static void OnClickedSignalImageOperate(
		GtkButton* pButton, gpointer pData);
	// floppy disk drive combobox changed signal
	static void OnChangedSignalDiskDriveCombo(
		GtkWidget* pWidget, gpointer pData);
	// floppy disk drive write-protect toggled signal
	static void OnToggledSignalDiskDriveProtect(
		GtkToggleButton* pToggleButton, gpointer pData);

#endif // X88_GUI

// operation
protected:
	// set children
	static void SetChildren(CX88WndHandle hdlg, int nUpdateOfs = -1);
	// set remove erase button
	static void SetRemoveEraseButton(CX88WndHandle hdlg);
	// on added disk image
	static void OnImageAdd(CX88WndHandle hdlg);
	// on removed disk image
	static void OnImageRemove(CX88WndHandle hdlg);
	// on erase all disk image
	static void OnImageErase(CX88WndHandle hdlg);
	// on changed disk selection
	static void OnDiskDriveChange(CX88WndHandle hdlg, int nID);
	// on changed disk write-protect
	static void OnDiskDriveProtect(CX88WndHandle hdlg, int nID);

public:
	// create modal dialog
	virtual int DoModal();
};

#endif // X88DiskImageDlg_DEFINED
