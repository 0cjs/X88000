////////////////////////////////////////////////////////////
// X88000 DiskImage Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88DiskImageDlg.h"

#include "X88FileDlg.h"
#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88DiskImageDlg

////////////////////////////////////////////////////////////
// attribute

// FDC

CPC88Fdc* CX88DiskImageDlg::m_pFdc;

// disk image collection

CDiskImageCollection* CX88DiskImageDlg::m_pDiskImageCollection;

// changed disk

bool CX88DiskImageDlg::m_bChangedDisk;

// popupped list of combobox

bool CX88DiskImageDlg::m_bPopuppedComboList;

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88DiskImageDlg::CX88DiskImageDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	m_pFdc = NULL;
	m_pDiskImageCollection = NULL;
	m_bChangedDisk = false;
	m_bPopuppedComboList = false;
}

// destructor

CX88DiskImageDlg::~CX88DiskImageDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88DiskImageDlg::DlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		{ // dummy block
			if (m_pFdc->GetDriveCount() <= 2) {
				RECT rectDrive1, rectDrive3;
				GetWindowRect(GetDlgItem(hdlg, IDC_DISK_DRIVE1), &rectDrive1);
				GetWindowRect(GetDlgItem(hdlg, IDC_DISK_DRIVE3), &rectDrive3);
				int nReduce = rectDrive3.top-rectDrive1.top;
				RECT rectReduce;
				GetWindowRect(hdlg, &rectReduce);
				SetWindowPos(
					hdlg, NULL,
					0,
					0,
					rectReduce.right-rectReduce.left,
					rectReduce.bottom-rectReduce.top-nReduce,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				HWND hstcGroup = GetDlgItem(hdlg, IDC_DISK_DRIVE_GROUP);
				GetWindowRect(hstcGroup, &rectReduce);
				SetWindowPos(
					hstcGroup, NULL,
					0,
					0,
					rectReduce.right-rectReduce.left,
					rectReduce.bottom-rectReduce.top-nReduce,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			for (int nDrive = 0; nDrive < 4; nDrive++) {
				SendDlgItemMessage(
					hdlg, IDC_DISK_DRIVE1+nDrive,
					CB_SETEXTENDEDUI, TRUE, 0);
				if (nDrive >= m_pFdc->GetDriveCount()) {
					ShowControl(
						GetDlgItem(hdlg, IDC_DISK_DRIVE1+nDrive),
						false);
					ShowControl(
						GetDlgItem(hdlg, IDC_DISK_DRIVE1_LABEL+nDrive),
						false);
					ShowControl(
						GetDlgItem(hdlg, IDC_DISK_DRIVE1_PROTECT+nDrive),
						false);
				}
			}
			Centering(hdlg);
			SetChildren(hdlg);
			SetRemoveEraseButton(hdlg);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, LOWORD(wParam));
			break;
		case IDC_IMAGES:
			SetRemoveEraseButton(hdlg);
			break;
		case IDC_IMAGE_ADD:
			OnImageAdd(hdlg);
			break;
		case IDC_IMAGE_REMOVE:
			OnImageRemove(hdlg);
			break;
		case IDC_IMAGE_ERASE:
			OnImageErase(hdlg);
			break;
		case IDC_DISK_DRIVE1:
		case IDC_DISK_DRIVE2:
		case IDC_DISK_DRIVE3:
		case IDC_DISK_DRIVE4:
			switch (HIWORD(wParam)) {
			case CBN_DROPDOWN:
				m_bPopuppedComboList = true;
				m_bChangedDisk = false;
				break;
			case CBN_SELENDOK:
			case CBN_SELENDCANCEL:
				m_bPopuppedComboList = false;
				if (m_bChangedDisk) {
					m_bChangedDisk = false;
					OnDiskDriveChange(hdlg, LOWORD(wParam));
				}
				break;
			case CBN_SELCHANGE:
				if (m_bPopuppedComboList) {
					m_bChangedDisk = true;
				} else {
					OnDiskDriveChange(hdlg, LOWORD(wParam));
				}
				break;
			}
			break;
		case IDC_DISK_DRIVE1_PROTECT:
		case IDC_DISK_DRIVE2_PROTECT:
		case IDC_DISK_DRIVE3_PROTECT:
		case IDC_DISK_DRIVE4_PROTECT:
			if (HIWORD(wParam) == BN_CLICKED) {
				OnDiskDriveProtect(hdlg, LOWORD(wParam));
			}
			break;
		}
		break;
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}

#elif defined(X88_GUI_GTK)

// dialog procedure(initialize & dispose)

bool CX88DiskImageDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		unsigned nState = (1 << (m_pFdc->GetDriveCount()-1))-1;
		if (!BuildDialog(pDialog, nID, nState)) {
			return false;
		}
		SetChildren(pDialog);
		SetRemoveEraseButton(pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_IMAGE_ADD), "clicked",
			G_CALLBACK(OnClickedSignalImageOperate),
			pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_IMAGE_REMOVE), "clicked",
			G_CALLBACK(OnClickedSignalImageOperate),
			pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_IMAGE_ERASE), "clicked",
			G_CALLBACK(OnClickedSignalImageOperate),
			pDialog);
		g_signal_connect(
			gtk_tree_view_get_selection(
				GTK_TREE_VIEW(
					GetDlgItem(pDialog, IDC_IMAGES))),
			"changed",
			G_CALLBACK(OnSelectionChangedSignalImages),
			pDialog);
		for (int nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
			int nID = IDC_DISK_DRIVE1+nDrive;
			GtkWidget* pcomboDrive = GetDlgItem(pDialog, nID);
			if (pcomboDrive != NULL) {
				g_signal_connect(
					pcomboDrive, "changed",
					G_CALLBACK(OnChangedSignalDiskDriveCombo),
					pDialog);
			}
			nID = IDC_DISK_DRIVE1_PROTECT+nDrive;
			GtkWidget* pcheckDiskProtect = GetDlgItem(pDialog, nID);
			if (pcheckDiskProtect != NULL) {
				g_signal_connect(
					pcheckDiskProtect, "toggled",
					G_CALLBACK(OnToggledSignalDiskDriveProtect),
					pDialog);
			}
		}
	}
	return true;
}

