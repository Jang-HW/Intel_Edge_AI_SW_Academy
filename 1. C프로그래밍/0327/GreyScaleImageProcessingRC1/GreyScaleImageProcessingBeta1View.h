
// GreyScaleImageProcessingBeta1View.h: CGreyScaleImageProcessingBeta1View 클래스의 인터페이스
//

#pragma once


class CGreyScaleImageProcessingBeta1View : public CView
{
protected: // serialization에서만 만들어집니다.
	CGreyScaleImageProcessingBeta1View() noexcept;
	DECLARE_DYNCREATE(CGreyScaleImageProcessingBeta1View)

// 특성입니다.
public:
	CGreyScaleImageProcessingBeta1Doc* GetDocument() const;

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
	virtual ~CGreyScaleImageProcessingBeta1View();
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
	afx_msg void OnLightImage();
	afx_msg void OnAddImage();
	afx_msg void OnReverseImage();
	afx_msg void OnGammaImage();
	afx_msg void OnCapImage();
	afx_msg void OnCupImage();
	afx_msg void OnMono127();
	afx_msg void OnMonoMid();
	afx_msg void OnMonoAvg();
	afx_msg void OnMaskAnd();
	afx_msg void OnMaskOr();
	afx_msg void OnMaskXor();
	afx_msg void OnNotImage();
	afx_msg void OnStractingImage();
	afx_msg void OnZipImage();
	afx_msg void OnPosterImage();
	afx_msg void OnAreaImpact();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnRotateOrigin();
	afx_msg void OnRotateNoCut();
	afx_msg void OnMoveVertical();
	afx_msg void OnMoveHorizonal();
	afx_msg void OnMirrorVertical();
	afx_msg void OnMirrorHorizonal();
	afx_msg void OnMorph();
	afx_msg void OnHistoStrectch();
	afx_msg void OnEndIn();
	afx_msg void OnHistoFlatten();
	afx_msg void OnEmboss();
	afx_msg void OnBlur();
};

#ifndef _DEBUG  // GreyScaleImageProcessingBeta1View.cpp의 디버그 버전
inline CGreyScaleImageProcessingBeta1Doc* CGreyScaleImageProcessingBeta1View::GetDocument() const
   { return reinterpret_cast<CGreyScaleImageProcessingBeta1Doc*>(m_pDocument); }
#endif

