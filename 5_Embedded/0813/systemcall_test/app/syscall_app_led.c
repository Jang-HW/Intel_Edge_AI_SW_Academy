#include <stdio.h>
#include <unistd.h>
#include <asm-generic/unistd.h>
//#pragma GCC diagnostic ignored "-Wunused-result"
int main()
{
        long i;
        printf("input value = ");
        scanf("%x",&i);
        i = syscall(__NR_mysyscall,i);
        if(i<0)
        {
                perror("syscall");
                return 1;
        }
        printf("mysyscall return value = %#04x\n",i);
        return 0;
}

