#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/////////////////
// ���� ������ //
/////////////////
FILE* rfp;
char fileName[200] = "D:/RAW/Etc_Raw(squre)/LENNA512.raw";
char fileMask[200] = "D:/RAW/Etc_Raw(squre)/circle512.raw";
#define height 512
#define width 512

// �޸� Ȯ��
unsigned char inImage[height][width], outImage[height][width], mask[height][width];


/////////////////
// �Լ� ����� //
/////////////////
// ����
void loadImage();
int getIntValue();
void printImage();

// ����ó�� �Լ�
void equalImage();		
void addImage(int);
void minusImage(int);
void multiImage(int);
void divImage(int);
void reverseImage();
void monoImage();
void gammaImage(int);
void paraImage();
void monoAvgImage();
void monoMidImage();
void multiRapImage(int);
void maskAndImage();
void maskOrImage();
void maskXorImage();
void notImage();
void constrastStratchImage();
void constrastZipImage();
void posterizeImage();
void areaStressImage();


void main() {
	// �̹��� ���� > �޸� (�ε�)
	loadImage();

	// ����ó�� �˰��� ����
	// (1) ����ϱ� (= ���ϱ�)
	printf("��� �ϱ� ");
	int value = getIntValue();
	addImage(value);

	// (2) ��Ӱ��ϱ�
	printf("��Ӱ� �ϱ� ");
	value = getIntValue();
	minusImage(value);

	// (3) ����
	printf("���� \n");
	reverseImage();

	// (4) ��� (128)
	printf("��� \n");
	monoImage();

	// ���� 
	printf("���� ��ȯ (1~255 ������ ��) ");
	value = getIntValue();
	if (value < 1 || value > 255) {
		printf("- ���� ������ ��� �н��մϴ�. \n");
	}
	else	gammaImage(value);

	// �Ķ󺼶�
	printf("�Ķ󺼶� ��ȯ \n");
	paraImage();

	// ��� (��հ�)
	printf("���(��հ�) ��ȯ - ");
	monoAvgImage();

	// ��� (�߾Ӱ�)
	printf("���(�߾Ӱ�) ��ȯ - ");
	monoMidImage();

	// ���ϱ�
	printf("�� ��� �ϱ� (���)");
	value = getIntValue();
	multiImage(value);

	// ������
	printf("�� ��Ӱ� �ϱ� (���)");
	value = getIntValue();
	divImage(value);

	// ���ϱ� + ���α��
	printf("�� ��� �ϱ� (���) + ���α��");
	value = getIntValue();
	multiRapImage(value);

	// ���� AND ����ũ �������� 
	printf("���� ����ũ ��������(AND) \n");
	maskAndImage();

	// ���� OR ����ũ �������� 
	printf("���� ����ũ ��������(OR) \n");
	maskOrImage();

	// ���� XOR ����ũ �������� 
	printf("���� ����ũ ��������(XOR) \n");
	maskXorImage();

	// NOT ���� ��Ű�� 
	printf("NOT ���� ��Ű�� \n");
	notImage();

	// ��� ��� ��Ʈ��Ī ��Ű�� 
	printf("��� ��� ��Ʈ��Ī ��Ű�� \n");
	constrastStratchImage();

	// ��� ��� ���� ��Ű�� 
	printf("��� ��� ���� ��Ű�� \n");
	constrastZipImage();

	// �����Ͷ���¡ ��Ű�� 
	printf("�����Ͷ���¡ ��Ű�� \n");
	posterizeImage();

	// ���� ����
	printf("���� ���� ��Ű�� \n");
	areaStressImage();

}


/////////////////
// �Լ� ���Ǻ� //
/////////////////

/////////// 
// �����Լ�
///////////
void loadImage() {
	rfp = fopen(fileName, "rb");
	fread(inImage, sizeof(unsigned char), height * width, rfp);
	fclose(rfp);

	rfp = fopen(fileMask, "rb");
	fread(mask, sizeof(unsigned char), height * width, rfp);
	fclose(rfp);

	equalImage();
}

