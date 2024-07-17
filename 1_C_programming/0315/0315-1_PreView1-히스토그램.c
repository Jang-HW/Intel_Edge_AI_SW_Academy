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
int inH, inW;
int outH, outW;
bool accumulation = false;
int backNum = 0, nowBack = 0;

// �޸� Ȯ��
// unsigned char inImage[height][width], outImage[height][width];
unsigned char** inImage = NULL, ** outImage = NULL;

/////////////////////
// �Լ� �����
/////////////////////
void		loadImage();
int			getIntValue();
void		printImage();
void		printMenu();
void		saveImage();
//void		saveCache(char name[]);
void		freeInputMemory();
void		freeOutputMemory();
void		mallocInputMemory();
void		mallocOutputMemory();
double**	mallocDoubleMemory(int, int);
void		freeDoubleMemory(double**, int);
void		backToBefore();

// ȭ���� ó��
void	equalImage();
void	addImage();
void	minusImage();
void	reverseImage();
void	monoImage();
void	mirror();
void	morph();

// ������
void	zoomOut3();	// �߾Ӱ����� ���̱�
void	zoomIn3();		// ���� ������
void	moveVerticle();
void	moveHorizon();
void	rotate2();

// ������׷�
void	histoStractch();		// �����ϰ� (���ս�)
void	endIn();				// �ս� �־ ���� �����ϰ�
void	histoEqual();			// ������׷� ��Ȱȭ

// ȭ�� ����ó��
void	emboss();				// ������
void	blur();					// ����
void	blurr();				// ���� 9*9
void	gausinaSmooth();		// ����þ� ������
void	sharf1();				// ������ 1
void	sharf2();				// ������ 2


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
	hdc = GetWindowDC(NULL);

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
		case 'F': case 'f': mirror();						break;
		case 'G': case 'g': morph();						break;

		case 'H': case 'h': zoomOut3();						break;
		case 'I': case 'i': zoomIn3();						break;
		case 'J': case 'j': rotate2();						break;
		case 'K': case 'k': moveVerticle();					break;
		case 'L': case 'l':	moveHorizon();					break;

		case 'M': case 'm':	histoStractch();				break;
		case 'N': case 'n':	endIn();						break;
		case 'O': case 'o':	histoEqual();					break;

		case 'P': case 'p':	emboss();						break;
		case 'Q': case 'q':	blur(); 						break;
		case 'R': case 'r':	gausinaSmooth(); 				break;
		case 'S': case 's':	sharf1(); 						break;
		case 'T': case 't':	sharf2();						break;
		case 'U': case 'u':	blurr();						break;

			

		case 'Z': case 'z':	accumulation = !accumulation;	break;
		//case '^':			backToBefore();	break;

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
	puts("\n 0. ����\t\t1. ����\t\t\t\t\t\t\t\t\t9. ����");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n A. ����\t\tB. ���\t\t\tC. ��Ӱ�\t\tD. ����\t\t\tE. ���(127)");
	puts("\n F. �̷���\t\tG. ����");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n H. �� �ƿ�(�߾�)\tI. ����(����)\t\tJ. ȸ��2\t\tK. ���� �̵�\t\tL. ���� �̵�");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n M. ������׷� ��Ʈ��Ī\tN. ������\t\tO. ��Ȱȭ");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n P. ������\t\tQ. ����\t\tR. ����þ� ������\tS. ������1\t\tT. ������2");
	puts("\n U. ����(�޴µ���)");
	puts("\n---------------------------------------------------------------------------------");
	printf("\n Z. ȿ�� ����(on/off)\t");
	if (accumulation)	printf("����� ��������\n");
	else				printf("����� ��������\n");
	//puts(" ^. �ǵ�����");
	puts("\n");
}

