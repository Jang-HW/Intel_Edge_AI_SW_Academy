
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
#include "ColorImageProcessingView.h"
#include "InIntDlg.h"
#include "ColorDlg.h"
#include "StringDlg.h"

#include <propkey.h>
#include <cmath>

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
	if (m_outImageR == NULL)	{
		MessageBox(NULL, L"이미지를 편집하고 저장해주세요.", L"저장 실패", NULL);
		return FALSE;
	}

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
	CString saveMessage;
	saveMessage.Format(_T("%s"), lpszPathName);
	saveMessage += L"으로 저장하였습니다.";
	MessageBox(NULL, saveMessage, L"저장 성공", NULL);

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


double** CColorImageProcessingDoc::OnMalloc2D_double(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	double** memory = new double* [h];

	for (int i = 0; i < h; i++)
		memory[i] = new double[w];

	return memory;
}


unsigned int** CColorImageProcessingDoc::OnMalloc2D_int(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned int** memory = new unsigned int* [h];

	for (int i = 0; i < h; i++)
		memory[i] = new unsigned int[w];

	return memory;
}


template <typename T>
void CColorImageProcessingDoc::OnFree2D(T** memory, int h)
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
	dlg.m_question.SetString(L"밝기를 얼마나 바꿀까요?\n(-255 ~ 255)");
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
	dlg.m_question.SetString(L"감마값을 얼마나 바꿀까요?\n(0 ~ 100)");
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
	dlg.m_question.SetString(L"얼마나 흐리게 바꿀까요?\n(0.0 ~ 1.0 / 소수점 3자리까지)");
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
	double satur = ceil(dlg.m_constant * 1000) / 1000;

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
			S -= satur;
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

	//		// 오렌지 추출
	//		double avg = 0;

	//		if (H >= 8 && H <= 20) {
	//			m_outImageR[i][k] = m_inImageR[i][k];
	//			m_outImageG[i][k] = m_inImageG[i][k];
	//			m_outImageB[i][k] = m_inImageB[i][k];
	//		}
	//		else {
	//			avg = 0;
	//			avg = m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k];
	//			avg /= 3.0;
	//			m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avg;
	//		}
	//	}
	//}
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
	//dlg.m_question.SetString(L"얼마나 밝기를 바꿀까요?\n(0 ~ 255)");
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
	//		// H(색상) : 0 ~ 360
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
	//		if (I < 0)		I = 0.0;
	//		if (I > 255.0)	I = 255.0;
	//		I = dlg.m_constant;

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


void CColorImageProcessingDoc::OnZoomIn()
{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	dlg.m_question.SetString(L"몇배나 확대할까요?\n(1 ~ 10)");
	if (dlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 변경할 값 가져오기
	int scale = (int)dlg.m_constant;
	if (scale < 0)		scale = 0;
	if (scale > 10)		scale = 10;

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH * scale;
	m_outW = m_inW * scale;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i * scale][k * scale] = m_inImageR[i][k];
			m_outImageG[i * scale][k * scale] = m_inImageG[i][k];
			m_outImageB[i * scale][k * scale] = m_inImageB[i][k];
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
				m_outImageR[i][k] = 255;
				m_outImageG[i][k] = 255;
				m_outImageB[i][k] = 255;
				// 현재 칸이 보간을 진행할 수 없다면 흰색으로 지정
				// 주로 왼쪽과 아래쪽이 그런 경향이 있음
			}
			else {
				C1 = (double)m_inImageR[i_h][i_w];				// 좌 상단점 지정
				C2 = (double)m_inImageR[i_h][i_w + 1];			// 우 상단점 지정
				C3 = (double)m_inImageR[i_h + 1][i_w + 1];		// 우 하단점 지정
				C4 = (double)m_inImageR[i_h + 1][i_w];			// 좌 하단점 지정
				m_outImageR[i][k] = (unsigned char)(C1 * (1 - s_H) * (1 - s_W)
					+ C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);

				C1 = (double)m_inImageG[i_h][i_w];				// 좌 상단점 지정
				C2 = (double)m_inImageG[i_h][i_w + 1];			// 우 상단점 지정
				C3 = (double)m_inImageG[i_h + 1][i_w + 1];		// 우 하단점 지정
				C4 = (double)m_inImageG[i_h + 1][i_w];			// 좌 하단점 지정
				m_outImageG[i][k] = (unsigned char)(C1 * (1 - s_H) * (1 - s_W)
					+ C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);

				C1 = (double)m_inImageB[i_h][i_w];				// 좌 상단점 지정
				C2 = (double)m_inImageB[i_h][i_w + 1];			// 우 상단점 지정
				C3 = (double)m_inImageB[i_h + 1][i_w + 1];		// 우 하단점 지정
				C4 = (double)m_inImageB[i_h + 1][i_w];			// 좌 하단점 지정
				m_outImageB[i][k] = (unsigned char)(C1 * (1 - s_H) * (1 - s_W)
					+ C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				// 각 점들과의 위치 비율을 반영해서 점의 색 지정 
			}

		}
	}
}


