#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <random>



void main() {
	srand((unsigned int) time(0));	// �ð� �ʱ�ȭ

	 // (0) ������ ������ ũ�⸸ŭ �޸� Ȯ�� (jolly.raw: 10*10 ũ���� �̹���)
	#define height 10
	#define weight 10


	// (1) 2���� �迭(=�޸�)�� �غ�
	unsigned char image[height][weight];	// int�� �ϸ� �뷮 ���� ���ϴϱ�
	

	// (2) �̹��� ���� �о 2���� �޸𸮷� �ε�(Loading /
	//											  Disk�� �ڷḦ RAM���� ��������)
	
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			image[i][k] = rand() % 256;
		}
	}

	// ���� �̹��� ���
	printf("���� �̹���\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			printf("%3d ", image[i][k]);
		}
		printf("\n");
	}
	printf("\n\n");

	// (3) ����ó�� ȿ�� (Image Processing) ===> ����ó�� �˰��� ����
	//// (3-1) ��� �ϱ� �˰��� (50 ����)
	//printf("����� �̹���\n");
	//for (int i = 0; i < height; i++) {
	//	for (int k = 0; k < weight; k++) {
	//		if (image[i][k] + 50 < 256) image[i][k] += 50;
	//		else image[i][k] = 255;
	//	}
	//}
	
	// (3-2) ��Ӱ� �ϱ� �˰��� (50 ����)
	printf("��Ӱ� �� �̹���\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			if (image[i][k] - 50 > 0) image[i][k] -= 50;
			else image[i][k] = 0;
		}
	}

	// ��� �̹��� ���
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < weight; k++) {
			printf("%3d ", image[i][k]);
		}
		printf("\n");
	}
}