void printImage() {
	system("cls");

	//// �ǵ����⸦ ���� outImage ����
	//char fileName[10] = "cache";
	//char fileNum[5];

	//sprintf(fileNum, "%d", (backNum % 10));    // %d�� �����Ͽ� ������ ���ڿ��� ����
	//strcat(fileName, fileNum);
	//saveCache(fileName);

	//if (backNum < 10) { 
	//	backNum++; 
	//	nowBack++; 
	//}
	//else  { 
	//	backNum = 10; 
	//	nowBack = (nowBack + 1) % 10;
	//}
	//

	// ���
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 900, i + 50, RGB(px, px, px));
		}
	}

	// ��� ������ ���� inImage�� ���� 
	if (accumulation) {
		freeInputMemory();
		inH = outH;		inW = outW;
		mallocInputMemory();

		for (int i = 0; i < outH; i++) {
			for (int k = 0; k < outW; k++) {
				inImage[i][k] = outImage[i][k];
			}
		}
		freeOutputMemory();
		outImage = NULL;
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

// �ӽõ����� ������ ���� ���� ���� �Լ�
//void saveCache(char name[]) {
//	char fullname[200] = "D:/RAW/";
//	strcat(fullname, name);
//	strcat(fullname, ".raw");
//	strcpy(fileName, fullname);
//
//	wfp = fopen(fileName, "wb");
//
//	// �޸� --> ���� (���྿)
//	for (int i = 0; i < outH; i++)
//		fwrite(fileName[i], sizeof(unsigned char), outW, wfp);
//	fclose(wfp);
//}

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

double** mallocDoubleMemory(int h, int w) {
	double** retMemory;
	retMemory = (double**)malloc(sizeof(double*) * h);
	for (int i = 0; i < h; i++)
		retMemory[i] = (double*)malloc(sizeof(double) * w);
	return retMemory;
}

void freeDoubleMemory(double** memory, int h) {
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		free(memory[i]);
	free(memory);
	memory = NULL;
}

void backToBefore() {
	if (backNum <= 0) {
		printf("�ǵ��� ������ �����ϴ�. \n");
		return;
	}
	backNum--;

	char fullName[200] = "D:/RAW/";
	char backFile[20] = "cache";
	if(nowBack > 0)	{ nowBack--; }
	else			{ nowBack = 9; }
	strcat(backFile, (char)nowBack);	
	strcat(fullName, backFile);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

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

	equalImage();
}


  ///////////////////
 //  ����ó�� �Լ�//
///////////////////

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

// �߾Ӱ����� ���̱�
void zoomOut3() {
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
			qsort(tmp, (sizeof(tmp) / sizeof(tmp[0])), sizeof(tmp[0]), compare);
			outImage[i][k] = tmp[scale * scale / 2];
		}
	}
	free(tmp);
	printImage();
}

// �缱�� ������
void zoomIn3() {
	// Ȯ�� �˰��� 333
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

	int i_h, i_w, point;
	double r_H, r_W, s_H, s_W;
	double C1, C2, C3, C4;

	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			r_H = i / scale;								// ���� ��ġ�� scale ũ���� ��� �� �� ��° ������ ����
			r_W = k / scale;

			i_h = (int)floor(r_H);							// ����� ���μ��� ĭ�� ����
			i_w = (int)floor(r_W);

			s_H = r_H - i_h;								// ���� ��� �������� ��ġ�� ���� 
			s_W = r_W - i_w;

			if ((i_h < 0) | (i_h >= (inH - 1)) | (i_w < 0) | (i_w >= (inW - 1))) {
				outImage[i][k] = 255;						// ���� ĭ�� ������ ������ �� ���ٸ� ������� ����
															// �ַ� ���ʰ� �Ʒ����� �׷� ������ ����
			}
			else {
				C1 = (double)inImage[i_h][i_w];				// �� ����� ����
				C2 = (double)inImage[i_h][i_w + 1];			// �� ����� ����
				C3 = (double)inImage[i_h + 1][i_w + 1];		// �� �ϴ��� ����
				C4 = (double)inImage[i_h + 1][i_w];			// �� �ϴ��� ����
				outImage[i][k] = (unsigned char)(C1 * (1 - s_H) * (1 - s_W)		
					+ C2 * s_W * (1 - s_H) + C3 * s_W * s_H + C4 * (1 - s_W) * s_H);
				// �� ������� ��ġ ������ �ݿ��ؼ� ���� �� ���� 
			}

		}
	}		
	printImage();
}

