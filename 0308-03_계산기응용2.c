#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {

	int iNum1, iNum2;
	float fResult;

	printf("숫자 1 ---> ");
	scanf("%d", &iNum1);
	printf("숫자 2 ---> ");
	scanf("%d", &iNum2);

	// 더하기
	fResult = iNum1 + iNum2;
	printf("%d + %d = %lf\n", iNum1, iNum2, fResult);

	// 빼기 
	fResult = iNum1 - iNum2;
	printf("%d - %d = %lf\n", iNum1, iNum2, fResult);

	// 곱하기
	fResult = iNum1 * iNum2;
	printf("%d * %d = %lf\n", iNum1, iNum2, fResult);

	// 나누기
	if(iNum2 == 0)	{
		printf("0은 나누기가 안되요!\n");
	}
	else {
		fResult = (float)iNum1 / iNum2;
		printf("%d / %d = %lf\n", iNum1, iNum2, fResult);
	}

}