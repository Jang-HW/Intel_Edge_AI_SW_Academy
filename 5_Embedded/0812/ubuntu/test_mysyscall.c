#include <linux/kernel.h>

long sys_mysyscall(long val )
{
	printk(KERN_INFO "Welcome to KCCI's Embedded System!! app value=%ld\n", val);
	return val*val;
}
