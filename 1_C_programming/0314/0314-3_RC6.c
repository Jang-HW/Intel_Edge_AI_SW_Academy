#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI 지원기능
#include <math.h>

/////////////////////
// 전역 변수부
/////////////////////
HWND hwnd;		// 윈도우 화면 (외부, 물리적)
HDC hdc;		// 윈도우 화면 (내부, 논리적)

FILE* rfp, * wfp;
char fileName[200];
char fileMask[200] = "D:/RAW/Etc_Raw(squre)/circle512.raw";
int inH, inW;
int outH, outW;
// 메모리 확보
// unsigned char inImage[height][width], outImage[height][width];
unsigned char** inImage = NULL, ** outImage = NULL, mask[512][512];

/////////////////////
// 함수 선언부
/////////////////////
void	loadImage();
int		getIntValue();
void	printImage();
void	printMenu();
void	saveImage();
void	freeInputMemory();
void	freeOutputMemory();
void	mallocInputMemory();
void	mallocOutputMemory();

// 화소점 처리
void	equalImage();
void	addImage();
void	minusImage();
void	reverseImage();
void	monoImage();
void	posterizeImage();
void	areaStressImage();
void	moveVerticle();
void	moveHorizon();
void	mirror();

// 기하학
void zoomOut();		// 기본
void zoomOut2();	// 평균으로 줄이기
void zoomOut3();	// 중앙값으로 줄이기
void zoomIn();		// 전방향 사상
void zoomIn2();		// 이웃화소 보간법
void zoomIn3();		// 선형 보간법
void rotate();
void rotate2();

int static compare(const void* first, const void* second)
{
	if (*(int*)first > *(int*)second)
		return 1;
	else if (*(int*)first < *(int*)second)
		return -1;
	else
		return 0;
}


void main() {
	char inKey = 0;

	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL);		// win10은 hwnd, 11은 NULL

	while (inKey != '9') {
		printMenu();
		inKey = _getch();
		system("cls");		// windows 명령어

		switch (inKey) {
		case '0':	loadImage();							break;
		case '1':	saveImage();							break;
		case '9':											break;

		case 'A': case 'a':	equalImage();					break;
		case 'B': case 'b':	addImage();						break;
		case 'C': case 'c':	minusImage();					break;
		case 'D': case 'd':	reverseImage();					break;
		case 'E': case 'e':	monoImage();					break;

		case 'N': case 'n':	zoomOut();						break;
		case 'O': case 'o':	zoomOut2();						break;
		case 'P': case 'p': zoomOut3();						break;
		case 'Q': case 'q': zoomIn();						break;
		case 'R': case 'r': zoomIn2();						break;
		case 'S': case 's': zoomIn3();						break;
		case 'T': case 't': rotate();						break;
		case 'U': case 'u': rotate2();						break;
		case 'V': case 'v': moveVerticle();					break;
		case 'W': case 'w': moveHorizon();					break;
		case 'X': case 'x': mirror();						break;


		default:	printf("다른 문자를 입력해 주세요!\n");	break;
		}

	}
	freeInputMemory();
	freeOutputMemory();
}


/////////////////////
// 함수 정의부
/////////////////////
/// 공통 함수
//////////////
void printMenu() {
	puts("\n###Grey Scale Image Processing (Beta 4) ###");
	puts("\n 0. 열기\t1. 저장	\t9. 종료");
	puts("\n A. 동일\tB. 밝게\t\tC. 어둡게\tD. 반전\t\tE. 흑백(127)	");
	puts("\n N. 줌 아웃\tO. 줌 아웃(평균)\tP. 줌 아웃(중앙)\tQ. 줌인(포워딩)");
	puts("\n R. 줌 인2(백워딩)\tS. 줌 인3(선형)\tT. 회전1\tU. 회전2");
	puts("\n V. 세로 이동\tW. 가로 이동 \tX. 미러링");
}

void printImage() {
	system("cls");
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 900, i + 50, RGB(px, px, px));
		}
	}
}

