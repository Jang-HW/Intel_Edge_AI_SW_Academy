#include <stdio.h>

int main() {

	// ���� ����Ģ: ������ �̸��� ���� �װ� ���� �뵵���� �� �� �ֵ��� �ϱ�
	//		integer type input first number
	//		integer_type_input_first_number 
	//		integerTypeInputFirstNumber
	//		integerFirstNumber
	//		inNnumber1
	//		iNum1
	// 
	// ���� �ڵ�? �ٸ� ����� ���ذ� ���� �ڵ�

	int iNum1, iNum2;			// ���� ����
	float fResult;

	iNum1 = 100;
	iNum2 = 200;


	// ���ϱ�
	fResult = iNum1 + iNum2;
	printf("%d + %d = %lf\n", iNum1, iNum2, fResult);

	// ���� 
	fResult = iNum1 - iNum2;
	printf("%d - %d = %lf\n", iNum1, iNum2, fResult);

	// ���ϱ�
	fResult = iNum1 * iNum2;
	printf("%d * %d = %lf\n", iNum1, iNum2, fResult);

	// ������
	fResult = (float)iNum1 / iNum2;
	printf("%d / %d = %lf\n", iNum1, iNum2, fResult);

	// ��
	fResult = iNum1 / iNum2;
	printf("%d // %d = %lf\n", iNum1, iNum2, fResult);

	// ������
	fResult = iNum1 % iNum2;
	printf("%d %% %d = %lf\n", iNum1, iNum2, fResult);


	return;
}