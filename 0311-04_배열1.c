#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

void main() {

	// �迭? ���� ���� ������ ���� �̸����� �θ��°�
	// 5��¥�� �迭�� 100���� 104���� �����ϱ�
	/*
	const int SIZE = 5;

	// int SIZE = 5;
	// int ary[SIZE]; �ϸ� ���ҰŰ����� ����
	// ��? ����� �ƴ϶�
	int ary[5];
	int num = 100;

	for (int i = 0; i < SIZE; i++) {
		// ary[i] = 100 + i; �̷��� �������� 
		//	i�� ÷�ڷθ� �������ϴ°� ������ ����
		ary[i] = num;
		num++;
	}
	*/


	// ����: �迭 10���� �غ��ϰ� 5���� 5�� ����� �����ϱ�
								// �迭�� �հ踦 ���Ͻÿ� 
	/*
	// ���� �������� ����
	// �غ�
	const int SIZE = 10;
	int ary[10];

	int iSum = 0;
	int iNum_mulFive = 5;

	// ä���
	for (int i = 0; i < SIZE; i++) {
		ary[i] = iNum_mulFive;
		iNum_mulFive += 5;
		printf("%d ", ary[i]);
	}

	// Ȱ��
	for (int i = 0; i < SIZE; i++) {
		iSum += ary[i];
	}

	printf("\n�迭�� �� - %d \n", iSum);
	*/



	// ����: �迭 20�� �غ� > 2���� �Ҽ��� 20�� �����ϱ� & �迭 ���� ��� 
	

	#define SIZE 20

	// (0) �迭 �غ��ϱ�
	int ary[SIZE];
	int iCount = 0;

	// (1) �迭 ä���
	int num = 2;

	while (1) {
		bool sosuNum = true;

		for (int div = 2; div < num; div += 1) {
			if (num % div == 0) {
				sosuNum = false;
				break;
			}
		}
		if (sosuNum) {
			ary[iCount] = num;
			iCount++;
		}
		if (iCount >= 20) break;

		num++;
	}

	for (int i = 0; i < SIZE; i++) {
		printf("%4d ", ary[i]);
	}
}