void loadImage() {
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("파일명-->"); // flower512, LENA256
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// (중요!) 이미지의 폭과 높이를 결정
	rfp = fopen(fileName, "rb");
	// 파일 부재시 load 종료
	if (rfp == NULL) {
		printf("파일이 존재하지 않습니다. 다른 이름으로 시도해 주세요.\n\n");
		return;
	}

	fseek(rfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(rfp); // 나 어디쯤이지?  262,144  --> 512
	fclose(rfp);
	// 메모리 해제
	freeInputMemory();
	// 입력 영상의 높이, 폭 구하기
	inH = inW = (int)sqrt(fsize);
	// 메모리 할당
	mallocInputMemory();
	// 파일 --> 메모리
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < inH; i++)
		fread(inImage[i], sizeof(unsigned char), inW, rfp);
	fclose(rfp);

	equalImage();

	rfp = fopen(fileMask, "rb");
	fread(mask, sizeof(unsigned char), 512 * 512, rfp);
	fclose(rfp);
}

int  getIntValue() {
	int retValue;
	printf("정수값-->"); scanf("%d", &retValue);
	return retValue;
}

void saveImage() {
	if (outImage == NULL)	 return;

	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("파일명-->"); // out01, out02......
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// 동명의 파일 존재확인
	rfp = fopen(fileName, "rb");
	char inKey = 0;
	if (rfp != NULL) {
		printf("동일한 이름의 파일이 존재합니다. \n덮어쓰시겠습니까?(y/n) ");
		inKey = _getch();

		while (inKey != 'y' && inKey != 'Y') {
			switch (inKey) {
			case 'N':case 'n': printf("\n저장되지 않았습니다. \n이름을 바꿔 시도해 주세요.\n");	return;
			case 'Y': case'y': printf("\n덮어쓰기를 진행합니다. \n"); break;
			default:		   printf("\n키를 잘못 입력하셨습니다.\n");
			}
		}
	}

	wfp = fopen(fileName, "wb");

	// 메모리 --> 파일 (한행씩)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);

	// MessageBox(hwnd, L"저장 완료", L"저장 창", NULL);
	printf("저장 완료\n");
}

void freeInputMemory() {
	if (inImage == NULL)
		return;
	for (int i = 0; i < inH; i++)
		free(inImage[i]);
	free(inImage);

	inImage = NULL;
}

void freeOutputMemory() {
	if (outImage == NULL)
		return;
	for (int i = 0; i < outH; i++)
		free(outImage[i]);
	free(outImage);

	outImage = NULL;
}

void mallocInputMemory() {
	inImage = (unsigned char**)malloc(sizeof(unsigned char*) * inH);
	for (int i = 0; i < inH; i++)
		inImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * inW);
}

void mallocOutputMemory() {
	outImage = (unsigned char**)malloc(sizeof(unsigned char*) * outH);
	for (int i = 0; i < outH; i++)
		outImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * outW);
}

//////////////////
//  영상처리 함수
//////////////////

void equalImage() { // 동일영상 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}

void addImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();


	// 입력 배열 --> 출력 배열
	int val = getIntValue();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] + val < 255)
				outImage[i][k] = inImage[i][k] + val;
			else
				outImage[i][k] = 255;
		}
	}
	printImage();
}

void minusImage() {
	// 어둡게
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// 입력배열 --> 빼서 --> 출력배열
	int val = getIntValue();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] - val < 0) outImage[i][k] = 0;
			else outImage[i][k] = inImage[i][k] - val;
		}
	}
	printImage();
}

void reverseImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// 반전
	// 입력배열 --> 255에서 빼서 --> 출력배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}

void monoImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < 128)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

// 기본
void zoomOut() { // 축소 알고리즘
	// 줌 아웃 배수
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		if (outH <= i / scale) break;

		for (int k = 0; k < inW; k++) {
			if (outW <= k / scale) break;
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}

// 평균으로 줄이기
void zoomOut2() { 
	// 축소 알고리즘 (각 픽셀을 겹치는 부분들의 중앙값으로 설정하는 것)
	// 줌 아웃 배수
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	mallocOutputMemory();

	int** tmp;
	tmp = (int**)malloc(sizeof(int*) * outH);
	for (int i = 0; i < outH; i++)
		tmp[i] = (int*)malloc(sizeof(int) * outW);

	// 입력 배열 --> 임시 배열
	for (int i = 0; i < inH; i++) {
		if (outH <= i / scale) break;

		for (int k = 0; k < inW; k++) {
			if (outW <= k / scale) break;
			tmp[(int)(i / scale)][(int)(k / scale)] += inImage[i][k];
		}
	}

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = (int)(tmp[i][k] / scale);
		}
	}

	for (int i = 0; i < outH; i++)
		free(tmp[i]);
	free(tmp);

	// 임시 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}

