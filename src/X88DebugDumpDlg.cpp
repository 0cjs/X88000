////////////////////////////////////////////////////////////
// X88000 Debug Dump Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88DebugDumpDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88DebugDumpDlg

////////////////////////////////////////////////////////////
// attribute

// Z80 adapter

CZ80Adapter* CX88DebugDumpDlg::m_pZ80A;

#ifdef X88_GUI_WINDOWS

// fixed pitch font

HFONT CX88DebugDumpDlg::m_hfontFix;

#endif // X88_GUI

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88DebugDumpDlg::CX88DebugDumpDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	m_pZ80A = NULL;

#ifdef X88_GUI_WINDOWS

	m_hfontFix = NULL;

#endif // X88_GUI

}

// destructor

CX88DebugDumpDlg::~CX88DebugDumpDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88DebugDumpDlg::DlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		{ // dummy block
			Centering(hdlg);
			HFONT hfontOrg = (HFONT)SendMessage(hdlg, WM_GETFONT, 0, 0);
			if (hfontOrg != NULL) {
				LOGFONT lf;
				if (GetObject(hfontOrg, sizeof(LOGFONT), &lf) != 0) {
					lf.lfPitchAndFamily = uint8_t(
						(lf.lfPitchAndFamily & 0xFC) | FIXED_PITCH);
					strcpy(lf.lfFaceName, RSTR("Courier New"));
					m_hfontFix = CreateFontIndirect(&lf);
					if (m_hfontFix != NULL) {
						SendDlgItemMessage(
							hdlg, IDC_DUMP_LIST,
							WM_SETFONT,
							(WPARAM)m_hfontFix, MAKELPARAM(TRUE, 0));
					}
				}
			}
			SetChildren(hdlg);
			DoDump(hdlg);
		}
		break;
	case WM_DESTROY:
		if (m_hfontFix != NULL) {
			DeleteObject(m_hfontFix);
			m_hfontFix = NULL;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, LOWORD(wParam));
			break;
		case IDC_DUMP:
			DoDump(hdlg);
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

bool CX88DebugDumpDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		CX88WndHandle heditList = GetDlgItem(pDialog, IDC_DUMP_LIST);
		if (heditList != NULL) {
			SetControlFont(heditList, "Monospace");
		}
		SetChildren(pDialog);
		DoDump(pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_DUMP),
			"clicked",
			G_CALLBACK(OnClickedSignalDump),
			pDialog);
	} else {
	}
	return true;
}

// dump clicked signal

void CX88DebugDumpDlg::OnClickedSignalDump(
	GtkButton* /*pButton*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	DoDump(pDialog);
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// set children

void CX88DebugDumpDlg::SetChildren(CX88WndHandle hdlg) {
	SetEditBoxText(
		GetDlgItem(hdlg, IDC_ADDRESS),
		FormatStr("%04X", m_pZ80A->RegHL().Get()));
	SetEditBoxText(
		GetDlgItem(hdlg, IDC_BYTE_COUNT),
		"256");
}

// execute dump

void CX88DebugDumpDlg::DoDump(CX88WndHandle hdlg) {
	CX88WndHandle heditList = GetDlgItem(hdlg, IDC_DUMP_LIST);
	if (heditList == NULL) {
		return;
	}
	std::string gstrTmp;
	GetEditBoxText(
		GetDlgItem(hdlg, IDC_ADDRESS),
		gstrTmp);
	char* pgszEnd;
	long nAddress = strtol(gstrTmp.c_str(), &pgszEnd, 16);
	if ((nAddress < 0x0000) || (nAddress > 0xFFFF) ||
		((*pgszEnd != 'H') && (*pgszEnd != '\0')))
	{
		ShowMessageBox(
			hdlg,
			MB_ICONEXCLAMATION, MB_OK,
			RSTR("Dump : Address"),
			RSTRF(
				"Please enter a hexadecimal number between %04XH-%04XH",
				0x0000, 0xFFFF));
		return;
	}
	GetEditBoxText(
		GetDlgItem(hdlg, IDC_BYTE_COUNT),
		gstrTmp);
	long nByteCount = strtol(gstrTmp.c_str(), &pgszEnd, 10);
	if ((nByteCount < 1) || (nByteCount > 4096) || (*pgszEnd != '\0')) {
		ShowMessageBox(
			hdlg,
			MB_ICONEXCLAMATION, MB_OK,
			RSTR("Dump : Byte Count"),
			RSTRF(
				"Please enter a number between %d-%d",
				1, 4096));
		return;
	}

#ifdef X88_GUI_WINDOWS

	enum {
		DUMP_WIDTH = 8
	};
	uint16_t wAddress2 = uint16_t((nAddress/DUMP_WIDTH)*DUMP_WIDTH);
	int nByte = int(wAddress2)-nAddress;
	std::string strDump;
	do {
		std::string strTmp, strTmp2;
		strTmp = FormatStr("0%04XH : ", wAddress2);
		int nOfs;
		for (nOfs = 0; nOfs < DUMP_WIDTH; nOfs++) {
			if ((nByte >= 0) && (nByte < nByteCount)) {
				uint8_t btData;
				btData = m_pZ80A->ReadMemory(wAddress2);
				strTmp += FormatStr("%02X ", btData);
				strTmp2 +=
					((btData >= 0x20) && (btData <= 0x7E))? char(btData): '.';
			} else {
				strTmp += "   ";
				strTmp2 += ' ';
			}
			wAddress2++;
			nByte++;
		}
		strDump += strTmp;
		strDump += ": ";
		strDump += strTmp2;
		strDump += "\r\n";
	} while (nByte < nByteCount);
	SetWindowText(heditList, strDump.c_str());
	SendMessage(heditList, EM_SETSEL, 0, 0);
	SendMessage(heditList, EM_SCROLLCARET, 0, 0);

#elif defined(X88_GUI_GTK)

	enum {
		DUMP_WIDTH = 8
	};
	uint16_t wAddress2 = uint16_t((nAddress/DUMP_WIDTH)*DUMP_WIDTH);
	int nByte = int(wAddress2)-nAddress;
	std::string strDump;
	do {
		std::string strTmp, strTmp2;
		strTmp = FormatStr("0%04XH : ", wAddress2);
		int nOfs;
		for (nOfs = 0; nOfs < DUMP_WIDTH; nOfs++) {
			if ((nByte >= 0) && (nByte < nByteCount)) {
				uint8_t btData;
				btData = m_pZ80A->ReadMemory(wAddress2);
				strTmp += FormatStr("%02X ", btData);
				strTmp2 +=
					((btData >= 0x20) && (btData <= 0x7E))? char(btData): '.';
			} else {
				strTmp += "   ";
				strTmp2 += ' ';
			}
			wAddress2++;
			nByte++;
		}
		strDump += strTmp;
		strDump += ": ";
		strDump += strTmp2;
		strDump += '\n';
	} while (nByte < nByteCount);
	GtkTextBuffer* pTextBuffer =
		gtk_text_view_get_buffer(
			GTK_TEXT_VIEW(heditList));
	if (pTextBuffer != NULL) {
		gtk_text_buffer_set_text(
			pTextBuffer,
			strDump.c_str(),
			strDump.length());
	}

#endif // X88_GUI

}

// create modal dialog

int CX88DebugDumpDlg::DoModal() {
	return ExecuteDialog(
		IDD_DEBUG_DUMP,
		GetParentWndHandle(),
		DlgProc);
}
