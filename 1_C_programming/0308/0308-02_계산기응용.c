#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {

	int iNum1, iNum2;
	float fResult;

	printf("���� 1 ---> ");
	scanf("%d", &iNum1);
	printf("���� 2 ---> ");
	scanf("%d", &iNum2);

	// ���ϱ�
	fResult = iNum1 + iNum2;
	printf("%d + %d = %lf\n", iNum1, iNum2, fResult);

	// ���� 
	fResult = iNum1 - iNum2;
	printf("%d - %d = %lf\n", iNum1, iNum2, fResult);

	// ���ϱ�
	fResult = iNum1 * iNum2;
	printf("%d * %d = %lf\n", iNum1, iNum2, fResult);

	// ������
	fResult = (float)iNum1 / iNum2;
	printf("%d / %d = %lf\n", iNum1, iNum2, fResult);

}