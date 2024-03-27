#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI �������
#include <math.h>

// �� �Լ� ���� 
#define min(x, y)	(x) < (y) ? (x) : (y)	
#define max(x, y)	(x) > (y) ? (x) : (y)	

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
bool accumulation = false;
int backNum = 0, nowBack = 0;

// �޸� Ȯ��
// unsigned char inImage[inH][width], outImage[height][width];
unsigned char** inImage = NULL, ** outImage = NULL, mask[512][512];

/////////////////////
// �Լ� �����
/////////////////////
void		loadImage();
int			getIntValue();
void		printImage();
void		printMenu();
void		saveImage();
void		saveCache();
void		freeInputMemory();
void		freeOutputMemory();
void		mallocInputMemory();
void		mallocOutputMemory();
double**	mallocDoubleMemory(int, int);
void		freeDoubleMemory(double**, int);
void		backToBefore();

void		dotImageProcessMenu();
void		geometryProcessMenu();
void		histAndAreaProcessMenu();


// ȭ���� ó��
void	equalImage();
void	addImage();
void	minusImage();
void	multiImage();
void	divImage();
void	reverseImage();
void	monoImage();
void	gammaImage();
void	paraImage();
void	monoAvgImage();
void	monoMidImage();
void	multiRapImage();
void	maskAndImage();
void	maskOrImage();
void	maskXorImage();
void	notImage();
void	constrastStratchImage();
void	constrastZipImage();
void	posterizeImage();
void	areaStressImage();

// ������
void	zoomOut();		// �⺻
void	zoomOut2();		// ������� ���̱�
void	zoomOut3();		// �߾Ӱ����� ���̱�
void	zoomIn();		// ������ ���
void	zoomIn2();		// �̿�ȭ�� ������
void	zoomIn3();		// ���� ������
void	rotate();
void	rotate2();
void	rotate3();
void	moveVerticle();
void	moveHorizon();
void	mirror();
void	morph();

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
void	edge1();				// ���� ���� ����� 
void	simular ();				// ���� ���� ����� 


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

		case 'A': case 'a':	dotImageProcessMenu();			break;
		case 'B': case 'b':	geometryProcessMenu();			break;
		case 'C': case 'c':	histAndAreaProcessMenu();		break;

		case 'Z': case 'z':	accumulation = !accumulation;	break;
		case '^':			backToBefore();					break;

		default:	
			system("cls");		// windows ��ɾ�
			printf("�ٸ� ���ڸ� �Է��� �ּ���!\n");	break;
		}

	}

	// �ӽ� ���� �����ϱ�
	for (int i = 0; i < backNum; i++) {
		char fullname[200] = "D:/RAW/cache";
		char fileNum[5];
		sprintf(fileNum, "%d", i);    // %d�� �����Ͽ� ���� ���ڿ��� ����
		strcat(fullname, fileNum);
		strcat(fullname, ".raw");
		remove(fullname);
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
	puts("\n 0. ����\t\t1. ����\t\t\t\t\t9. ����");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n A. ȭ�� ��ó��");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n B. ������ ó��");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n C. ������׷� & ȭ�� ���� ó��");
	puts("\n---------------------------------------------------------------------------------");
	printf("\n Z. ȿ�� ����(on/off)\t");
	if (accumulation)	printf("����� ��������\n");
	else				printf("����� ��������\n");
	printf(" ^. �ǵ����� (10ȸ �� %dȸ)\n", backNum);
}

void printImage() {
	system("cls");

	saveCache();
	
	// ���
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + 900, i + 80, RGB(px, px, px));
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

