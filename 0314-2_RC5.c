#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
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
void	monoAvgImage();
void	monoMidImage();;
void	maskAndImage();
void	maskOrImage();
void	maskXorImage();
void	notImage();
void	constrastStratchImage();
void	constrastZipImage();
void	posterizeImage();
void	areaStressImage();

// ������
void zoomOut();
void zoomIn();		// ������ ���
void zoomIn2();		// �̿�ȭ�� ������
void zoomIn3();		// ���� ������
void rotate();
void rotate2();


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
		case 'F': case 'f':	monoAvgImage();					break;
		case 'G': case 'g':	monoMidImage();					break;
		case 'H': case 'h':	maskAndImage();					break;
		case 'I': case 'i':	maskOrImage();					break;
		case 'J': case 'j':	maskXorImage();					break;
		case 'K': case 'k':	notImage();						break;
		case 'L': case 'l':	constrastStratchImage();		break;
		case 'M': case 'm':	constrastZipImage();			break;
		case 'N': case 'n':	posterizeImage();				break;
		case 'O': case 'o':	areaStressImage();				break;

		case 'P': case 'p': zoomOut();						break;
		case 'Q': case 'q': zoomIn();						break;
		case 'R': case 'r': zoomIn2();						break;
		case 'S': case 's': zoomIn3();						break;
		case 'T': case 't': rotate();						break;
		case 'U': case 'u': rotate2();						break;

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
	puts("\n F. ���(���)\tG. ���(�߰���)\tH. AND\t\tI. OR\t\tJ. XOR	");
	puts("\n K. NOT\t\tL. ��� ��� ��Ʈ��Ī\t\tM. ��� ��� ����");
	puts("\n N.�����Ͷ���¡\t\tO.���� ����\tP.�� �ƿ�\tQ.����(������)");
	puts("\n R. �� ��2(�����)\tS. �� ��3(����)\tT. ȸ��1\tU. ȸ��2");
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

void monoAvgImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	int cMid = 0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			cMid += inImage[i][k];
		}
	}
	cMid = cMid / (inH * inW);
	printf("��հ�: %d\n", cMid);

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void monoMidImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	int cMid = 0, count = 0;
	unsigned short cValCount[256] = { 0, };

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			cValCount[inImage[i][k]] += 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		count += cValCount[i];
		if (count > (inW * inH) / 2) {
			cMid = i;
			break;
		}
	}
	printf("�߾Ӱ�: %d\n", cMid);

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
}

void maskAndImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] & mask[i][k];
		}
	}
	printImage();
}

void maskOrImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] | mask[i][k];
		}
	}
	printImage();
}

void maskXorImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] ^ mask[i][k];
		}
	}
	printImage();
}

void notImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = ~inImage[i][k];
		}
	}
	printImage();
}

void constrastStratchImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < 50)			outImage[i][k] = 0;
			else if (inImage[i][k] > 200)	outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] * 51 / 30 - 85;
		}
	}
	printImage();
}

void constrastZipImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] * 30 / 51 + 50;
		}
	}
	printImage();
}

void posterizeImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
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
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < 75)			outImage[i][k] = inImage[i][k];
			else if (inImage[i][k] > 175)		outImage[i][k] = inImage[i][k];
			else 						outImage[i][k] = 255;
		}
	}
	printImage();
}

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

void zoomIn2() { // Ȯ�� �˰���2222
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

  /////////////////////////
 /// ���������� �尡�� ///
/////////////////////////
void zoomIn3() { // Ȯ�� �˰��� 3333
	// �� �� ���
	//int scale = getIntValue();

	//// �޸� ����
	//freeOutputMemory();
	//// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	//outH = (int)(inH * scale);
	//outW = (int)(inW * scale);
	//mallocOutputMemory();

	//// �Է� �迭 --> ��� �迭
	//for (int i = 0; i < inH; i++) {
	//	for (int k = 0; k < inW; k++) {
	//		outImage[i * scale][k * scale] = inImage[i][k];
	//	}
	//}
	//for (int i = 0; i <	; i++) {
	//	for (int k = 0; k < inW; k++) {
	//		outImage[i * scale][k * scale] = inImage[i][k];
	//	}
	//}
	//printImage();
}

void rotate() { // ȸ�� �˰���
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

			if (0 <= xd && xd < outH && 0<=yd && yd < outW)
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