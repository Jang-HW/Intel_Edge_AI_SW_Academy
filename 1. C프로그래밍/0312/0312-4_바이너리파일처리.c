#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

void main() {

	// 가변메모리를 잘 쓰는게 좋음
	FILE* rfp;

	char cFileName[200] = "D:/RAW/Etc_Raw(squre)/LENNA512.raw";
	#define height 512
	#define width 512

	unsigned char image[height][width];

	// 파일처리 3단계
	// 1. 파일 열기
	rfp = fopen(cFileName, "rb");		// r: read, t: text, b: binary

	// 2. 파일 읽거나 쓰기
	// 한번에 512*512 읽기 
	fread(image, 1, height * width, rfp);	// image에 1바이트씩 전체 크기(가로*세로)만큼 rfp에서 

	// 3. 파일 닫기
	fclose(rfp);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			printf("%3d ", image[i][k]);
		}
		printf("\n");
	}
}