#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

#define height 3
#define weight 5

void main() {
	
	// 2차원 배열
	// 
	// (0) 배열 준비 
	int ary[height][weight];

	// (1) 배열 값 불러오기 (나중에는 그림 파일을 읽어오기)
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			ary[i][k] = 100;
		}
	}
	
	// (2) 배열 처리하기 (활용하기)
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			ary[i][k] += 10;		// 영상일 경우, 10만큼 밝게 처리 <<< 영상 처리 알고리즘
		}
	}

	// (3) 배열 출력 (=영상을 화면에 보이기)
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			printf("%3d  ", ary[i][k]);
		}
		printf("\n");
	}
}