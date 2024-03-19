#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/////////////////
// 전역 변수부 //
/////////////////
FILE* rfp;
char fileName[200] = "D:/RAW/Etc_Raw(squre)/LENNA512.raw";
char fileMask[200] = "D:/RAW/Etc_Raw(squre)/circle512.raw";
#define height 512
#define width 512

// 메모리 확보
unsigned char inImage[height][width], outImage[height][width], mask[height][width];


/////////////////
// 함수 선언부 //
/////////////////
// 공통
void loadImage();
int getIntValue();
void printImage();

// 영상처리 함수
void equalImage();		
void addImage(int);
void minusImage(int);
void multiImage(int);
void divImage(int);
void reverseImage();
void monoImage();
void gammaImage(int);
void paraImage();
void monoAvgImage();
void monoMidImage();
void multiRapImage(int);
void maskAndImage();
void maskOrImage();
void maskXorImage();
void notImage();
void constrastStratchImage();
void constrastZipImage();
void posterizeImage();
void areaStressImage();


void main() {
	// 이미지 파일 > 메모리 (로딩)
	loadImage();

	// 영상처리 알고리즘 구현
	// (1) 밝게하기 (= 더하기)
	printf("밝게 하기 ");
	int value = getIntValue();
	addImage(value);

	// (2) 어둡게하기
	printf("어둡게 하기 ");
	value = getIntValue();
	minusImage(value);

	// (3) 반전
	printf("반전 \n");
	reverseImage();

	// (4) 흑백 (128)
	printf("흑백 \n");
	monoImage();

	// 감마 
	printf("감마 변환 (1~255 사이의 값) ");
	value = getIntValue();
	if (value < 1 || value > 255) {
		printf("- 값이 범위를 벗어나 패스합니당. \n");
	}
	else	gammaImage(value);

	// 파라볼라
	printf("파라볼라 변환 \n");
	paraImage();

	// 흑백 (평균값)
	printf("흑백(평균값) 변환 - ");
	monoAvgImage();

	// 흑백 (중앙값)
	printf("흑백(중앙값) 변환 - ");
	monoMidImage();

	// 곱하기
	printf("더 밝게 하기 (배수)");
	value = getIntValue();
	multiImage(value);

	// 나누기
	printf("더 어둡게 하기 (배수)");
	value = getIntValue();
	divImage(value);

	// 곱하기 + 랩핑기법
	printf("더 밝게 하기 (배수) + 랩핑기법");
	value = getIntValue();
	multiRapImage(value);

	// 원형 AND 마스크 씌워보기 
	printf("원형 마스크 씌워보기(AND) \n");
	maskAndImage();

	// 원형 OR 마스크 씌워보기 
	printf("원형 마스크 씌워보기(OR) \n");
	maskOrImage();

	// 원형 XOR 마스크 씌워보기 
	printf("원형 마스크 씌워보기(XOR) \n");
	maskXorImage();

	// NOT 반전 시키기 
	printf("NOT 반전 시키기 \n");
	notImage();

	// 명암 대비 스트레칭 시키기 
	printf("명암 대비 스트레칭 시키기 \n");
	constrastStratchImage();

	// 명암 대비 압축 시키기 
	printf("명암 대비 압축 시키기 \n");
	constrastZipImage();

	// 포스터라이징 시키기 
	printf("포스터라이징 시키기 \n");
	posterizeImage();

	// 범위 강조
	printf("범위 강조 시키기 \n");
	areaStressImage();

}


/////////////////
// 함수 정의부 //
/////////////////

/////////// 
// 공통함수
///////////
void loadImage() {
	rfp = fopen(fileName, "rb");
	fread(inImage, sizeof(unsigned char), height * width, rfp);
	fclose(rfp);

	rfp = fopen(fileMask, "rb");
	fread(mask, sizeof(unsigned char), height * width, rfp);
	fclose(rfp);

	equalImage();
}

int getIntValue() {
	int retValue;

	printf("정수값:  ");
	scanf("%d", &retValue);
	return retValue;
}

void printImage() {
	for (int i = height / 2 - 5; i < height / 2 + 5; i++) {
		for (int j = width / 2 - 5; j < width / 2 + 5; j++) {
			printf("%3d ", outImage[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/////////// 
// 영상처리 알고리즘 함수
///////////
void equalImage() {
	// 동일영상 알고리즘
	// 입력배열 --> 출력배열 복사하기
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}

void addImage(int val) {
	// 밝게
	// 입력배열 --> 더해서 --> 출력배열
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] + val > 256) outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] + val;
		}
	}
	printImage();
}

void minusImage(int val) {
	// 어둡게
	// 입력배열 --> 빼서 --> 출력배열
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] - val < 0) outImage[i][k] = 0;
			else outImage[i][k] = inImage[i][k] - val;
		}
	}
	printImage();
}

void multiImage(int val) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] * val > 255) outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] * val;
		}
	}
	printImage();
}

void divImage(int val) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] / val;
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

void gammaImage(int val) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = pow((float)inImage[i][k], 1.f / val);
		}
	}
	printImage();
}

void paraImage() {
	printf("- CAP효과 \n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = 255.f * pow((inImage[i][k] / 127.f) - 1, 2.f);
		}
	}
	printImage();

	printf("- CUP효과 \n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = 255.f - 255.f * pow((inImage[i][k] / 127.f) - 1, 2.f);
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

void multiRapImage(int val) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = (inImage[i][k] * val) % 256;
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