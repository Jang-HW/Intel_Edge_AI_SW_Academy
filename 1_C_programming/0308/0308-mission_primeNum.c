#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {

	int iSum = 0;
	bool bPass = false;

	for (int i = 2; i < 1001; i++) {

		for (int j = 2; j < i; j++) {
			if (i % j == 0)		bPass = true;
			if (bPass)			break;
		}

		if (!bPass) {
			iSum += i;
		}
		bPass = false;
	}
		

	printf("1���� 1000���� �Ҽ��� �� ---> %d\n", iSum);

}