
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
#include "ConstantDlg.h"

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

	m_inH = m_inW = (int)sqrt((double)fileLength);
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


double** CGreyScaleImageProcessingBeta1Doc::mallocDoubleMemory(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	double** image = new double* [h];

	for (int i = 0; i < h; i++)
		image[i] = new double[w];

	return image;
}


void CGreyScaleImageProcessingBeta1Doc::freeDoubleMemory(double** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < h; i++)
		delete memory[i];
	delete[] memory;
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


BOOL CGreyScaleImageProcessingBeta1Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFile file;
	CFileDialog saveDlg(FALSE, L"raw", NULL, OFN_HIDEREADONLY);

	if (saveDlg.DoModal() != IDOK)
		return 0;

	file.Open(saveDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite);
	for (int i = 0; i < m_outH; i++) {
		file.Write(m_outImage[i], m_outW);
	}
	file.Close();


	return true;
}


// 동일 이미지 알고리즘
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


void CGreyScaleImageProcessingBeta1Doc::OnLightImage()
{
}


void CGreyScaleImageProcessingBeta1Doc::OnAddImage()
{
	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;

	// modal	: 창을 띄워놔도 뒤에 거를 누를 수 '없는' 창
	// modaless : 창을 띄워놔도 뒤에 거를 누를 수 '있는' 창
	if (dlg.DoModal() != IDOK)
		return;
	
	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 변경할 값 가져오기
	int value = (int)dlg.m_constant;
	if (value > 255)		value = 255;
	else if (value < -255)	value = -255;

	// 영상처리 알고리즘 
	int px;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			px = m_inImage[i][k] + value;
			if (px > 255)
				px = 255;
			else if (px < 0)
				px = 0;
			m_outImage[i][k] = px;
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnReverseImage()
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
			m_outImage[i][k] = 255 - m_inImage[i][k];
}


void CGreyScaleImageProcessingBeta1Doc::OnGammaImage()
{
	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;
	 	
	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 변경할 값 가져오기
	int scale = (int)dlg.m_constant;
	if (scale < 0)		scale = 0;
	if (scale > 100)	scale = 100;


	// 영상처리 알고리즘 
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)
			m_outImage[i][k] = int(255 * pow(m_inImage[i][k] / 255., 30 / scale));
}


void CGreyScaleImageProcessingBeta1Doc::OnCapImage()
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
			m_outImage[i][k] = int(255. * pow(((m_inImage[i][k] / 127.) - 1), 2));
}


void CGreyScaleImageProcessingBeta1Doc::OnCupImage()
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
			m_outImage[i][k] = int(255 - 255. * pow(((m_inImage[i][k] / 127.) - 1), 2));
}


void CGreyScaleImageProcessingBeta1Doc::OnMono127()
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
	for (int i = 0; i < m_inH; i++)	{
		for (int k = 0; k < m_inW; k++)	{
			if (m_inImage[i][k] > 128) {
				m_outImage[i][k] = 255;
			}
			else {
				m_outImage[i][k] = 0;
			}
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnMonoMid()
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
	int cMid = 0, count = 0;
	int cValCount[256] = { 0, };

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			cValCount[m_inImage[i][k]] += 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		count += cValCount[i];
		if (count > (m_inW * m_inH) / 2) {
			cMid = i;
			break;
		}
	}
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (m_inImage[i][k] < cMid)
				m_outImage[i][k] = 0;
			else m_outImage[i][k] = 255;
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnMonoAvg()
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
	int cMid = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			cMid += m_inImage[i][k];
		}
	}
	cMid = cMid / (m_inH * m_inW);

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (m_inImage[i][k] < cMid)
				m_outImage[i][k] = 0;
			else m_outImage[i][k] = 255;
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnMaskAnd()
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
	CFile File;
	CFileDialog openDlg(TRUE, L"raw", NULL, OFN_HIDEREADONLY);
	if (openDlg.DoModal() != IDOK)
		return;
	File.Open(openDlg.GetPathName(), CFile::modeRead | CFile::typeBinary);

	ULONGLONG fileLength = File.GetLength();
	double logLength = log(sqrt((double)fileLength)) / log(2.0);
	if (logLength != (int)logLength) {
		AfxMessageBox(L"정사각형의 이미지를 열어주세요");
		return;
	}

	// 입.출력 이미지를 free하고 (재)입력 저장
	unsigned char** mask; 
	int maskH, maskW;

	maskH = maskW = (int)sqrt((double)fileLength);
	mask = OnMalloc2D(maskH, maskW, 0);

	for (int i = 0; i < m_inH; i++)
		File.Read(mask[i], maskW);

	File.Close();

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImage[i][k] = m_inImage[i][k] & mask[i][k];
		}
	}

	OnFree2D(mask, maskH);
}