// �ӽõ����� ������ ���� ���� ���� �Լ�
void saveCache() {

	// �ǵ����⸦ ���� outImage ����
	char fileName[20] = "cache";
	char fileNum[5];
	char fullname[200] = "D:/RAW/";

	sprintf(fileNum, "%d", (backNum % 10));    // %d�� �����Ͽ� ������ ���ڿ��� ����
	strcat(fileName, fileNum);
	strcat(fullname, fileName);
	strcat(fullname, ".raw");

	wfp = fopen(fullname, "wb");

	// �޸� --> ���� (���྿)
	for (int i = 0; i < outH; i++)
		fwrite(outImage[i], sizeof(unsigned char), outW, wfp);
	fclose(wfp);

	if (backNum < 10) {
		backNum++;
		nowBack++;
	}
	else {
		backNum = 10;
		nowBack = (nowBack + 1) % 10;
	}
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
	char fullName[200] = "D:/RAW/";
	char backFile[20] = "cache";
	char fileNum[5];

	if (backNum <= 0) {
		printf("�ǵ��� ������ �����ϴ�. \n");
		return;
	}

	backNum--;
	if (nowBack > 0) { nowBack--; }
	else { nowBack = 9; }

	sprintf(fileNum, "%d", nowBack);    // %d�� �����Ͽ� ������ ���ڿ��� ����
	strcat(backFile, fileNum);
	strcat(fullName, backFile);
	strcat(fullName, ".raw");
	rfp = fopen(fullName, "rb");

	fseek(rfp, 0L, SEEK_END); // ������ ������ �̵�
	long long fsize = ftell(rfp); // �� ���������?  262,144  --> 512
	fclose(rfp);

	if (accumulation) {
		// �޸� ����
		freeInputMemory();
		// �Է� ������ ����, �� ���ϱ�
		inH = inW = (int)sqrt(fsize);
		// �޸� �Ҵ�
		mallocInputMemory();
		// ���� --> �޸�
		rfp = fopen(fullName, "rb");
		for (int i = 0; i < inH; i++)
			fread(inImage[i], sizeof(unsigned char), inW, rfp);
		fclose(rfp);
		equalImage();
	}
	else {
		// �޸� ����
		freeOutputMemory();
		// �Է� ������ ����, �� ���ϱ�
		outH = outW = (int)sqrt(fsize);
		// �޸� �Ҵ�
		mallocOutputMemory();
		// ���� --> �޸�
		rfp = fopen(fullName, "rb");
		for (int i = 0; i < inH; i++)
			fread(outImage[i], sizeof(unsigned char), outW, rfp);
		fclose(rfp); 
		
		// ���
		for (int i = 0; i < outH; i++) {
			for (int k = 0; k < outW; k++) {
				int px = outImage[i][k];
				SetPixel(hdc, k + 900, i + 50, RGB(px, px, px));
			}
		}
	}
}

void dotImageProcessMenu() {
	system("cls");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n 0. ���Ͽ��� �˰���");
	puts("\n A. ��� �ϱ�\t\tB. ��Ӱ� �ϱ�\t\tC. ����\t\t\tD. ������ȯ");
	puts("\n E. �Ķ󺼶� ��ȯ\tF. ���(127)\t\tG. ���(��հ�)\t\tH. ���(�߾Ӱ�)");
	puts("\n I. �� ��� �ϱ� (���)\tJ. �� ��Ӱ� �ϱ� (���)K. �� ��� �ϱ� (���) + ���α��");
	puts("\n L. ���� ����ũ(AND)\tM. ���� ����ũ(OR)\tN. ���� ����ũ(XOR)\tO. NOT ����");
	puts("\n P. ��� ��� ��Ʈ��Ī\tQ. ��� ��� ����\tR. �����Ͷ���¡\t\tS. ���� ����");
	puts("\n---------------------------------------------------------------------------------");

	char inKey = 0;

	inKey = _getch();

	switch (inKey) {
	case '0':			equalImage();					break;
	case 'A': case 'a':	addImage();						break;
	case 'B': case 'b':	minusImage();					break;
	case 'C': case 'c':	reverseImage();					break;
	case 'D': case 'd':	gammaImage();					break;
	case 'E': case 'e':	paraImage();					break;

	case 'F': case 'f': monoImage();					break;
	case 'G': case 'g': monoAvgImage();					break;
	case 'H': case 'h': monoMidImage();					break;
	case 'I': case 'i': multiImage();					break;
	case 'J': case 'j': divImage();						break;
	case 'K': case 'k': multiRapImage();				break;

	case 'L': case 'l':	maskAndImage();					break;
	case 'M': case 'm':	maskOrImage();					break;
	case 'N': case 'n':	maskXorImage();					break;
	case 'O': case 'o':	notImage();						break;

	case 'P': case 'p':	constrastStratchImage();		break;
	case 'Q': case 'q':	constrastZipImage(); 			break;
	case 'R': case 'r':	posterizeImage(); 				break;
	case 'S': case 's':	areaStressImage(); 				break;

	default:
		system("cls");		// windows ��ɾ�
		printf("�ش� ���ڰ� �����ϴ�!\n");		break;
	}
}

