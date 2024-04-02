// InIntDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageProcessing.h"
#include "afxdialogex.h"
#include "InIntDlg.h"


// CInIntDlg 대화 상자

IMPLEMENT_DYNAMIC(CInIntDlg, CDialog)

CInIntDlg::CInIntDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_IN_INT_DLG, pParent)
	, m_constant(0)
{

}

CInIntDlg::~CInIntDlg()
{
}

void CInIntDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONSTANT, m_constant);
}


BEGIN_MESSAGE_MAP(CInIntDlg, CDialog)
END_MESSAGE_MAP()


// CInIntDlg 메시지 처리기