void CGreyScaleImageProcessingBeta1Doc::OnMaskOr()
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
	CFile File;
	CFileDialog openDlg(TRUE, L"raw", NULL, OFN_HIDEREADONLY);
	if (openDlg.DoModal() != IDOK)
		return;
	File.Open(openDlg.GetPathName(), CFile::modeRead | CFile::typeBinary);

	ULONGLONG fileLength = File.GetLength();
	double logLength = log(sqrt((double)fileLength)) / log(2.0);
	if (logLength != (int)logLength) {
		AfxMessageBox(L"정사각형의 이미지를 열어주세요");
		return;
	}

	// 입.출력 이미지를 free하고 (재)입력 저장
	unsigned char** mask;
	int maskH, maskW;

	maskH = maskW = (int)sqrt((double)fileLength);
	mask = OnMalloc2D(maskH, maskW, 0);

	for (int i = 0; i < m_inH; i++)
		File.Read(mask[i], maskW);

	File.Close();

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImage[i][k] = m_inImage[i][k] | mask[i][k];
		}
	}

	OnFree2D(mask, maskH);
}


void CGreyScaleImageProcessingBeta1Doc::OnMaskXor()
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
	CFile File;
	CFileDialog openDlg(TRUE, L"raw", NULL, OFN_HIDEREADONLY);
	if (openDlg.DoModal() != IDOK)
		return;
	File.Open(openDlg.GetPathName(), CFile::modeRead | CFile::typeBinary);

	ULONGLONG fileLength = File.GetLength();
	double logLength = log(sqrt((double)fileLength)) / log(2.0);
	if (logLength != (int)logLength) {
		AfxMessageBox(L"정사각형의 이미지를 열어주세요");
		return;
	}

	// 입.출력 이미지를 free하고 (재)입력 저장
	unsigned char** mask;
	int maskH, maskW;

	maskH = maskW = (int)sqrt((double)fileLength);
	mask = OnMalloc2D(maskH, maskW, 0);

	for (int i = 0; i < m_inH; i++)
		File.Read(mask[i], maskW);

	File.Close();

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImage[i][k] = m_inImage[i][k] ^ mask[i][k];
		}
	}

	OnFree2D(mask, maskH);
}


void CGreyScaleImageProcessingBeta1Doc::OnNotImage()
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
			m_outImage[i][k] = ~m_inImage[i][k];

}


