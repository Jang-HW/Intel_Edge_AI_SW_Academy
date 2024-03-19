#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <random>



void main() {
	srand((unsigned int) time(0));	// 시계 초기화

	 // (0) 선택한 영상의 크기만큼 메모리 확보 (jolly.raw: 10*10 크기의 이미지)
	#define height 10
	#define weight 10


	// (1) 2차원 배열(=메모리)을 준비
	unsigned char image[height][weight];	// int로 하면 용량 낭비가 심하니까
	

	// (2) 이미지 파일 읽어서 2차원 메모리로 로딩(Loading /
	//											  Disk의 자료를 RAM으로 가져오기)
	
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			image[i][k] = rand() % 256;
		}
	}

	// 원본 이미지 출력
	printf("원본 이미지\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			printf("%3d ", image[i][k]);
		}
		printf("\n");
	}
	printf("\n\n");

	// (3) 영상처리 효과 (Image Processing) ===> 영상처리 알고리즘 적용
	//// (3-1) 밝게 하기 알고리즘 (50 정도)
	//printf("밝게한 이미지\n");
	//for (int i = 0; i < height; i++) {
	//	for (int k = 0; k < weight; k++) {
	//		if (image[i][k] + 50 < 256) image[i][k] += 50;
	//		else image[i][k] = 255;
	//	}
	//}
	
	// (3-2) 어둡게 하기 알고리즘 (50 정도)
	printf("어둡게 한 이미지\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			if (image[i][k] - 50 > 0) image[i][k] -= 50;
			else image[i][k] = 0;
		}
	}

	// 결과 이미지 출력
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			printf("%3d ", image[i][k]);
		}
		printf("\n");
	}
}