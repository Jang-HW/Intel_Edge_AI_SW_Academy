#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

void main() {
	srand((unsigned int)time(0)); // 시계 초기화

	// (0) 선택한 영상의 크기만큼 메모리 확보 (jolly.raw : 10x10 크기 이미지)
	# define height 10  // 높이
	# define width  10  // 폭


	// (1) 2차원 배열(=메모리) 준비
	unsigned char image[height][width];
	unsigned char reversal_image[height][width];
	unsigned char blackWhite_image[height][width];


	// (2) 이미지 파일 --> 2차원 배열로 로딩(Loading)
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			image[i][k] = rand() % 256;
		}
	}
	// 원본 이미지 출력
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			printf("%3d ", image[i][k]);
		}
		printf("\n");
	}
	printf("\n\n");
	
	// (3) 영상처리 효과 ( Image Processing )  ==> 영상처리 알고리즘
	// (3-1) 반전 처리
	printf("반전 처리\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			reversal_image[i][k] = 255 - image[i][k];
		}
	}
	// 결과 이미지 출력
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			printf("%3d ", reversal_image[i][k]);
		}
		printf("\n");
	}
	printf("\n\n");

	// (3-2) 흑백 처리
	printf("흑백 처리 - ");
	int cMid = 0;
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			cMid += image[i][k];
		}
	}
	cMid = cMid / (height * width);
	printf("평균값(심화): %d\n", cMid);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] < cMid)
				blackWhite_image[i][k] = 0;
			else blackWhite_image[i][k] = 255;
		}
	}
	// 결과 이미지 출력
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			printf("%3d ", blackWhite_image[i][k]);
		}
		printf("\n");
	}

	// (심화)
	printf("\n(심화부분)\n");

	unsigned char cMin = 255, cMax = 0;
	unsigned char minW = 0, minH = 0, maxW = 0, maxH = 0;
	unsigned char cMedium, cMany;
	short count = 0;
	unsigned char cValCount[256] = { 0, };

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] < cMin) {
				cMin = image[i][k];
				minH = i;
				minW = k;
			}
			if (image[i][k] > cMax) {
				cMax = image[i][k];
				maxH = i;
				maxW = k;
			}
			cValCount[image[i][k]] += 1;
		}
	}
	
	for (int i = 0; i < 255; i++) {
		count += cValCount[i];
		if (count > (width * height) / 2) {
			cMedium = i;
			break;
		}
	}
	count = 0;
	for (int i = 0; i < 255; i++) {
		if (count < cValCount[i]) {
			count = cValCount[i];
			cMany = i;
		}
	}

	printf("최댓값: %d (%d, %d)\n", cMax, maxW, maxH);
	printf("최솟값: %d (%d, %d)\n", cMin, minW, minH);
	printf("중앙값: %d\n", cMid);
	printf("최빈값: %d (%d회)\n", cMany, count);
}