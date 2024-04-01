
// ColorImageProcessingDoc.cpp: CColorImageProcessingDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImageProcessing.h"
#endif

#include "ColorImageProcessingDoc.h"
#include "InIntDlg.h"
#include "ColorDlg.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColorImageProcessingDoc

IMPLEMENT_DYNCREATE(CColorImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CColorImageProcessingDoc, CDocument)
END_MESSAGE_MAP()


// CColorImageProcessingDoc 생성/소멸

CColorImageProcessingDoc::CColorImageProcessingDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CColorImageProcessingDoc::~CColorImageProcessingDoc()
{
}

BOOL CColorImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CColorImageProcessingDoc serialization

void CColorImageProcessingDoc::Serialize(CArchive& ar)
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
void CColorImageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CColorImageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CColorImageProcessingDoc::SetSearchContent(const CString& value)
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

// CColorImageProcessingDoc 진단

#ifdef _DEBUG
void CColorImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CColorImageProcessingDoc 명령


BOOL CColorImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	// 기존 메모리 해제
	if (m_inImageR != NULL) {
		OnFree2D(m_inImageR, m_inH);
		OnFree2D(m_inImageG, m_inH);
		OnFree2D(m_inImageB, m_inH);
		m_inImageR = m_inImageG = m_inImageB = NULL;
		m_inH = m_inW = 0;

		OnFree2D(m_outImageR, m_outH);
		OnFree2D(m_outImageG, m_outH);
		OnFree2D(m_outImageB, m_outH);
		m_outImageR = m_outImageG = m_outImageB = NULL;
		m_outH = m_outW = 0;
	}

	CImage image;
	image.Load(lpszPathName);

	//(중요) 입력 영상 크기 알아내기
	// 얘는 image에 다 있음
	m_inH = image.GetHeight();
	m_inW = image.GetWidth();

	// 메모리 할당
	m_inImageR = OnMalloc2D(m_inH, m_inW);
	m_inImageG = OnMalloc2D(m_inH, m_inW);
	m_inImageB = OnMalloc2D(m_inH, m_inW);

	// CImage의 객체값 --> 메모리
	COLORREF px;
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			px = image.GetPixel(k, i);
			m_inImageR[i][k] = GetRValue(px);
			m_inImageG[i][k] = GetGValue(px);
			m_inImageB[i][k] = GetBValue(px);
		}
	return TRUE;
}


BOOL CColorImageProcessingDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_outImageR == NULL)
		return FALSE;

	CImage image;
	image.Create(m_outW, m_outH, 32);

	unsigned char R, G, B;
	COLORREF px;

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			R = m_outImageR[i][k];
			G = m_outImageG[i][k];
			B = m_outImageB[i][k];
			px = RGB(R, G, B);

			image.SetPixel(k, i, px);
		}
	image.Save(lpszPathName, Gdiplus::ImageFormatPNG);
	MessageBox(NULL, L"저장", L"성공", NULL);

	return TRUE;
}


void CColorImageProcessingDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnFree2D(m_inImageR, m_inH);
	OnFree2D(m_inImageG, m_inH);
	OnFree2D(m_inImageB, m_inH);

	OnFree2D(m_outImageR, m_outH);
	OnFree2D(m_outImageG, m_outH);
	OnFree2D(m_outImageB, m_outH);

	CDocument::OnCloseDocument();
}


unsigned char** CColorImageProcessingDoc::OnMalloc2D(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** memory = new unsigned char* [h];

	for (int i = 0; i < h; i++)
		memory[i] = new unsigned char[w];

	return memory;
}


void CColorImageProcessingDoc::OnFree2D(unsigned char** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.
	if (memory == NULL)
		return;

	for (int i = 0; i < h; i++)
		delete memory[i];

	delete[] memory;
}


unsigned char CColorImageProcessingDoc::OnCheckValue(unsigned char value)
{
	// TODO: 여기에 구현 코드 추가.
	
	return 0;
}


unsigned char CColorImageProcessingDoc::OnLightCheck(int value)
{
	// TODO: 여기에 구현 코드 추가.
	if (value > 255)
		return 255;
	else if (value < 0)
		return 0;
	else 
		return value;
}


void CColorImageProcessingDoc::OnFreeOutImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_outImageR != NULL) {
		OnFree2D(m_outImageR, m_outH);
		OnFree2D(m_outImageG, m_outH);
		OnFree2D(m_outImageB, m_outH);
		m_outImageR = m_outImageG = m_outImageB = NULL;
		m_outH = m_outW = 0;
	}
}


