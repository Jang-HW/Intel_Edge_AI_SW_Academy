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
int inH, inW;
int outH, outW;
bool accumulation = false;
int backNum = 0, nowBack = 0;

// 메모리 확보
// unsigned char inImage[height][width], outImage[height][width];
unsigned char** inImage = NULL, ** outImage = NULL;

/////////////////////
// 함수 선언부
/////////////////////
void		loadImage();
int			getIntValue();
void		printImage();
void		printMenu();
void		saveImage();
//void		saveCache(char name[]);
void		freeInputMemory();
void		freeOutputMemory();
void		mallocInputMemory();
void		mallocOutputMemory();
double**	mallocDoubleMemory(int, int);
void		freeDoubleMemory(double**, int);
void		backToBefore();

// 화소점 처리
void	equalImage();
void	addImage();
void	minusImage();
void	reverseImage();
void	monoImage();
void	mirror();
void	morph();

// 기하학
void	zoomOut3();	// 중앙값으로 줄이기
void	zoomIn3();		// 선형 보간법
void	moveVerticle();
void	moveHorizon();
void	rotate2();

// 히스토그램
void	histoStractch();		// 선명하게 (무손실)
void	endIn();				// 손실 있어도 더더 선명하게
void	histoEqual();			// 히스토그램 평활화

// 화소 영역처리
void	emboss();				// 엠보싱
void	blur();					// 블러링
void	blurr();				// 블러링 9*9
void	gausinaSmooth();		// 가우시안 스무딩
void	sharf1();				// 샤프닝 1
void	sharf2();				// 샤프닝 2


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
	hdc = GetWindowDC(NULL);

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
		case 'F': case 'f': mirror();						break;
		case 'G': case 'g': morph();						break;

		case 'H': case 'h': zoomOut3();						break;
		case 'I': case 'i': zoomIn3();						break;
		case 'J': case 'j': rotate2();						break;
		case 'K': case 'k': moveVerticle();					break;
		case 'L': case 'l':	moveHorizon();					break;

		case 'M': case 'm':	histoStractch();				break;
		case 'N': case 'n':	endIn();						break;
		case 'O': case 'o':	histoEqual();					break;

		case 'P': case 'p':	emboss();						break;
		case 'Q': case 'q':	blur(); 						break;
		case 'R': case 'r':	gausinaSmooth(); 				break;
		case 'S': case 's':	sharf1(); 						break;
		case 'T': case 't':	sharf2();						break;
		case 'U': case 'u':	blurr();						break;

			

		case 'Z': case 'z':	accumulation = !accumulation;	break;
		//case '^':			backToBefore();	break;

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
	puts("\n 0. 열기\t\t1. 저장\t\t\t\t\t\t\t\t\t9. 종료");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n A. 동일\t\tB. 밝게\t\t\tC. 어둡게\t\tD. 반전\t\t\tE. 흑백(127)");
	puts("\n F. 미러링\t\tG. 모핑");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n H. 줌 아웃(중앙)\tI. 줌인(선형)\t\tJ. 회전2\t\tK. 세로 이동\t\tL. 가로 이동");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n M. 히스토그램 스트레칭\tN. 엔드인\t\tO. 평활화");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n P. 엠보싱\t\tQ. 블러링\t\tR. 가우시안 스무딩\tS. 샤프닝1\t\tT. 샤프닝2");
	puts("\n U. 블러링(받는데로)");
	puts("\n---------------------------------------------------------------------------------");
	printf("\n Z. 효과 누적(on/off)\t");
	if (accumulation)	printf("현재는 켜져있음\n");
	else				printf("현재는 꺼져있음\n");
	//puts(" ^. 되돌리기");
	puts("\n");
}

