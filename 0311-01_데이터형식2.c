#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {

	// float, double 자리수 보기
	/*
	float fVal = 12345678.901234567890;
	double dVal = 12345678.901234567890;

	printf("%.20f\n", fVal);
	printf("%.20f\n", dVal);
	*/

	// char 문자형, 상수형 교환
	char ch1 = 100;
	char ch2 = 'A';

	printf("%d %d\n", ch1, ch2);
	printf("%c %c\n", ch1, ch2);
	printf("%d\n", ch1+ch2);

}