void CColorImageProcessingDoc::OnZoomOut()
{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	dlg.m_question.SetString(L"몇 배나 줄일까요?\n(0 ~ 100)");
	if (dlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

	// 변경할 값 가져오기
	int scale = (int)dlg.m_constant;
	if (scale < 0)		scale = 0;
	if (scale > 100)	scale = 100;

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH / scale;
	m_outW = m_inW / scale;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	unsigned int** tmpR = OnMalloc2D_int(m_outH, m_outW);
	unsigned int** tmpG = OnMalloc2D_int(m_outH, m_outW);
	unsigned int** tmpB = OnMalloc2D_int(m_outH, m_outW);

	// 임시배열 초기화
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			tmpR[i][k] = 0;
			tmpG[i][k] = 0;
			tmpB[i][k] = 0;
		}
	}

	// 입력 배열 --> 임시 배열
	for (int i = 0; i < m_inH; i++) {
		if (m_outH <= i / scale) break;

		for (int k = 0; k < m_inW; k++) {
			if (m_outW <= k / scale) break;
			tmpR[(int)(i / scale)][(int)(k / scale)] += m_inImageR[i][k];
			tmpG[(int)(i / scale)][(int)(k / scale)] += m_inImageG[i][k];
			tmpB[(int)(i / scale)][(int)(k / scale)] += m_inImageB[i][k];
		}
	}
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = tmpR[i][k] / (scale * scale);
			m_outImageG[i][k] = tmpG[i][k] / (scale * scale);
			m_outImageB[i][k] = tmpB[i][k] / (scale * scale);
		}
	}

	OnFree2D(tmpR, m_outH);
	OnFree2D(tmpG, m_outH);
	OnFree2D(tmpB, m_outH);
}


void CColorImageProcessingDoc::OnRotateOrigin()

{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	dlg.m_question.SetString(L"얼마나 돌릴까요?\n(-360 ~ 360)");
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
	if (scale < -360.0)	{
		scale = abs(scale);
		scale %= 360;
		scale = 360 - scale;
	}
	if (scale > 360)	scale %= 360;

	double radian = scale * 3.1415925386 / 180.0;

	//// xd = cos*xs - sin*ys
	//// yd = sin*xs + cos*ys
	int cx = m_inH / 2;
	int cy = m_inW / 2;

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			int xd = i;
			int yd = k;

			int	xs = (int)(cos(radian) * (xd - (m_outH / 2)) + sin(radian) * (yd - (m_outW / 2)));
			int ys = (int)(-sin(radian) * (xd - (m_outH / 2)) + cos(radian) * (yd - (m_outW / 2)));
			xs += cx;
			ys += cy;

			if (0 <= xs && xs < m_inH && 0 <= ys && ys < m_inW)	{
				m_outImageR[xd][yd] = m_inImageR[xs][ys];
				m_outImageG[xd][yd] = m_inImageG[xs][ys];
				m_outImageB[xd][yd] = m_inImageB[xs][ys];
			}
		}
	}
}


void CColorImageProcessingDoc::OnRotateNoCut()

{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	dlg.m_question.SetString(L"얼마나 돌릴까요?\n(-360 ~ 360)");
	if (dlg.DoModal() != IDOK)
		return;

	// 기존 메모리 해제
	OnFreeOutImage();

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
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);


	// 진짜 영상처리 알고리즘
	//// xd = cos*xs - sin*ys
	//// yd = sin*xs + cos*ys
	int cx = m_inH / 2;
	int cy = m_inW / 2;

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			int xd = i;
			int yd = k;
			int	xs = (int)(cos(radian) * (xd - (m_outH / 2)) + sin(radian) * (yd - (m_outW / 2)));
			int ys = (int)(-sin(radian) * (xd - (m_outH / 2)) + cos(radian) * (yd - (m_outW / 2)));

			xs += cx;
			ys += cy;

			if (0 <= xs && xs < m_inH && 0 <= ys && ys < m_inW) {
				m_outImageR[i][k] = m_inImageR[xs][ys];
				m_outImageG[i][k] = m_inImageG[xs][ys];
				m_outImageB[i][k] = m_inImageB[xs][ys];
			}
		}
	}
}