// image selection changed signal

void CX88DiskImageDlg::OnSelectionChangedSignalImages(
	GtkTreeSelection* /*pTreeSelection*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	CSignalBlocker blocker(pDialog);
	if (blocker.IsBlocked()) {
		return;
	}
	SetRemoveEraseButton(pDialog);
}

// image operation clicked signal

void CX88DiskImageDlg::OnClickedSignalImageOperate(
	GtkButton* pButton, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	CSignalBlocker blocker(pDialog);
	if (blocker.IsBlocked()) {
		return;
	}
	int nID = GetDlgItemID(
		pDialog, GTK_WIDGET(pButton));
	switch (nID) {
	case IDC_IMAGE_ADD:
		OnImageAdd(pDialog);
		break;
	case IDC_IMAGE_REMOVE:
		OnImageRemove(pDialog);
		break;
	case IDC_IMAGE_ERASE:
		OnImageErase(pDialog);
		break;
	}
}

// floppy disk drive combobox changed signal

void CX88DiskImageDlg::OnChangedSignalDiskDriveCombo(
		GtkWidget* pWidget, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	CSignalBlocker blocker(pDialog);
	if (blocker.IsBlocked()) {
		return;
	}
	int nID = GetDlgItemID(
		pDialog, GTK_WIDGET(pWidget));
	switch (nID) {
	case IDC_DISK_DRIVE1:
	case IDC_DISK_DRIVE2:
	case IDC_DISK_DRIVE3:
	case IDC_DISK_DRIVE4:
		OnDiskDriveChange(pDialog, nID);
		break;
	}
}

// floppy disk drive write-protect toggled signal

void CX88DiskImageDlg::OnToggledSignalDiskDriveProtect(
	GtkToggleButton* pToggleButton, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	CSignalBlocker blocker(pDialog);
	if (blocker.IsBlocked()) {
		return;
	}
	int nID = GetDlgItemID(
		pDialog, GTK_WIDGET(pToggleButton));
	switch (nID) {
	case IDC_DISK_DRIVE1_PROTECT:
	case IDC_DISK_DRIVE2_PROTECT:
	case IDC_DISK_DRIVE3_PROTECT:
	case IDC_DISK_DRIVE4_PROTECT:
		OnDiskDriveProtect(pDialog, nID);
		break;
	}
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// set children

void CX88DiskImageDlg::SetChildren(CX88WndHandle hdlg, int nUpdateOfs) {
	CX88WndHandle hlistImages,
		ahcomboDiskDrive[CPC88Fdc::DRIVE_MAX],
		ahcheckDiskProtect[CPC88Fdc::DRIVE_MAX];
	if ((hlistImages = GetDlgItem(hdlg, IDC_IMAGES)) == NULL) {
		return;
	}
	int nDrive;
	for (nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
		ahcomboDiskDrive[nDrive] =
			GetDlgItem(hdlg, IDC_DISK_DRIVE1+nDrive);
		if (ahcomboDiskDrive[nDrive] == NULL) {
			return;
		}
		ahcheckDiskProtect[nDrive] =
			GetDlgItem(hdlg, IDC_DISK_DRIVE1_PROTECT+nDrive);
		if (ahcheckDiskProtect[nDrive] == NULL) {
			return;
		}
	}
	std::string strTmp;
	CDiskImage* apDriveImage[CPC88Fdc::DRIVE_MAX];
	int anDriveIndex[CPC88Fdc::DRIVE_MAX];
	for (nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
		apDriveImage[nDrive] = m_pFdc->GetDiskImage(nDrive);
		anDriveIndex[nDrive] = 0;
	}
	int nFileIndex = 0, nDiskIndex = 1;
	CDiskImageCollection::iterator itImageFile =
		m_pDiskImageCollection->begin();
	if (nUpdateOfs < 0) {
		ClearListBox(
			hlistImages);
		for (nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
			ClearComboBox(
				ahcomboDiskDrive[nDrive]);
			AddComboBoxItem(
				ahcomboDiskDrive[nDrive],
				RSTR("<none>"));
		}
	} else {
		int nDeleteCount;
		for (
			nDeleteCount =
				GetListBoxItemCount(
					hlistImages)-nUpdateOfs;
			nDeleteCount > 0;
			nDeleteCount--)
		{
			DeleteListBoxItem(
				hlistImages, nUpdateOfs);
		}
		for (
			;
			itImageFile != m_pDiskImageCollection->end();
			itImageFile++)
		{
			if (nFileIndex >= nUpdateOfs) {
				break;
			}
			for (
				CDiskImageFile::iterator itImage = (*itImageFile).begin();
				itImage != (*itImageFile).end();
				itImage++)
			{
				for (nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
					if ((apDriveImage[nDrive] != NULL) &&
						(*apDriveImage[nDrive] == *itImage))
					{
						anDriveIndex[nDrive] = nDiskIndex;
					}
				}
				nDiskIndex++;
			}
			nFileIndex++;
		}
		for (nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
			for (
				nDeleteCount =
					GetComboBoxItemCount(
						ahcomboDiskDrive[nDrive])-nDiskIndex;
				nDeleteCount > 0;
				nDeleteCount--)
			{
				DeleteComboBoxItem(
					ahcomboDiskDrive[nDrive], nDiskIndex);
			}
		}
	}
	for (
		;
		itImageFile != m_pDiskImageCollection->end();
		itImageFile++)
	{
		std::string fstrFileName = (*itImageFile).GetFileName(),
			gstrFileName = ConvFStoGUI(fstrFileName);
		const char* pgszFileName = gstrFileName.c_str();

#ifdef X88_PLATFORM_WINDOWS

		char fszName[_MAX_FNAME], fszExt[_MAX_EXT];
		_splitpath(
			pgszFileName,
			NULL, NULL, fszName, fszExt);
		strTmp = FormatStr(
			"%d:%s%s(%d)%s",
			nFileIndex+1, fszName, fszExt, (*itImageFile).size(),
			(*itImageFile).IsReadOnly()? "<R>": "");

#elif defined(X88_PLATFORM_UNIX)

		const char* pgszSep = strrchr(pgszFileName, '/');
		strTmp = FormatStr(
			"%d:%s(%d)%s",
			nFileIndex+1,
			(pgszSep != NULL)? (pgszSep+1): pgszFileName,
			(*itImageFile).size(),
			(*itImageFile).IsReadOnly()? "<R>": "");

#endif // X88_PLATFORM

		AddListBoxItem(
			hlistImages, strTmp);
		for (
			CDiskImageFile::iterator itImage = (*itImageFile).begin();
			itImage != (*itImageFile).end();
			itImage++)
		{
			std::string fstrImageName = (*itImage).GetImageName(),
				gstrImageName = ConvFStoGUI(fstrImageName);
			strTmp = FormatStr(
				"%d:%s",
				nFileIndex+1, gstrImageName.c_str());
			for (nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
				AddComboBoxItem(
					ahcomboDiskDrive[nDrive], strTmp);
				if ((apDriveImage[nDrive] != NULL) &&
					(*apDriveImage[nDrive] == *itImage))
				{
					anDriveIndex[nDrive] = nDiskIndex;
				}
			}
			nDiskIndex++;
		}
		nFileIndex++;
	}
	for (nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
		SetComboBoxSel(
			ahcomboDiskDrive[nDrive], anDriveIndex[nDrive]);
		bool bEnable = false, bCheck = false;
		if (anDriveIndex[nDrive] > 0) {
			bEnable = true;
			if (apDriveImage[nDrive]->IsWriteProtected()) {
				bCheck = true;
			}
		}
		SetCheckButtonChecked(
			ahcheckDiskProtect[nDrive], bCheck);
		EnableControl(
			ahcheckDiskProtect[nDrive], bEnable);
	}
}

// set remove erase button

void CX88DiskImageDlg::SetRemoveEraseButton(CX88WndHandle hdlg) {
	CX88WndHandle hbuttonRemove = GetDlgItem(hdlg, IDC_IMAGE_REMOVE),
		hbuttonErase = GetDlgItem(hdlg, IDC_IMAGE_ERASE);
	if ((hbuttonRemove != NULL) && (hbuttonErase != NULL)) {
		int nCount = m_pDiskImageCollection->size(),
			nSelCount = GetListBoxSel(
				GetDlgItem(hdlg, IDC_IMAGES), NULL);
		EnableControl(
			hbuttonRemove, nSelCount > 0);
		EnableControl(
			hbuttonErase, nCount > 0);
	}
}

// on added disk image

void CX88DiskImageDlg::OnImageAdd(CX88WndHandle hdlg) {
	CX88FileDlg dlgFile(
		hdlg,
		true,
		RSTR("Add Disk Image"),
		RSTR("Disk Image(d88)|*.d88||"),
		"d88",
		true, true);
	if (dlgFile.DoModal() == IDOK) {
		int nUpdateOfs = m_pDiskImageCollection->size(),
			nOldDiskCount = m_pDiskImageCollection->GetDiskImageCount();
		bool bResult = true;
		for (int nIndex = 0; nIndex < dlgFile.GetFileNameCount(); nIndex++) {
			if (!m_pDiskImageCollection->AddDiskImageFile(
					dlgFile.GetFileName(nIndex), dlgFile.IsReadOnly()))
			{
				bResult = false;
				break;
			}
		}
		if (!bResult) {
			ShowMessageBox(
				hdlg,
				MB_ICONEXCLAMATION, MB_OK,
				RSTR("Disk Image"),
				RSTR("An error occurred while reading."));
		} else {
			int nDiskIndex = nOldDiskCount,
				nDiskIndexMax = m_pDiskImageCollection->GetDiskImageCount();
			for (int nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
				if (nDiskIndex >= nDiskIndexMax) {
					break;
				}
				if (m_pFdc->GetDiskImage(nDrive) == NULL) {
					m_pFdc->SetDiskImage(
						nDrive,
						m_pDiskImageCollection->GetDiskImage(nDiskIndex));
					nDiskIndex++;
				}
			}
		}
		SetChildren(hdlg, nUpdateOfs);
		SetRemoveEraseButton(hdlg);
	}
}

// on removed disk image

void CX88DiskImageDlg::OnImageRemove(CX88WndHandle hdlg) {
	int nIndexMax = m_pDiskImageCollection->size();
	std::vector<int> vectRemove;
	GetListBoxSel(
		GetDlgItem(hdlg, IDC_IMAGES), &vectRemove);
	if (vectRemove.size() <= 0) {
		return;
	}
	CDiskImageCollection::iterator itImageFile =
		m_pDiskImageCollection->begin();
	int nUpdateOfs = -1, nSelIndex = 0;
	for (int nIndex = 0; nIndex < nIndexMax; nIndex++) {
		if (vectRemove[nSelIndex] == nIndex) {
			nSelIndex++;
			if (nUpdateOfs < 0) {
				nUpdateOfs = nIndex;
			}
			for (int nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
				CDiskImage* pDriveImage =
					m_pFdc->GetDiskImage(nDrive);
				if ((pDriveImage != NULL) &&
					(pDriveImage->GetFileID() ==
						(*itImageFile).GetFileID()))
				{
					m_pFdc->SetDiskImage(nDrive, NULL);
				}
			}
			itImageFile =
				m_pDiskImageCollection->erase(itImageFile);
			if (nSelIndex >= (int)vectRemove.size()) {
				break;
			}
		} else {
			itImageFile++;
		}
	}
	SetChildren(hdlg, nUpdateOfs);
	SetRemoveEraseButton(hdlg);
	SetControlFocus(
		GetDlgItem(hdlg, IDOK));
}

// on erase all disk image

void CX88DiskImageDlg::OnImageErase(CX88WndHandle hdlg) {
	int nCount = m_pDiskImageCollection->size();
	if (nCount <= 0) {
		return;
	}
	for (int nDrive = 0; nDrive < m_pFdc->GetDriveCount(); nDrive++) {
		m_pFdc->SetDiskImage(nDrive, NULL);
	}
	m_pDiskImageCollection->clear();
	SetChildren(hdlg);
	SetRemoveEraseButton(hdlg);
	SetControlFocus(
		GetDlgItem(hdlg, IDOK));
}

// on changed disk selection

void CX88DiskImageDlg::OnDiskDriveChange(CX88WndHandle hdlg, int nID) {
	int nDrive = nID-IDC_DISK_DRIVE1;
	int nDiskIndex = GetComboBoxSel(
		GetDlgItem(hdlg, nID));
	CX88WndHandle hcheckDiskProtect = GetDlgItem(
		hdlg, IDC_DISK_DRIVE1_PROTECT+nDrive);
	if (nDiskIndex <= 0) {
		m_pFdc->SetDiskImage(nDrive, NULL);
		if (nDiskIndex < 0) {
			SetComboBoxSel(
				GetDlgItem(hdlg, nID), 0);
		}
		SetCheckButtonChecked(
			hcheckDiskProtect, false);
		EnableControl(
			hcheckDiskProtect, false);
	} else {
		SetCheckButtonChecked(
			hcheckDiskProtect,
			m_pDiskImageCollection->GetDiskImage(nDiskIndex-1)->
				IsWriteProtected());
		EnableControl(
			hcheckDiskProtect, true);
		for (int nDrive2 = 0; nDrive2 < m_pFdc->GetDriveCount(); nDrive2++) {
			if (nDrive2 == nDrive) {
				continue;
			}
			int nDiskIndex2 = m_pDiskImageCollection->GetDiskImageIndex(
				m_pFdc->GetDiskImage(nDrive2));
			if (nDiskIndex2 >= 0) {
				nDiskIndex2++;
			}
			if (nDiskIndex2 == nDiskIndex) {
				m_pFdc->SetDiskImage(nDrive2, NULL);
				SetComboBoxSel(
					GetDlgItem(hdlg, IDC_DISK_DRIVE1+nDrive2), 0);
				hcheckDiskProtect = GetDlgItem(
					hdlg, IDC_DISK_DRIVE1_PROTECT+nDrive2);
				SetCheckButtonChecked(
					hcheckDiskProtect, false);
				EnableControl(
					hcheckDiskProtect, false);
			}
		}
		m_pFdc->SetDiskImage(
			nDrive,
			m_pDiskImageCollection->GetDiskImage(nDiskIndex-1));
	}
}

// on changed disk write-protect

void CX88DiskImageDlg::OnDiskDriveProtect(CX88WndHandle hdlg, int nID) {
	int nDrive = nID-IDC_DISK_DRIVE1_PROTECT;
	bool bProtect = IsCheckButtonChecked(
		GetDlgItem(hdlg, nID));
	CDiskImage* pDriveImage = m_pFdc->GetDiskImage(nDrive);
	if (pDriveImage == NULL) {
		return;
	}
	pDriveImage->SetWriteProtect(bProtect);
}

// create modal dialog

int CX88DiskImageDlg::DoModal() {
	return ExecuteDialog(
		IDD_DISK_IMAGE,
		GetParentWndHandle(),
		DlgProc);
}