double* CColorImageProcessingDoc::RGB2HSI(int R, int G, int B)
{
	// TODO: 여기에 구현 코드 추가.
	double H, S, I;
	double* HSI = new double[3];
	double min_value, angle;
	I = (R + G + B) / 3.0; // 밝기
	if ((R == G) && (G == B)) { // 그레이
		S = 0.0;
		H = 0.0;
	}
	else {

		min_value = min(min(R, G), B); //최소값 추출
		angle = (R - 0.5 * G - 0.5 * B) / (double)sqrt((R - G) * (R - G) + (R - B) * (G - B));

		H = (double)acos(angle) * 57.29577951;
		S = 1.0f - (3.0 / (R + G + B)) * min_value;
	}
	if (B > G) H = 360. - H;

	HSI[0] = H;
	HSI[1] = S;
	HSI[2] = I;

	return HSI;
}


unsigned char* CColorImageProcessingDoc::HSI2RGB(double H, double S, double I)
{
	// TODO: 여기에 구현 코드 추가.
	double R, G, B;
	unsigned char* RGB = new unsigned char[3];
	double angle1, angle2, scale;

	if (I == 0.0) { // Black
		RGB[0] = 0;
		RGB[1] = 0;
		RGB[2] = 0;
		return RGB;
	}

	if (H <= 0.0) H += 360.0f;

	scale = 3.0 * I;
	if (H <= 120.0)
	{
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		B = (1.0 - S) / 3.0f;
		R = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		G = 1.0 - R - B;
		B *= scale;
		R *= scale;
		G *= scale;
	}


	else if ((H > 120.0) && (H <= 240.0)) {
		H -= 120.0;
		angle1 = H * 0.017453293;

		angle2 = (60.0 - H) * 0.017453293;
		R = (1.0 - S) / 3.0;
		G = (double)(1.0f + (S * cos(angle1) / cos(angle2))) / 3.0;
		B = 1.0 - R - G;
		R *= scale;
		G *= scale;
		B *= scale;
	}
	else {
		H -= 240.0;
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		G = (1.0f - S) / 3.0;
		B = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		R = 1.0 - G - B;

		R *= scale;
		G *= scale;
		B *= scale;
	}

	RGB[0] = (unsigned char)R;
	RGB[1] = (unsigned char)G;
	RGB[2] = (unsigned char)B;

	return RGB;
}


void CColorImageProcessingDoc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[i][k];
			m_outImageG[i][k] = m_inImageG[i][k];
			m_outImageB[i][k] = m_inImageB[i][k];
		}
	}
}


void CColorImageProcessingDoc::OnGreyScale()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	double avg = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = 0;
			avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
			avg /= 3.0;
			m_outImageR[i][k] = (unsigned char)avg;
			m_outImageG[i][k] = (unsigned char)avg;
			m_outImageB[i][k] = (unsigned char)avg;
		}
	}
}


void CColorImageProcessingDoc::OnLightChange()
{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	int R, G, B = 0;

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			R = int(m_inImageR[i][k] + dlg.m_constant);
			G = int(m_inImageG[i][k] + dlg.m_constant);
			B = int(m_inImageB[i][k] + dlg.m_constant);

			m_outImageR[i][k] = OnLightCheck(R);
			m_outImageG[i][k] = OnLightCheck(G);
			m_outImageB[i][k] = OnLightCheck(B);
		}
	}
}


void CColorImageProcessingDoc::OnReverse()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = 255 - m_inImageR[i][k];
			m_outImageG[i][k] = 255 - m_inImageG[i][k];
			m_outImageB[i][k] = 255 - m_inImageB[i][k];
		}
	}
}


void CColorImageProcessingDoc::OnGammaChange()
{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	// 변경할 값 가져오기
	int scale = (int)dlg.m_constant;
	if (scale < 0)		scale = 0;
	if (scale > 100)	scale = 100;

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = int(255 * pow(m_inImageR[i][k] / 255., 30. / scale));
			m_outImageG[i][k] = int(255 * pow(m_inImageG[i][k] / 255., 30. / scale));
			m_outImageB[i][k] = int(255 * pow(m_inImageB[i][k] / 255., 30. / scale));
		}
	}
}


void CColorImageProcessingDoc::OnParaCap()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = int(255. * pow(((m_inImageR[i][k] / 127.) - 1), 2));
			m_outImageG[i][k] = int(255. * pow(((m_inImageG[i][k] / 127.) - 1), 2));
			m_outImageB[i][k] = int(255. * pow(((m_inImageB[i][k] / 127.) - 1), 2));
		}
	}
}