void CGreyScaleImageProcessingBeta1Doc::OnStractingImage()
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
	for (int i = 0; i < m_inH; i++)	{
		for (int k = 0; k < m_inW; k++) {
			if (m_inImage[i][k] < 50)		m_outImage[i][k] = 0;
			else if (m_inImage[i][k] > 200)	m_outImage[i][k] = 255;
			else							m_outImage[i][k] = m_inImage[i][k] * 51 / 30 - 85;
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnZipImage()
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
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImage[i][k] = m_inImage[i][k] * 30 / 51 + 50;
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnPosterImage()
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
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			switch (m_inImage[i][k] / 32) {
			case 0: m_outImage[i][k] = 0;		break;
			case 1: m_outImage[i][k] = 32;	break;
			case 2: m_outImage[i][k] = 64;	break;
			case 3: m_outImage[i][k] = 96;	break;
			case 4: m_outImage[i][k] = 128;	break;
			case 5: m_outImage[i][k] = 160;	break;
			case 6: m_outImage[i][k] = 192;	break;
			case 7: m_outImage[i][k] = 224;	break;
			case 8: m_outImage[i][k] = 255;	break;
			}
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnAreaImpact()
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
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (m_inImage[i][k] < 75)		m_outImage[i][k] = m_inImage[i][k];
			else if (m_inImage[i][k] > 175)	m_outImage[i][k] = m_inImage[i][k];
			else 							m_outImage[i][k] = 255;
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnZoomIn()
{
	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	// 변경할 값 가져오기
	int scale = (int)dlg.m_constant;
	if (scale < 0)		scale = 0;
	if (scale > 100)	scale = 100;

	m_outH = int(m_inH * scale);
	m_outW = int(m_inW * scale);
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 영상처리 알고리즘 
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImage[i * scale][k * scale] = m_inImage[i][k];
		}
	}

	int i_h, i_w;
	double r_H, r_W, s_H, s_W;
	double C1, C2, C3, C4;

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			r_H = i / scale;								// 현재 위치를 scale 크기의 블록 중 몇 번째 쯤인지 연산
			r_W = k / scale;

			i_h = (int)floor(r_H);							// 블록의 가로세로 칸을 저장
			i_w = (int)floor(r_W);

			s_H = r_H - i_h;								// 현재 블록 내에서의 위치를 저장 
			s_W = r_W - i_w;

			if ((i_h < 0) || (i_h >= (m_inH - 1)) || (i_w < 0) || (i_w >= (m_inW - 1))) {
				m_outImage[i][k] = 255;						// 현재 칸이 보간을 진행할 수 없다면 흰색으로 지정
				// 주로 왼쪽과 아래쪽이 그런 경향이 있음
			}
			else {
				C1 = (double)m_inImage[i_h][i_w];				// 좌 상단점 지정
				C2 = (double)m_inImage[i_h][i_w + 1];			// 우 상단점 지정
				C3 = (double)m_inImage[i_h + 1][i_w + 1];		// 우 하단점 지정
				C4 = (double)m_inImage[i_h + 1][i_w];			// 좌 하단점 지정
				m_outImage[i][k] = (unsigned char)(C1 * (1 - s_H) * (1 - s_W)
					+ C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				// 각 점들과의 위치 비율을 반영해서 점의 색 지정 
			}

		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnZoomOut()
{
	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	// 변경할 값 가져오기
	int scale = (int)dlg.m_constant;
	if (scale < 0)		scale = 0;
	if (scale > 100)	scale = 100;

	m_outH = int(m_inH / scale);
	m_outW = int(m_inW / scale);
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 영상처리 알고리즘 
	unsigned int** tmp = new unsigned int* [m_outH];
	for (int i = 0; i < m_outH; i++)
		tmp[i] = new unsigned int [m_outW];
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			tmp[i][k] = 0;
		}
	}

	// 입력 배열 --> 임시 배열
	for (int i = 0; i < m_inH; i++) {
		if (m_outH <= i / scale) break;
		for (int k = 0; k < m_inW; k++) {
			if (m_outW <= k / scale) break;
			tmp[(int)(i / scale)][(int)(k / scale)] += m_inImage[i][k];
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImage[i][k] = tmp[i][k] / (scale * scale);
		}
	}

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImage[i][k] = (unsigned char)m_outImage[i][k];
		}
	}


	for (int i = 0; i < m_outH; i++)
		delete tmp[i];
	delete[] tmp;
}