void geometryProcessMenu() {
	system("cls");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n A. �� ��(������)\tB. �� ��(�̿�ȭ�� ������)\tC. �� ��(���� ������)");
	puts("\n D. �� �ƿ�\t\tE. �� �ƿ�(���)\t\tF. �� �ƿ�(�߾Ӱ�)");
	puts("\n G. ȸ��(����)\t\tH. ȸ��(�� ����)\t\tI. ȸ��(�ȱ���+��©��)");
	puts("\n J. ���� �̵�\t\tK. ���� �̵� L. �̷���\t\tM. ����");
	puts("\n---------------------------------------------------------------------------------");

	char inKey = 0;

	inKey = _getch();

	switch (inKey) {

	case 'A': case 'a':	zoomIn();					break;
	case 'B': case 'b':	zoomIn2();					break;
	case 'C': case 'c':	zoomIn3();					break;

	case 'D': case 'd':	zoomOut();					break;
	case 'E': case 'e':	zoomOut2();					break;
	case 'F': case 'f': zoomOut3();					break;

	case 'G': case 'g': rotate();					break;
	case 'H': case 'h': rotate2();					break;
	case 'I': case 'i': rotate3();					break;

	case 'J': case 'j': moveVerticle();				break;
	case 'K': case 'k': moveHorizon();				break;
	case 'L': case 'l': mirror();					break;
	case 'M': case 'm':	morph();					break;

	default:
		system("cls");		// windows ��ɾ�
		printf("�ش� ���ڰ� �����ϴ�!\n");	break;
	}
}

void histAndAreaProcessMenu() {
	system("cls");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n A. ������ ��Ʈ��Ī\tB. ������\t\tC. ������׷� ��Ȱȭ");
	puts("\n---------------------------------------------------------------------------------");
	puts("\n D. ������\t\tE. ����(3 x 3)\tF. ����(N x N)");
	puts("\n G. ����þ� ������\tH. ������ 1\t\tI. ������ 2");
	puts("\n J. ���� ���� ����ũ\tK. ���� ������");
	puts("\n---------------------------------------------------------------------------------");

	char inKey = 0;
	inKey = _getch();

	switch (inKey) {

	case 'A': case 'a':	histoStractch();			break;
	case 'B': case 'b':	endIn();					break;
	case 'C': case 'c':	histoEqual();				break;

	case 'D': case 'd':	emboss();					break;
	case 'E': case 'e':	blur();						break;
	case 'F': case 'f': blurr();					break;

	case 'G': case 'g': gausinaSmooth();			break;
	case 'H': case 'h': sharf1();					break;
	case 'I': case 'i': sharf2();					break;
	case 'J': case 'j': edge1();					break;

	case 'K': case 'k': simular();					break;

	default:	
		system("cls");		// windows ��ɾ�
		printf("�ش� ���ڰ� �����ϴ�!\n");	break;
	}
}


///////////////////
//  ����ó�� �Լ�//
///////////////////

void equalImage() {
	// ���Ͽ��� �˰���
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է¹迭 --> ��¹迭 �����ϱ�
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

void multiImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է¹迭 --> ���ؼ� --> ��¹迭
	int val = getIntValue();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] * val > 255) outImage[i][k] = 255;
			else outImage[i][k] = inImage[i][k] * val;
		}
	}
	printImage();
}

void divImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է¹迭 --> ������ --> ��¹迭
	int val = getIntValue();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = inImage[i][k] / val;
		}
	}
	printImage();
}

void reverseImage() {
	// ����
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

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

void gammaImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	int val = getIntValue();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = pow((float)inImage[i][k], 1.f / val);
		}
	}
	printImage();
}

void paraImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	printf("- CAPȿ�� \n");
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = 255.f * pow((inImage[i][k] / 127.f) - 1, 2.f);
		}
	}
	printImage();
	_getch();

	printf("- CUPȿ�� \n");
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = 255.f - 255.f * pow((inImage[i][k] / 127.f) - 1, 2.f);
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

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
	printf("��հ�: %d\n", cMid);
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

	// �˰��� ã�Ƽ� ä��� 
	// quick sort result

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			if (inImage[i][k] < cMid)
				outImage[i][k] = 0;
			else outImage[i][k] = 255;
		}
	}
	printImage();
	printf("�߾Ӱ�: %d\n", cMid);
}

void multiRapImage() {
	// �޸� ����
	freeOutputMemory();
	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	outH = inH;		outW = inW;
	mallocOutputMemory();

	// �Է� �迭 --> ��� �迭
	int val = getIntValue();

	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			outImage[i][k] = (inImage[i][k] * val) % 256;
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
			else if (inImage[i][k] > 175)	outImage[i][k] = inImage[i][k];
			else 							outImage[i][k] = 255;
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
			outImage[i][k] = (int)(tmp[i][k] / (scale * scale));
		}
	}

	for (int i = 0; i < outH; i++)
		free(tmp[i]);
	free(tmp);

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
			int ys = (int)(-sin(radian) * (xd - cy) + cos(radian) * (yd - cy));
			xs += cx;
			ys += cy;

			if (0 <= xs & xs < inH & 0 <= ys & ys < inW)
				outImage[xd][yd] = inImage[xs][ys];
		}
	}
	printImage();
}

