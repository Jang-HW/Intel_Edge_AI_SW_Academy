#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main() {
	
	// ���������� 
	/*
	int a = 100;
	
	a += 1;		
	printf("%d\n", a);		// 101
	
	++a;
	printf("%d\n", a);		// 102
	a++;
	printf("%d\n", a);		// 103


	// �򰥸��� �̷��� ���� ����
	printf("%d\n", a++);	// 103
	printf("%d\n", a);		// 104
	*/

	// �ݺ���
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
	printf("1~100���� �� : %d\n\n", hap);
	*/

	/*
	// ����: 100-200���� �հ�
	int hap = 0;
	for (int i = 100; i <= 200; i++) {
		hap += i;
	}
	printf("100-200���� �հ� : %d\n\n", hap);



	// ����: 4444-1111���� �հ�
	hap = 0;
	for (int i = 4444; i >= 1111; i--) {
		hap += i;
	}
	printf("4444-1111���� �հ� : %d\n\n", hap);



	// ����: 1-1000���� ¦���� �հ�
	hap = 0;
	for (int i = 2; i <= 1000; i= i+2) {
		hap += i;
	}
	printf("1-1000���� ¦���� �հ� : %d\n\n", hap);
	


	// ����: 12345-67890���� 1877�� ����� �հ�
	hap = 0;
	for (int i = 12345; i <= 67890; i++) {
		if (i % 1877 == 0)
			hap += i;
	}
	printf("12345-67890���� 1877�� ����� �հ� : %d\n\n", hap);



	// ����: 2-1000������ �Ҽ��� ����ϰ� �հ�
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
	printf("2-1000������ �Ҽ��� �հ� : %d\n", hap);
	*/

	// while
	/*
	int hap = 0;
	int i = 0;

	while (i <= 100) {
		hap += i;
		i += 1;
	}
	printf("1-100���� �հ� : %d\n\n", hap);
	
	// 100-200���� �հ�
	hap = 0;
	i = 100;
	while (i <= 200) {
		hap += i;
		i += 1;
	}
	printf("100-200���� �հ� : %d\n\n", hap);


	// 4444-1111���� �հ�
	hap = 0;
	i = 4444;
	while (i >= 1111) {
		hap += i;
		i = i - 1;
	}
	printf("4444-1111���� �հ� : %d\n\n", hap);


	// 1-1000���� ¦���� �հ�
	hap = 0;
	i = 2;
	while (i <= 1000) {
		hap += i;
		i += 2;
	}
	printf("1-1000���� ¦���� �հ� : %d\n\n", hap);



	// 12345-67890���� 1877�� ����� �հ�
	hap = 0;
	i = 12345;
	while (i <= 67890){
		if (i % 1877 == 0){
			hap += i;
		}
		i++;
	}
	printf("12345-67890���� 1877�� ����� �հ� : %d\n\n", hap);
	*/

	// ���� �ݺ�: ���� break�� �����ؼ� �ߴ����� �����ϴ°� �Ϲ���
	/*
	* for(;;){ // ����� }
	* while( true ){ // ����� }
	* while( 1 ){ // ����� }
	* while( 2 ){ // ����� }
	*/

	// 1���� ���ڸ� ���ϰ� ������ ���� ���ڰ� 1���� ������ �׸��ҷ�
	int hap = 0;
	int i = 1;
	while (1) {
		hap += i;
		if (hap >= 100000000) {
			break;
		}
		i++;
	}
	printf("1���� ���ڸ� ���ϴµ� 1���� ������ �׸��ҷ�\n---  i: %d\n--- ��: %d\n", i, hap);

	// exit: ���α׷� ���� (���� �� ����)
	// 
	// return: �Լ� ���� (�׳��� ����)
}