void CGreyScaleImageProcessingBeta1Doc::OnRotateOrigin()
{
	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 변경할 값 가져오기
	int degree = (int)dlg.m_constant;
	
	double radian = degree * 3.1415925386 / 180.0;
	//// xd = cos*xs - sin*ys
	//// yd = sin*xs + cos*ys

	int cx = m_inH / 2;
	int cy = m_inW / 2;

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			int xd = i;
			int yd = k;

			int	xs = (int)(cos(radian) * (xd - cx) + sin(radian) * (yd - cx));
			int ys = (int)(-sin(radian) * (xd - cy) + cos(radian) * (yd - cy));
			xs += cx;
			ys += cy;

			if (0 <= xs && xs < m_inH && 0 <= ys && ys < m_inW)
				m_outImage[xd][yd] = m_inImage[xs][ys];
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnRotateNoCut()
{

	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 변경할 값 가져오기
	int degree = (int)dlg.m_constant;
	if (degree > 360) {
		degree %= 360;
	}
	else if (degree < 0) {
		degree = abs(degree);
		degree %= 360;
		degree = 360 - degree;
	}
	double radian = degree * 3.1415925386 / 180.0;

	// 출력 이미지 크기 결정
	if (degree < 90) {
		m_outH = (int)((double)m_inW * sin(radian) + m_inH * cos(radian));
		m_outW = (int)((double)m_inW * cos(radian) + m_inH * sin(radian));
	}
	else if (degree < 180) {
		m_outH = (int)((double)m_inW * sin(radian) + m_inH * -cos(radian));
		m_outW = (int)((double)m_inW * -cos(radian) + m_inH * sin(radian));
	}
	else if (degree < 270) {
		m_outH = (int)((double)m_inW * -sin(radian) + m_inH * -cos(radian));
		m_outW = (int)((double)m_inW * -cos(radian) + m_inH * -sin(radian));
	}
	else {
		m_outH = (int)((double)m_inW * -sin(radian) + m_inH * cos(radian));
		m_outW = (int)((double)m_inW * cos(radian) + m_inH * -sin(radian));
	}
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);
	//// xd = cos*xs - sin*ys
	//// yd = sin*xs + cos*ys

	int cx = m_inH / 2;
	int cy = m_inW / 2;

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			int xd = i;
			int yd = k;
			int	xs = (int)(cos(radian) * (xd - (m_outH / 2)) + sin(radian) * (yd - (m_outH / 2)));
			int ys = (int)(-sin(radian) * (xd - (m_outW / 2)) + cos(radian) * (yd - (m_outW / 2)));

			xs += cx;
			ys += cy;

			if (0 <= xs && xs < m_inH && 0 <= ys && ys < m_inW) {
				m_outImage[xd][yd] = m_inImage[xs][ys];
			}
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnMoveVertical()
{
	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 영상처리 알고리즘 
	// 변경할 값 가져오기
	int degree = (int)dlg.m_constant;
	if (degree > m_inH) degree = m_inH;
	
	for (int i = 0; i < m_inH; i++)	{
		for (int k = 0; k < m_inW; k++)	{
			if (i + degree >= m_outH || i + degree < 0)	break;
			m_outImage[i + degree][k] = m_inImage[i][k];
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnMoveHorizonal()
{
	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 영상처리 알고리즘 
	// 변경할 값 가져오기
	int degree = (int)dlg.m_constant;

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (k + degree >= m_outW || k + degree < 0)		continue;
			m_outImage[i][k + degree] = m_inImage[i][k];
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnMirrorVertical()
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
			m_outImage[m_inH - i - 1][k] = m_inImage[i][k];
}


void CGreyScaleImageProcessingBeta1Doc::OnMirrorHorizonal()
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
			m_outImage[i][m_inW - k - 1] = m_inImage[i][k];
}


void CGreyScaleImageProcessingBeta1Doc::OnMorph()
{
	// TODO: 여기에 구현 코드 추가.
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
	CFile File;
	CFileDialog openDlg(TRUE, L"raw", NULL, OFN_HIDEREADONLY);
	if (openDlg.DoModal() != IDOK)
		return;
	File.Open(openDlg.GetPathName(), CFile::modeRead | CFile::typeBinary);

	ULONGLONG fileLength = File.GetLength();
	double logLength = log(sqrt((double)fileLength)) / log(2.0);
	if (logLength != (int)logLength) {
		AfxMessageBox(L"정사각형의 이미지를 열어주세요");
		return;
	}

	// 입.출력 이미지를 free하고 (재)입력 저장
	unsigned char** mask;
	int maskH, maskW;

	maskH = maskW = (int)sqrt((double)fileLength);
	mask = OnMalloc2D(maskH, maskW, 0);

	for (int i = 0; i < m_inH; i++)
		File.Read(mask[i], maskW);

	File.Close();

	// 5단계 모핑
	for (int n = 0; n < 5; n++) {
		for (int i = 0; i < maskH; i++) {
			for (int k = 0; k < maskW; k++) {
				m_outImage[i][k] = (unsigned char)((4. - n) / 4 * m_inImage[i][k] + n / 4. * mask[i][k]);
			}
		}
	}
	OnFree2D(mask, maskH);
}


void CGreyScaleImageProcessingBeta1Doc::OnHistoStrectch()
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
	int high = m_inImage[0][0], low = m_inImage[0][0];
	for (int i = 1; i < m_inH; i++) {
		for (int k = 1; k < m_inW; k++) {
			if (m_inImage[i][k] < low)	low = m_inImage[i][k];
			if (m_inImage[i][k] > high)	high = m_inImage[i][k];
		}
	}

	// 입력 배열 --> 출력 배열
	int arrive, old;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			old = m_inImage[i][k];
			arrive = (int)(((double)old - low) / (high - low) * 255.0);

			if (arrive > 255)	arrive = 255;
			if (arrive < 0)	arrive = 0;
			m_outImage[i][k] = arrive;
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnEndIn()
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
	int high = m_inImage[0][0], low = m_inImage[0][0];
	for (int i = 1; i < m_inH; i++) {
		for (int k = 1; k < m_inW; k++) {
			if (m_inImage[i][k] < low)	low = m_inImage[i][k];
			if (m_inImage[i][k] > high)	high = m_inImage[i][k];
		}
	}
	high -= 50;
	low += 50;

	// 입력 배열 --> 출력 배열
	int arrive, old;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			old = m_inImage[i][k];
			arrive = (int)(((double)old - low) / (high - low) * 255.0);

			if (arrive > 255)	arrive = 255;
			if (arrive < 0)	arrive = 0;
			m_outImage[i][k] = arrive;
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnHistoFlatten()
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
	// 1) 빈도수 세기			histo[256]
	int histo[256] = { 0, };

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)
			histo[m_inImage[i][k]]++;

	// 2) 누적 히스토그램 생성
	int sumHisto[256] = { 0, };

	sumHisto[0] = histo[0];
	for (int i = 1; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];

	// 3) 정규화 히스토그램 생성
	// normalHisto = sumHisto*(1.0/(inH*inW))*255.0
	double normalHisto[256] = { 1.0, };
	for (int i = 1; i < 256; i++)
		normalHisto[i] = sumHisto[i] * (1.0 / (m_inH * m_inW)) * 255.0;

	// 4) 입력 배열을 치환 --> 출력 배열
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImage[i][k] = (unsigned char)normalHisto[m_inImage[i][k]];
		}
	}
}