// 중앙값으로 줄이기
void zoomOut3(){
	// 중앙값으로 줄이기
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	mallocOutputMemory();

	int* tmp;
	tmp = (int*)malloc(sizeof(int) * scale * scale);
	int num = 0;

	// 입력 배열 --> 임시 배열
	for (int i = 0; i < inH / scale; i++) {					// y축으로 덩어리 짓기
		for (int k = 0; k < inW / scale; k++) {				// x축으로 덩어리 짓기
			for (int m = 0; m < scale; m++) {					// 덩어리 내의 y축
				for (int n = 0; n < scale; n++) {				// 덩어리 내의 x축
					// 이 안의 칸은 즉 inImage[scale*i + m][scale*k + n]
					tmp[num] = inImage[scale * i + m][scale * k + n];
					num++;
				}
				printf("\n");
			}
			num = 0;
			qsort(tmp, sizeof(tmp)/sizeof(tmp[0]), sizeof(tmp[0]), compare);
			outImage[i][k] = tmp[scale * scale / 2];
		}
	}
	free(tmp);
	printImage();
}

// 전방향 사상
void zoomIn() { // 확대 알고리즘
	// 줌 인 배수
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i * scale][k * scale] = inImage[i][k];
		}
	}
	printImage();
}

// 이웃화소 보간법
void zoomIn2() { 
	// 확대 알고리즘2222
	// 줌 인 배수
	int scale = getIntValue();

	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = inImage[i / scale][k / scale];
		}
	}
	printImage();
}

// 선형 보간법
void zoomIn3() { }

void rotate() { 
	// 회전 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	int degree = getIntValue();
	degree %= 360;
	double radian = degree * 3.1415925386 / 180.0;
	// xd = cos*xs - sin*ys
	// yd = sin*xs + cos*ys


	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			int xs = i;
			int ys = k;

			int	xd = (int)(cos(radian) * xs - sin(radian) * ys);
			int yd = (int)(sin(radian) * xs + cos(radian) * ys);

			if (0 <= xd && xd < outH && 0 <= yd && yd < outW)
				outImage[xd][yd] = inImage[xs][ys];
		}
	}
	printImage();
}

void rotate2() { // 회전 알고리즘 + 중앙 백워딩
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	int degree = getIntValue();
	degree %= 360;
	double radian = degree * 3.1415925386 / 180.0;
	//// xd = cos*xs - sin*ys
	//// yd = sin*xs + cos*ys

	int cx = inH / 2;
	int cy = inW / 2;

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int	xs = (int)(cos(radian) * (xd - cx) + sin(radian) * (yd - cx));
			int ys = (int)(-sin(radian) * (xd - cx) + cos(radian) * (yd - cx));
			xs += cx;
			ys += cy;

			if (0 <= xs && xs < outH && 0 <= ys && ys < outW)
				outImage[xd][yd] = inImage[xs][ys];
		}
	}
	printImage();
}

void moveVerticle() {
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// 움직일 정도
	int val = getIntValue();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		if (i + val >= outH)	break;
		if (i + val < 0)		break;

		for (int k = 0; k < inW; k++) {
			outImage[i + val][k] = inImage[i][k];
		}
	}
	printImage();
}

void moveHorizon() {
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// 움직일 정도
	int val = getIntValue();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (k + val < 0 & k + val >= outH)	break;
			outImage[i][k + val] = inImage[i][k];
		}
	}
	printImage();
}

void mirror() {
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = 2*inH;		outW = 2*inW;
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			printf("%d %d\n", i, k);
			outImage[i][k] = inImage[i][k];
			outImage[2 * inH - i - 1][k] = inImage[i][k];
			outImage[i][2 * inW - k - 1] = inImage[i][k];
			outImage[2 * inH - i - 1][2 * inW - k - 1] = inImage[i][k];
		}
	}
	printImage();
}