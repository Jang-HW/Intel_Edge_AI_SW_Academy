#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

int plusNumber(int, int );					// �Լ��� ����: ���� �̸� �Ƚᵵ ok

void main() {

	int iNum1 = 100, iNum2 = 200;
	int iHap;

	iHap = plusNumber(iNum1, iNum2);

	printf("%d \n", iHap);

}

// �Լ��� ���Ǳ� Ŀ�� 
//	 (�� �ڽ�: ���ΰ� �Ⱥ����� ���� ������ �ƴϱ�)
// - �Ķ����:	����
// - Ŀ��:		���� �ִ°�(������ ���ٰ�)
// 
// �Լ��� ����� �ΰ��� ���
// - �Լ��� ����� ����
// 

int plusNumber(int iNum1, int iNum2) {		// �Լ��� ����
	int returnVal;
	returnVal = iNum1 + iNum2;

	return returnVal;
}