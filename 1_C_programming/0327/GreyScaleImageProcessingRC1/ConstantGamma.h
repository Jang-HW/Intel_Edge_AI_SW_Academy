#pragma once
#include "afxdialogex.h"


// CConstantGamma 대화 상자

class CConstantGamma : public CDialog
{
	DECLARE_DYNAMIC(CConstantGamma)

public:
	CConstantGamma(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConstantGamma();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONSTANT_GAMMA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