// ȸ�� (�� ����)
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
	outH = 2 * inH;		outW = 2 * inW;
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k];
			outImage[2 * inH - i - 1][k] = inImage[i][k];
			outImage[i][2 * inW - k - 1] = inImage[i][k];
			outImage[2 * inH - i - 1][2 * inW - k - 1] = inImage[i][k];
		}
	}
	printImage();
}

// �����ϰ� (���ս�)
void histoStractch() { 
	// ������׷� ��Ʈ��Ī
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();


	// new = (old - low) / (high-low) * 255.0

	// high, low ã��
	int high = inImage[0][0], low = inImage[0][0];
	for (int i = 1; i < inH; i++) {
		for (int k = 1; k < inW; k++) {
			if (inImage[i][k] < low)	low = inImage[i][k];
			if (inImage[i][k] > high)	high = inImage[i][k];
		}
	}	 

	// �Է� �迭 --> ��� �迭
	int new, old;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			old = inImage[i][k];
			new = (int)(((double)old - low) / (high - low) * 255.0);

			if (new > 255)	new = 255;
			if (new < 0)	new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}

// �ս��� �־ ���� �����ϰ�
void endIn() {
	// end-in Ž�� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();


	// new = (old - low) / (high-low) * 255.0

	// high, low ã��
	int high = inImage[0][0], low = inImage[0][0];
	for (int i = 1; i < inH; i++) {
		for (int k = 1; k < inW; k++) {
			if (inImage[i][k] < low)	low = inImage[i][k];
			if (inImage[i][k] > high)	high = inImage[i][k];
		}
	}

	//////////���Ⱑ �ٸ�
	high -= 50;
	low += 50;

	// �Է� �迭 --> ��� �迭
	int new, old;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			old = inImage[i][k];
			new = (int)(((double)old - low) / (high - low) * 255.0);

			if (new > 255)	new = 255;
			if (new < 0)	new = 0;
			outImage[i][k] = new;
		}
	}
	printImage();
}

// ������׷� ��Ȱȭ �˰��� 
void histoEqual(){			
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// 1) �󵵼� ����			histo[256]
	int histo[256] = { 0, };

	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++) 
			histo[inImage[i][k]]++;

	// 2) ���� ������׷� ����
	int sumHisto[256] = { 0, };

	sumHisto[0] = histo[0];
	for (int i = 1; i < 256; i++)
		sumHisto[i] = sumHisto[i - 1] + histo[i];
		
	// 3) ����ȭ ������׷� ����
	// normalHisto = sumHisto*(1.0/(inH*inW))*255.0
	double normalHisto[256] = { 1.0, };
	for (int i = 1; i < 256; i++)
		normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW)) * 255.0;

	// 4) �Է� �迭�� ġȯ --> ��� �迭
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (unsigned char) normalHisto[inImage[i][k]];
		}
	}
	printImage();
}

