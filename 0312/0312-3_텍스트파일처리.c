#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

void main() {

	char str[100];									// 100�� ���� ���� ������
													// �����޸𸮸� �� ���°� ����
	FILE* rfp;

	// ����ó�� 3�ܰ�
	// 1. ���� ����
	rfp = fopen("c:/windows/win.ini", "rt");		// r: read, t: text
	
	// 2. ���� �аų� ����
	int iLineNum = 1;

	while(1) {
		fgets(str, 100, rfp);
		if(feof(rfp)){
			break;
		}
		printf("%3d | %s", iLineNum, str);
		iLineNum++;
	}
	
	// 3. ���� �ݱ�
	fclose(rfp);
}