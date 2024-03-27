
// GreyScaleImageProcessingBeta1Doc.h: CGreyScaleImageProcessingBeta1Doc 클래스의 인터페이스
//


#pragma once


class CGreyScaleImageProcessingBeta1Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CGreyScaleImageProcessingBeta1Doc() noexcept;
	DECLARE_DYNCREATE(CGreyScaleImageProcessingBeta1Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CGreyScaleImageProcessingBeta1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	int m_inH;		int m_inW;
	int m_outH;		int m_outW;
	// 입력 이미지
	unsigned char** m_inImage = NULL;
	// 출력 이미지
	unsigned char** m_outImage = NULL;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	// 2차원 배열을 만드는 함수
	unsigned char** OnMalloc2D(int h, int w, int init);
	void OnFree2D(unsigned char** image, int h);
	virtual void OnCloseDocument();
	void OnEqualImage();
	// 밝기 변경 함수
	void OnLightImage();
	void OnAddImage();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void OnReverseImage();
	void OnGammaImage();
	void OnCapImage();
	void OnCupImage();
	void OnMono127();
	void OnMonoMid();
	void OnMonoAvg();
	void OnMaskAnd();
	void OnMaskOr();
	void OnMaskXor();
	void OnNotImage();
	void OnStractingImage();
	void OnZipImage();
	void OnPosterImage();
	void OnAreaImpact();
	void OnZoomIn();
	void OnZoomOut();
	void OnRotateOrigin();
	void OnRotateNoCut();
	void OnMoveVertical();
	void OnMoveHorizonal();
	void OnMirrorVertical();
	void OnMirrorHorizonal();
	void OnMorph();
	void OnHistoStrectch();
	void OnEndIn();
	void OnHistoFlatten();
	void OnEmboss();
	void freeDoubleMemory(double** memory, int h);
	double** mallocDoubleMemory(int h, int w);
	void OnBlur();
};
