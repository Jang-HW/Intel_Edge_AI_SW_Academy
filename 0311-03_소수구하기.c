#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

void main() {

	// 2부터 1000까지의 소수 출력하기
	// 소수? 1과 자기 자신 이외에는 나누어 떨어지지 않는 수 

	int hap = 0;
	printf("%4d ", 2);
	hap += 2;

	for (int num = 3; num <= 1000; num += 2) {
		// 일단 num을 소수로 가정
		bool sosuNum = true;

		for (int div = 2; div < num; div++) {	// 2부터 num 앞에 숫자까지
			if (num % div == 0) {	// 나누어 떨어지니?
				sosuNum = false;
			}
		}
		if (sosuNum) {
			printf("%4d ", num);
			hap += num;
		}
	}
	printf("\n2부터 1000까지의 소수의 합: %d\n\n", hap);


	// 퀴즈: 더 효율적으로
	hap = 0;
	printf("%4d ", 2);
	hap += 2;

	for (int num = 3; num <= 1000; num += 2) {
		// 일단 num을 소수로 가정
		bool sosuNum = true;

		for (int div = 3; div < num / 2 + 1; div += 2) {	// 3부터 num 앞에 숫자까지
			if (num % div == 0) {	// 나누어 떨어지니?
				sosuNum = false;
				break;
			}
		}
		if (sosuNum) {
			printf("%4d ", num);
			hap += num;
		}
	}
	printf("\n2부터 1000까지의 소수의 합: %d\n\n", hap);


	// 퀴즈: while문 
	hap = 0;
	printf("%4d ", 2);
	hap += 2;
	int num = 3;

	while (num <= 1000) {
		// 일단 num을 소수로 가정
		bool sosuNum = true;
		int div = 3;		// 3부터 

		while (div < num) {	// num 앞에 숫자까지
			if (num % div == 0) {	// 나누어 떨어지니?
				sosuNum = false;
			}
			div += 2;
		}
		if (sosuNum) {
			printf("%4d ", num);
			hap += num;
		}
		num += 2;
	}
	printf("\n2부터 1000까지의 소수의 합: %d\n\n", hap);
}