void CColorImageProcessingDoc::OnMoveHorizon()
{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	CString question;
	question.Format(_T("가로로 얼마나 이동할까요? \n(-%d ~ %d)"), m_inW, m_inW);
	dlg.m_question.SetString(question);
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
	int degree = (int)dlg.m_constant;

	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (k + degree >= m_outW || k + degree < 0)		continue;
			m_outImageR[i][k + degree] = m_inImageR[i][k];
			m_outImageG[i][k + degree] = m_inImageG[i][k];
			m_outImageB[i][k + degree] = m_inImageB[i][k];
		}
	}
}


void CColorImageProcessingDoc::OnMoveVertical()
{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	CString question;
	question.Format(_T("세로로 얼마나 이동할까요? \n(-%d ~ %d)"), m_inH, m_inH);
	dlg.m_question.SetString(question);
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
	int degree = (int)dlg.m_constant;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)	{
			if (i + degree >= m_outH || i + degree < 0)	break;
			m_outImageR[i + degree][k] = m_inImageR[i][k];
			m_outImageG[i + degree][k] = m_inImageG[i][k];
			m_outImageB[i + degree][k] = m_inImageB[i][k];
		}
}


void CColorImageProcessingDoc::OnMirrorHorizon()
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
			m_outImageR[i][m_inW - k - 1] = m_inImageR[i][k];
			m_outImageG[i][m_inW - k - 1] = m_inImageG[i][k];
			m_outImageB[i][m_inW - k - 1] = m_inImageB[i][k];
		}
	}
}


void CColorImageProcessingDoc::OnMirrorVertical()

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
			m_outImageR[m_inH - i - 1][k] = m_inImageR[i][k];
			m_outImageG[m_inH - i - 1][k] = m_inImageG[i][k];
			m_outImageB[m_inH - i - 1][k] = m_inImageB[i][k];
		}
	}
}


void CColorImageProcessingDoc::OnMorph()
{
	// TODO: 여기에 구현 코드 추가.
	CFileDialog openDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY);
	if (openDlg.DoModal() != IDOK)
		return;

	CInIntDlg dlg;
	dlg.m_question.SetString(L"모핑을 몇 단계로 할까요?\n(2 ~ 100)");
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

	// 모핑할 이미지 가져오기
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

	// view 클래스의 포인터 생성 
	POSITION pos = GetFirstViewPosition();
	CColorImageProcessingView* pView = (CColorImageProcessingView*)GetNextView(pos);

	// 영상처리 알고리즘
	int scale = dlg.m_constant;
	if (scale < 2)		scale = 2;
	if (scale > 100)	scale = 100;

	int R, G, B;
	for (int n = 0; n < scale; n++) {
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				m_outImageR[i][k] = (unsigned char)(((double)(scale - 1) - n) / (scale - 1) * m_inImageR[i][k]);
				m_outImageG[i][k] = (unsigned char)(((double)(scale - 1) - n) / (scale - 1) * m_inImageG[i][k]);
				m_outImageB[i][k] = (unsigned char)(((double)(scale - 1) - n) / (scale - 1) * m_inImageB[i][k]);
			}
		}
		for (int i = 0; i < m_inH; i++) {
			if (i >= maskH)		continue;
			for (int k = 0; k < m_inW; k++) {
				if (k >= maskW)		break;
				R = (unsigned char)(((double)(scale - 1) - n) / (scale - 1) * m_inImageR[i][k] + ((double)n / (scale - 1)) * maskR[i][k]);
				if (R <= 255 && R >= 0)			m_outImageR[i][k] = R;
				else if(R>255)					m_outImageR[i][k] = 255;
				else							m_outImageR[i][k] = 0;

				G = (unsigned char)(((double)(scale - 1) - n) / (scale - 1) * m_inImageG[i][k] + ((double)n / (scale - 1)) * maskG[i][k]);
				if (G <= 255 && G >= 0)			m_outImageG[i][k] = G;
				else if (G > 255)				m_outImageG[i][k] = 255;
				else							m_outImageG[i][k] = 0;

				B = (unsigned char)(((double)(scale - 1) - n) / (scale - 1) * m_inImageB[i][k] + ((double)n / (scale - 1)) * maskB[i][k]);
				if (B <= 255 && B >= 0)			m_outImageB[i][k] = B;
				else if (B > 255)				m_outImageB[i][k] = 255;
				else							m_outImageB[i][k] = 0;
			}
		}
		pView->OnInvalidate();
		CString szTest;
		szTest.Format(_T("%d / %d"), n+1, scale);
		MessageBox(NULL, L"모핑 단계: " + szTest, L"모핑단계", NULL);
	}

	OnFree2D(maskR, maskH);
	OnFree2D(maskG, maskH);
	OnFree2D(maskB, maskH);
}


