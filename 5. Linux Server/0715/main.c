#include <stdio.h>
#include "main.h"
#define __CRT_SECURE_NO_WARNINGS

char *my_strcat(char*, const char*);

// start up code로 return 되는 부분
// 인자를 받는 부분을 필수적으로 추가
	// (int argc, char* argv[])
int main(int argc, char* argv[]) {

	// 초기화를 하는 경우
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

	/*/ 초기화를 안 하는 경우
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