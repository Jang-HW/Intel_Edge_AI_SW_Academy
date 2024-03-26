
// GreyScale2View.cpp: CGreyScale2View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GreyScale2.h"
#endif

#include "GreyScale2Doc.h"
#include "GreyScale2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGreyScale2View

IMPLEMENT_DYNCREATE(CGreyScale2View, CView)

BEGIN_MESSAGE_MAP(CGreyScale2View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGreyScale2View 생성/소멸

CGreyScale2View::CGreyScale2View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CGreyScale2View::~CGreyScale2View()
{
}

BOOL CGreyScale2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CGreyScale2View 그리기

void CGreyScale2View::OnDraw(CDC* /*pDC*/)
{
	CGreyScale2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CGreyScale2View 인쇄

BOOL CGreyScale2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGreyScale2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGreyScale2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CGreyScale2View 진단

#ifdef _DEBUG
void CGreyScale2View::AssertValid() const
{
	CView::AssertValid();
}

void CGreyScale2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGreyScale2Doc* CGreyScale2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGreyScale2Doc)));
	return (CGreyScale2Doc*)m_pDocument;
}
#endif //_DEBUG


// CGreyScale2View 메시지 처리기
