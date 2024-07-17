#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI 지원기능

/////////////////////
// 전역 변수부
/////////////////////
HWND hwnd;		// 윈도우 화면 (외부, 물리적)
HDC hdc;		// 윈도우 화면 (내부, 논리적)

FILE* rfp, * wfp;
char fileName[200];
char fileMask[200] = "D:/RAW/Etc_Raw(squre)/circle512.raw";
#define height 512
#define width 512
// 메모리 확보
unsigned char inImage[height][width], outImage[height][width], mask[height][width];


/////////////////////
// 함수 선언부
/////////////////////
void loadImage(); int getIntValue(); void printImage();	void printMenu(); void saveImage();

void equalImage();
void addImage();
void minusImage();
void reverseImage();
void monoImage();
void monoAvgImage();
void monoMidImage();;
void maskAndImage();
void maskOrImage();
void maskXorImage();
void notImage();
void constrastStratchImage();
void constrastZipImage();
void posterizeImage();
void areaStressImage();

void main() {
	char inKey = 0;

	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL);		// win10은 hwnd, 11은 NULL

	while (inKey != '9') {
		printMenu();
		inKey = _getch();
		system("cls");		// windows 명령어

		switch (inKey) {
		case '0':	loadImage();		break;
		case '1':	saveImage();		break;
		case '9':						break;
		case 'A':
		case 'a':	equalImage();		break;
		case 'B':
		case 'b':	addImage();			break;
		case 'C':
		case 'c':	minusImage();		break;
		case 'D':
		case 'd':	reverseImage();		break;
		case 'E':
		case 'e':	monoImage();		break;
		case 'F':
		case 'f':	monoAvgImage();		break;
		case 'G':
		case 'g':	monoMidImage();		break;
		case 'H':
		case 'h':	maskAndImage();		break;
		case 'I':
		case 'i':	maskOrImage();		break;
		case 'J':
		case 'j':	maskXorImage();		break;
		case 'K':
		case 'k':	notImage();			break;
		case 'L':
		case 'l':	constrastStratchImage();		break;
		case 'M':
		case 'm':	constrastZipImage();			break;
		case 'N':
		case 'n':	posterizeImage();	break;
		case 'O':
		case 'o':	areaStressImage();	break;

		default:	printf("다른 문자를 입력해 주세요!\n");	 break;
		}

	}
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
	puts("\n N. 포스터라이징\t\tO. 범위 강조");
}

void printImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 350, i + 200, RGB(px, px, px));
		}
		printf("\n");
	}
	printf("\n");
}

void loadImage() {
	char fullName[200] = "D:/RAW/Pet_RAW(squre)/Pet_RAW(512x512)/";
	char tmpName[50];
	printf("파일명--> "); // cat01, dog05
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, "_512.raw");
	strcpy(fileName, fullName);

	rfp = fopen(fileName, "rb");

	if (rfp != NULL)
	{
		fread(inImage, sizeof(unsigned char), height * width, rfp);
		fclose(rfp);
		equalImage();
	}
	else {
		printf("파일이 없거나 열 수 없습니다. 다른 파일로 실행해 주세요. \n");
	}

	rfp = fopen(fileMask, "rb");
	fread(mask, sizeof(unsigned char), height * width, rfp);
	fclose(rfp);
}

int getIntValue() {
	int retValue;
	printf("정수값-->"); scanf("%d", &retValue);
	return retValue;
}

void saveImage() {
	char fullName[200] = "D:/RAW/Pet_RAW(squre)/Pet_RAW(512x512)/";
	char tmpName[50];
	printf("저장할 파일명--> "); // out01, out02, ....
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, "_512.raw");
	strcpy(fileName, fullName);

	wfp = fopen(fileName, "wb");

	for (int i = 0; i < height; i++) {
		fwrite(outImage[i], sizeof(unsigned char), width, wfp);
	}
	fclose(wfp);
	MessageBox(hwnd, "saved", "About saving", NULL);
}

//////////////////
//  영상처리 함수
//////////////////
void minusImage() {
	// 어둡게
	// 입력배열 --> 빼서 --> 출력배열
	int val = getIntValue();

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] - val < 0) outImage[i][k] = 0;
			else outImage[i][k] = inImage[i][k] - val;
		}
	}
	printImage();
}

void reverseImage() {
	// 반전
	// 입력배열 --> 255에서 빼서 --> 출력배열
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}

void monoImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < 128)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void equalImage() {
	// 동일영상 알고리즘
	// 입력 배열 --> 출력 배열
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}

void addImage() { // 더하기 알고리즘
	// 입력 배열 --> 출력 배열
	int val = getIntValue();

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] + val < 255)
				outImage[i][k] = inImage[i][k] + val;
			else
				outImage[i][k] = 255;
		}
	}
	printImage();
}

void monoAvgImage() {
	int cMid = 0;
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			cMid += inImage[i][k];
		}
	}
	cMid = cMid / (height * width);
	printf("평균값: %d\n", cMid);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void monoMidImage() {
	int cMid = 0, count = 0;
	unsigned short cValCount[256] = { 0, };

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			cValCount[inImage[i][k]] += 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		count += cValCount[i];
		if (count > (width * height) / 2) {
			cMid = i;
			break;
		}
	}

	// 알고리즘 찾아서 채우기 
	// quick sort result
	printf("중앙값: %d\n", cMid);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void maskAndImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] & mask[i][k];
		}
	}
	printImage();
}

void maskOrImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] | mask[i][k];
		}
	}
	printImage();
}

void maskXorImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] ^ mask[i][k];
		}
	}
	printImage();
}

void notImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = ~inImage[i][k];
		}
	}
	printImage();
}

void constrastStratchImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < 50)			outImage[i][k] = 0;
			else if (inImage[i][k] > 200)	outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] * 51 / 30 - 85;
		}
	}
	printImage();
}

void constrastZipImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] * 30 / 51 + 50;
		}
	}
	printImage();
}

void posterizeImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
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
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < 75)			outImage[i][k] = inImage[i][k];
			else if (inImage[i][k] > 175)		outImage[i][k] = inImage[i][k];
			else 						outImage[i][k] = 255;
		}
	}
	printImage();
}