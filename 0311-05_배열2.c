#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

#define height 3
#define weight 5

void main() {
	
	// 2���� �迭
	// 
	// (0) �迭 �غ� 
	int ary[height][weight];

	// (1) �迭 �� �ҷ����� (���߿��� �׸� ������ �о����)
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			ary[i][k] = 100;
		}
	}
	
	// (2) �迭 ó���ϱ� (Ȱ���ϱ�)
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			ary[i][k] += 10;		// ������ ���, 10��ŭ ��� ó�� <<< ���� ó�� �˰���
		}
	}

	// (3) �迭 ��� (=������ ȭ�鿡 ���̱�)
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			printf("%3d  ", ary[i][k]);
		}
		printf("\n");
	}
}