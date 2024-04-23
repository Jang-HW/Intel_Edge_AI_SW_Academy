#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

int plusNumber(int, int );					// 함수의 선언: 변수 이름 안써도 ok

void main() {

	int iNum1 = 100, iNum2 = 200;
	int iHap;

	iHap = plusNumber(iNum1, iNum2);

	printf("%d \n", iHap);

}

// 함수는 자판기 커피 
//	 (블랙 박스: 내부가 안보여도 뭐가 나올지 아니까)
// - 파라미터:	동전
// - 커피:		돌려 주는거(없으면 없다고)
// 
// 함수를 만드는 두가지 방법
// - 함수의 선언과 정의
// 

int plusNumber(int iNum1, int iNum2) {		// 함수의 정의
	int returnVal;
	returnVal = iNum1 + iNum2;

	return returnVal;
}