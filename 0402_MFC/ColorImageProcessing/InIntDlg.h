#pragma once
#include "afxdialogex.h"


// CInIntDlg 대화 상자

class CInIntDlg : public CDialog
{
	DECLARE_DYNAMIC(CInIntDlg)

public:
	CInIntDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CInIntDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IN_INT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	double m_constant;
	CString m_question;
};
