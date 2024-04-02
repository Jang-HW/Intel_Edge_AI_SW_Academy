#pragma once
#include "afxdialogex.h"


// CColorDlg 대화 상자

class CColorDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorDlg)

public:
	CColorDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColorDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	bool m_red;
	BOOL m_red;
	BOOL m_orange;
	BOOL m_yellow;
	BOOL m_green;
	BOOL m_blue;
	BOOL m_purple;
};
