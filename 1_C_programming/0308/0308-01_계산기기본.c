#include <stdio.h>

int main() {

	// 변수 명명규칙: 변수는 이름만 봐도 그게 무슨 용도인지 알 수 있도록 하기
	//		integer type input first number
	//		integer_type_input_first_number 
	//		integerTypeInputFirstNumber
	//		integerFirstNumber
	//		inNnumber1
	//		iNum1
	// 
	// 좋은 코드? 다른 사람이 이해가 쉬운 코드

	int iNum1, iNum2;			// 변수 선언
	float fResult;

	iNum1 = 100;
	iNum2 = 200;


	// 더하기
	fResult = iNum1 + iNum2;
	printf("%d + %d = %lf\n", iNum1, iNum2, fResult);

	// 빼기 
	fResult = iNum1 - iNum2;
	printf("%d - %d = %lf\n", iNum1, iNum2, fResult);

	// 곱하기
	fResult = iNum1 * iNum2;
	printf("%d * %d = %lf\n", iNum1, iNum2, fResult);

	// 나누기
	fResult = (float)iNum1 / iNum2;
	printf("%d / %d = %lf\n", iNum1, iNum2, fResult);

	// 몫
	fResult = iNum1 / iNum2;
	printf("%d // %d = %lf\n", iNum1, iNum2, fResult);

	// 나머지
	fResult = iNum1 % iNum2;
	printf("%d %% %d = %lf\n", iNum1, iNum2, fResult);


	return;
}