void CGreyScaleImageProcessingBeta1Doc::OnEmboss()
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
	double mask[3][3] = { {-1.0, 0.0, 0.0},
						   {0.0, 0.0, 0.0},
						   {0.0, 0.0, 1.0} };

	// 임시메모리 할당(실수형으로)
	double** tmpInImage = mallocDoubleMemory(m_inH + 2, m_inW + 2);
	double** tmpOutImage = mallocDoubleMemory(m_outH, m_outW);

	// 임시 입력 메모리 127으로 초기화
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImage[i + 1][k + 1] = m_inImage[i][k];
		}
	}

	//  *****mask*****     회선 연산
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크와 한 점을 중심으로한 3*3 곱하기
			S = 0.0;	// 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	// 후처리 : 마스크의 합계에 따라
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++)
			tmpOutImage[i][k] += 127.0;

	// 임시출력영상 --> 출력영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				m_outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				m_outImage[i][k] = 0;
			else m_outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, m_inH + 2);
	freeDoubleMemory(tmpOutImage, m_inH);
}


void CGreyScaleImageProcessingBeta1Doc::OnBlur()
{
	// TODO: 여기에 구현 코드 추가.
	// 대화상자 띄우기 (취소 누를 거를 고려)
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 출력 배열 재로딩 
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	// 출력 이미지 크기 결정
	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	// 변경할 값 가져오기
	int scale = (int)dlg.m_constant;

	// 영상처리 알고리즘 
	double** mask;
	int lenScale = scale * 2 + 1;
	mask = mallocDoubleMemory(lenScale, lenScale);

	for (int i = 0; i < lenScale; i++) {
		for (int k = 0; k < lenScale; k++)
			mask[i][k] = 1. / pow(lenScale, 2);
	}

	// 임시메모리 할당(실수형으로)
	double** tmpInImage = mallocDoubleMemory(m_inH + scale * 2, m_inW + scale * 2);
	double** tmpOutImage = mallocDoubleMemory(m_outH, m_outW);

	// 임시 입력 메모리 127으로 초기화
	for (int i = 0; i < m_inH + 2 * scale; i++) {
		for (int k = 0; k < m_inW + 2 * scale; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImage[i + 1][k + 1] = m_inImage[i][k];
		}
	}

	//  *****mask*****     회선 연산
	double S = 0.0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// 마스크와 한 점을 중심으로한 3*3 곱하기
			S = 0.0;	// 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < lenScale; m++)
				for (int n = 0; n < lenScale; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// 후처리 : 마스크의 합계에 따라
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시출력영상 --> 출력영상
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				m_outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				m_outImage[i][k] = 0;
			else m_outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(mask, scale);
	freeDoubleMemory(tmpInImage, m_inH + scale * 2);
	freeDoubleMemory(tmpOutImage, m_inH);
}
