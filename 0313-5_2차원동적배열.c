#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h> // GUI 지원 기능


void main() {
	//int row = 3; 
	//int column = 5;

	//// int p[row][column];

	//int** p;

	//p = (int**)malloc(sizeof(int*) * row);
	//for (int i = 0; i < row; i++) {
	//	p[i] = (int*)malloc(sizeof(int) * column);
	//}

	//int num = 1;
	//for (int i = 0; i < row; i++) {
	//	for (int k = 0; k < column; k++) {
	//		p[i][k] = num;
	//		num++;
	//	}
	//}	
	//for (int i = 0; i < row; i++) {
	//	for (int k = 0; k < column; k++) {
	//		printf("%3d ", p[i][k]);
	//	}
	//	printf("\n");
	//}

	//for (int i = 0; i < row; i++) {
	//	free(p[i]);
	//}

	//free(p);



	// 퀴즈: 512*512 크기의 unsigned char 2차원 동적배열 준비 > 각 랜덤값 대입후 평균
	int row = 512, column = 512;
	long long  sum = 0;
	unsigned char** p;

	srand((unsigned int)time(0)); // 시계 초기화

	p = (unsigned char**)malloc(sizeof(unsigned char*) * row);
	for (int i = 0; i < row; i++) {
		p[i] = (unsigned char*)malloc(sizeof(unsigned char) * column);
	}
	for (int i = 0; i < row; i++) {
		for (int k = 0; k < column; k++) {
			p[i][k] = rand() % 256;
			sum += p[i][k];
		}
	}
	sum /= (row * column);
	printf("평균: %lld", sum);

	for (int i = 0; i < row; i++)
		free(p[i]);
	free(p);
		

}