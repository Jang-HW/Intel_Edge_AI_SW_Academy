#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI �������

void main() {
	
	//int size;
	//scanf("%d", &size);
	//unsigned char ary[size];

	int size = 50;
	int* p;

	p = (int*)malloc(sizeof(int) * size);		// == int p[50]

	p[0] = 100;		//	*(p + 0) = 100;
	p[1] = 200;		//	*(p + 1) = 200;
	p[2] = 300;		//	*(p + 2) = 300;

	printf("%d %d\n", *(p + 1), p[1]);

	free(p);




	// ����: ������ ���� �޸� 100ĭ Ȯ�� ��, �ű⿡ 1234, 1235, ... �� �����ϱ�
	//					�׸��� �հ� ����ϱ�
	float tmp = 1234;
	long long sum = 0;			// �������� 21���� �ݹ��̶� �̷��� long long����
	
	size = 100;
	int* quiz1 = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++) {
		quiz1[i] = tmp;
		tmp++;
	}
	for (int i = 0; i < size; i++) {
		sum += quiz1[i];
	}
	printf("sum: %d\n", sum);

	free(quiz1);

	// ���� : �Ǽ��� �����޸� 5ĭ 2�� �غ��ϰ�, ù��°���� 0.1~0.5���� ����
	//											  �ι��翡�� ù��°���� �������� ����
	tmp = 0.0;

	size = 5;
	double *quiz2_1 = (double*)malloc(sizeof(double) * size);
	double* quiz2_2 = (double*)malloc(sizeof(double) * size);

	for (int i = 0; i < size; i++) {
		tmp += 0.1;
		quiz2_1[i] = tmp;
		printf("%lf ", quiz2_1[i]);
	}
	printf("\n");

	for (int i = 0; i < size; i++) {
		quiz2_2[i] = tmp;
		printf("%lf ", quiz2_2[i]);
		tmp -= 0.1;
	}

	free(quiz2_1);
	free(quiz2_2);



	// 2���� �迭�� �����޸�
	
}