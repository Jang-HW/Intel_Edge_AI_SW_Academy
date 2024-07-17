#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

void main() {

	char str[100];									// 100줄 넘을 수도 있으니
													// 가변메모리를 잘 쓰는게 좋음
	FILE* rfp;

	// 파일처리 3단계
	// 1. 파일 열기
	rfp = fopen("c:/windows/win.ini", "rt");		// r: read, t: text
	
	// 2. 파일 읽거나 쓰기
	int iLineNum = 1;

	while(1) {
		fgets(str, 100, rfp);
		if(feof(rfp)){
			break;
		}
		printf("%3d | %s", iLineNum, str);
		iLineNum++;
	}
	
	// 3. 파일 닫기
	fclose(rfp);
}