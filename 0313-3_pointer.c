#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI 지원기능

void main() {

	int a = 100;
	int b = 200;

	int aa[3] = { 10, 20, 30 };

	printf("변수 a 주소: %x\n", &a);
	printf("변수 b 주소: %x\n", &b);
	printf("\n");
	
	// 10 = 2048 (XXXX)
	// aa = 2048

	aa[0] = 111;

	printf("변수 aa : %x\n", &aa);
	printf("변수 aa+0 : %x\n", aa + 0);
	printf("변수 aa[0] 주소: %x\n", &aa[0]);
	printf("\n");
	printf("변수 aa+1 : %x\n", aa + 1);
	printf("변수 aa[1] 주소: %x\n", &aa[1]);
	printf("\n");

	char ch;		// 문자형 변수 선언
	char* p;		// 문자형 포인터 변수 선언
	char* q;

	//printf("ch 값: %c\n", ch);
	//printf("ch 주소: %x\n", &ch);
	//printf("p 값: %x\n", p);
	//printf("p가 가리키는 곳의 값: %c\n", *p);
	//printf("\n");

	ch = 'A';
	p = &ch;
	q = p;
	*q = 'Z';
	printf("ch값 : %c \n", *q);
	printf("\n");


	// 포인터로 두 값 교환하기

	char ch1 = 'A', ch2 = 'Z', tmp;
	// char* q, * q;

	p = &ch1;
	q = &ch2;
	printf("before\n");
	printf("*p: %c\n", *p);
	printf("*q: %c\n", *q);

	tmp = *p;
	*p = *q;
	*q = tmp;
	printf("after\n");
	printf("*p: %c\n", *p);
	printf("*q: %c\n", *q);
	printf("\n");


	// 배열과 포인터 
	char s[8] = "Basic-C";		// 문자열의 마지막은 \0 있음
								// 따라서 7글자여도 한 칸이 더 있어야함
	// char* p;
	p = s;

	printf("%c \n", *(p + 2));

	for (int i = 0; i < 8; i++)
		printf("%c ", *(p + i));
	printf("\n");

}