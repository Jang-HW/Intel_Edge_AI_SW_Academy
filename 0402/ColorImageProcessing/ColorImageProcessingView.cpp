
// ColorImageProcessingView.cpp: CColorImageProcessingView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImageProcessing.h"
#endif

#include "ColorImageProcessingDoc.h"
#include "ColorImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColorImageProcessingView

IMPLEMENT_DYNCREATE(CColorImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CColorImageProcessingView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CColorImageProcessingView::OnEqualImage)
	ON_COMMAND(IDM_GREY_SCALE, &CColorImageProcessingView::OnGreyScale)
	ON_COMMAND(IDM_LIGHT_CHANGE, &CColorImageProcessingView::OnLightChange)
	ON_COMMAND(IDM_REVERSE, &CColorImageProcessingView::OnReverse)
	ON_COMMAND(IDM_GAMMA_CHANGE, &CColorImageProcessingView::OnGammaChange)
	ON_COMMAND(IDM_PARA_CAP, &CColorImageProcessingView::OnParaCap)
	ON_COMMAND(IDM_PARA_CUP, &CColorImageProcessingView::OnParaCup)
	ON_COMMAND(IDM_MONO_127, &CColorImageProcessingView::OnMono127)
	ON_COMMAND(IDM_MONO_MID, &CColorImageProcessingView::OnMonoMid)
	ON_COMMAND(IDM_MONO_AVG, &CColorImageProcessingView::OnMonoAvg)
	ON_COMMAND(IDM_MASK_AND, &CColorImageProcessingView::OnMaskAnd)
	ON_COMMAND(IDM_CHANGE_SATUR, &CColorImageProcessingView::OnChangeSatur)
	ON_COMMAND(IDM_PICK_ORANGE, &CColorImageProcessingView::OnPickOrange)
	ON_COMMAND(IDM_MASK_OR, &CColorImageProcessingView::OnMaskOr)
	ON_COMMAND(IDM_MASK_XOR, &CColorImageProcessingView::OnMaskXor)
	ON_COMMAND(IDM_MASK_NOT, &CColorImageProcessingView::OnMaskNot)
	ON_COMMAND(IDM_CONSTRACT_STRETCH, &CColorImageProcessingView::OnConstractStretch)
	ON_COMMAND(IDM_CONSTRACT_ZIP, &CColorImageProcessingView::OnConstractZip)
	ON_COMMAND(IDM_POSTERIZE, &CColorImageProcessingView::OnPosterize)
	ON_COMMAND(IDM_AREA_STRESS, &CColorImageProcessingView::OnAreaStress)
	ON_COMMAND(IDM_COLOR_IMPACT, &CColorImageProcessingView::OnColorImpact)
	ON_COMMAND(IDM_INTENSITY_CHANGE, &CColorImageProcessingView::OnIntensityChange)
	ON_COMMAND(IDM_EMBOSS, &CColorImageProcessingView::OnEmboss)
	ON_COMMAND(IDM_EMBOS, &CColorImageProcessingView::OnEmbos)
	ON_COMMAND(IDM_ZOOM_IN, &CColorImageProcessingView::OnZoomIn)
	ON_COMMAND(IDM_ZOOM_OUT, &CColorImageProcessingView::OnZoomOut)
	ON_COMMAND(ID_ROTATE_ORIGIN, &CColorImageProcessingView::OnRotateOrigin)
	ON_COMMAND(IDM_ROTATE_NO_CUT, &CColorImageProcessingView::OnRotateNoCut)
	ON_COMMAND(IDM_MOVE_HORIZON, &CColorImageProcessingView::OnMoveHorizon)
	ON_COMMAND(IDM_MOVE_VERTICAL, &CColorImageProcessingView::OnMoveVertical)
	ON_COMMAND(IDM_MIRROR_HORIZON, &CColorImageProcessingView::OnMirrorHorizon)
	ON_COMMAND(IDM_MIRROR_VERTICAL, &CColorImageProcessingView::OnMirrorVertical)
	ON_COMMAND(IDM_MORPH, &CColorImageProcessingView::OnMorph)
	ON_COMMAND(IDM_HISTO_STRETCH, &CColorImageProcessingView::OnHistoStretch)
	ON_COMMAND(IDM_FLAT, &CColorImageProcessingView::OnFlat)
	ON_COMMAND(ID_HISTO_STRECHH, &CColorImageProcessingView::OnHistoStrechh)
	ON_COMMAND(IDM_FLATT, &CColorImageProcessingView::OnFlatt)
	ON_COMMAND(IDM_HISTO_STRECHH, &CColorImageProcessingView::OnHistoStrecH)
	ON_COMMAND(IDM_WANT_FILTER, &CColorImageProcessingView::OnWantFilter)
	ON_COMMAND(IDM_WANT_FILTER2, &CColorImageProcessingView::OnWantFilter2)
END_MESSAGE_MAP()

// CColorImageProcessingView 생성/소멸

CColorImageProcessingView::CColorImageProcessingView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CColorImageProcessingView::~CColorImageProcessingView()
{
}

BOOL CColorImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CColorImageProcessingView 그리기