void printImage() {
	system("cls");

	//// 되돌리기를 위한 outImage 저장
	//char fileName[10] = "cache";
	//char fileNum[5];

	//sprintf(fileNum, "%d", (backNum % 10));    // %d를 지정하여 정수를 문자열로 저장
	//strcat(fileName, fileNum);
	//saveCache(fileName);

	//if (backNum < 10) { 
	//	backNum++; 
	//	nowBack++; 
	//}
	//else  { 
	//	backNum = 10; 
	//	nowBack = (nowBack + 1) % 10;
	//}
	//

	// 출력
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 900, i + 50, RGB(px, px, px));
		}
	}

	// 계속 편집을 위한 inImage의 변경 
	if (accumulation) {
		freeInputMemory();
		inH = outH;		inW = outW;
		mallocInputMemory();

		for (int i = 0; i < outH; i++) {
			for (int k = 0; k < outW; k++) {
				inImage[i][k] = outImage[i][k];
			}
		}
		freeOutputMemory();
		outImage = NULL;
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

// 임시데이터 저장을 위한 파일 저장 함수
//void saveCache(char name[]) {
//	char fullname[200] = "D:/RAW/";
//	strcat(fullname, name);
//	strcat(fullname, ".raw");
//	strcpy(fileName, fullname);
//
//	wfp = fopen(fileName, "wb");
//
//	// 메모리 --> 파일 (한행씩)
//	for (int i = 0; i < outH; i++)
//		fwrite(fileName[i], sizeof(unsigned char), outW, wfp);
//	fclose(wfp);
//}

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

double** mallocDoubleMemory(int h, int w) {
	double** retMemory;
	retMemory = (double**)malloc(sizeof(double*) * h);
	for (int i = 0; i < h; i++)
		retMemory[i] = (double*)malloc(sizeof(double) * w);
	return retMemory;
}

void freeDoubleMemory(double** memory, int h) {
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
	memory = NULL;
}

void backToBefore() {
	if (backNum <= 0) {
		printf("되돌릴 파일이 없습니다. \n");
		return;
	}
	backNum--;

	char fullName[200] = "D:/RAW/";
	char backFile[20] = "cache";
	if(nowBack > 0)	{ nowBack--; }
	else			{ nowBack = 9; }
	strcat(backFile, (char)nowBack);	
	strcat(fullName, backFile);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	rfp = fopen(fileName, "rb");
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
}


  ///////////////////
 //  영상처리 함수//
///////////////////

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

// 중앙값으로 줄이기
void zoomOut3() {
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
			qsort(tmp, (sizeof(tmp) / sizeof(tmp[0])), sizeof(tmp[0]), compare);
			outImage[i][k] = tmp[scale * scale / 2];
		}
	}
	free(tmp);
	printImage();
}

// 양선형 보간법
void zoomIn3() {
	// 확대 알고리즘 333
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

	int i_h, i_w, point;
	double r_H, r_W, s_H, s_W;
	double C1, C2, C3, C4;

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			r_H = i / scale;								// 현재 위치를 scale 크기의 블록 중 몇 번째 쯤인지 연산
			r_W = k / scale;

			i_h = (int)floor(r_H);							// 블록의 가로세로 칸을 저장
			i_w = (int)floor(r_W);

			s_H = r_H - i_h;								// 현재 블록 내에서의 위치를 저장 
			s_W = r_W - i_w;

			if ((i_h < 0) | (i_h >= (inH - 1)) | (i_w < 0) | (i_w >= (inW - 1))) {
				outImage[i][k] = 255;						// 현재 칸이 보간을 진행할 수 없다면 흰색으로 지정
															// 주로 왼쪽과 아래쪽이 그런 경향이 있음
			}
			else {
				C1 = (double)inImage[i_h][i_w];				// 좌 상단점 지정
				C2 = (double)inImage[i_h][i_w + 1];			// 우 상단점 지정
				C3 = (double)inImage[i_h + 1][i_w + 1];		// 우 하단점 지정
				C4 = (double)inImage[i_h + 1][i_w];			// 좌 하단점 지정
				outImage[i][k] = (unsigned char)(C1 * (1 - s_H) * (1 - s_W)		
					+ C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				// 각 점들과의 위치 비율을 반영해서 점의 색 지정 
			}

		}
	}		
	printImage();
}

// 회전 (안 깨짐)
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
	outH = 2 * inH;		outW = 2 * inW;
	mallocOutputMemory();

	// 입력 배열 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k];
			outImage[2 * inH - i - 1][k] = inImage[i][k];
			outImage[i][2 * inW - k - 1] = inImage[i][k];
			outImage[2 * inH - i - 1][2 * inW - k - 1] = inImage[i][k];
		}
	}
	printImage();
}

// 선명하게 (무손실)
void histoStractch() { 
	// 히스토그램 스트래칭
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();


	// new = (old - low) / (high-low) * 255.0

	// high, low 찾기
	int high = inImage[0][0], low = inImage[0][0];
	for (int i = 1; i < inH; i++) {
		for (int k = 1; k < inW; k++) {
			if (inImage[i][k] < low)	low = inImage[i][k];
			if (inImage[i][k] > high)	high = inImage[i][k];
		}
	}	 

	// 입력 배열 --> 출력 배열
	int new, old;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			old = inImage[i][k];
			new = (int)(((double)old - low) / (high - low) * 255.0);

			if (new > 255)	new = 255;
			if (new < 0)	new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}