int getIntValue() {
	int retValue;

	printf("������:  ");
	scanf("%d", &retValue);
	return retValue;
}

void printImage() {
	for (int i = height / 2 - 5; i < height / 2 + 5; i++) {
		for (int j = width / 2 - 5; j < width / 2 + 5; j++) {
			printf("%3d ", outImage[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/////////// 
// ����ó�� �˰��� �Լ�
///////////
void equalImage() {
	// ���Ͽ��� �˰���
	// �Է¹迭 --> ��¹迭 �����ϱ�
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}

void addImage(int val) {
	// ���
	// �Է¹迭 --> ���ؼ� --> ��¹迭
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] + val > 256) outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] + val;
		}
	}
	printImage();
}

void minusImage(int val) {
	// ��Ӱ�
	// �Է¹迭 --> ���� --> ��¹迭
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] - val < 0) outImage[i][k] = 0;
			else outImage[i][k] = inImage[i][k] - val;
		}
	}
	printImage();
}

void multiImage(int val) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] * val > 255) outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] * val;
		}
	}
	printImage();
}

void divImage(int val) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] / val;
		}
	}
	printImage();
}

void reverseImage() {
	// ����
	// �Է¹迭 --> 255���� ���� --> ��¹迭
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}

void monoImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < 128)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void gammaImage(int val) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = pow((float)inImage[i][k], 1.f / val);
		}
	}
	printImage();
}

void paraImage() {
	printf("- CAPȿ�� \n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = 255.f * pow((inImage[i][k] / 127.f) - 1, 2.f);
		}
	}
	printImage();

	printf("- CUPȿ�� \n");
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = 255.f - 255.f * pow((inImage[i][k] / 127.f) - 1, 2.f);
		}
	}
	printImage();
}

void monoAvgImage() {
	int cMid = 0;
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			cMid += inImage[i][k];
		}
	}
	cMid = cMid / (height * width);
	printf("��հ�: %d\n", cMid);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void monoMidImage() {
	int cMid = 0, count = 0;
	unsigned short cValCount[256] = { 0, };

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			cValCount[inImage[i][k]] += 1;
		}
	}		
	for (int i = 0; i < 256; i++) {
		count += cValCount[i];
		if (count > (width * height) / 2) {
			cMid = i;
			break;
		}
	}
	
	// �˰��� ã�Ƽ� ä��� 
	// quick sort result
	printf("�߾Ӱ�: %d\n", cMid);

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void multiRapImage(int val) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = (inImage[i][k] * val) % 256;
		}
	}
	printImage();
}

void maskAndImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] & mask[i][k];
		}
	}
	printImage();
}

void maskOrImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] | mask[i][k];
		}
	}
	printImage();
}

void maskXorImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] ^ mask[i][k];
		}
	}
	printImage();
}

void notImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = ~inImage[i][k];
		}
	}
	printImage();
}

void constrastStratchImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < 50)			outImage[i][k] = 0;
			else if (inImage[i][k] > 200)	outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] * 51 / 30 - 85;
		}
	}
	printImage();
}

void constrastZipImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k] * 30 / 51 + 50;
		}
	}
	printImage();
}

void posterizeImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			switch (inImage[i][k] / 32) {
			case 0: outImage[i][k] = 0;		break;
			case 1: outImage[i][k] = 32;	break;
			case 2: outImage[i][k] = 64;	break;
			case 3: outImage[i][k] = 96;	break;
			case 4: outImage[i][k] = 128;	break;
			case 5: outImage[i][k] = 160;	break;
			case 6: outImage[i][k] = 192;	break;
			case 7: outImage[i][k] = 224;	break;
			case 8: outImage[i][k] = 255;	break;
			}
		}
	}
	printImage();
}

void areaStressImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] < 75)			outImage[i][k] = inImage[i][k];
			else if (inImage[i][k] > 175)		outImage[i][k] = inImage[i][k];
			else 						outImage[i][k] = 255;
		}
	}
	printImage();
}