void CColorImageProcessingDoc::OnHistoStretch()
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
	int high = m_inImageR[0][0], low = m_inImageR[0][0];
	for (int i = 1; i < m_inH; i++) {
		for (int k = 1; k < m_inW; k++) {
			if (m_inImageR[i][k] < low)		low = m_inImageR[i][k];
			if (m_inImageR[i][k] > high)	high = m_inImageR[i][k];
		}
	}
	// 입력 배열 --> 출력 배열
	int arrive, old;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			old = m_inImageR[i][k];
			arrive = (int)(((double)old - low) / (high - low) * 255.0);

			if (arrive > 255)	arrive = 255;
			if (arrive < 0)	arrive = 0;
			m_outImageR[i][k] = arrive;
		}
	}
	CString szTest;
	szTest.Format(_T("%d / %d"), high, low);
	MessageBox(NULL, L"스트레치 R: " + szTest, L"스트레치", NULL);

	high = m_inImageG[0][0], low = m_inImageG[0][0];
	for (int i = 1; i < m_inH; i++) {
		for (int k = 1; k < m_inW; k++) {
			if (m_inImageG[i][k] < low)		low = m_inImageG[i][k];
			if (m_inImageG[i][k] > high)	high = m_inImageG[i][k];
		}
	}
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			old = m_inImageG[i][k];
			arrive = (int)(((double)old - low) / (high - low) * 255.0);

			if (arrive > 255)	arrive = 255;
			if (arrive < 0)	arrive = 0;
			m_outImageG[i][k] = arrive;
		}
	}
	szTest.Format(_T("%d / %d"), high, low);
	MessageBox(NULL, L"스트레치 G: " + szTest, L"스트레치", NULL);

	high = m_inImageB[0][0], low = m_inImageB[0][0];
	for (int i = 1; i < m_inH; i++) {
		for (int k = 1; k < m_inW; k++) {
			if (m_inImageB[i][k] < low)		low = m_inImageB[i][k];
			if (m_inImageB[i][k] > high)	high = m_inImageB[i][k];
		}
	}
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			old = m_inImageB[i][k];
			arrive = (int)(((double)old - low) / (high - low) * 255.0);

			if (arrive > 255)	arrive = 255;
			if (arrive < 0)	arrive = 0;
			m_outImageB[i][k] = arrive;
		}
	}
	szTest.Format(_T("%d / %d"), high, low);
	MessageBox(NULL, L"스트레치 B: " + szTest, L"스트레치", NULL);
}


