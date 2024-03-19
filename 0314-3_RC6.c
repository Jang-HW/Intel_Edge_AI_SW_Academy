#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI �������
#include <math.h>

/////////////////////
// ���� ������
/////////////////////
HWND hwnd;		// ������ ȭ�� (�ܺ�, ������)
HDC hdc;		// ������ ȭ�� (����, ����)

FILE* rfp, * wfp;
char fileName[200];
char fileMask[200] = "D:/RAW/Etc_Raw(squre)/circle512.raw";
int inH, inW;
int outH, outW;
// �޸� Ȯ��
// unsigned char inImage[height][width], outImage[height][width];
unsigned char** inImage = NULL, ** outImage = NULL, mask[512][512];

/////////////////////
// �Լ� �����
/////////////////////
void	loadImage();
int		getIntValue();
void	printImage();
void	printMenu();
void	saveImage();
void	freeInputMemory();
void	freeOutputMemory();
void	mallocInputMemory();
void	mallocOutputMemory();

// ȭ���� ó��
void	equalImage();
void	addImage();
void	minusImage();
void	reverseImage();
void	monoImage();
void	posterizeImage();
void	areaStressImage();
void	moveVerticle();
void	moveHorizon();
void	mirror();

// ������
void zoomOut();		// �⺻
void zoomOut2();	// ������� ���̱�
void zoomOut3();	// �߾Ӱ����� ���̱�
void zoomIn();		// ������ ���
void zoomIn2();		// �̿�ȭ�� ������
void zoomIn3();		// ���� ������
void rotate();
void rotate2();

int static compare(const void* first, const void* second)
{
	if (*(int*)first > *(int*)second)
		return 1;
	else if (*(int*)first < *(int*)second)
		return -1;
	else
		return 0;
}


void main() {
	char inKey = 0;

	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL);		// win10�� hwnd, 11�� NULL

	while (inKey != '9') {
		printMenu();
		inKey = _getch();
		system("cls");		// windows ��ɾ�

		switch (inKey) {
		case '0':	loadImage();							break;
		case '1':	saveImage();							break;
		case '9':											break;

		case 'A': case 'a':	equalImage();					break;
		case 'B': case 'b':	addImage();						break;
		case 'C': case 'c':	minusImage();					break;
		case 'D': case 'd':	reverseImage();					break;
		case 'E': case 'e':	monoImage();					break;

		case 'N': case 'n':	zoomOut();						break;
		case 'O': case 'o':	zoomOut2();						break;
		case 'P': case 'p': zoomOut3();						break;
		case 'Q': case 'q': zoomIn();						break;
		case 'R': case 'r': zoomIn2();						break;
		case 'S': case 's': zoomIn3();						break;
		case 'T': case 't': rotate();						break;
		case 'U': case 'u': rotate2();						break;
		case 'V': case 'v': moveVerticle();					break;
		case 'W': case 'w': moveHorizon();					break;
		case 'X': case 'x': mirror();						break;


		default:	printf("�ٸ� ���ڸ� �Է��� �ּ���!\n");	break;
		}

	}
	freeInputMemory();
	freeOutputMemory();
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
	puts("\n N. �� �ƿ�\tO. �� �ƿ�(���)\tP. �� �ƿ�(�߾�)\tQ. ����(������)");
	puts("\n R. �� ��2(�����)\tS. �� ��3(����)\tT. ȸ��1\tU. ȸ��2");
	puts("\n V. ���� �̵�\tW. ���� �̵� \tX. �̷���");
}

void printImage() {
	system("cls");
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 900, i + 50, RGB(px, px, px));
		}
	}
}

void loadImage() {
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("���ϸ�-->"); // flower512, LENA256
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// (�߿�!) �̹����� ���� ���̸� ����
	rfp = fopen(fileName, "rb");
	// ���� ����� load ����
	if (rfp == NULL) {
		printf("������ �������� �ʽ��ϴ�. �ٸ� �̸����� �õ��� �ּ���.\n\n");
		return;
	}

	fseek(rfp, 0L, SEEK_END); // ������ ������ �̵�
	long long fsize = ftell(rfp); // �� ���������?  262,144  --> 512
	fclose(rfp);
	// �޸� ����
	freeInputMemory();
	// �Է� ������ ����, �� ���ϱ�
	inH = inW = (int)sqrt(fsize);
	// �޸� �Ҵ�
	mallocInputMemory();
	// ���� --> �޸�
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < inH; i++)
		fread(inImage[i], sizeof(unsigned char), inW, rfp);
	fclose(rfp);

	equalImage();

	rfp = fopen(fileMask, "rb");
	fread(mask, sizeof(unsigned char), 512 * 512, rfp);
	fclose(rfp);
}

