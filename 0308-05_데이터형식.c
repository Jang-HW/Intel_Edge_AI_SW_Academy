#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {

	// 정수형: char, short, int, long
	// 부호 없이 쓰는거 : unsigned

	char cNum1 = 200;									// 1 byte -128 ~ +127 (2^7)
	printf("char ---> %d \n", cNum1);
	unsigned char cNum2 = 200;							// 1 byte 0 ~ +255 
	printf("unsigned char ---> %d \n\n", cNum2);


	short cShort1 = 40000;								// 2 byte -32768 ~ +32767 (2^15)
	printf("short ---> %d \n", cShort1);
	unsigned short cShort2 = 40000;						// 0 ~ +65535 (2^15)
	printf("unsigned short ---> %d \n\n", cShort2);


	int iNum1 = 2147483648;								// 4 byte -2147483648 ~ +2147483647 (2^31)
	printf("int ---> %d \n", iNum1);
	unsigned int iNum2 = 216748483648;					// 0 ~ 42억 쯔음
	printf("unsigned int ---> %u \n\n", iNum2);


	long long lNum1 = 12345678901234567890;				// 8 or 4 byte(컴파일러 차이): -900경 ~ 900경
	printf("long ---> %lld \n", lNum1);
	unsigned long long lNum2 = 12345678901234567890;	// 0 ~ 180경 쯔음
	printf("long ---> %llu \n", lNum2);

}