void CColorImageProcessingDoc::OnEmbos()
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
	// 엠보싱 마스크
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {
		{-1.0, 0.0, 0.0},
		{ 0.0, 0.0, 0.0},
		{ 0.0, 0.0, 1.0},
	};

	// 임시메모리 할당
	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB;
	double** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	// 임시 배열 초기화
	for (int i = 0; i < m_inH + 2; i++) {
		for (int k = 0; k < m_inW + 2; k++) {
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;
		}
	}

	// 입력 이미지 --> 임시 메모리
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}
	}

	// 회선 연산: 마스크로 긁어가며 계산하기
	double S = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S = 0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageR[i + m][k + n] * mask[m][n];
			tmpOutImageR[i][k] = S;

			S = 0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageG[i + m][k + n] * mask[m][n];
			tmpOutImageG[i][k] = S;

			S = 0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageB[i + m][k + n] * mask[m][n];
			tmpOutImageB[i][k] = S;
		}

		// 후처리: 마스크의 합계에 따라 127을 더할지 결정
		for (int i = 0; i < m_inH; i++) {
			for (int k = 0; k < m_inW; k++) {
				tmpOutImageR[i][k] += 127;
				tmpOutImageG[i][k] += 127;
				tmpOutImageB[i][k] += 127;
			}
		}

		// 임시 출력 메모리 --> 출력 이미지
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				if (tmpOutImageR[i][k] < 0.0)			m_outImageR[i][k] = 0;
				else if (tmpOutImageR[i][k] < 255.0)	m_outImageR[i][k] = 255;
				else									m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

				if (tmpOutImageG[i][k] < 0.0)			m_outImageG[i][k] = 0;
				else if (tmpOutImageG[i][k] < 255.0)	m_outImageG[i][k] = 255;
				else									m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

				if (tmpOutImageB[i][k] < 0.0)			m_outImageB[i][k] = 0;
				else if (tmpOutImageB[i][k] < 255.0)	m_outImageB[i][k] = 255;
				else									m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];
			}
		}
	}

	OnFree2D(tmpInImageR, m_inH + 2);
	OnFree2D(tmpInImageG, m_inH + 2);
	OnFree2D(tmpInImageB, m_inH + 2);
	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnEmboss()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();
	// 중요! 출력 이미지 크기 결정 --> 알고리즘에 따름...
	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
	// ** 진짜 영상처리 알고리즘 **
	const int MSIZE = 3;
	double mask[MSIZE][MSIZE] = {  // 엠보싱 마스크
		{ -1.0, 0.0, 0.0 },
		{  0.0, 0.0, 0.0 },
		{  0.0, 0.0, 1.0 } };

	// 임시 메모리 할당
	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	double** tmpInImageH, ** tmpInImageS, ** tmpInImageI;
	tmpInImageR = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageG = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageB = OnMalloc2D_double(m_inH + 2, m_inW + 2);

	tmpInImageH = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageS = OnMalloc2D_double(m_inH + 2, m_inW + 2);
	tmpInImageI = OnMalloc2D_double(m_inH + 2, m_inW + 2);

	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	// 임시 메모리 초기화 (127)
	for (int i = 0; i < m_inH + 2; i++)
		for (int k = 0; k < m_inW + 2; k++)
			tmpInImageR[i][k] = tmpInImageG[i][k] = tmpInImageB[i][k] = 127.0;

	// 입력메모리 --> 임시입력메모리
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + 1][k + 1] = m_inImageR[i][k];
			tmpInImageG[i + 1][k + 1] = m_inImageG[i][k];
			tmpInImageB[i + 1][k + 1] = m_inImageB[i][k];
		}

	///////// RGB 모델 --> HSI 모델 ///////////////
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = (unsigned char)tmpInImageR[i][k];
			G = (unsigned char)tmpInImageG[i][k];
			B = (unsigned char)tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0]; S = hsi[1]; I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
		}
	}
	// *** 회선 연산 : 마스크로 긁어가면서 계산하기.	
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double S_VALUE = 0.0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S_VALUE += tmpInImageI[i + m][k + n] * mask[m][n];
			tmpInImageI[i][k] = S_VALUE;
		}
	}

	// 후처리 (마스크의 합계에 따라서 127을 더할지 결정)
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			tmpInImageI[i][k] += 127;
		}

	////// HSI --> RGB ////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k]; S = tmpInImageS[i][k]; I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);
			tmpOutImageR[i][k] = rgb[0]; tmpOutImageG[i][k] = rgb[1]; tmpOutImageB[i][k] = rgb[2];
		}


	// 임시 출력 이미지 ---> 출력 이미지
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}

	// 임시 메모리 해제
	OnFree2D(tmpInImageR, m_inH + 2);
	OnFree2D(tmpInImageG, m_inH + 2);
	OnFree2D(tmpInImageB, m_inH + 2);
	OnFree2D(tmpInImageH, m_inH + 2);
	OnFree2D(tmpInImageS, m_inH + 2);
	OnFree2D(tmpInImageI, m_inH + 2);
	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);
}


void CColorImageProcessingDoc::OnFlat()
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
	// 1) 빈도수 세기			histo[256]
	int histo[256] = { 0, };

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)
			histo[m_inImageR[i][k]]++;

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
			m_outImageR[i][k] = (unsigned char)normalHisto[m_inImageR[i][k]];
		}
	}


	// 1) 빈도수 세기			histo[256]
	for (int i = 0; i < 256; i++)
		histo[i] = 0;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)
			histo[m_inImageG[i][k]]++;

	// 2) 누적 히스토그램 생성
	for (int i = 0; i < 256; i++)
		sumHisto[i] = 0;

	sumHisto[0] = histo[0];
	for (int i = 1; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];

	// 3) 정규화 히스토그램 생성
	// normalHisto = sumHisto*(1.0/(inH*inW))*255.0
	for (int i = 0; i < 256; i++)
		normalHisto[i] = 1.0;
	for (int i = 1; i < 256; i++)
		normalHisto[i] = sumHisto[i] * (1.0 / (m_inH * m_inW)) * 255.0;

	// 4) 입력 배열을 치환 --> 출력 배열
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageG[i][k] = (unsigned char)normalHisto[m_inImageG[i][k]];
		}
	}


	// 1) 빈도수 세기			histo[256]
	for (int i = 0; i < 256; i++)
		histo[i] = 0;

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)
			histo[m_inImageB[i][k]]++;

	// 2) 누적 히스토그램 생성
	for (int i = 0; i < 256; i++)
		sumHisto[i] = 0;

	sumHisto[0] = histo[0];
	for (int i = 1; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];

	// 3) 정규화 히스토그램 생성
	// normalHisto = sumHisto*(1.0/(inH*inW))*255.0
	for (int i = 0; i < 256; i++)
		normalHisto[i] = 1.0;
	for (int i = 1; i < 256; i++)
		normalHisto[i] = sumHisto[i] * (1.0 / (m_inH * m_inW)) * 255.0;

	// 4) 입력 배열을 치환 --> 출력 배열
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageB[i][k] = (unsigned char)normalHisto[m_inImageB[i][k]];
		}
	}
}


