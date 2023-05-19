////////////////////////////////////////////////////////////
// X88000 Debug BreakPoint Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88DebugBreakPointDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88DebugBreakPointDlg

////////////////////////////////////////////////////////////
// attribute

// program-counter

uint16_t CX88DebugBreakPointDlg::m_wPC;

// break points

std::set<uint16_t>* CX88DebugBreakPointDlg::m_psetBreakPoint;

// break point list

std::list<uint16_t> CX88DebugBreakPointDlg::m_listBreakPoint;

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88DebugBreakPointDlg::CX88DebugBreakPointDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	m_listBreakPoint.clear();
}

// destructor

CX88DebugBreakPointDlg::~CX88DebugBreakPointDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88DebugBreakPointDlg::DlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		{ // dummy block
			Centering(hdlg);
			InitializeBreakPoint(hdlg);
			SetRemoveEraseButton(hdlg);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			FinalizeBreakPoint(hdlg);
			EndDialog(hdlg, LOWORD(wParam));
			break;
		case IDC_ADDRESS_ADD:
			OnAddressAdd(hdlg);
			break;
		case IDC_BREAK_POINT:
			SetRemoveEraseButton(hdlg);
			break;
		case IDC_BREAK_POINT_REMOVE:
			OnBreakPointRemove(hdlg);
			break;
		case IDC_BREAK_POINT_ERASE:
			OnBreakPointErase(hdlg);
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

bool CX88DebugBreakPointDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		InitializeBreakPoint(pDialog);
		SetRemoveEraseButton(pDialog);
		g_signal_connect(
			gtk_tree_view_get_selection(
				GTK_TREE_VIEW(
					GetDlgItem(pDialog, IDC_BREAK_POINT))),
			"changed",
			G_CALLBACK(OnSelectionChangedSignalBreakPoint),
			pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_ADDRESS_ADD),
			"clicked",
			G_CALLBACK(OnClickedSignalBreakPointOperate),
			pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_BREAK_POINT_REMOVE),
			"clicked",
			G_CALLBACK(OnClickedSignalBreakPointOperate),
			pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_BREAK_POINT_ERASE),
			"clicked",
			G_CALLBACK(OnClickedSignalBreakPointOperate),
			pDialog);
	} else {
		FinalizeBreakPoint(pDialog);
	}
	return true;
}

// break point operation clicked signal

void CX88DebugBreakPointDlg::OnClickedSignalBreakPointOperate(
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
	case IDC_ADDRESS_ADD:
		OnAddressAdd(pDialog);
		break;
	case IDC_BREAK_POINT_REMOVE:
		OnBreakPointRemove(pDialog);
		break;
	case IDC_BREAK_POINT_ERASE:
		OnBreakPointErase(pDialog);
		break;
	}
}

// break point selection changed signal

void CX88DebugBreakPointDlg::OnSelectionChangedSignalBreakPoint(
	GtkTreeSelection* /*pTreeSelection*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	CSignalBlocker blocker(pDialog);
	if (blocker.IsBlocked()) {
		return;
	}
	SetRemoveEraseButton(pDialog);
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// initialize break point

void CX88DebugBreakPointDlg::InitializeBreakPoint(CX88WndHandle hdlg) {
	m_listBreakPoint.clear();
	for (
		std::set<uint16_t>::iterator itBreakPointSet = m_psetBreakPoint->begin();
		itBreakPointSet != m_psetBreakPoint->end();
		itBreakPointSet++)
	{
		m_listBreakPoint.push_back(*itBreakPointSet);
	}
	m_listBreakPoint.sort();
	for (
		std::list<uint16_t>::iterator itBreakPoint = m_listBreakPoint.begin();
		itBreakPoint != m_listBreakPoint.end();
		itBreakPoint++)
	{
		AddListBoxItem(
			GetDlgItem(hdlg, IDC_BREAK_POINT),
			FormatStr("%04X", *itBreakPoint));
	}
	SetEditBoxText(
		GetDlgItem(hdlg, IDC_ADDRESS),
		FormatStr("%04X", m_wPC));
}

// finalize break point

void CX88DebugBreakPointDlg::FinalizeBreakPoint(CX88WndHandle /*hdlg*/) {
	m_psetBreakPoint->clear();
	for (
		std::list<uint16_t>::iterator itBreakPoint = m_listBreakPoint.begin();
		itBreakPoint != m_listBreakPoint.end();
		itBreakPoint++)
	{
		m_psetBreakPoint->insert(*itBreakPoint);
	}
}