void CColorImageProcessingDoc::OnParaCup()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = 255 - int(255. * pow(((m_inImageR[i][k] / 127.) - 1), 2));
			m_outImageG[i][k] = 255 - int(255. * pow(((m_inImageG[i][k] / 127.) - 1), 2));
			m_outImageB[i][k] = 255 - int(255. * pow(((m_inImageB[i][k] / 127.) - 1), 2));
		}
	}
}


void CColorImageProcessingDoc::OnMono127()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	double avg = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = 0;
			avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
			avg /= 3.0;
			if (avg > 128) { 
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 0;
			}
		}
	}
}


void CColorImageProcessingDoc::OnMonoMid()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	double avg = 0;
	int cMid = 0, count = 0;
	int cValCount[256] = { 0, };

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = 0.0;
			avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
			avg /= 3.0;
			cValCount[(int)avg] += 1;
		}
	}

	for (int i = 0; i < 256; i++) {
		count += cValCount[i];
		if (count > (m_inW * m_inH) / 2) {
			cMid = i;
			break;
		}
	}

	{
		CString szTest;
		szTest.Format(_T("%d"), cMid);

		MessageBox(NULL, L"중간값: " + szTest, L"중간값", NULL);
	}

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = 0.0;
			avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
			avg /= 3.0;

			if (avg < cMid)
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 0;
			else
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 255;
		}
	}
}


void CColorImageProcessingDoc::OnMonoAvg()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	double avg = 0;
	long long fullAvg = 0;

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = 0.0;
			avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
			avg /= 3.0;
			fullAvg += (long long)avg;
		}
	}

	fullAvg = (long long)(fullAvg / (m_inH * m_inW));

	{
		CString szTest;
		szTest.Format(_T("%d"), (int)fullAvg);

		MessageBox(NULL, L"평균값: " + szTest, L"평균값", NULL);
	}

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = 0.0;
			avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
			avg /= 3.0;

			if (avg < fullAvg)
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 0;
			else
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 255;
		}
	}
}


void CColorImageProcessingDoc::OnMaskAnd()
{
	// TODO: 여기에 구현 코드 추가.
	CFileDialog openDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY);
	if (openDlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	CImage image;
	int maskH, maskW = 0;
	unsigned char **maskR, **maskG, **maskB = NULL;

	//(중요) 입력 영상 크기 알아내기
	// 얘는 image에 다 있음
	image.Load(openDlg.GetPathName());
	maskH = image.GetHeight();
	maskW = image.GetWidth();

	// 메모리 할당
	maskR = OnMalloc2D(maskH, maskW);
	maskG = OnMalloc2D(maskH, maskW);
	maskB = OnMalloc2D(maskH, maskW);

	// CImage의 객체값 --> 메모리
	COLORREF px;
	for (int i = 0; i < maskH; i++)
		for (int k = 0; k < maskW; k++) {
			px = image.GetPixel(k, i);
			maskR[i][k] = GetRValue(px);
			maskG[i][k] = GetGValue(px);
			maskB[i][k] = GetBValue(px);
		}

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (i >= maskH || k >= maskW){
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else {
				m_outImageR[i][k] = m_inImageR[i][k] & maskR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k] & maskG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k] & maskB[i][k];
			}
		}
	}

	OnFree2D(maskR, maskH);
	OnFree2D(maskG, maskH);
	OnFree2D(maskB, maskH);
}


void CColorImageProcessingDoc::OnMaskOr()
{
	// TODO: 여기에 구현 코드 추가.
	CFileDialog openDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY);
	if (openDlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	CImage image;
	int maskH, maskW = 0;
	unsigned char** maskR, ** maskG, ** maskB = NULL;

	//(중요) 입력 영상 크기 알아내기
	// 얘는 image에 다 있음
	image.Load(openDlg.GetPathName());
	maskH = image.GetHeight();
	maskW = image.GetWidth();

	// 메모리 할당
	maskR = OnMalloc2D(maskH, maskW);
	maskG = OnMalloc2D(maskH, maskW);
	maskB = OnMalloc2D(maskH, maskW);

	// CImage의 객체값 --> 메모리
	COLORREF px;
	for (int i = 0; i < maskH; i++)
		for (int k = 0; k < maskW; k++) {
			px = image.GetPixel(k, i);
			maskR[i][k] = GetRValue(px);
			maskG[i][k] = GetGValue(px);
			maskB[i][k] = GetBValue(px);
		}

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (i >= maskH || k >= maskW) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else {
				m_outImageR[i][k] = m_inImageR[i][k] | maskR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k] | maskG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k] | maskB[i][k];
			}
		}
	}

	OnFree2D(maskR, maskH);
	OnFree2D(maskG, maskH);
	OnFree2D(maskB, maskH);
}


