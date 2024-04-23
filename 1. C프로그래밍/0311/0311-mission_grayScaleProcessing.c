#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

void main() {
	srand((unsigned int)time(0)); // �ð� �ʱ�ȭ

	// (0) ������ ������ ũ�⸸ŭ �޸� Ȯ�� (jolly.raw : 10x10 ũ�� �̹���)
	# define height 10  // ����
	# define width  10  // ��


	// (1) 2���� �迭(=�޸�) �غ�
	unsigned char image[height][width];
	unsigned char reversal_image[height][width];
	unsigned char blackWhite_image[height][width];


	// (2) �̹��� ���� --> 2���� �迭�� �ε�(Loading)
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			image[i][k] = rand() % 256;
		}
	}
	// ���� �̹��� ���
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			printf("%3d ", image[i][k]);
		}
		printf("\n");
	}
	printf("\n\n");
	
	// (3) ����ó�� ȿ�� ( Image Processing )  ==> ����ó�� �˰���
	// (3-1) ���� ó��
	printf("���� ó��\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			reversal_image[i][k] = 255 - image[i][k];
		}
	}
	// ��� �̹��� ���
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			printf("%3d ", reversal_image[i][k]);
		}
		printf("\n");
	}
	printf("\n\n");

	// (3-2) ��� ó��
	printf("��� ó�� - ");
	int cMid = 0;
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			cMid += image[i][k];
		}
	}
	cMid = cMid / (height * width);
	printf("��հ�(��ȭ): %d\n", cMid);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] < cMid)
				blackWhite_image[i][k] = 0;
			else blackWhite_image[i][k] = 255;
		}
	}
	// ��� �̹��� ���
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			printf("%3d ", blackWhite_image[i][k]);
		}
		printf("\n");
	}

	// (��ȭ)
	printf("\n(��ȭ�κ�)\n");

	unsigned char cMin = 255, cMax = 0;
	unsigned char minW = 0, minH = 0, maxW = 0, maxH = 0;
	unsigned char cMedium, cMany;
	short count = 0;
	unsigned char cValCount[256] = { 0, };

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] < cMin) {
				cMin = image[i][k];
				minH = i;
				minW = k;
			}
			if (image[i][k] > cMax) {
				cMax = image[i][k];
				maxH = i;
				maxW = k;
			}
			cValCount[image[i][k]] += 1;
		}
	}
	
	for (int i = 0; i < 255; i++) {
		count += cValCount[i];
		if (count > (width * height) / 2) {
			cMedium = i;
			break;
		}
	}
	count = 0;
	for (int i = 0; i < 255; i++) {
		if (count < cValCount[i]) {
			count = cValCount[i];
			cMany = i;
		}
	}

	printf("�ִ�: %d (%d, %d)\n", cMax, maxW, maxH);
	printf("�ּڰ�: %d (%d, %d)\n", cMin, minW, minH);
	printf("�߾Ӱ�: %d\n", cMid);
	printf("�ֺ�: %d (%dȸ)\n", cMany, count);
}