// 손실이 있어도 더더 선명하게
void endIn() {
	// end-in 탐색 알고리즘
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();


	// new = (old - low) / (high-low) * 255.0

	// high, low 찾기
	int high = inImage[0][0], low = inImage[0][0];
	for (int i = 1; i < inH; i++) {
		for (int k = 1; k < inW; k++) {
			if (inImage[i][k] < low)	low = inImage[i][k];
			if (inImage[i][k] > high)	high = inImage[i][k];
		}
	}

	//////////여기가 다름
	high -= 50;
	low += 50;

	// 입력 배열 --> 출력 배열
	int new, old;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			old = inImage[i][k];
			new = (int)(((double)old - low) / (high - low) * 255.0);

			if (new > 255)	new = 255;
			if (new < 0)	new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}

// 히스토그램 평활화 알고리즘 
void histoEqual(){			
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// 1) 빈도수 세기			histo[256]
	int histo[256] = { 0, };

	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++) 
			histo[inImage[i][k]]++;

	// 2) 누적 히스토그램 생성
	int sumHisto[256] = { 0, };

	sumHisto[0] = histo[0];
	for (int i = 1; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];
		
	// 3) 정규화 히스토그램 생성
	// normalHisto = sumHisto*(1.0/(inH*inW))*255.0
	double normalHisto[256] = { 1.0, };
	for (int i = 1; i < 256; i++)
		normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW)) * 255.0;

	// 4) 입력 배열을 치환 --> 출력 배열
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (unsigned char) normalHisto[inImage[i][k]];
		}
	}
	printImage();
}

