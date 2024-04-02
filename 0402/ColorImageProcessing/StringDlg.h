#pragma once
#include "afxdialogex.h"


// CStringDlg 대화 상자

class CStringDlg : public CDialog
{
	DECLARE_DYNAMIC(CStringDlg)

public:
	CStringDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStringDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IN_STRING_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_question;
	CString m_string;
};
