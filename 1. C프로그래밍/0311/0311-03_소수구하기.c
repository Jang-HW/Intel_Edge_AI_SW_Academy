#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

void main() {

	// 2���� 1000������ �Ҽ� ����ϱ�
	// �Ҽ�? 1�� �ڱ� �ڽ� �̿ܿ��� ������ �������� �ʴ� �� 

	int hap = 0;
	printf("%4d ", 2);
	hap += 2;

	for (int num = 3; num <= 1000; num += 2) {
		// �ϴ� num�� �Ҽ��� ����
		bool sosuNum = true;

		for (int div = 2; div < num; div++) {	// 2���� num �տ� ���ڱ���
			if (num % div == 0) {	// ������ ��������?
				sosuNum = false;
			}
		}
		if (sosuNum) {
			printf("%4d ", num);
			hap += num;
		}
	}
	printf("\n2���� 1000������ �Ҽ��� ��: %d\n\n", hap);


	// ����: �� ȿ��������
	hap = 0;
	printf("%4d ", 2);
	hap += 2;

	for (int num = 3; num <= 1000; num += 2) {
		// �ϴ� num�� �Ҽ��� ����
		bool sosuNum = true;

		for (int div = 3; div < num / 2 + 1; div += 2) {	// 3���� num �տ� ���ڱ���
			if (num % div == 0) {	// ������ ��������?
				sosuNum = false;
				break;
			}
		}
		if (sosuNum) {
			printf("%4d ", num);
			hap += num;
		}
	}
	printf("\n2���� 1000������ �Ҽ��� ��: %d\n\n", hap);


	// ����: while�� 
	hap = 0;
	printf("%4d ", 2);
	hap += 2;
	int num = 3;

	while (num <= 1000) {
		// �ϴ� num�� �Ҽ��� ����
		bool sosuNum = true;
		int div = 3;		// 3���� 

		while (div < num) {	// num �տ� ���ڱ���
			if (num % div == 0) {	// ������ ��������?
				sosuNum = false;
			}
			div += 2;
		}
		if (sosuNum) {
			printf("%4d ", num);
			hap += num;
		}
		num += 2;
	}
	printf("\n2���� 1000������ �Ҽ��� ��: %d\n\n", hap);
}
