
// ColorImageProcessingDoc.h: CColorImageProcessingDoc 클래스의 인터페이스
//


#pragma once


class CColorImageProcessingDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessingDoc() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessingDoc)

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
	virtual ~CColorImageProcessingDoc();
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
	int m_inH = 0;		int m_inW = 0;
	unsigned char** m_inImageR = NULL;
	unsigned char** m_inImageG = NULL;
	unsigned char** m_inImageB = NULL;
	int m_outH = 0;		int m_outW = 0;
	unsigned char** m_outImageR = NULL;
	unsigned char** m_outImageG = NULL;
	unsigned char** m_outImageB = NULL;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	unsigned char** OnMalloc2D(int h, int w);
	void OnFree2D(unsigned char** memory, int h);
	void OnFreeOutImage();
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void OnEqualImage();
	void OnGreyScale();
	void OnLightChange();
	unsigned char OnCheckValue(unsigned char value);
	unsigned char OnLightCheck(int value);
	void OnReverse();
	void OnGammaChange();
	void OnParaCap();
	void OnParaCup();
	void OnMono127();
	void OnMonoMid();
	void OnMonoAvg();
	void OnMaskAnd();
	void OnChangeSatur();
	double* RGB2HSI(int R, int G, int B);
	unsigned char* HSI2RGB(double H, double S, double I);
	void OnPickOrange();
	void OnMaskOr();
	void OnMaskXor();
	void OnMaskNot();
	void OnConstractStretch();
	void OnConstractZip();
	void OnPosterize();
	void OnAreaStress();
	void OnColorImpact();
	void OnIntensityChange();
};