// ȭ�� ����ó�� - ������ 
void emboss() {
	freeOutputMemory();						// �޸� ����
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	  ///////////////////
	 // ȭ�� ����ó�� //
	///////////////////
	double mask[3][3] = { {-1.0, 0.0, 0.0}, {0.0, 0.0, 0.0},{0.0, 0.0, 1.0} };	

	// �ӽø޸� �Ҵ�(�Ǽ�������)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� 127���� �ʱ�ȭ
	for (int i = 0; i < inH+2; i++) {
		for (int k = 0; k < inW+2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     ȸ�� ����
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ�� �� ���� �߽������� 3*3 ���ϱ�
			S = 0.0;	// ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++) 
				for (int n = 0; n < 3; n++) 
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	// ��ó�� : ����ũ�� �հ迡 ����
	for (int i = 0; i < outH; i++)
		for (int k = 0; k < outW; k++)
			tmpOutImage[i][k] += 127.0;

	// �ӽ���¿��� --> ��¿���
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// ȭ�� ����ó�� - ���� 
void blur() {
	freeOutputMemory();						// �޸� ����
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	///////////////////
   // ȭ�� ����ó�� //
  ///////////////////
	double mask[3][3] = { {1.0/9, 1.0/9, 1.0/9}, {1.0/9, 1.0/9, 1.0/9},{1.0/9, 1.0/9, 1.0/9} };

	// �ӽø޸� �Ҵ�(�Ǽ�������)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� 127���� �ʱ�ȭ
	for (int i = 0; i < inH + 2; i++) {
		for (int k = 0; k < inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     ȸ�� ����
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ�� �� ���� �߽������� 3*3 ���ϱ�
			S = 0.0;	// ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// ��ó�� : ����ũ�� �հ迡 ����
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ���¿��� --> ��¿���
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// ȭ�� ����ó�� - ������ 
void gausinaSmooth() {
	freeOutputMemory();						// �޸� ����
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	///////////////////
   // ȭ�� ����ó�� //
  ///////////////////
	double mask[3][3] = { {1./16 ,1./8, 1./16}, {1. / 8 ,1. / 4, 1. / 8},{1. / 16 ,1. / 8, 1. / 16} };

	// �ӽø޸� �Ҵ�(�Ǽ�������)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� 127���� �ʱ�ȭ
	for (int i = 0; i < inH + 2; i++) {
		for (int k = 0; k < inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     ȸ�� ����
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ�� �� ���� �߽������� 3*3 ���ϱ�
			S = 0.0;	// ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// ��ó�� : ����ũ�� �հ迡 ����
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ���¿��� --> ��¿���
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// ȭ�� ����ó�� - ������1
void sharf1() {
	freeOutputMemory();						// �޸� ����
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	///////////////////
   // ȭ�� ����ó�� //
  ///////////////////
	double mask[3][3] = { {-1., -1., -1.}, {-1., 9 ,-1.},{-1., -1., -1.} };

	// �ӽø޸� �Ҵ�(�Ǽ�������)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� 127���� �ʱ�ȭ
	for (int i = 0; i < inH + 2; i++) {
		for (int k = 0; k < inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     ȸ�� ����
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ�� �� ���� �߽������� 3*3 ���ϱ�
			S = 0.0;	// ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// ��ó�� : ����ũ�� �հ迡 ����
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ���¿��� --> ��¿���
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// ȭ�� ����ó�� - ������2
void sharf2() {
	freeOutputMemory();						// �޸� ����
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	///////////////////
   // ȭ�� ����ó�� //
  ///////////////////
	double mask[3][3] = { {0., -1., 0.}, {-1., 5 ,-1.},{0., -1., 0.} };

	// �ӽø޸� �Ҵ�(�Ǽ�������)
	double** tmpInImage = mallocDoubleMemory(inH + 2, inW + 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� 127���� �ʱ�ȭ
	for (int i = 0; i < inH + 2; i++) {
		for (int k = 0; k < inW + 2; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     ȸ�� ����
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ�� �� ���� �߽������� 3*3 ���ϱ�
			S = 0.0;	// ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < 3; m++)
				for (int n = 0; n < 3; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// ��ó�� : ����ũ�� �հ迡 ����
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ���¿��� --> ��¿���
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(tmpInImage, inH + 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// ȭ�� ����ó�� - �Է� �޴´�� ����
void blurr() {
	freeOutputMemory();						// �޸� ����

	int scale = getIntValue();
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	///////////////////
   // ȭ�� ����ó�� //
  ///////////////////

	double** mask;
	int lenScale = scale * 2 + 1;
	mask = mallocDoubleMemory(lenScale, lenScale);

	for (int i = 0; i < lenScale; i++) {
		for (int k = 0; k < lenScale; k++)
			mask[i][k] = 1. / pow(lenScale, 2);
	}

	// �ӽø޸� �Ҵ�(�Ǽ�������)
	double** tmpInImage = mallocDoubleMemory(inH + scale * 2, inW + scale * 2);
	double** tmpOutImage = mallocDoubleMemory(outH, outW);

	// �ӽ� �Է� �޸� 127���� �ʱ�ȭ
	for (int i = 0; i < inH + 2 * scale; i++) {
		for (int k = 0; k < inW + 2 * scale; k++) {
			tmpInImage[i][k] = 127;
		}
	}

	// �Է� �̹��� --> �ӽ� �Է� �̹���
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			tmpInImage[i + 1][k + 1] = inImage[i][k];
		}
	}

	//  *****mask*****     ȸ�� ����
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ����ũ�� �� ���� �߽������� 3*3 ���ϱ�
			S = 0.0;	// ����ũ 9���� �Է°� 9���� ���� ���ؼ� ���� ��.

			for (int m = 0; m < lenScale; m++)
				for (int n = 0; n < lenScale; n++)
					S += tmpInImage[i + m][k + n] * mask[m][n];

			tmpOutImage[i][k] = S;
		}
	}

	//// ��ó�� : ����ũ�� �հ迡 ����
	//for (int i = 0; i < outH; i++)
	//	for (int k = 0; k < outW; k++)
	//		tmpOutImage[i][k] += 127.0;

	// �ӽ���¿��� --> ��¿���
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			if (tmpOutImage[i][k] > 255.0)
				outImage[i][k] = 255;
			else if (tmpOutImage[i][k] < 0.0)
				outImage[i][k] = 0;
			else outImage[i][k] = (unsigned char)tmpOutImage[i][k];
		}
	}
	freeDoubleMemory(mask, scale);
	freeDoubleMemory(tmpInImage, inH + scale * 2);
	freeDoubleMemory(tmpOutImage, inH);

	printImage();
}

// ���� 
void morph() {
	char fullName[200] = "D:/RAW/Etc_Raw(squre)/";
	char tmpName[50];
	unsigned char** moImage = NULL;
	int moH, moW;


	// ������ �̹��� �ҷ�����
	printf("���ϸ�-->"); // flower512, LENA256
	scanf("%s", tmpName);
	strcat(fullName, tmpName);
	strcat(fullName, ".raw");
	strcpy(fileName, fullName);

	rfp = fopen(fileName, "rb");
	if (rfp == NULL) {
		printf("������ �������� �ʽ��ϴ�. �ٸ� �̸����� �õ��� �ּ���.\n\n");
		return;
	}
	fseek(rfp, 0L, SEEK_END); // ������ ������ �̵�
	long long fsize = ftell(rfp); // �� ���������?  262,144  --> 512
	fclose(rfp);

	// ���� ������ ����, �� ���ϱ�
	moH = moW = (int)sqrt(fsize);
	if (inH > moH & inW > moW)	{
		moH = inH;
		moW = inW;
	}
	// �޸� �Ҵ� �� �ʱ�ȭ
	moImage = (unsigned char**)malloc(sizeof(unsigned char*) * moH);
	for (int i = 0; i < moH; i++)
		moImage[i] = (unsigned char*)malloc(sizeof(unsigned char) * moW);
	for (int i = 0; i < moH; i++)
		for (int k = 0; k < moW; k++)
			moImage[i][k] = 0;
	// ���� --> �޸�
	rfp = fopen(fileName, "rb");
	for (int i = 0; i < moH; i++)
		fread(moImage[i], sizeof(unsigned char), moW, rfp);
	fclose(rfp);

	bool tmpAccumulation = accumulation;
	accumulation = false;


	// 5�ܰ� ����
	for (int n = 0; n < 5; n++) {
		for (int i = 0; i < moH; i++) {
			for (int k = 0; k < moW; k++) {
				outImage[i][k] = (unsigned char)((4. - n) / 4 * inImage[i][k] + n / 4. * moImage[i][k]);
			}
		}
		printImage(); 
		_getch();
	}

	accumulation = tmpAccumulation;

	for (int i = 0; i < moH; i++)
		free(moImage[i]);
	free(moImage);
}