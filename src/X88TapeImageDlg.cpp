////////////////////////////////////////////////////////////
// X88000 TapeImage Dialog
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88TapeImageDlg.h"

#include "X88FileDlg.h"
#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88TapeImageDlg

////////////////////////////////////////////////////////////
// attribute

// tape image for loading

CTapeImage* CX88TapeImageDlg::m_ptimageLoad;

// tape image for saving

CTapeImage* CX88TapeImageDlg::m_ptimageSave;

////////////////////////////////////////////////////////////
// create & destroy

// standard constructor

CX88TapeImageDlg::CX88TapeImageDlg(CX88WndHandle hwndParent) :
	CX88Dialog(hwndParent)
{
}

// destructor

CX88TapeImageDlg::~CX88TapeImageDlg() {
}

////////////////////////////////////////////////////////////
// implementation

#ifdef X88_GUI_WINDOWS

// dialog procedure

BOOL CALLBACK CX88TapeImageDlg::DlgProc(
	HWND hdlg, UINT nMessage, WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bResult = TRUE;
	switch (nMessage) {
	case WM_INITDIALOG:
		Centering(hdlg);
		SetChildren(hdlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hdlg, LOWORD(wParam));
			break;
		case IDC_LIMAGE_OPEN:
			OnLoadImageOpen(hdlg);
			break;
		case IDC_LIMAGE_FWD:
			OnLoadImageFwd(hdlg);
			break;
		case IDC_LIMAGE_REW:
			OnLoadImageRew(hdlg);
			break;
		case IDC_SIMAGE_SAVE:
			OnSaveImageSave(hdlg);
			break;
		case IDC_SIMAGE_ERASE:
			OnSaveImageErase(hdlg);
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

bool CX88TapeImageDlg::DlgProc(
	GtkWidget* pDialog,
	bool bInitialize,
	int nID)
{
	if (bInitialize) {
		if (!BuildDialog(pDialog, nID)) {
			return false;
		}
		SetChildren(pDialog);
		for (int i = 0; i < 5; i++) {
			int nID = 0;
			switch (i) {
			case 0:
				nID = IDC_LIMAGE_OPEN;
				break;
			case 1:
				nID = IDC_LIMAGE_FWD;
				break;
			case 2:
				nID = IDC_LIMAGE_REW;
				break;
			case 3:
				nID = IDC_SIMAGE_SAVE;
				break;
			case 4:
				nID = IDC_SIMAGE_ERASE;
				break;
			}
			g_signal_connect(
				GetDlgItem(pDialog, nID), "clicked",
				G_CALLBACK(OnClickedSignalImageOperate),
				pDialog);
		}
	}
	return true;
}

// image operation clicked signal

void CX88TapeImageDlg::OnClickedSignalImageOperate(
	GtkButton* pButton, gpointer pData)
{
	GtkWidget* pDialog = GTK_WIDGET(pData);
	int nID = GetDlgItemID(
		pDialog, GTK_WIDGET(pButton));
	switch (nID) {
	case IDC_LIMAGE_OPEN:
		OnLoadImageOpen(pDialog);
		break;
	case IDC_LIMAGE_FWD:
		OnLoadImageFwd(pDialog);
		break;
	case IDC_LIMAGE_REW:
		OnLoadImageRew(pDialog);
		break;
	case IDC_SIMAGE_SAVE:
		OnSaveImageSave(pDialog);
		break;
	case IDC_SIMAGE_ERASE:
		OnSaveImageErase(pDialog);
		break;
	}
}

#endif // X88_GUI

////////////////////////////////////////////////////////////
// operation

// is file ext equals CMT

bool CX88TapeImageDlg::IsFileExtCMT(
	const std::string& fstrFileName)
{
	std::string strExt = ConvFStoUTF8(GetFileExt(fstrFileName));
	return StrCaseCmp(strExt, ".cmt") == 0;
}

// set children

void CX88TapeImageDlg::SetChildren(CX88WndHandle hdlg) {
	std::string fstrFileName = m_ptimageLoad->GetFileName(),
		gstrFileName = ConvFStoGUI(fstrFileName),
		gstrDispName = !gstrFileName.empty()?
			gstrFileName:
			std::string(RSTR("<none>"));
	SetStaticText(
		GetDlgItem(hdlg, IDC_LIMAGE_NAME),
		gstrDispName);
	std::string gstrTmp = FormatStr(
		RSTR("%dByte/%dBlock"),
		m_ptimageLoad->GetDataSize(),
		m_ptimageLoad->GetRealDataBlockCount());
	SetStaticText(
		GetDlgItem(hdlg, IDC_LIMAGE_INFO),
		gstrTmp);
	int nTick = m_ptimageLoad->GetCounterTick();
	gstrTmp = FormatStr(
		"%02d:%05.2lf",
		nTick/(60*4800), (nTick%(60*4800))/4800.0);
	SetStaticText(
		GetDlgItem(hdlg, IDC_LIMAGE_COUNTER),
		gstrTmp);
	bool bEnable = (m_ptimageLoad->GetDataSize() > 0);
	EnableControl(
		GetDlgItem(hdlg, IDC_LIMAGE_FWD),
		bEnable);
	EnableControl(
		GetDlgItem(hdlg, IDC_LIMAGE_REW),
		bEnable);
	gstrTmp = FormatStr(
		RSTR("%dByte/%dBlock"),
		m_ptimageSave->GetDataSize(),
		m_ptimageSave->GetRealDataBlockCount());
	SetStaticText(
		GetDlgItem(hdlg, IDC_SIMAGE_INFO),
		gstrTmp);
	nTick = m_ptimageSave->GetTotalTick();
	gstrTmp = FormatStr(
		"%02d:%05.2lf",
		nTick/(60*4800), (nTick%(60*4800))/4800.0);
	SetStaticText(
		GetDlgItem(hdlg, IDC_SIMAGE_COUNTER),
		gstrTmp);
	bEnable = (m_ptimageSave->GetDataSize() > 0);
	EnableControl(
		GetDlgItem(hdlg, IDC_SIMAGE_SAVE),
		bEnable);
	EnableControl(
		GetDlgItem(hdlg, IDC_SIMAGE_ERASE),
		bEnable);
}

// on open image for loading

void CX88TapeImageDlg::OnLoadImageOpen(CX88WndHandle hdlg) {
	CX88FileDlg dlgFile(
		hdlg,
		true,
		RSTR("Open Tape Image"),
		RSTR("Tape Image(t88, cmt)|*.t88;*.cmt||"),
		"t88");
	if (dlgFile.DoModal() == IDOK) {
		int nResult;
		if (IsFileExtCMT(dlgFile.GetFileName())) {
			nResult = m_ptimageLoad->LoadCMT(
				dlgFile.GetFileName());
		} else {
			nResult = m_ptimageLoad->Load(
				dlgFile.GetFileName());
		}
		if (nResult != CTapeImage::ERR_NOERROR) {
			ShowMessageBox(
				hdlg,
				MB_ICONEXCLAMATION, MB_OK,
				RSTR("Tape Image"),
				RSTR("An error occurred while reading."));
		}
		SetChildren(hdlg);
	}
}

// on forward image for loading

void CX88TapeImageDlg::OnLoadImageFwd(CX88WndHandle hdlg) {
	m_ptimageLoad->CounterFWD();
	SetChildren(hdlg);
}

// on rewind image for loading

void CX88TapeImageDlg::OnLoadImageRew(CX88WndHandle hdlg) {
	m_ptimageLoad->CounterREW();
	SetChildren(hdlg);
}

// on save image for saving

void CX88TapeImageDlg::OnSaveImageSave(CX88WndHandle hdlg) {
	CX88FileDlg dlgFile(
		hdlg,
		false,
		RSTR("Save Tape Image"),
		RSTR("Tape Image(t88, cmt)|*.t88;*.cmt||"),
		"t88");
	if (dlgFile.DoModal() == IDOK) {
		int nResult;
		if (IsFileExtCMT(dlgFile.GetFileName())) {
			nResult = m_ptimageSave->SaveCMT(
				dlgFile.GetFileName());
		} else {
			nResult = m_ptimageSave->Save(
				dlgFile.GetFileName());
		}
		if (nResult != CTapeImage::ERR_NOERROR) {
			ShowMessageBox(
				hdlg,
				MB_ICONEXCLAMATION, MB_OK,
				RSTR("Tape Image"),
				RSTR("An error occurred while writing."));
		}
	}
}

// on erase image for saving

void CX88TapeImageDlg::OnSaveImageErase(CX88WndHandle hdlg) {
	if (ShowMessageBox(
			hdlg,
			MB_ICONQUESTION, MB_YESNO,
			RSTR("Saved Data"),
			RSTR("Erase saved data?")) == IDYES)
	{
		m_ptimageSave->Erase();
		SetChildren(hdlg);
		SetControlFocus(
			GetDlgItem(hdlg, IDOK));
	}
}

// create modal dialog

int CX88TapeImageDlg::DoModal() {
	m_ptimageSave->MakeBlockInfo();
	return ExecuteDialog(
		IDD_TAPE_IMAGE,
		GetParentWndHandle(),
		DlgProc);
}
