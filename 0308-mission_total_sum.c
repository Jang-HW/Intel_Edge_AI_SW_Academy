#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {

	int sum = 0;

	for (int i = 1; i < 101; i++)
		sum += i;

	printf("1부터 100까지의 합 ---> %d\n", sum);
	
}