void CColorImageProcessingDoc::OnMaskXor()
{
	// TODO: 여기에 구현 코드 추가.
	CFileDialog openDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY);
	if (openDlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	CImage image;
	int maskH, maskW = 0;
	unsigned char** maskR, ** maskG, ** maskB = NULL;

	//(중요) 입력 영상 크기 알아내기
	// 얘는 image에 다 있음
	image.Load(openDlg.GetPathName());
	maskH = image.GetHeight();
	maskW = image.GetWidth();

	// 메모리 할당
	maskR = OnMalloc2D(maskH, maskW);
	maskG = OnMalloc2D(maskH, maskW);
	maskB = OnMalloc2D(maskH, maskW);

	// CImage의 객체값 --> 메모리
	COLORREF px;
	for (int i = 0; i < maskH; i++)
		for (int k = 0; k < maskW; k++) {
			px = image.GetPixel(k, i);
			maskR[i][k] = GetRValue(px);
			maskG[i][k] = GetGValue(px);
			maskB[i][k] = GetBValue(px);
		}

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (i >= maskH || k >= maskW) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else {
				m_outImageR[i][k] = m_inImageR[i][k] ^ maskR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k] ^ maskG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k] ^ maskB[i][k];
			}
		}
	}

	OnFree2D(maskR, maskH);
	OnFree2D(maskG, maskH);
	OnFree2D(maskB, maskH);
}


void CColorImageProcessingDoc::OnMaskNot()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = ~m_inImageR[i][k];
			m_outImageG[i][k] = ~m_inImageG[i][k];
			m_outImageB[i][k] = ~m_inImageB[i][k];
		}
	}
}


void CColorImageProcessingDoc::OnConstractStretch()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (m_inImageR[i][k] < 50)			m_outImageR[i][k] = 0;
			else if (m_inImageR[i][k] > 200)	m_outImageR[i][k] = 255;
			else								m_outImageR[i][k] = m_inImageR[i][k] * 51 / 30 - 85;

			if (m_inImageG[i][k] < 50)			m_outImageG[i][k] = 0;
			else if (m_inImageG[i][k] > 200)	m_outImageG[i][k] = 255;
			else								m_outImageG[i][k] = m_inImageG[i][k] * 51 / 30 - 85;

			if (m_inImageB[i][k] < 50)			m_outImageB[i][k] = 0;
			else if (m_inImageB[i][k] > 200)	m_outImageB[i][k] = 255;
			else								m_outImageB[i][k] = m_inImageB[i][k] * 51 / 30 - 85;
		}
	}
}


void CColorImageProcessingDoc::OnConstractZip()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[i][k] * 30 / 51 + 50;
			m_outImageG[i][k] = m_inImageG[i][k] * 30 / 51 + 50;
			m_outImageB[i][k] = m_inImageB[i][k] * 30 / 51 + 50;
		}
	}
}


void CColorImageProcessingDoc::OnPosterize()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			switch (m_inImageR[i][k] / 32) {
			case 0: m_outImageR[i][k] = 0;	break;
			case 1: m_outImageR[i][k] = 32;	break;
			case 2: m_outImageR[i][k] = 64;	break;
			case 3: m_outImageR[i][k] = 96;	break;
			case 4: m_outImageR[i][k] = 128;	break;
			case 5: m_outImageR[i][k] = 160;	break;
			case 6: m_outImageR[i][k] = 192;	break;
			case 7: m_outImageR[i][k] = 224;	break;
			case 8: m_outImageR[i][k] = 255;	break;
			}

			switch (m_inImageG[i][k] / 32) {
			case 0: m_outImageG[i][k] = 0;	break;
			case 1: m_outImageG[i][k] = 32;	break;
			case 2: m_outImageG[i][k] = 64;	break;
			case 3: m_outImageG[i][k] = 96;	break;
			case 4: m_outImageG[i][k] = 128;	break;
			case 5: m_outImageG[i][k] = 160;	break;
			case 6: m_outImageG[i][k] = 192;	break;
			case 7: m_outImageG[i][k] = 224;	break;
			case 8: m_outImageG[i][k] = 255;	break;
			}

			switch (m_inImageB[i][k] / 32) {
			case 0: m_outImageB[i][k] = 0;	break;
			case 1: m_outImageB[i][k] = 32;	break;
			case 2: m_outImageB[i][k] = 64;	break;
			case 3: m_outImageB[i][k] = 96;	break;
			case 4: m_outImageB[i][k] = 128;	break;
			case 5: m_outImageB[i][k] = 160;	break;
			case 6: m_outImageB[i][k] = 192;	break;
			case 7: m_outImageB[i][k] = 224;	break;
			case 8: m_outImageB[i][k] = 255;	break;
			}
		}
	}
}


