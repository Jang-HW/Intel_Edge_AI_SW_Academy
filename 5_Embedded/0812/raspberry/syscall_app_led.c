#include <stdio.h>
#include <unistd.h>
#include <asm-generic/unistd.h>
#pragma GCC diagnostic ignored "-Wunused-result"

#define GPIO_CNT 8

int main(){
	int i;
	long val;

	printf("input value = ");
	scanf("%x", &val);

	val = syscall(__NR_mysyscall, val);

	if(val < 0) {
		perror("syscall");
		return 1;
	}
	printf("mysyscall return value = %04x\n", val);

	puts("0:1:2:3:4:5:6:7");
	for(i = 0; i < GPIO_CNT; i++){
		if(val & (0x01 << i))
			putchar('O');
		else 
			putchar('X');

		if (i != 7)
			putchar(':');
		else 
			putchar('\n');
	}
	return 0;
}