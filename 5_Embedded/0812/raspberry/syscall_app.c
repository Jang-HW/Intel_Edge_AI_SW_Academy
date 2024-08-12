#include <stdio.h>
#include <unistd.h>
#include <asm-generic/unistd.h>
#pragma GCC diagnostic ignored "-Wunused-result"

#define GPIO_CNT 8

int main(){
	long val;

	printf("input value = ");
	scanf("%ld", &val);

	val = syscall(__NR_mysyscall, val);

	if(val < 0) {
		perror("syscall");
		return 1;
	}
	printf("mysyscall return value = %ld\n", val);

	return 0;
}
