#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>


// (0) 선택한 영상의 가로*세로 파악하기
#define height 5
#define width 5

// (1) 메모리 준비 
unsigned char image[height][width];

// 함수의 선언부 + 정의부
void loadImage() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image[i][j] = rand() % 256;
		}
	}
}

void printImage() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf("%3d ", image[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void reverseImage() {
	printf("반전\n");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image[i][j] = 255 - image[i][j];
		}
	}
	printImage();
}

void addImage(int val){

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (image[i][j] + val < 255)	image[i][j] += val;
			else image[i][j] = 255;
		}
	}
	printImage();
}

void brightImage(){
	printf("밝게 (50)\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] + 50 < 256) image[i][k] += 50;
			else image[i][k] = 255;
		}
	}
	printImage();
}

void darkImage() {
	printf("어둡게 (50)\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] - 50 > 0) image[i][k] -= 50;
			else image[i][k] = 0;
		}
	}
	printImage();
}

void monoImage() {
	printf("평균값으로 흑백 - ");
	int cMid = 0;
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			cMid += image[i][k];
		}
	}
	cMid = cMid / (height * width);
	printf("(평균값: %d)\n", cMid);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] < cMid)
				image[i][k] = 0;
			else image[i][k] = 255;
		}
	}
	printImage();
}




void main() {
	// 랜덤 초기화
	srand((unsigned int)time(0));


	// (2) 이미지 파일 준비 ---> 2차원 배열로 loading
	loadImage();

	//		원본 출력
	printf("원본\n");
	printImage();

	// (3-1) 영상 반전처리
	reverseImage();

	// (3-2) 영상 밝기 변경
	int value;
	printf("얼마나 밝게 할까요? ");	 scanf("%d", &value);
	if (value > 0)	addImage(value);
	else printf("음수라서 넘어갑니당!\n\n");


	// 퀴즈: 어제 진행한 미션을 함수화
	//			밝게
	brightImage();
	 
	//			어둡게
	darkImage();

	//			반전 
	reverseImage();
	 
	//			흑백
	monoImage();
}