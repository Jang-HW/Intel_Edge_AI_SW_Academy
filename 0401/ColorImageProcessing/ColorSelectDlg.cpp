// ColorSelectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageProcessing.h"
#include "afxdialogex.h"
#include "ColorSelectDlg.h"


// CColorSelectDlg 대화 상자

IMPLEMENT_DYNAMIC(CColorSelectDlg, CDialog)

CColorSelectDlg::CColorSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CColorSelectDlg::~CColorSelectDlg()
{
}

void CColorSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CColorSelectDlg, CDialog)
END_MESSAGE_MAP()


// CColorSelectDlg 메시지 처리기
