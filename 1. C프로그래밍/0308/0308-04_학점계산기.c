#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {
	
	int iScore;

	printf("점수를 입력하세요 > ");
	scanf("%d", &iScore);

	//if (iScore >= 90) 
	//	printf("A");
	//else if (iScore >= 60) 
	//	printf("B");
	//else if (iScore >= 40) 
	//	printf("C");
	//else if (iScore >= 30) 
	//	printf("D");
	//else 
	//	printf("F");
	

	switch (iScore / 10) {
		case 0 :
		case 1 :
		case 2 :
			printf("F");
			break;
		case 3 :
			printf("E");
			break;
		case 4 :
		case 5 :
			printf("C");
			break;
		case 6 :
			printf("B");
			break;
		default: 
			printf("A");
			break;
	}

	printf(" 학점입니다. ");
}