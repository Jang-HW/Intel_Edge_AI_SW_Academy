#include <stdio.h>
#include "main.h"
#define __CRT_SECURE_NO_WARNINGS

char *my_strcat(char*, const char*);

// start up code�� return �Ǵ� �κ�
// ���ڸ� �޴� �κ��� �ʼ������� �߰�
	// (int argc, char* argv[])
int main(int argc, char* argv[]) {

	// �ʱ�ȭ�� �ϴ� ���
	char fruit[80] = "straw";

	printf("%p \n", "straw");
	printf("%p \n", "berry");
	
	//printf("%x - main_b \n", fruit);
	my_strcat(fruit, "berry");
	//printf("%x - main_a \n", fruit);

	printf("strcat string : %s\n", fruit);
	printf("strcat string : %s\n", my_strcat(fruit, "berry"));
	return 0;
	//*/

	/*/ �ʱ�ȭ�� �� �ϴ� ���
	char fruit[80];
	strcpy(fruit, "straw");
	my_strcat(fruit, "berry");

	printf("strcat string : %s\n", fruit);
	printf("strcat string : %s\n", my_strcat(fruit, "berry"));
	return 0;
	//*/
}

char* my_strcat(char *pd, const char *ps) {
	char *po = pd;

	while(*pd != '\0') 
		pd++;
	//printf("%x - my_strcat_cnt\n", pd);

	while (*ps != '\0') {
		*pd = *ps;
		pd++;
		ps++;
		//printf("%x - my_strcat_cp\n", pd);
	}
	*pd = '\0';
	return po;
}