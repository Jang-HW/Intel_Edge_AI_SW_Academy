
// GreyScaleImageProcessingBeta1View.cpp: CGreyScaleImageProcessingBeta1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GreyScaleImageProcessingBeta1.h"
#endif

#include "GreyScaleImageProcessingBeta1Doc.h"
#include "GreyScaleImageProcessingBeta1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGreyScaleImageProcessingBeta1View

IMPLEMENT_DYNCREATE(CGreyScaleImageProcessingBeta1View, CView)

BEGIN_MESSAGE_MAP(CGreyScaleImageProcessingBeta1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CGreyScaleImageProcessingBeta1View::OnEqualImage)
	ON_COMMAND(IDM_LIGHT_IMAGE, &CGreyScaleImageProcessingBeta1View::OnLightImage)
	ON_COMMAND(IDM_ADD_IMAGE, &CGreyScaleImageProcessingBeta1View::OnAddImage)
	ON_COMMAND(IDM_REVERSE_IMAGE, &CGreyScaleImageProcessingBeta1View::OnReverseImage)
	ON_COMMAND(IDM_GAMMA_IMAGE, &CGreyScaleImageProcessingBeta1View::OnGammaImage)
	ON_COMMAND(IDM_CAP_IMAGE, &CGreyScaleImageProcessingBeta1View::OnCapImage)
	ON_COMMAND(IDM_CUP_IMAGE, &CGreyScaleImageProcessingBeta1View::OnCupImage)
	ON_COMMAND(IDM_MONO_127, &CGreyScaleImageProcessingBeta1View::OnMono127)
	ON_COMMAND(IDM_MONO_MID, &CGreyScaleImageProcessingBeta1View::OnMonoMid)
	ON_COMMAND(IDM_MONO_AVG, &CGreyScaleImageProcessingBeta1View::OnMonoAvg)
	ON_COMMAND(IDM_MASK_AND, &CGreyScaleImageProcessingBeta1View::OnMaskAnd)
	ON_COMMAND(IDM_MASK_OR, &CGreyScaleImageProcessingBeta1View::OnMaskOr)
	ON_COMMAND(IDM_MASK_XOR, &CGreyScaleImageProcessingBeta1View::OnMaskXor)
	ON_COMMAND(IDM_NOT_IMAGE, &CGreyScaleImageProcessingBeta1View::OnNotImage)
	ON_COMMAND(IDM_STRACTING_IMAGE, &CGreyScaleImageProcessingBeta1View::OnStractingImage)
	ON_COMMAND(IDM_ZIP_IMAGE, &CGreyScaleImageProcessingBeta1View::OnZipImage)
	ON_COMMAND(IDM_POSTER_IMAGE, &CGreyScaleImageProcessingBeta1View::OnPosterImage)
	ON_COMMAND(IDM_AREA_IMPACT, &CGreyScaleImageProcessingBeta1View::OnAreaImpact)
	ON_COMMAND(IDM_ZOOM_IN, &CGreyScaleImageProcessingBeta1View::OnZoomIn)
	ON_COMMAND(IDM_ZOOM_OUT, &CGreyScaleImageProcessingBeta1View::OnZoomOut)
	ON_COMMAND(IDM_ROTATE_ORIGIN, &CGreyScaleImageProcessingBeta1View::OnRotateOrigin)
	ON_COMMAND(IDM_ROTATE_NO_CUT, &CGreyScaleImageProcessingBeta1View::OnRotateNoCut)
	ON_COMMAND(IDM_MOVE_VERTICAL, &CGreyScaleImageProcessingBeta1View::OnMoveVertical)
	ON_COMMAND(IDM_MOVE_HORIZONAL, &CGreyScaleImageProcessingBeta1View::OnMoveHorizonal)
	ON_COMMAND(IDM_MIRROR_VERTICAL, &CGreyScaleImageProcessingBeta1View::OnMirrorVertical)
	ON_COMMAND(IDM_MIRROR_HORIZONAL, &CGreyScaleImageProcessingBeta1View::OnMirrorHorizonal)
	ON_COMMAND(IDM_MORPH, &CGreyScaleImageProcessingBeta1View::OnMorph)
	ON_COMMAND(IDM_HISTO_STRECTCH, &CGreyScaleImageProcessingBeta1View::OnHistoStrectch)
	ON_COMMAND(IDM_END_IN, &CGreyScaleImageProcessingBeta1View::OnEndIn)
	ON_COMMAND(IDM_HISTO_FLATTEN, &CGreyScaleImageProcessingBeta1View::OnHistoFlatten)
	ON_COMMAND(IDM_EMBOSS, &CGreyScaleImageProcessingBeta1View::OnEmboss)
	ON_COMMAND(IDM_BLUR, &CGreyScaleImageProcessingBeta1View::OnBlur)
