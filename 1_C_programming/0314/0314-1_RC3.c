#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h> // GUI ���� ���
#include <math.h>

/////////////////////
// ���� ������
/////////////////////
HWND hwnd; // ���� ȭ��(�ܺ�, ������)
HDC hdc; // ���� ȭ�� (����, ������)

FILE* rfp, * wfp;
char fileName[200];
int inH, inW;
int outH, outW;
// �޸� Ȯ��
// unsigned char inImage[height][width], outImage[height][width];
unsigned char** inImage = NULL, ** outImage = NULL;


/////////////////////
// �Լ� �����
/////////////////////
void loadImage();			int getIntValue();			void printImage();			void printMenu();
void saveImage();			void freeInputMemory();		void freeOutputMemory();	void mallocInputMemory();				
void mallocOutputMemory();

void equalImage(); void addImage(); void subImage(); void reverseImage();
void bwImage();


void main() {
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL);  // Windows 10 : hwnd

	char inKey = 0;
	while (inKey != '9') {
		printMenu();
		inKey = _getch();
		// system("cls");
		switch (inKey) {
			case '0': loadImage(); break;
			case '1': saveImage(); break;
			case '9': break;
			case 'a': case 'A': equalImage();			break;
			case 'b': case 'B': addImage();				break;
			case 'c': case 'C': subImage();				break;
			case 'd': case 'D': reverseImage();			break;
			case 'e': case 'E': bwImage();				break;
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
	puts("\n ## GrayScale Image Processing (Beta 4) ##");
	puts("0.����  1.����  9. ����");
	puts("A.���� B.��� C.��Ӱ� D.���� E.��� ~~~~");
}

void printImage() {
	system("cls");
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 50, i + 250, RGB(px, px, px));
		}
		printf("\n");
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

	// printf("%d ", inImage[100][100]);

	equalImage();
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

	wfp = fopen(fileName, "wb");
	// �޸� --> ���� (���྿)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);
	// MessageBox(hwnd, L"���� �Ϸ�", L"���� â", NULL);
	printf("���� �Ϸ�");
}

int getIntValue() {
	int retValue;
	printf("������-->"); scanf("%d", &retValue);
	return retValue;
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
////////////////////
void equalImage() { // ���Ͽ��� �˰�����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰����� ������)
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

void addImage() { // ���ϱ� �˰�����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰����� ������)
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

void subImage() { // ���� �˰�����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰����� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	int val = getIntValue();
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] - val > 0)
				outImage[i][k] = inImage[i][k] - val;
			else
				outImage[i][k] = 0;
		}
	}
	printImage();
}

void reverseImage() { // ���� �˰�����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰����� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = 255 - inImage[i][k];
		}
	}
	printImage();
}

void bwImage() { // ��� �˰�����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰����� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] > 127)
				outImage[i][k] = 255;
			else
				outImage[i][k] = 0;
		}
	}
	printImage();
}