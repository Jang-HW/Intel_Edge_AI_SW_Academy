// StringDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageProcessing.h"
#include "afxdialogex.h"
#include "StringDlg.h"


// CStringDlg 대화 상자

IMPLEMENT_DYNAMIC(CStringDlg, CDialog)

CStringDlg::CStringDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_IN_STRING_DLG, pParent)
	, m_question(_T(""))
	, m_string(_T(""))
{

}

CStringDlg::~CStringDlg()
{
}

void CStringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_QUESTION_STATIC, m_question);
	DDX_Text(pDX, IDC_STRING_CONSTANCE, m_string);
}


BEGIN_MESSAGE_MAP(CStringDlg, CDialog)
END_MESSAGE_MAP()


// CStringDlg 메시지 처리기
