#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI �������

void main() {

	int a = 100;
	int b = 200;

	int aa[3] = { 10, 20, 30 };

	printf("���� a �ּ�: %x\n", &a);
	printf("���� b �ּ�: %x\n", &b);
	printf("\n");
	
	// 10 = 2048 (XXXX)
	// aa = 2048

	aa[0] = 111;

	printf("���� aa : %x\n", &aa);
	printf("���� aa+0 : %x\n", aa + 0);
	printf("���� aa[0] �ּ�: %x\n", &aa[0]);
	printf("\n");
	printf("���� aa+1 : %x\n", aa + 1);
	printf("���� aa[1] �ּ�: %x\n", &aa[1]);
	printf("\n");

	char ch;		// ������ ���� ����
	char* p;		// ������ ������ ���� ����
	char* q;

	//printf("ch ��: %c\n", ch);
	//printf("ch �ּ�: %x\n", &ch);
	//printf("p ��: %x\n", p);
	//printf("p�� ����Ű�� ���� ��: %c\n", *p);
	//printf("\n");

	ch = 'A';
	p = &ch;
	q = p;
	*q = 'Z';
	printf("ch�� : %c \n", *q);
	printf("\n");


	// �����ͷ� �� �� ��ȯ�ϱ�

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


	// �迭�� ������ 
	char s[8] = "Basic-C";		// ���ڿ��� �������� \0 ����
								// ���� 7���ڿ��� �� ĭ�� �� �־����
	// char* p;
	p = s;

	printf("%c \n", *(p + 2));

	for (int i = 0; i < 8; i++)
		printf("%c ", *(p + i));
	printf("\n");

}