void CColorImageProcessingDoc::OnAreaStress()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	double avg = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = 0;
			avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
			avg /= 3.0;

			if (avg > 75 && avg < 175) {
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 255;
			}
			else {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
		}
	}
}


void CColorImageProcessingDoc::OnChangeSatur()
{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// HSI 모델의 값
			// H(색상) : 0~360
			// S(채도) : 0.0 ~ 1.0
			// I(명도) : 0 ~ 255
			double H, S, I;
			unsigned char R, G, B;

			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			// RGB --> HSI
			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];

			// 채도(S) 흐리게
			S -= dlg.m_constant;
			if (S < 0)	S = 0.0;
			if (S > 1)	S = 1.0;

			// HSI --> RGB
			unsigned char* rgb = HSI2RGB(H, S, I);
			R = rgb[0];
			G = rgb[1];
			B = rgb[2];

			m_outImageR[i][k] = R;
			m_outImageG[i][k] = G;
			m_outImageB[i][k] = B;
		}
	}
}


void CColorImageProcessingDoc::OnPickOrange()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// HSI 모델의 값
			// H(색상) : 0~360
			// S(채도) : 0.0 ~ 1.0
			// I(명도) : 0 ~ 255
			double H, S, I;
			unsigned char R, G, B;

			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			// RGB --> HSI
			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];

			// 오렌지 추출
			double avg = 0;

			if (H >= 8 && H <= 20) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else {
				avg = 0;
				avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
				avg /= 3.0;
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avg;
			}
		}
	}
}


void CColorImageProcessingDoc::OnColorImpact()
{
	// TODO: 여기에 구현 코드 추가.
	CColorDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// HSI 모델의 값
			// H(색상) : 0~360
			// S(채도) : 0.0 ~ 1.0
			// I(명도) : 0 ~ 255
			double H, S, I;
			unsigned char R, G, B;

			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			// RGB --> HSI
			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];
			S = hsi[1];
			I = hsi[2];

			// 각 색 추출
			double avg = 0;
			if (dlg.m_red && (H >= 330 || H <= 30)) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else if (dlg.m_orange && H >= 10 && H <= 60) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else if (dlg.m_yellow && H >= 30 && H <= 90) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else if (dlg.m_green && H >= 60 && H <= 165) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else if (dlg.m_blue && H >= 165 && H <= 260) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else if (dlg.m_purple && H >= 260 && H <= 330) {
				m_outImageR[i][k] = m_inImageR[i][k];
				m_outImageG[i][k] = m_inImageG[i][k];
				m_outImageB[i][k] = m_inImageB[i][k];
			}
			else {
				avg = 0;
				avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
				avg /= 3.0;
				m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avg;
			}
		}
	}
}


void CColorImageProcessingDoc::OnIntensityChange()
{
	//// TODO: 여기에 구현 코드 추가.
	//CInIntDlg dlg;
	//if (dlg.DoModal() != IDOK)
	//	return;

	//// 기존 메모리 해제
	//OnFreeOutImage();

	//// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	//m_outH = m_inH;
	//m_outW = m_inW;

	//m_outImageR = OnMalloc2D(m_outH, m_outW);
	//m_outImageG = OnMalloc2D(m_outH, m_outW);
	//m_outImageB = OnMalloc2D(m_outH, m_outW);

	//// 진짜 영상처리 알고리즘
	//for (int i = 0; i < m_inH; i++) {
	//	for (int k = 0; k < m_inW; k++) {
	//		// HSI 모델의 값
	//		// H(색상) : 0~360
	//		// S(채도) : 0.0 ~ 1.0
	//		// I(명도) : 0 ~ 255
	//		double H, S, I;
	//		unsigned char R, G, B;

	//		R = m_inImageR[i][k];
	//		G = m_inImageG[i][k];
	//		B = m_inImageB[i][k];

	//		// RGB --> HSI
	//		double* hsi = RGB2HSI(R, G, B);
	//		H = hsi[0];
	//		S = hsi[1];
	//		I = hsi[2];

	//		// 명도(I) 변경
	//		I += dlg.m_constant;
	//		if (I < 0)	I = 0.0;
	//		if (I > 255)I = 255.0;

	//		// HSI --> RGB
	//		unsigned char* rgb = HSI2RGB(H, S, I);
	//		R = rgb[0];
	//		G = rgb[1];
	//		B = rgb[2];

	//		m_outImageR[i][k] = R;
	//		m_outImageG[i][k] = G;
	//		m_outImageB[i][k] = B;
	//	}
	//}
}
