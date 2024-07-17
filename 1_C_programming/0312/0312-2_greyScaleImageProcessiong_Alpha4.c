#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>


// (0) ������ ������ ����*���� �ľ��ϱ�
#define height 5
#define width 5

// (1) �޸� �غ� 
unsigned char image[height][width];

// �Լ��� ����� + ���Ǻ�
void loadImage() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image[i][j] = rand() % 256;
		}
	}
}

void printImage() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf("%3d ", image[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void reverseImage() {
	printf("����\n");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image[i][j] = 255 - image[i][j];
		}
	}
	printImage();
}

void addImage(int val){

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (image[i][j] + val < 255)	image[i][j] += val;
			else image[i][j] = 255;
		}
	}
	printImage();
}

void brightImage(){
	printf("��� (50)\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] + 50 < 256) image[i][k] += 50;
			else image[i][k] = 255;
		}
	}
	printImage();
}

void darkImage() {
	printf("��Ӱ� (50)\n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] - 50 > 0) image[i][k] -= 50;
			else image[i][k] = 0;
		}
	}
	printImage();
}

void monoImage() {
	printf("��հ����� ��� - ");
	int cMid = 0;
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			cMid += image[i][k];
		}
	}
	cMid = cMid / (height * width);
	printf("(��հ�: %d)\n", cMid);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (image[i][k] < cMid)
				image[i][k] = 0;
			else image[i][k] = 255;
		}
	}
	printImage();
}




void main() {
	// ���� �ʱ�ȭ
	srand((unsigned int)time(0));


	// (2) �̹��� ���� �غ� ---> 2���� �迭�� loading
	loadImage();

	//		���� ���
	printf("����\n");
	printImage();

	// (3-1) ���� ����ó��
	reverseImage();

	// (3-2) ���� ��� ����
	int value;
	printf("�󸶳� ��� �ұ��? ");	 scanf("%d", &value);
	if (value > 0)	addImage(value);
	else printf("������ �Ѿ�ϴ�!\n\n");


	// ����: ���� ������ �̼��� �Լ�ȭ
	//			���
	brightImage();
	 
	//			��Ӱ�
	darkImage();

	//			���� 
	reverseImage();
	 
	//			���
	monoImage();
}