int  getIntValue() {
	int retValue;
	printf("������-->"); scanf("%d", &retValue);
	return retValue;
}

void saveImage() {
	if (outImage == NULL)	 return;

	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	printf("���ϸ�-->"); // out01, out02......
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	// ������ ���� ����Ȯ��
	rfp = fopen(fileName, "rb");
	char inKey = 0;
	if (rfp != NULL) {
		printf("������ �̸��� ������ �����մϴ�. \n����ðڽ��ϱ�?(y/n) ");
		inKey = _getch();

		while (inKey != 'y' && inKey != 'Y') {
			switch (inKey) {
			case 'N':case 'n': printf("\n������� �ʾҽ��ϴ�. \n�̸��� �ٲ� �õ��� �ּ���.\n");	return;
			case 'Y': case'y': printf("\n����⸦ �����մϴ�. \n"); break;
			default:		   printf("\nŰ�� �߸� �Է��ϼ̽��ϴ�.\n");
			}
		}
	}

	wfp = fopen(fileName, "wb");

	// �޸� --> ���� (���྿)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);

	// MessageBox(hwnd, L"���� �Ϸ�", L"���� â", NULL);
	printf("���� �Ϸ�\n");
}

void freeInputMemory() {
	if (inImage == NULL)
		return;
	for (int i = 0; i < inH; i++)
		free(inImage[i]);
	free(inImage);

	inImage = NULL;
}

void freeOutputMemory() {
	if (outImage == NULL)
		return;
	for (int i = 0; i < outH; i++)
		free(outImage[i]);
	free(outImage);

	outImage = NULL;
}

void mallocInputMemory() {
	inImage = (unsigned char**)malloc(sizeof(unsigned char*) * inH);
	for (int i = 0; i < inH; i++)
		inImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * inW);
}

void mallocOutputMemory() {
	outImage = (unsigned char**)malloc(sizeof(unsigned char*) * outH);
	for (int i = 0; i < outH; i++)
		outImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * outW);
}

//////////////////
//  ����ó�� �Լ�
//////////////////

void equalImage() { // ���Ͽ��� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
	printImage();
}

void addImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();


	// �Է� �迭 --> ��� �迭
	int val = getIntValue();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] + val < 255)
				outImage[i][k] = inImage[i][k] + val;
			else
				outImage[i][k] = 255;
		}
	}
	printImage();
}

void minusImage() {
	// ��Ӱ�
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է¹迭 --> ���� --> ��¹迭
	int val = getIntValue();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] - val < 0) outImage[i][k] = 0;
			else outImage[i][k] = inImage[i][k] - val;
		}
	}
	printImage();
}

void reverseImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// ����
	// �Է¹迭 --> 255���� ���� --> ��¹迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}

void monoImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < 128)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

// �⺻
void zoomOut() { // ��� �˰���
	// �� �ƿ� ���
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		if (outH <= i / scale) break;

		for (int k = 0; k < inW; k++) {
			if (outW <= k / scale) break;
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}

// ������� ���̱�
void zoomOut2() { 
	// ��� �˰��� (�� �ȼ��� ��ġ�� �κе��� �߾Ӱ����� �����ϴ� ��)
	// �� �ƿ� ���
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	mallocOutputMemory();

	int** tmp;
	tmp = (int**)malloc(sizeof(int*) * outH);
	for (int i = 0; i < outH; i++)
		tmp[i] = (int*)malloc(sizeof(int) * outW);

	// �Է� �迭 --> �ӽ� �迭
	for (int i = 0; i < inH; i++) {
		if (outH <= i / scale) break;

		for (int k = 0; k < inW; k++) {
			if (outW <= k / scale) break;
			tmp[(int)(i / scale)][(int)(k / scale)] += inImage[i][k];
		}
	}

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = (int)(tmp[i][k] / scale);
		}
	}

	for (int i = 0; i < outH; i++)
		free(tmp[i]);
	free(tmp);

	// �ӽ� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[(int)(i / scale)][(int)(k / scale)] = inImage[i][k];
		}
	}
	printImage();
}

