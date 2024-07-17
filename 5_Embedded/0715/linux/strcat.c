#include <stdio.h>
#define __CRT_SECURE_NO_WARNINGS

char *my_strcat(char*, const char*);

int main(int argc, char* argv[]) {

	// 초기화를 하는 경우
	char fruit[80] = "straw";

	printf("%p \n", "straw");
	printf("%p \n", "berry");

	my_strcat(fruit, "berry");

	printf("strcat string : %s\n", fruit);
	printf("strcat string : %s\n", my_strcat(fruit, "berry"));
	return 0;
}

char* my_strcat(char *pd, const char *ps) {
	char *po = pd;

	while(*pd != '\0') 
		pd++;

	while (*ps != '\0') {
		*pd = *ps;
		pd++;
		ps++;
	}
	*pd = '\0';
	return po;
}
