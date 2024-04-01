// ColorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageProcessing.h"
#include "afxdialogex.h"
#include "ColorDlg.h"


// CColorDlg 대화 상자

IMPLEMENT_DYNAMIC(CColorDlg, CDialog)

CColorDlg::CColorDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_COLOR_SELECT, pParent)
	, m_red(FALSE)
	, m_orange(FALSE)
	, m_yellow(FALSE)
	, m_green(FALSE)
	, m_blue(FALSE)
	, m_purple(FALSE)
{

}

CColorDlg::~CColorDlg()
{
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_RED_CHECK, m_red);
	DDX_Check(pDX, IDC_RED_CHECK, m_red);
	DDX_Check(pDX, IDC_ORANGE_CHECK, m_orange);
	DDX_Check(pDX, IDC_YELLOW_CHECK, m_yellow);
	DDX_Check(pDX, IDC_GREEN_CHECK, m_green);
	DDX_Check(pDX, IDC_BLUE_CHECK, m_blue);
	DDX_Check(pDX, IDC_PURPLE_CHECK, m_purple);
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
END_MESSAGE_MAP()


// CColorDlg 메시지 처리기
