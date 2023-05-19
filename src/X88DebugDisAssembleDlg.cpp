////////////////////////////////////////////////////////////
// X88000 Debug DisAssemble Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88DebugDisAssembleDlg.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88DebugDisAssembleDlg

////////////////////////////////////////////////////////////
// attribute

// Z80 adapter

CZ80Adapter* CX88DebugDisAssembleDlg::m_pZ80A;

#ifdef X88_GUI_WINDOWS

// fixed pitch font

HFONT CX88DebugDisAssembleDlg::m_hfontFix;

#endif // X88_GUI

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88DebugDisAssembleDlg::CX88DebugDisAssembleDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
	m_pZ80A = NULL;

#ifdef X88_GUI_WINDOWS

	m_hfontFix = NULL;

#endif // X88_GUI

}

// destructor

CX88DebugDisAssembleDlg::~CX88DebugDisAssembleDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88DebugDisAssembleDlg::DlgProc(
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
							hdlg, IDC_DIS_ASSEMBLE_LIST,
							WM_SETFONT,
							(WPARAM)m_hfontFix, MAKELPARAM(TRUE, 0));
					}
				}
			}
			SetChildren(hdlg);
			DoDisAssemble(hdlg);
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
		case IDC_DIS_ASSEMBLE:
			DoDisAssemble(hdlg);
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

bool CX88DebugDisAssembleDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		CX88WndHandle heditList = GetDlgItem(
			pDialog, IDC_DIS_ASSEMBLE_LIST);
		if (heditList != NULL) {
			SetControlFont(heditList, "Monospace");
		}
		SetChildren(pDialog);
		DoDisAssemble(pDialog);
		g_signal_connect(
			GetDlgItem(pDialog, IDC_DIS_ASSEMBLE),
			"clicked",
			G_CALLBACK(OnClickedSignalDisAssemble),
			pDialog);
	} else {
	}
	return true;
}

// dis-assemble clicked signal

void CX88DebugDisAssembleDlg::OnClickedSignalDisAssemble(
	GtkButton* /*pButton*/, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	DoDisAssemble(pDialog);
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// set children

void CX88DebugDisAssembleDlg::SetChildren(CX88WndHandle hdlg) {
	SetEditBoxText(
		GetDlgItem(hdlg, IDC_ADDRESS),
		FormatStr("%04X", m_pZ80A->RegPC().Get()));
	SetEditBoxText(
		GetDlgItem(hdlg, IDC_LINE_COUNT),
		"16");
}

// execute dis-assemble

void CX88DebugDisAssembleDlg::DoDisAssemble(CX88WndHandle hdlg) {
	CX88WndHandle heditList = GetDlgItem(
		hdlg, IDC_DIS_ASSEMBLE_LIST);
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
			RSTR("Disassemble : Address"),
			RSTRF(
				"Please enter a hexadecimal number between %04XH-%04XH",
				0x0000, 0xFFFF));
		return;
	}
	GetEditBoxText(
		GetDlgItem(hdlg, IDC_LINE_COUNT),
		gstrTmp);
	long nLineCount = strtol(gstrTmp.c_str(), &pgszEnd, 10);
	if ((nLineCount < 1) || (nLineCount > 999) || (*pgszEnd != '\0')) {
		ShowMessageBox(
			hdlg,
			MB_ICONEXCLAMATION, MB_OK,
			RSTR("Disassemble : Line Count"),
			RSTRF(
				"Please enter a number between %d-%d",
				1, 999));
		return;
	}
	uint16_t wPCOrg = m_pZ80A->RegPC().Get();
	m_pZ80A->RegPC().Set(uint16_t(nAddress));

#ifdef X88_GUI_WINDOWS

	std::string strDisAssemble;
	for (int nLine = 0; nLine < nLineCount; nLine++) {
		uint16_t wPC = m_pZ80A->RegPC().Get();
		m_pZ80A->DisAssemble();
		strDisAssemble += FormatStr(
			"0%04XH\t%s\r\n", wPC, m_pZ80A->GetMnemonic());
	}
	SetWindowText(heditList, strDisAssemble.c_str());
	SendMessage(heditList, EM_SETSEL, 0, 0);
	SendMessage(heditList, EM_SCROLLCARET, 0, 0);

#elif defined(X88_GUI_GTK)

	std::string strDisAssemble;
	for (int nLine = 0; nLine < nLineCount; nLine++) {
		uint16_t wPC = m_pZ80A->RegPC().Get();
		m_pZ80A->DisAssemble();
		strDisAssemble += FormatStr(
			"0%04XH\t%s\n", wPC, m_pZ80A->GetMnemonic());
	}
	GtkTextBuffer* pTextBuffer =
		gtk_text_view_get_buffer(
			GTK_TEXT_VIEW(heditList));
	if (pTextBuffer != NULL) {
		gtk_text_buffer_set_text(
			pTextBuffer,
			strDisAssemble.c_str(),
			strDisAssemble.length());
	}

#endif // X88_GUI

	m_pZ80A->RegPC().Set(uint16_t(wPCOrg));
	m_pZ80A->DisAssemble();
	m_pZ80A->RegPC().Set(uint16_t(wPCOrg));
}

// create modal dialog

int CX88DebugDisAssembleDlg::DoModal() {
	return ExecuteDialog(
		IDD_DEBUG_DIS_ASSEMBLE,
		GetParentWndHandle(),
		DlgProc);
}