// �߾Ӱ����� ���̱�
void zoomOut3(){
	// �߾Ӱ����� ���̱�
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = (int)(inH / scale);
	outW = (int)(inW / scale);
	mallocOutputMemory();

	int* tmp;
	tmp = (int*)malloc(sizeof(int) * scale * scale);
	int num = 0;

	// �Է� �迭 --> �ӽ� �迭
	for (int i = 0; i < inH / scale; i++) {					// y������ ��� ����
		for (int k = 0; k < inW / scale; k++) {				// x������ ��� ����
			for (int m = 0; m < scale; m++) {					// ��� ���� y��
				for (int n = 0; n < scale; n++) {				// ��� ���� x��
					// �� ���� ĭ�� �� inImage[scale*i + m][scale*k + n]
					tmp[num] = inImage[scale * i + m][scale * k + n];
					num++;
				}
				printf("\n");
			}
			num = 0;
			qsort(tmp, sizeof(tmp)/sizeof(tmp[0]), sizeof(tmp[0]), compare);
			outImage[i][k] = tmp[scale * scale / 2];
		}
	}
	free(tmp);
	printImage();
}

// ������ ���
void zoomIn() { // Ȯ�� �˰���
	// �� �� ���
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i * scale][k * scale] = inImage[i][k];
		}
	}
	printImage();
}

// �̿�ȭ�� ������
void zoomIn2() { 
	// Ȯ�� �˰���2222
	// �� �� ���
	int scale = getIntValue();

	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = (int)(inH * scale);
	outW = (int)(inW * scale);
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImage[i][k] = inImage[i / scale][k / scale];
		}
	}
	printImage();
}

// ���� ������
void zoomIn3() { }

void rotate() { 
	// ȸ�� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	int degree = getIntValue();
	degree %= 360;
	double radian = degree * 3.1415925386 / 180.0;
	// xd = cos*xs - sin*ys
	// yd = sin*xs + cos*ys


	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			int xs = i;
			int ys = k;

			int	xd = (int)(cos(radian) * xs - sin(radian) * ys);
			int yd = (int)(sin(radian) * xs + cos(radian) * ys);

			if (0 <= xd && xd < outH && 0 <= yd && yd < outW)
				outImage[xd][yd] = inImage[xs][ys];
		}
	}
	printImage();
}

void rotate2() { // ȸ�� �˰��� + �߾� �����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	int degree = getIntValue();
	degree %= 360;
	double radian = degree * 3.1415925386 / 180.0;
	//// xd = cos*xs - sin*ys
	//// yd = sin*xs + cos*ys

	int cx = inH / 2;
	int cy = inW / 2;

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;

			int	xs = (int)(cos(radian) * (xd - cx) + sin(radian) * (yd - cx));
			int ys = (int)(-sin(radian) * (xd - cx) + cos(radian) * (yd - cx));
			xs += cx;
			ys += cy;

			if (0 <= xs && xs < outH && 0 <= ys && ys < outW)
				outImage[xd][yd] = inImage[xs][ys];
		}
	}
	printImage();
}

void moveVerticle() {
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// ������ ����
	int val = getIntValue();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		if (i + val >= outH)	break;
		if (i + val < 0)		break;

		for (int k = 0; k < inW; k++) {
			outImage[i + val][k] = inImage[i][k];
		}
	}
	printImage();
}

void moveHorizon() {
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// ������ ����
	int val = getIntValue();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (k + val < 0 & k + val >= outH)	break;
			outImage[i][k + val] = inImage[i][k];
		}
	}
	printImage();
}

void mirror() {
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = 2*inH;		outW = 2*inW;
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			printf("%d %d\n", i, k);
			outImage[i][k] = inImage[i][k];
			outImage[2 * inH - i - 1][k] = inImage[i][k];
			outImage[i][2 * inW - k - 1] = inImage[i][k];
			outImage[2 * inH - i - 1][2 * inW - k - 1] = inImage[i][k];
		}
	}
	printImage();
}