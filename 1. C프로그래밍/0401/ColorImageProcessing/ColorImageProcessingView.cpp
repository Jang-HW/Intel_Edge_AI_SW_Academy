
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

	// 메모리 DC에 그리기
	for (i = 0; i < pDoc->m_inH; i++) {
		for (k = 0; k < pDoc->m_inW; k++) {
			R = pDoc->m_inImageR[i][k];
			G = pDoc->m_inImageG[i][k];
			B = pDoc->m_inImageB[i][k];
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

	// 메모리 DC에 그리기
	for (i = 0; i < pDoc->m_outH; i++) {
		for (k = 0; k < pDoc->m_outW; k++) {
			R = pDoc->m_outImageR[i][k];
			G = pDoc->m_outImageG[i][k];
			B = pDoc->m_outImageB[i][k];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(pDoc->m_inW + 10, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

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
