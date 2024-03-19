#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
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
void	monoAvgImage();
void	monoMidImage();;
void	maskAndImage();
void	maskOrImage();
void	maskXorImage();
void	notImage();
void	constrastStratchImage();
void	constrastZipImage();
void	posterizeImage();
void	areaStressImage();

// 기하학
void zoomOut();
void zoomIn();		// 전방향 사상
void zoomIn2();		// 이웃화소 보간법
void zoomIn3();		// 선형 보간법
void rotate();
void rotate2();


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
		case 'F': case 'f':	monoAvgImage();					break;
		case 'G': case 'g':	monoMidImage();					break;
		case 'H': case 'h':	maskAndImage();					break;
		case 'I': case 'i':	maskOrImage();					break;
		case 'J': case 'j':	maskXorImage();					break;
		case 'K': case 'k':	notImage();						break;
		case 'L': case 'l':	constrastStratchImage();		break;
		case 'M': case 'm':	constrastZipImage();			break;
		case 'N': case 'n':	posterizeImage();				break;
		case 'O': case 'o':	areaStressImage();				break;

		case 'P': case 'p': zoomOut();						break;
		case 'Q': case 'q': zoomIn();						break;
		case 'R': case 'r': zoomIn2();						break;
		case 'S': case 's': zoomIn3();						break;
		case 'T': case 't': rotate();						break;
		case 'U': case 'u': rotate2();						break;

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
	puts("\n F. 흑백(평균)\tG. 흑백(중간값)\tH. AND\t\tI. OR\t\tJ. XOR	");
	puts("\n K. NOT\t\tL. 명암 대비 스트레칭\t\tM. 명암 대비 압축");
	puts("\n N.포스터라이징\t\tO.범위 강조\tP.줌 아웃\tQ.줌인(포워딩)");
	puts("\n R. 줌 인2(백워딩)\tS. 줌 인3(선형)\tT. 회전1\tU. 회전2");
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

void monoAvgImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	int cMid = 0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			cMid += inImage[i][k];
		}
	}
	cMid = cMid / (inH * inW);
	printf("평균값: %d\n", cMid);

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void monoMidImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	int cMid = 0, count = 0;
	unsigned short cValCount[256] = { 0, };

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			cValCount[inImage[i][k]] += 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		count += cValCount[i];
		if (count > (inW * inH) / 2) {
			cMid = i;
			break;
		}
	}
	printf("중앙값: %d\n", cMid);

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void maskAndImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] & mask[i][k];
		}
	}
	printImage();
}

void maskOrImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] | mask[i][k];
		}
	}
	printImage();
}

void maskXorImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] ^ mask[i][k];
		}
	}
	printImage();
}

void notImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = ~inImage[i][k];
		}
	}
	printImage();
}

void constrastStratchImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < 50)			outImage[i][k] = 0;
			else if (inImage[i][k] > 200)	outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] * 51 / 30 - 85;
		}
	}
	printImage();
}

void constrastZipImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] * 30 / 51 + 50;
		}
	}
	printImage();
}

void posterizeImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			switch (inImage[i][k] / 32) {
			case 0: outImage[i][k] = 0;		break;
			case 1: outImage[i][k] = 32;	break;
			case 2: outImage[i][k] = 64;	break;
			case 3: outImage[i][k] = 96;	break;
			case 4: outImage[i][k] = 128;	break;
			case 5: outImage[i][k] = 160;	break;
			case 6: outImage[i][k] = 192;	break;
			case 7: outImage[i][k] = 224;	break;
			case 8: outImage[i][k] = 255;	break;
			}
		}
	}
	printImage();
}

void areaStressImage() {
	// 메모리 해제
	freeOutputMemory();
	// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < 75)			outImage[i][k] = inImage[i][k];
			else if (inImage[i][k] > 175)		outImage[i][k] = inImage[i][k];
			else 						outImage[i][k] = 255;
		}
	}
	printImage();
}

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

void zoomIn2() { // 확대 알고리즘2222
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

  /////////////////////////
 /// 선형보간법 드가자 ///
/////////////////////////
void zoomIn3() { // 확대 알고리즘 3333
	// 줌 인 배수
	//int scale = getIntValue();

	//// 메모리 해제
	//freeOutputMemory();
	//// (중요!!) 출력 이미지의 크기 결정 (알고리즘에 의존적)
	//outH = (int)(inH * scale);
	//outW = (int)(inW * scale);
	//mallocOutputMemory();

	//// 입력 배열 --> 출력 배열
	//for (int i = 0; i < inH; i++) {
	//	for (int k = 0; k < inW; k++) {
	//		outImage[i * scale][k * scale] = inImage[i][k];
	//	}
	//}
	//for (int i = 0; i <	; i++) {
	//	for (int k = 0; k < inW; k++) {
	//		outImage[i * scale][k * scale] = inImage[i][k];
	//	}
	//}
	//printImage();
}

void rotate() { // 회전 알고리즘
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

			if (0 <= xd && xd < outH && 0<=yd && yd < outW)
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