void CColorImageProcessingDoc::OnHistoStrechh()
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
	// 입력 배열 --> 임시 입력 배열
	double** tmpInImageH, ** tmpInImageS, ** tmpInImageI;
	tmpInImageH = OnMalloc2D_double(m_inH, m_inW);
	tmpInImageS = OnMalloc2D_double(m_inH, m_inW);
	tmpInImageI = OnMalloc2D_double(m_inH, m_inW);

	///////// RGB 모델 --> HSI 모델 ///////////////
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = (unsigned char)m_inImageR[i][k];
			G = (unsigned char)m_inImageG[i][k];
			B = (unsigned char)m_inImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0]; S = hsi[1]; I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
		}
	}

	int high = tmpInImageI[0][0], low = tmpInImageI[0][0];
	for (int i = 1; i < m_inH; i++) {
		for (int k = 1; k < m_inW; k++) {
			if (tmpInImageI[i][k] < low)	low = tmpInImageI[i][k];
			if (tmpInImageI[i][k] > high)	high = tmpInImageI[i][k];
		}
	}

	int arrive, old;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			old = tmpInImageI[i][k];
			arrive = (int)(((double)old - low) / (high - low) * 255.0);

			if (arrive > 255)	arrive = 255;
			if (arrive < 0)	arrive = 0;
			m_outImageR[i][k] = arrive;
		}
	}

	////// HSI --> RGB ////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k]; 
			S = tmpInImageS[i][k]; 
			I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);
			m_outImageR[i][k] = rgb[0]; 
			m_outImageG[i][k] = rgb[1];
			m_outImageB[i][k] = rgb[2];
		}
}


void CColorImageProcessingDoc::OnFlatt()
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

	double** tmpInImageH, ** tmpInImageS, ** tmpInImageI;
	tmpInImageH = OnMalloc2D_double(m_inH, m_inW);
	tmpInImageS = OnMalloc2D_double(m_inH, m_inW);
	tmpInImageI = OnMalloc2D_double(m_inH, m_inW);

	///////// RGB 모델 --> HSI 모델 ///////////////
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = (unsigned char)m_inImageR[i][k];
			G = (unsigned char)m_inImageG[i][k];
			B = (unsigned char)m_inImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0]; S = hsi[1]; I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
		}
	}

	// 진짜 영상처리 알고리즘
	// 1) 빈도수 세기			histo[256]
	int histo[256] = { 0, };

	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++)
			histo[int(tmpInImageI[i][k])]++;

	// 2) 누적 히스토그램 생성
	int sumHisto[256] = { 0, };

	sumHisto[0] = histo[0];
	for (int i = 1; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];

	// 3) 정규화 히스토그램 생성
	// normalHisto = sumHisto*(1.0/(inH*inW))*255.0
	double normalHisto[256] = { 1.0, };
	for (int i = 1; i < 256; i++)
		normalHisto[i] = sumHisto[i] * (1.0 / (m_inH * m_inW)) * 256;

	// 4) 입력 배열을 치환 --> 출력 배열
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageI[i][k] = (unsigned char)normalHisto[int(tmpInImageI[i][k])];
		}
	}

	////// HSI --> RGB ////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i][k]; S = tmpInImageS[i][k]; I = tmpInImageI[i][k];

			rgb = HSI2RGB(H, S, I);
			m_outImageR[i][k] = rgb[0]; m_outImageG[i][k] = rgb[1]; m_outImageB[i][k] = rgb[2];
		}
}