// 화소 영역처리 - 엠보싱 
void emboss() {
	freeOutputMemory();						// 메모리 해제
	outH = inH;		outW = inW;				// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	mallocOutputMemory();

	  ///////////////////
	 // 화소 영역처리 //
	///////////////////
	double mask[3][3] = { {-1.0, 0.0, 0.0}, {0.0, 0.0, 0.0},{0.0, 0.0, 1.0} };	

	// 임시메모리 할당(실수형으로)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 127으로 초기화
	for (int i = 0; i < inH+2; i++) {
		for (int k = 0; k < inW+2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     회선 연산
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크와 한 점을 중심으로한 3*3 곱하기
			S = 0.0;	// 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++) 
				for (int n = 0; n < 3; n++) 
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	// 후처리 : 마스크의 합계에 따라
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpOutImage[i][k] += 127.0;

	// 임시출력영상 --> 출력영상
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// 화소 영역처리 - 블러링 
void blur() {
	freeOutputMemory();						// 메모리 해제
	outH = inH;		outW = inW;				// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	mallocOutputMemory();

	///////////////////
   // 화소 영역처리 //
  ///////////////////
	double mask[3][3] = { {1.0/9, 1.0/9, 1.0/9}, {1.0/9, 1.0/9, 1.0/9},{1.0/9, 1.0/9, 1.0/9} };

	// 임시메모리 할당(실수형으로)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 127으로 초기화
	for (int i = 0; i < inH + 2; i++) {
		for (int k = 0; k < inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     회선 연산
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크와 한 점을 중심으로한 3*3 곱하기
			S = 0.0;	// 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// 후처리 : 마스크의 합계에 따라
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시출력영상 --> 출력영상
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// 화소 영역처리 - 엠보싱 
void gausinaSmooth() {
	freeOutputMemory();						// 메모리 해제
	outH = inH;		outW = inW;				// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	mallocOutputMemory();

	///////////////////
   // 화소 영역처리 //
  ///////////////////
	double mask[3][3] = { {1./16 ,1./8, 1./16}, {1. / 8 ,1. / 4, 1. / 8},{1. / 16 ,1. / 8, 1. / 16} };

	// 임시메모리 할당(실수형으로)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 127으로 초기화
	for (int i = 0; i < inH + 2; i++) {
		for (int k = 0; k < inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     회선 연산
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크와 한 점을 중심으로한 3*3 곱하기
			S = 0.0;	// 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// 후처리 : 마스크의 합계에 따라
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시출력영상 --> 출력영상
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// 화소 영역처리 - 샤프닝1
void sharf1() {
	freeOutputMemory();						// 메모리 해제
	outH = inH;		outW = inW;				// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	mallocOutputMemory();

	///////////////////
   // 화소 영역처리 //
  ///////////////////
	double mask[3][3] = { {-1., -1., -1.}, {-1., 9 ,-1.},{-1., -1., -1.} };

	// 임시메모리 할당(실수형으로)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 127으로 초기화
	for (int i = 0; i < inH + 2; i++) {
		for (int k = 0; k < inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     회선 연산
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크와 한 점을 중심으로한 3*3 곱하기
			S = 0.0;	// 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// 후처리 : 마스크의 합계에 따라
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시출력영상 --> 출력영상
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// 화소 영역처리 - 샤프닝2
void sharf2() {
	freeOutputMemory();						// 메모리 해제
	outH = inH;		outW = inW;				// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	mallocOutputMemory();

	///////////////////
   // 화소 영역처리 //
  ///////////////////
	double mask[3][3] = { {0., -1., 0.}, {-1., 5 ,-1.},{0., -1., 0.} };

	// 임시메모리 할당(실수형으로)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 127으로 초기화
	for (int i = 0; i < inH + 2; i++) {
		for (int k = 0; k < inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     회선 연산
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// 마스크와 한 점을 중심으로한 3*3 곱하기
			S = 0.0;	// 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// 후처리 : 마스크의 합계에 따라
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// 임시출력영상 --> 출력영상
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// 화소 영역처리 - 입력 받는대로 블러링
void blurr() {
	freeOutputMemory();						// 메모리 해제

	int scale = getIntValue();
	outH = inH;		outW = inW;				// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	mallocOutputMemory();

	///////////////////
   // 화소 영역처리 //
  ///////////////////

	double** mask;
	int lenScale = scale * 2 + 1;
	mask = mallocDoubleMemory(lenScale, lenScale);

	for (int i = 0; i < lenScale; i++) {
		for (int k = 0; k < lenScale; k++)
			mask[i][k] = 1. / pow(lenScale, 2);
	}

	// 임시메모리 할당(실수형으로)
	double** tmpInImage = mallocDoubleMemory(inH + scale * 2, inW + scale * 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// 임시 입력 메모리 127으로 초기화
	for (int i = 0; i < inH + 2 * scale; i++) {
		for (int k = 0; k < inW + 2 * scale; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// 입력 이미지 --> 임시 입력 이미지
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     회선 연산
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
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
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(mask, scale);
	freeDoubleMemory(tmpInImage, inH + scale * 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// 모핑 
void morph() {
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	unsigned char** moImage = NULL;
	int moH, moW;


	// 모핑할 이미지 불러오기
	printf("파일명-->"); // flower512, LENA256
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	rfp = fopen(fileName, "rb");
	if (rfp == NULL) {
		printf("파일이 존재하지 않습니다. 다른 이름으로 시도해 주세요.\n\n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); // 파일의 끝으로 이동
	long long fsize = ftell(rfp); // 나 어디쯤이지?  262,144  --> 512
	fclose(rfp);

	// 모핑 영상의 높이, 폭 구하기
	moH = moW = (int)sqrt(fsize);
	if (inH > moH & inW > moW)	{
		moH = inH;
		moW = inW;
	}
	// 메모리 할당 및 초기화
	moImage = (unsigned char**)malloc(sizeof(unsigned char*) * moH);
	for (int i = 0; i < moH; i++)
		moImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * moW);
	for (int i = 0; i < moH; i++)
		for (int k = 0; k < moW; k++)
			moImage[i][k] = 0;
	// 파일 --> 메모리
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < moH; i++)
		fread(moImage[i], sizeof(unsigned char), moW, rfp);
	fclose(rfp);

	bool tmpAccumulation = accumulation;
	accumulation = false;


	// 5단계 모핑
	for (int n = 0; n < 5; n++) {
		for (int i = 0; i < moH; i++) {
			for (int k = 0; k < moW; k++) {
				outImage[i][k] = (unsigned char)((4. - n) / 4 * inImage[i][k] + n / 4. * moImage[i][k]);
			}
		}
		printImage(); 
		_getch();
	}

	accumulation = tmpAccumulation;

	for (int i = 0; i < moH; i++)
		free(moImage[i]);
	free(moImage);
}