// ȸ�� (�� ����) + ũ�� Ȯ��
void rotate3() {
	// ȸ�� �˰��� + �߾� �����
	// �޸� ����
	freeOutputMemory();

	int degree = getIntValue();
	if (degree > 360) {
		degree %= 360;
	}
	else if (degree < 0) {
		degree = abs(degree);
		degree %= 360;
		degree = 360 - degree;
	}
	double radian = degree * 3.1415925386 / 180.0;
	// xd = cos*xs - sin*ys
	// yd = sin*xs + cos*ys

	// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	if (degree < 90) {
		outH = (int)((double)inW * sin(radian) + inH * cos(radian));
		outW = (int)((double)inW * cos(radian) + inH * sin(radian));
	}
	else if (degree < 180) {
		outH = (int)((double)inW * sin(radian) + inH * -cos(radian));
		outW = (int)((double)inW * -cos(radian) + inH * sin(radian));
	}
	else if (degree < 270) {
		outH = (int)((double)inW * -sin(radian) + inH * -cos(radian));
		outW = (int)((double)inW * -cos(radian) + inH * -sin(radian));
	}
	else {
		outH = (int)((double)inW * -sin(radian) + inH * cos(radian));
		outW = (int)((double)inW * cos(radian) + inH * -sin(radian));
	}
	mallocOutputMemory();	

	int cx = inH / 2;
	int cy = inW / 2;

	// �Է� �迭 --> ��� �迭
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			int xd = i;
			int yd = k;
			int	xs = (int)(cos(radian) * (xd - (outH/2)) + sin(radian) * (yd - (outH / 2)));
			int ys = (int)(-sin(radian) * (xd - (outW / 2)) + cos(radian) * (yd - (outW / 2)));

			xs += cx;
			ys += cy;

			if (0 <= xs && xs < inH && 0 <= ys && ys < inW) {
				outImage[xd][yd] = inImage[xs][ys];
			}
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
			if (k + val >= outW || k + val < 0)		continue;
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
	if ((inH > moH) & (inW > moW)) {
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
void histoEqual() {
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
			outImage[i][k] = (unsigned char)normalHisto[inImage[i][k]];
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
	double mask[3][3] = { {-1.0, 0.0, 0.0}, 
						   {0.0, 0.0, 0.0},
						   {0.0, 0.0, 1.0} };

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
	double mask[3][3] = { {1.0 / 9, 1.0 / 9, 1.0 / 9}, 
						  {1.0 / 9, 1.0 / 9, 1.0 / 9},
						  {1.0 / 9, 1.0 / 9, 1.0 / 9} };

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

// ȭ�� ����ó�� - ����þ�
void gausinaSmooth() {
	freeOutputMemory();						// �޸� ����
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	///////////////////
   // ȭ�� ����ó�� //
  ///////////////////
	double mask[3][3] = { {1. / 16 ,1. / 8, 1. / 16}, 
						  {1. / 8 , 1. / 4, 1. / 8},
						  {1. / 16 ,1. / 8, 1. / 16} };

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
	double mask[3][3] = { {-1., -1., -1.}, 
						  {-1.,  9 , -1.},
						  {-1., -1., -1.} };

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
	double mask[3][3] = { {0., -1., 0.}, 
						  {-1., 5 ,-1.},
						  {0., -1., 0.} };

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

// ȭ�� ����ó�� - ���� �𼭸� 
void edge1() {
	freeOutputMemory();						// �޸� ����
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	///////////////////
   // ȭ�� ����ó�� //
  ///////////////////
	// ���� ���� ���� ����ũ
	double mask[3][3] = { {0., 0., 0.}, 
						  {-1., 1 ,0.},
						  {0., 0., 0.} };

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

// ���� ������
void simular() {
	freeOutputMemory();						// �޸� ����
	outH = inH;		outW = inW;				// (�߿�!!) ��� �̹����� ũ�� ���� (�˰��� ������)
	mallocOutputMemory();

	///////////////////
   // ȭ�� ����ó�� //
  ///////////////////
  // 
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

	// ȸ�� ����
	double S = 0.0;
	for (int i = 0; i < inH; i++) {
		for (int k = 0; k < inW; k++) {
			// ���� �������� ���, �߾Ӱ� �� ��ġ�� ���� ���� �����ؼ� ���ϹǷ�
			// ù ��° ��� ���� �ʱⰪ���� �ִ°� �ո�����
			S = abs(tmpInImage[i + 1][k + 1] - tmpInImage[i][k]);

			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					if ((m != 1) & (n != 1))
						S = max(abs(tmpInImage[i + 1][k + 1] - tmpInImage[i + m][k + n]), S);
				}
			}

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