void CColorImageProcessingDoc::OnWantFilter()
{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	dlg.m_question.SetString(L"필터 크기는 몇으로 할까요?\n( 3, 5, 7, 9 )"); 
	if (dlg.DoModal() != IDOK)
		return;	
	if (dlg.m_constant != 3 && dlg.m_constant != 5 && dlg.m_constant != 7 && dlg.m_constant != 9) {
		MessageBox(NULL, L"필터의 크기는 지정된 홀수로만 가능합니다. ", L"필터 크기 오류", NULL);
		return;
	}

	CString filter[9];
	for (int i = 0; i < dlg.m_constant; i++) {
		CStringDlg dlgString;
		CString question;
		question.Format(_T("필터의 %d번째 줄을 입력해 주세요.\n예) 1 2 3 4 5 6 7"), i + 1);
		dlgString.m_question.SetString(question);
		if (dlgString.DoModal() != IDOK)
			return;

		filter[i] = dlgString.m_string;
	}


	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	// 마스크
	const int MSIZE = (const int)dlg.m_constant;
	double** mask = OnMalloc2D_double(MSIZE, MSIZE);
	double avg = 0;
	for (int i = 0; i < MSIZE; i++) {
		CString tmp[9] = { L"", };
		for (int k = 0; k < MSIZE; k++) {
			TCHAR ttmp = ' ';
			if (AfxExtractSubString(tmp[k], filter[i], k, ttmp)) {
				if (tmp[k] != "")
					mask[i][k] = _wtof(tmp[k]);
				else mask[i][k] = 0.0;
			}
			else  mask[i][k] = 0.0;
			
			avg += mask[i][k];
		}
	}
	if (avg != 0) {
		avg /= MSIZE * MSIZE;
		for (int i = 0; i < MSIZE; i++) {
			for (int k = 0; k < MSIZE; k++) {
				mask[i][k] /= avg;
			}
		}
	}

	// 임시메모리 할당
	int appendSize = MSIZE - 1;
	double** tmpInImage;
	double** tmpOutImage;
	tmpInImage = OnMalloc2D_double(m_inH + appendSize, m_inW + appendSize);
	tmpOutImage = OnMalloc2D_double(m_outH, m_outW);

	// 임시 배열 초기화
	for (int i = 0; i < m_inH + appendSize; i++) {
		for (int k = 0; k < m_inW + appendSize; k++) {
			tmpInImage[i][k] = 127.0;
		}
	}

	// 입력 이미지 --> 임시 메모리
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImage[i + (appendSize / 2)][k + (appendSize / 2)] = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3;
		}
	}

	// 회선 연산: 마스크로 긁어가며 계산하기
	double S = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S = 0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];
			tmpOutImage[i][k] = S;
		}

		// 임시 출력 메모리 --> 출력 이미지
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				if (tmpOutImage[i][k] < 0.0)			
					m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 0;
				else if (tmpOutImage[i][k] > 255.0)		
					m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = 255;
				else									
					m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)tmpOutImage[i][k];
			}
		}
	}

	OnFree2D(tmpInImage, m_inH + appendSize);
	OnFree2D(tmpOutImage, m_outH);
	OnFree2D(mask, MSIZE);
}


void CColorImageProcessingDoc::OnWantFilter2()

