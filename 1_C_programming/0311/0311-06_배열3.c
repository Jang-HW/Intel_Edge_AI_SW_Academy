#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

#define height1 5
#define weight1 7

void main() {
	// 5*7 �迭 �غ�, 1, 2, 3, ... �� ä���
	// �迭 ���� ��� 2��� �ϱ�
	// �迭�� ����ϱ� 

	int iAry[height1][weight1];
	int iCount = 1;

	// (initalize)
	for (int i = 0; i < height1; i++) {
		for (int k = 0; k < weight1; k++) {
			iAry[i][k] = iCount;
			iCount++;
		}
	}

	// (calculate)
	for (int i = 0; i < height1; i++) {
		for (int k = 0; k < weight1; k++) {
			iAry[i][k] *= 2;
		}
	}

	// (print)
	for (int i = 0; i < height1; i++) {
		for (int k = 0; k < weight1; k++) {
			printf("%2d ", iAry[i][k]);
		}
		printf("\n");
	}

}