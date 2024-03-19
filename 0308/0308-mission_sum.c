#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {

	int iSum = 0;

	for (int i = 12345; i < 67891; i++)
		if (i % 1398 == 0) iSum += i;

	printf("12345부터 67891까지 1398의 배수 합 ---> %d\n", iSum);

}