{
	// TODO: 여기에 구현 코드 추가.
	CInIntDlg dlg;
	dlg.m_question.SetString(L"필터 크기는 몇으로 할까요?\n( 3, 5, 7, 9 )");
	if (dlg.DoModal() != IDOK)
		return;
	if (dlg.m_constant != 3 && dlg.m_constant != 5 && dlg.m_constant != 7 && dlg.m_constant != 9) {
		MessageBox(NULL, L"필터의 크기는 지정된 홀수로만 가능합니다. ", L"필터 크기 오류", NULL);
		return;
	}

	CString filter[9];
	for (int i = 0; i < dlg.m_constant; i++) {
		CStringDlg dlgString;
		CString question;
		question.Format(_T("필터의 %d번째 줄을 입력해 주세요.\n예) 1 2 3 4 5 6 7"), i + 1);
		dlgString.m_question.SetString(question);
		if (dlgString.DoModal() != IDOK)
			return;

		filter[i] = dlgString.m_string;
	}


	// 기존 메모리 해제
	OnFreeOutImage();

	// 중요!! : 출력 이미지 크기 결정 / 알고리즘에 따라
	m_outH = m_inH;
	m_outW = m_inW;

	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);

	// 진짜 영상처리 알고리즘
	// 마스크
	const int MSIZE = (const int)dlg.m_constant;
	double** mask = OnMalloc2D_double(MSIZE, MSIZE);
	double avg = 0;
	for (int i = 0; i < MSIZE; i++) {
		CString tmp[9] = { L"", };
		for (int k = 0; k < MSIZE; k++) {
			TCHAR ttmp = ' ';
			if (AfxExtractSubString(tmp[k], filter[i], k, ttmp)) {
				if (tmp[k] != "")
					mask[i][k] = _wtof(tmp[k]);
				else mask[i][k] = 0.0;
			}
			else  mask[i][k] = 0.0;

			avg += mask[i][k];
		}
	}
	if (avg != 0) {
		avg /= MSIZE * MSIZE;
		for (int i = 0; i < MSIZE; i++) {
			for (int k = 0; k < MSIZE; k++) {
				mask[i][k] /= avg;
			}
		}
	}

	// 임시메모리 할당
	int appendSize = MSIZE - 1;
	double** tmpInImageR, ** tmpInImageG, ** tmpInImageB, ** tmpOutImageR, ** tmpOutImageG, ** tmpOutImageB;
	double** tmpInImageH, ** tmpInImageS, ** tmpInImageI;
	double** tmpMasked;
	tmpInImageR = OnMalloc2D_double(m_inH + appendSize, m_inW + appendSize);
	tmpInImageG = OnMalloc2D_double(m_inH + appendSize, m_inW + appendSize);
	tmpInImageB = OnMalloc2D_double(m_inH + appendSize, m_inW + appendSize);

	tmpInImageH = OnMalloc2D_double(m_inH + appendSize, m_inW + appendSize);
	tmpInImageS = OnMalloc2D_double(m_inH + appendSize, m_inW + appendSize);
	tmpInImageI = OnMalloc2D_double(m_inH + appendSize, m_inW + appendSize);

	tmpOutImageR = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageG = OnMalloc2D_double(m_outH, m_outW);
	tmpOutImageB = OnMalloc2D_double(m_outH, m_outW);

	tmpMasked = OnMalloc2D_double(m_outH, m_outW);

	// 임시 배열 초기화
	for (int i = 0; i < m_inH + appendSize; i++) {
		for (int k = 0; k < m_inW + appendSize; k++) {
			tmpInImageR[i][k] = 127.0;
			tmpInImageG[i][k] = 127.0;
			tmpInImageB[i][k] = 127.0;
		}
	}

	// 입력 이미지 --> 임시 메모리
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			tmpInImageR[i + (appendSize / 2)][k + (appendSize / 2)] = m_inImageR[i][k];
			tmpInImageG[i + (appendSize / 2)][k + (appendSize / 2)] = m_inImageG[i][k];
			tmpInImageB[i + (appendSize / 2)][k + (appendSize / 2)] = m_inImageB[i][k];
		}
	}

	///////// RGB 모델 --> HSI 모델 ///////////////
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			double* hsi;
			unsigned char R, G, B;
			R = (unsigned char)tmpInImageR[i][k];
			G = (unsigned char)tmpInImageG[i][k];
			B = (unsigned char)tmpInImageB[i][k];
			hsi = RGB2HSI(R, G, B);

			double H, S, I;
			H = hsi[0]; S = hsi[1]; I = hsi[2];
			tmpInImageH[i][k] = H;
			tmpInImageS[i][k] = S;
			tmpInImageI[i][k] = I;
		}
	}

	// 회선 연산: 마스크로 긁어가며 계산하기
	double S = 0;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			S = 0;
			for (int m = 0; m < MSIZE; m++)
				for (int n = 0; n < MSIZE; n++)
					S += tmpInImageI[i + m][k + n] * mask[m][n];
			tmpMasked[i][k] = S;
		}

		// 임시 출력 메모리 --> 출력 이미지
		for (int i = 0; i < m_outH; i++) {
			for (int k = 0; k < m_outW; k++) {
				if (tmpMasked[i][k] < 0.0)
					tmpMasked[i][k] = 0;
				else if (tmpMasked[i][k] > 255.0)
					tmpMasked[i][k] = 255;
			}
		}
	}

	////// HSI --> RGB ////////
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			unsigned char* rgb;
			double H, S, I;

			H = tmpInImageH[i + (appendSize / 2)][k + (appendSize / 2)];
			S = tmpInImageS[i + (appendSize / 2)][k + (appendSize / 2)];
			I = tmpMasked[i][k];

			rgb = HSI2RGB(H, S, I);
			tmpOutImageR[i][k] = rgb[0]; 
			tmpOutImageG[i][k] = rgb[1]; 
			tmpOutImageB[i][k] = rgb[2];
		}


	// 임시 출력 이미지 ---> 출력 이미지
	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			if (tmpOutImageR[i][k] < 0.0)
				m_outImageR[i][k] = 0;
			else if (tmpOutImageR[i][k] > 255.0)
				m_outImageR[i][k] = 255;
			else
				m_outImageR[i][k] = (unsigned char)tmpOutImageR[i][k];

			if (tmpOutImageG[i][k] < 0.0)
				m_outImageG[i][k] = 0;
			else if (tmpOutImageG[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageG[i][k] = (unsigned char)tmpOutImageG[i][k];

			if (tmpOutImageB[i][k] < 0.0)
				m_outImageB[i][k] = 0;
			else if (tmpOutImageB[i][k] > 255.0)
				m_outImageB[i][k] = 255;
			else
				m_outImageB[i][k] = (unsigned char)tmpOutImageB[i][k];

		}

	OnFree2D(tmpInImageR, m_inH + appendSize);
	OnFree2D(tmpInImageG, m_inH + appendSize);
	OnFree2D(tmpInImageB, m_inH + appendSize);

	OnFree2D(tmpInImageH, m_inH + appendSize);
	OnFree2D(tmpInImageS, m_inH + appendSize);
	OnFree2D(tmpInImageI, m_inH + appendSize);

	OnFree2D(tmpOutImageR, m_outH);
	OnFree2D(tmpOutImageG, m_outH);
	OnFree2D(tmpOutImageB, m_outH);

	OnFree2D(tmpMasked, m_outH);
	OnFree2D(mask, MSIZE);
}