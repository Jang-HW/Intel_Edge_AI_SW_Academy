#include <stdio.h>
#include <unistd.h>
#include <asm-generic/unistd.h>
#pragma GCC diagnostic ignored "-Wunused-result"

int main(){
	int i; 
	long key_data;
	long key_data_old=0;
	long val;

	printf("input value = ");
	scanf("%x", &val);

	do{
		key_data = syscall(__NR_mysyscall, val);
		
		if(key_data != key_data_old){
			if(key_data < 0) {
				perror("syscall");
				return 1;
			}
			if (key_data == 0){
				key_data_old = 0;
				continue;
			}

			puts("0:1:2:3:4:5:6:7");
			for(i = 0; i < 8; i++){
				if(key_data & (0x01 << i))
					putchar('O');
				else 
					putchar('X');

				if (i != 7)
					putchar(':');
				else 
					putchar('\n');
			}
			putchar('\n');

			key_data_old = key_data;

			if(key_data >= 0x80)
				break;
		}

	} while(1);
	return 0;
}
