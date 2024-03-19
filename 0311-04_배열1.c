#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

void main() {

	// 배열? 여러 개의 변수를 같은 이름으로 부르는것
	// 5개짜리 배열에 100부터 104까지 저장하기
	/*
	const int SIZE = 5;

	// int SIZE = 5;
	// int ary[SIZE]; 하면 편할거같은데 오류
	// 왜? 상수가 아니라서
	int ary[5];
	int num = 100;

	for (int i = 0; i < SIZE; i++) {
		// ary[i] = 100 + i; 이렇게 하지말고 
		//	i는 첨자로만 쓰도록하는게 가독성 좋음
		ary[i] = num;
		num++;
	}
	*/


	// 퀴즈: 배열 10개를 준비하고 5부터 5의 배수를 대입하기
								// 배열의 합계를 구하시오 
	/*
	// 가장 전형적인 형태
	// 준비
	const int SIZE = 10;
	int ary[10];

	int iSum = 0;
	int iNum_mulFive = 5;

	// 채우기
	for (int i = 0; i < SIZE; i++) {
		ary[i] = iNum_mulFive;
		iNum_mulFive += 5;
		printf("%d ", ary[i]);
	}

	// 활용
	for (int i = 0; i < SIZE; i++) {
		iSum += ary[i];
	}

	printf("\n배열의 합 - %d \n", iSum);
	*/



	// 퀴즈: 배열 20개 준비 > 2부터 소수를 20개 저장하기 & 배열 내용 출력 
	

	#define SIZE 20

	// (0) 배열 준비하기
	int ary[SIZE];
	int iCount = 0;

	// (1) 배열 채우기
	int num = 2;

	while (1) {
		bool sosuNum = true;

		for (int div = 2; div < num; div += 1) {
			if (num % div == 0) {
				sosuNum = false;
				break;
			}
		}
		if (sosuNum) {
			ary[iCount] = num;
			iCount++;
		}
		if (iCount >= 20) break;

		num++;
	}

	for (int i = 0; i < SIZE; i++) {
		printf("%4d ", ary[i]);
	}
}