// set remove erase button

void CX88DebugBreakPointDlg::SetRemoveEraseButton(CX88WndHandle hdlg) {
	CX88WndHandle hbuttonRemove = GetDlgItem(hdlg, IDC_BREAK_POINT_REMOVE),
		hbuttonErase = GetDlgItem(hdlg, IDC_BREAK_POINT_ERASE);
	if ((hbuttonRemove != NULL) && (hbuttonErase != NULL)) {
		int nCount = (int)m_listBreakPoint.size(),
			nSelCount = GetListBoxSel(
				GetDlgItem(hdlg, IDC_BREAK_POINT), NULL);
		EnableControl(
			hbuttonRemove, nSelCount > 0);
		EnableControl(
			hbuttonErase, nCount > 0);
	}
}

// on add address

void CX88DebugBreakPointDlg::OnAddressAdd(CX88WndHandle hdlg) {
	std::string gstrBreakPoint;
	GetEditBoxText(
		GetDlgItem(hdlg, IDC_ADDRESS),
		gstrBreakPoint);
	char* pgszEnd;
	long lAddress = strtol(gstrBreakPoint.c_str(), &pgszEnd, 16);
	if ((lAddress < 0x0000) ||
		(lAddress > 0xFFFF) ||
		((*pgszEnd != 'H') && (*pgszEnd != '\0')))
	{
		ShowMessageBox(
			hdlg,
			MB_ICONEXCLAMATION, MB_OK,
			RSTR("Breakpoint : Address"),
			RSTRF(
				"Please enter a hexadecimal number between %04XH-%04XH",
				0x0000, 0xFFFF));
		return;
	}
	uint16_t wAddress =(uint16_t)lAddress;
	int nIndex = 0;
	std::list<uint16_t>::iterator itBreakPoint;
	for (
		itBreakPoint = m_listBreakPoint.begin();
		itBreakPoint != m_listBreakPoint.end();
		itBreakPoint++, nIndex++)
	{
		if (*itBreakPoint == wAddress) {
			ShowMessageBox(
				hdlg,
				MB_ICONEXCLAMATION, MB_OK,
				RSTR("Breakpoint : Address"),
				RSTR("This address is already registered."));
			return;
		} else if (*itBreakPoint > wAddress) {
			break;
		}
	}
	m_listBreakPoint.insert(itBreakPoint, wAddress);
	InsertListBoxItem(
		GetDlgItem(hdlg, IDC_BREAK_POINT),
		nIndex,
		FormatStr("%04X", wAddress));
	SetRemoveEraseButton(hdlg);
}

// on remove break point

void CX88DebugBreakPointDlg::OnBreakPointRemove(CX88WndHandle hdlg) {
	std::vector<int> vectRemove;
	GetListBoxSel(
		GetDlgItem(hdlg, IDC_BREAK_POINT), &vectRemove);
	if (vectRemove.size() <= 0) {
		return;
	}
	for (
		int nSelIndex = (int)vectRemove.size()-1;
		nSelIndex >= 0;
		nSelIndex--)
	{
		int nIndex = vectRemove[nSelIndex];
		std::list<uint16_t>::iterator itBreakPoint = m_listBreakPoint.begin();
		std::advance(itBreakPoint, nIndex);
		m_listBreakPoint.erase(itBreakPoint);
		DeleteListBoxItem(
			GetDlgItem(hdlg, IDC_BREAK_POINT),
			nIndex);
	}
	SetRemoveEraseButton(hdlg);
	SetControlFocus(
		GetDlgItem(hdlg, IDOK));
}

// on erase break point

void CX88DebugBreakPointDlg::OnBreakPointErase(CX88WndHandle hdlg) {
	m_listBreakPoint.clear();
	ClearListBox(
		GetDlgItem(hdlg, IDC_BREAK_POINT));
	SetRemoveEraseButton(hdlg);
	SetControlFocus(
		GetDlgItem(hdlg, IDOK));
}

// create modal dialog

int CX88DebugBreakPointDlg::DoModal() {
	return ExecuteDialog(
		IDD_DEBUG_BREAK_POINT,
		GetParentWndHandle(),
		DlgProc);
}