void CColorImageProcessingView::OnDraw(CDC* pDC)
{
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	//int R, G, B;

	//for (int i = 0; i < pDoc->m_inH; i++)
	//	for (int k = 0; k < pDoc->m_inW; k++) {
	//		R = pDoc->m_inImageR[i][k];
	//		G = pDoc->m_inImageG[i][k];
	//		B = pDoc->m_inImageB[i][k];
	//		pDC->SetPixel(k + 5, i + 5, RGB(R, G, B));
	//	}

	//for (int i = 0; i < pDoc->m_outH; i++)
	//	for (int k = 0; k < pDoc->m_outW; k++) {
	//		R = pDoc->m_outImageR[i][k];
	//		G = pDoc->m_outImageG[i][k];
	//		B = pDoc->m_outImageB[i][k];
	//		pDC->SetPixel(5 + pDoc->m_inW + 5 + k, i + 5, RGB(R, G, B));
	//	}
	

		/////////////////////
	/// 성능 개선을 위한 더블 버퍼링 
	////////////////////
	int i, k;
	unsigned char R, G, B;
	// 메모리 DC 선언
	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_inW, pDoc->m_inH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_inW, pDoc->m_inH, WHITENESS); // 흰색으로 초기화


	// 출력 영상의 크기를 자동 조절
	double MAXSIZE = 800;  // 필요시 실 모니터 또는 화면의 해상도에 따라서 변경 가능!
	int inH = pDoc->m_inH;
	int inW = pDoc->m_inW;
	double hop = 1.0; // 기본

	if (inH > MAXSIZE || inW > MAXSIZE) {
		// hop을 새로 계산.
		if (inW > inH)
			hop = (inW / MAXSIZE);
		else
			hop = (inH / MAXSIZE);

		inW = (int)(inW / hop);
		inH = (int)(inH / hop);
	}

	// 메모리 DC에 그리기
	for (i = 0; i < inH; i++) {
		for (k = 0; k < inW; k++) {
			R = pDoc->m_inImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_inImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_inImageB[(int)(i * hop)][(int)(k * hop)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}

	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(5, 5, pDoc->m_inW, pDoc->m_inH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	///////////////////

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_outW, pDoc->m_outH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_outW, pDoc->m_outH, WHITENESS); // 흰색으로 초기화

	int outH = pDoc->m_outH;
	int outW = pDoc->m_outW;
	hop = 1.0; // 기본

	if (outH > MAXSIZE || outW > MAXSIZE) {
		// hop을 새로 계산.
		if (outW > outH)
			hop = (outW / MAXSIZE);
		else
			hop = (outH / MAXSIZE);

		outW = (int)(outW / hop);
		outH = (int)(outH / hop);
	}

	// 메모리 DC에 그리기
	for (i = 0; i < outH; i++) {
		for (k = 0; k < outW; k++) {
			R = pDoc->m_outImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_outImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_outImageB[(int)(i * hop)][(int)(k * hop)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(inW + 10, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


// CColorImageProcessingView 인쇄

BOOL CColorImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CColorImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CColorImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CColorImageProcessingView 진단

#ifdef _DEBUG
void CColorImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CColorImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColorImageProcessingDoc* CColorImageProcessingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorImageProcessingDoc)));
	return (CColorImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


void CColorImageProcessingView::OnInvalidate()
{
	// TODO: 여기에 구현 코드 추가.
	Invalidate(TRUE);
}


// CColorImageProcessingView 메시지 처리기


void CColorImageProcessingView::OnEqualImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEqualImage();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnGreyScale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGreyScale();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnLightChange()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLightChange();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnReverse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnReverse();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnGammaChange()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGammaChange();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnParaCap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnParaCap();
	Invalidate(TRUE); 
}


void CColorImageProcessingView::OnParaCup()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnParaCup();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMono127()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMono127();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMonoMid()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMonoMid();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMonoAvg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMonoAvg();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMaskAnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMaskAnd();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMaskOr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMaskOr();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMaskXor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMaskXor();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMaskNot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMaskNot();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnConstractStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnConstractStretch();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnConstractZip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnConstractZip();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnPosterize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPosterize();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnAreaStress()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAreaStress();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnChangeSatur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnChangeSatur();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnPickOrange()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPickOrange();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnColorImpact()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnColorImpact();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnIntensityChange()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnIntensityChange();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEmbos()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEmbos();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnEmboss()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEmboss();
	Invalidate(TRUE);
}

void CColorImageProcessingView::OnZoomIn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomIn();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnZoomOut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomOut();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnRotateOrigin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotateOrigin();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnRotateNoCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotateNoCut();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMoveHorizon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMoveHorizon();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMoveVertical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMoveVertical();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMirrorHorizon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMirrorHorizon();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMirrorVertical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMirrorVertical();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnMorph()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMorph();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnHistoStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoStretch();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnFlat()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFlat();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnFlatt()
{	
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFlatt();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnHistoStrecH()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoStrechh();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnWantFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnWantFilter();
	Invalidate(TRUE);
}


void CColorImageProcessingView::OnWantFilter2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnWantFilter2();
	Invalidate(TRUE);
}
