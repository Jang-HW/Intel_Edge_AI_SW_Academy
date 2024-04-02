
// ColorImageProcessingView.h: CColorImageProcessingView 클래스의 인터페이스
//

#pragma once


class CColorImageProcessingView : public CView
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessingView() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessingView)

// 특성입니다.
public:
	CColorImageProcessingDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CColorImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEqualImage();
	afx_msg void OnGreyScale();
	afx_msg void OnLightChange();
	afx_msg void OnReverse();
	afx_msg void OnGammaChange();
	afx_msg void OnParaCap();
	afx_msg void OnParaCup();
	afx_msg void OnMono127();
	afx_msg void OnMonoMid();
	afx_msg void OnMonoAvg();
	afx_msg void OnMaskAnd();
	afx_msg void OnChangeSatur();
	afx_msg void OnPickOrange();
	afx_msg void OnMaskOr();
	afx_msg void OnMaskXor();
	afx_msg void OnMaskNot();
	afx_msg void OnConstractStretch();
	afx_msg void OnConstractZip();
	afx_msg void OnPosterize();
	afx_msg void OnAreaStress();
	afx_msg void OnColorImpact();
	afx_msg void OnIntensityChange();
};

#ifndef _DEBUG  // ColorImageProcessingView.cpp의 디버그 버전
inline CColorImageProcessingDoc* CColorImageProcessingView::GetDocument() const
   { return reinterpret_cast<CColorImageProcessingDoc*>(m_pDocument); }
#endif

