
// GreyScaleImageProcessingDoc.cpp: CGreyScaleImageProcessingDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GreyScaleImageProcessing.h"
#endif

#include "GreyScaleImageProcessingDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGreyScaleImageProcessingDoc

IMPLEMENT_DYNCREATE(CGreyScaleImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CGreyScaleImageProcessingDoc, CDocument)
END_MESSAGE_MAP()


// CGreyScaleImageProcessingDoc 생성/소멸

CGreyScaleImageProcessingDoc::CGreyScaleImageProcessingDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CGreyScaleImageProcessingDoc::~CGreyScaleImageProcessingDoc()
{
}

BOOL CGreyScaleImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGreyScaleImageProcessingDoc serialization

void CGreyScaleImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGreyScaleImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CGreyScaleImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGreyScaleImageProcessingDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGreyScaleImageProcessingDoc 진단

#ifdef _DEBUG
void CGreyScaleImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGreyScaleImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGreyScaleImageProcessingDoc 명령


unsigned char** CGreyScaleImageProcessingDoc::OnMalloc2D(int h, int w, int initVal)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** retMemory = NULL;

	retMemory = new unsigned char* [h];
	for (int i = 0; i < h; i++)
		retMemory[i] = new unsigned char[w];

	return retMemory;
}


void CGreyScaleImageProcessingDoc::OnFree2D(unsigned char** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.
	if (memory == NULL)
		return;

	for (int i = 0; i < h; i++)
		delete[] memory[i];

	delete[] memory;
}


BOOL CGreyScaleImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	// 선택된 파일 열기
	CFile File;
	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);

	// 파일 크기 파악 -> 가로세로 크기 결정
	ULONGLONG fLength = File.GetLength();			// ULONGLONG = unsigned long long 

	// 입력 이미지의 높이와 폭을 결정
	// 기존 메모리 해제 및, 관련 값 초기화
	OnFree2D(m_inImage, m_inH);
	m_inImage = NULL;
	m_inH = m_inW = 0;
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// (중!요) 입력이미지의 높이와 폭을 결정 + 메모리할당
	m_inH = m_inW = (int) sqrt((double)fLength);
	m_inImage = OnMalloc2D(m_inH, m_inW, 0);

	// 파일 -> 메모리 
	for (int i = 0; i < m_inH; i++)
		File.Read(m_inImage[i], m_inW);

	File.Close();

	return TRUE;
}


void CGreyScaleImageProcessingDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.	
	// 프로그램 종료시 시행
	
	// 입력 이미지의 높이와 폭을 결정
	// 기존 메모리 해제 및, 관련 값 초기화
	OnFree2D(m_inImage, m_inH);
	m_inImage = NULL;
	m_inH = m_inW = 0;
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	CDocument::OnCloseDocument();
}


void CGreyScaleImageProcessingDoc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
	// 출력 메모리 해제
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// (중요!) 알고리즘에 따라 출력이미지 크기 결정
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)
			m_outImage[i][k] = m_inImage[i][k];
}
