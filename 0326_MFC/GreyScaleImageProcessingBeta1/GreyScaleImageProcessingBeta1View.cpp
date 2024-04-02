
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
	unsigned char R, G, B;
	
	// 입력 이미지 출력
	for (int i = 0; i < pDoc->m_inH; i++){
		for (int k = 0; k < pDoc->m_inW; k++) {
			R = G = B = pDoc->m_inImage[i][k];
			pDC->SetPixel(k + 5, i + 5, RGB(R, G, B));
		}
	}
	// 출력 이미지 출력 
	for (int i = 0; i < pDoc->m_outH; i++) {
		for (int k = 0; k < pDoc->m_outW; k++) {
			R = G = B = pDoc->m_outImage[i][k];
			pDC->SetPixel(pDoc->m_inW + 5 + 5 + k, i + 5, RGB(R, G, B));
		}
	}
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
