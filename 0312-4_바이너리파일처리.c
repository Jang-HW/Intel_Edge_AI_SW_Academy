#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

void main() {

	// �����޸𸮸� �� ���°� ����
	FILE* rfp;

	char cFileName[200] = "D:/RAW/Etc_Raw(squre)/LENNA512.raw";
	#define height 512
	#define width 512

	unsigned char image[height][width];

	// ����ó�� 3�ܰ�
	// 1. ���� ����
	rfp = fopen(cFileName, "rb");		// r: read, t: text, b: binary

	// 2. ���� �аų� ����
	// �ѹ��� 512*512 �б� 
	fread(image, 1, height * width, rfp);	// image�� 1����Ʈ�� ��ü ũ��(����*����)��ŭ rfp���� 

	// 3. ���� �ݱ�
	fclose(rfp);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			printf("%3d ", image[i][k]);
		}
		printf("\n");
	}
}