END_MESSAGE_MAP()

// CGreyScaleImageProcessingBeta1View 생성/소멸

CGreyScaleImageProcessingBeta1View::CGreyScaleImageProcessingBeta1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CGreyScaleImageProcessingBeta1View::~CGreyScaleImageProcessingBeta1View()
{
}

BOOL CGreyScaleImageProcessingBeta1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CGreyScaleImageProcessingBeta1View 그리기

void CGreyScaleImageProcessingBeta1View::OnDraw(CDC* pDC)
{
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	/*unsigned char R, G, B;
	for (int i = 0; i < pDoc->m_inH; i++) {
		for (int k = 0; k < pDoc->m_inW; k++) {
			R = G = B = pDoc->m_inImage[i][k];
			pDC->SetPixel(k + 5, i + 5, RGB(R, G, B));
		}
	}
	for (int i = 0; i < pDoc->m_outH; i++) {
		for (int k = 0; k < pDoc->m_outW; k++) {
			R = G = B = pDoc->m_outImage[i][k];
			pDC->SetPixel(k + pDoc->m_inW + 10, i + 5, RGB(R, G, B));
		}
	}*/


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
			R = G = B = pDoc->m_inImage[i][k];
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
			R = G = B = pDoc->m_outImage[i][k];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(pDoc->m_inW + 10, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


// CGreyScaleImageProcessingBeta1View 인쇄

BOOL CGreyScaleImageProcessingBeta1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGreyScaleImageProcessingBeta1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGreyScaleImageProcessingBeta1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CGreyScaleImageProcessingBeta1View 진단

#ifdef _DEBUG
void CGreyScaleImageProcessingBeta1View::AssertValid() const
{
	CView::AssertValid();
}

void CGreyScaleImageProcessingBeta1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGreyScaleImageProcessingBeta1Doc* CGreyScaleImageProcessingBeta1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGreyScaleImageProcessingBeta1Doc)));
	return (CGreyScaleImageProcessingBeta1Doc*)m_pDocument;
}
#endif //_DEBUG


// CGreyScaleImageProcessingBeta1View 메시지 처리기


void CGreyScaleImageProcessingBeta1View::OnEqualImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEqualImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnLightImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLightImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnAddImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAddImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnReverseImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnReverseImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnGammaImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGammaImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnCapImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnCapImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnCupImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnCupImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMono127()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMono127();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMonoMid()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMonoMid();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMonoAvg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMonoAvg();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMaskAnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMaskAnd();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMaskOr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMaskOr();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMaskXor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMaskXor();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnNotImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnNotImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnStractingImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnStractingImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnZipImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZipImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnPosterImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPosterImage();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnAreaImpact()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAreaImpact();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnZoomIn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomIn();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnZoomOut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomOut();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnRotateOrigin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotateOrigin();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnRotateNoCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotateNoCut();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMoveVertical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMoveVertical();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMoveHorizonal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMoveHorizonal();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMirrorVertical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMirrorVertical();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMirrorHorizonal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMirrorHorizonal();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnMorph()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMorph();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnHistoStrectch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoStrectch();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnEndIn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEndIn();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnHistoFlatten()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoFlatten();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnEmboss()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEmboss();
	Invalidate(TRUE);
}


void CGreyScaleImageProcessingBeta1View::OnBlur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGreyScaleImageProcessingBeta1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBlur();
	Invalidate(TRUE);
}
