#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>		// GUI 지원기능

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




	// 퀴즈: 정수형 동적 메모리 100칸 확보 후, 거기에 1234, 1235, ... 값 대입하기
	//					그리고 합계 출력하기
	float tmp = 1234;
	long long sum = 0;			// 누적합은 21억이 금방이라 이런건 long long으로
	
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

	// 퀴즈 : 실수형 동적메모리 5칸 2개 준비하고, 첫번째에는 0.1~0.5까지 대입
	//											  두번재에는 첫번째꺼의 역순으로 대입
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



	// 2차원 배열과 동적메모리
	
}