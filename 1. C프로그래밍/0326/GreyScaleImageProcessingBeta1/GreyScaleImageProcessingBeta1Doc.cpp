
// GreyScaleImageProcessingBeta1Doc.cpp: CGreyScaleImageProcessingBeta1Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GreyScaleImageProcessingBeta1.h"
#endif

#include "GreyScaleImageProcessingBeta1Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGreyScaleImageProcessingBeta1Doc

IMPLEMENT_DYNCREATE(CGreyScaleImageProcessingBeta1Doc, CDocument)

BEGIN_MESSAGE_MAP(CGreyScaleImageProcessingBeta1Doc, CDocument)
END_MESSAGE_MAP()


// CGreyScaleImageProcessingBeta1Doc 생성/소멸

CGreyScaleImageProcessingBeta1Doc::CGreyScaleImageProcessingBeta1Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CGreyScaleImageProcessingBeta1Doc::~CGreyScaleImageProcessingBeta1Doc()
{
}

BOOL CGreyScaleImageProcessingBeta1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGreyScaleImageProcessingBeta1Doc serialization

void CGreyScaleImageProcessingBeta1Doc::Serialize(CArchive& ar)
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
void CGreyScaleImageProcessingBeta1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CGreyScaleImageProcessingBeta1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGreyScaleImageProcessingBeta1Doc::SetSearchContent(const CString& value)
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

// CGreyScaleImageProcessingBeta1Doc 진단

#ifdef _DEBUG
void CGreyScaleImageProcessingBeta1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGreyScaleImageProcessingBeta1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGreyScaleImageProcessingBeta1Doc 명령


BOOL CGreyScaleImageProcessingBeta1Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CFile File;
	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);

	ULONGLONG fileLength = File.GetLength();
	double logLength = log(sqrt((double)fileLength)) / log(2.0);
	if (logLength != (int)logLength) {
		AfxMessageBox(L"정사각형의 이미지를 열어주세요");
		return 0;
	}
	
	// 입.출력 이미지를 free하고 (재)입력 저장
	OnFree2D(m_inImage, m_inH);
	m_inH = m_inW = 0;
	m_inImage = NULL;
	OnFree2D(m_outImage, m_outH);
	m_outH = m_inW = 0;
	m_outImage = NULL;

	m_inH = m_inW = (int)sqrt(fileLength);
	m_inImage = OnMalloc2D(m_inH, m_inW, 0);

	for (int i = 0; i < m_inH; i++)
		File.Read(m_inImage[i], m_inW);

	File.Close();
	return TRUE;
}


// 2차원 배열을 만드는 함수
unsigned char** CGreyScaleImageProcessingBeta1Doc::OnMalloc2D(int h, int w, int init)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** image = new unsigned char* [h];

	for (int i = 0; i < h; i++)
		image[i] = new unsigned char[w];

	return image;
}


void CGreyScaleImageProcessingBeta1Doc::OnFree2D(unsigned char** image, int h)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < h; i++)
		delete image[i];
	delete[] image;
}


void CGreyScaleImageProcessingBeta1Doc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnFree2D(m_inImage, m_inH);
	m_inImage = NULL;
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;

	CDocument::OnCloseDocument();
}


void CGreyScaleImageProcessingBeta1Doc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 영상처리 알고리즘 
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)
			m_outImage[i][k] = m_inImage[i][k];
}
