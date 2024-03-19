#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {
	
	// 증감연산자 
	/*
	int a = 100;
	
	a += 1;		
	printf("%d\n", a);		// 101
	
	++a;
	printf("%d\n", a);		// 102
	a++;
	printf("%d\n", a);		// 103


	// 헷갈리게 이런거 하지 말자
	printf("%d\n", a++);	// 103
	printf("%d\n", a);		// 104
	*/

	// 반복문
	// for
	/*
	for (int i = 1; i <= 10; i++) {
		printf("%d ", i);
	}
	printf("\n\n");


	int hap = 0;
	for (int i = 0; i <= 100; i++) {
		hap += i;
	}
	printf("1~100까지 합 : %d\n\n", hap);
	*/

	/*
	// 퀴즈: 100-200까지 합계
	int hap = 0;
	for (int i = 100; i <= 200; i++) {
		hap += i;
	}
	printf("100-200까지 합계 : %d\n\n", hap);



	// 퀴즈: 4444-1111까지 합계
	hap = 0;
	for (int i = 4444; i >= 1111; i--) {
		hap += i;
	}
	printf("4444-1111까지 합계 : %d\n\n", hap);



	// 퀴즈: 1-1000까지 짝수의 합계
	hap = 0;
	for (int i = 2; i <= 1000; i= i+2) {
		hap += i;
	}
	printf("1-1000까지 짝수의 합계 : %d\n\n", hap);
	


	// 퀴즈: 12345-67890까지 1877의 배수의 합계
	hap = 0;
	for (int i = 12345; i <= 67890; i++) {
		if (i % 1877 == 0)
			hap += i;
	}
	printf("12345-67890까지 1877의 배수의 합계 : %d\n\n", hap);



	// 퀴즈: 2-1000까지의 소수를 출력하고 합계
	hap = 0;
	char cPrime = 0;

	for (int i = 2; i <= 1000; i++) {

		for (int j = 2; j < i; j++) {
			if (i % j == 0) { 
				cPrime = 123;
				break; 
			}
		}

		if (cPrime == 0) {
			hap += i;
			printf("%4d ", i);
		}
		cPrime = 0;
	}
	printf("\n");
	printf("2-1000까지의 소수의 합계 : %d\n", hap);
	*/

	// while
	/*
	int hap = 0;
	int i = 0;

	while (i <= 100) {
		hap += i;
		i += 1;
	}
	printf("1-100까지 합계 : %d\n\n", hap);
	
	// 100-200까지 합계
	hap = 0;
	i = 100;
	while (i <= 200) {
		hap += i;
		i += 1;
	}
	printf("100-200까지 합계 : %d\n\n", hap);


	// 4444-1111까지 합계
	hap = 0;
	i = 4444;
	while (i >= 1111) {
		hap += i;
		i = i - 1;
	}
	printf("4444-1111까지 합계 : %d\n\n", hap);


	// 1-1000까지 짝수의 합계
	hap = 0;
	i = 2;
	while (i <= 1000) {
		hap += i;
		i += 2;
	}
	printf("1-1000까지 짝수의 합계 : %d\n\n", hap);



	// 12345-67890까지 1877의 배수의 합계
	hap = 0;
	i = 12345;
	while (i <= 67890){
		if (i % 1877 == 0){
			hap += i;
		}
		i++;
	}
	printf("12345-67890까지 1877의 배수의 합계 : %d\n\n", hap);
	*/

	// 무한 반복: 보통 break랑 같이해서 중단점이 존재하는게 일반적
	/*
	* for(;;){ // 블라블라 }
	* while( true ){ // 블라블라 }
	* while( 1 ){ // 블라블라 }
	* while( 2 ){ // 블라블라 }
	*/

	// 1부터 숫자를 더하고 싶은데 더한 숫자가 1억을 넘으면 그만할래
	int hap = 0;
	int i = 1;
	while (1) {
		hap += i;
		if (hap >= 100000000) {
			break;
		}
		i++;
	}
	printf("1부터 숫자를 더하는데 1억을 넘으면 그만할래\n---  i: %d\n--- 합: %d\n", i, hap);

	// exit: 프로그램 종료 (별로 안 좋음)
	// 
	// return: 함수 종료 (그나마 나음)
}