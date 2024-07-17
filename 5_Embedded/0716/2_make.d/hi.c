#include <stdio.h>

int input();
int output(int);

int main(int argc, char *argv[]){
		int n = input();

		if(!(output(n)))
				printf("Good bye!\n");

		return 0;
}
