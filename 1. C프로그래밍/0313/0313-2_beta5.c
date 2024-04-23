#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI �������

/////////////////////
// ���� ������
/////////////////////
HWND hwnd;		// ������ ȭ�� (�ܺ�, ������)
HDC hdc;		// ������ ȭ�� (����, ����)

FILE* rfp, * wfp;
char fileName[200];
char fileMask[200] = "D:/RAW/Etc_Raw(squre)/circle512.raw";
#define height 512
#define width 512
// �޸� Ȯ��
unsigned char inImage[height][width], outImage[height][width], mask[height][width];


/////////////////////
// �Լ� �����
/////////////////////
void loadImage(); int getIntValue(); void printImage();	void printMenu(); void saveImage();

void equalImage();
void addImage();
void minusImage();
void reverseImage();
void monoImage();
void monoAvgImage();
void monoMidImage();;
void maskAndImage();
void maskOrImage();
void maskXorImage();
void notImage();
void constrastStratchImage();
void constrastZipImage();
void posterizeImage();
void areaStressImage();

void main() {
	char inKey = 0;

	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL);		// win10�� hwnd, 11�� NULL

	while (inKey != '9') {
		printMenu();
		inKey = _getch();
		system("cls");		// windows ��ɾ�

		switch (inKey) {
		case '0':	loadImage();		break;
		case '1':	saveImage();		break;
		case '9':						break;
		case 'A':
		case 'a':	equalImage();		break;
		case 'B':
		case 'b':	addImage();			break;
		case 'C':
		case 'c':	minusImage();		break;
		case 'D':
		case 'd':	reverseImage();		break;
		case 'E':
		case 'e':	monoImage();		break;
		case 'F':
		case 'f':	monoAvgImage();		break;
		case 'G':
		case 'g':	monoMidImage();		break;
		case 'H':
		case 'h':	maskAndImage();		break;
		case 'I':
		case 'i':	maskOrImage();		break;
		case 'J':
		case 'j':	maskXorImage();		break;
		case 'K':
		case 'k':	notImage();			break;
		case 'L':
		case 'l':	constrastStratchImage();		break;
		case 'M':
		case 'm':	constrastZipImage();			break;
		case 'N':
		case 'n':	posterizeImage();	break;
		case 'O':
		case 'o':	areaStressImage();	break;

		default:	printf("�ٸ� ���ڸ� �Է��� �ּ���!\n");	 break;
		}

	}
}


/////////////////////
// �Լ� ���Ǻ�
/////////////////////
/// ���� �Լ�
//////////////
void printMenu() {
	puts("\n###Grey Scale Image Processing (Beta 4) ###");
	puts("\n 0. ����\t1. ����	\t9. ����");
	puts("\n A. ����\tB. ���\t\tC. ��Ӱ�\tD. ����\t\tE. ���(127)	");
	puts("\n F. ���(���)\tG. ���(�߰���)\tH. AND\t\tI. OR\t\tJ. XOR	");
	puts("\n K. NOT\t\tL. ��� ��� ��Ʈ��Ī\t\tM. ��� ��� ����");
	puts("\n N. �����Ͷ���¡\t\tO. ���� ����");
}

void printImage() {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 350, i + 200, RGB(px, px, px));
		}
		printf("\n");
	}
	printf("\n");
}

void loadImage() {
	char fullName[200] = "D:/RAW/Pet_RAW(squre)/Pet_RAW(512x512)/";
	char tmpName[50];
	printf("���ϸ�--> "); // cat01, dog05
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, "_512.raw");
	strcpy(fileName, fullName);

	rfp = fopen(fileName, "rb");

	if (rfp != NULL)
	{
		fread(inImage, sizeof(unsigned char), height * width, rfp);
		fclose(rfp);
		equalImage();
	}
	else {
		printf("������ ���ų� �� �� �����ϴ�. �ٸ� ���Ϸ� ������ �ּ���. \n");
	}

	rfp = fopen(fileMask, "rb");
	fread(mask, sizeof(unsigned char), height * width, rfp);
	fclose(rfp);
}

int getIntValue() {
	int retValue;
	printf("������-->"); scanf("%d", &retValue);
	return retValue;
}

void saveImage() {
	char fullName[200] = "D:/RAW/Pet_RAW(squre)/Pet_RAW(512x512)/";
	char tmpName[50];
	printf("������ ���ϸ�--> "); // out01, out02, ....
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, "_512.raw");
	strcpy(fileName, fullName);

	wfp = fopen(fileName, "wb");

	for (int i = 0; i < height; i++) {
		fwrite(outImage[i], sizeof(unsigned char), width, wfp);
	}
	fclose(wfp);
	MessageBox(hwnd, "saved", "About saving", NULL);
}

//////////////////
//  ����ó�� �Լ�
//////////////////
void minusImage() {
	// ��Ӱ�
	// �Է¹迭 --> ���� --> ��¹迭
	int val = getIntValue();

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] - val < 0) outImage[i][k] = 0;
			else outImage[i][k] = inImage[i][k] - val;
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

void equalImage() {
	// ���Ͽ��� �˰���
	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}

void addImage() { // ���ϱ� �˰���
	// �Է� �迭 --> ��� �迭
	int val = getIntValue();

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (inImage[i][k] + val < 255)
				outImage[i][k] = inImage[i][k] + val;
			else
				outImage[i][k] = 255;
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