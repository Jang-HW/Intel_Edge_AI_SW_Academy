#include <linux/kernel.h>
#include <linux/gpio.h>
 
#define OFF 0
#define ON 1
#define GPIOCNT 8

int gpioLed[] = {6,7,8,9,10,11,12,13};
int gpioKey[] = {16,17,18,19,20,21,22,23};
asmlinkage long sys_mysyscall(long val)
{
//	printk(KERN_INFO "Welcome to ~ val : %ld\n",val);
//	return val * val;
	unsigned long keyData=0;
	unsigned long temp;
	char gpioName[10];
	int ret = 0;
	int i;
//	int  gpioCnt = sizeof(gpioLed)/sizeof(gpioLed[0] );
	for(i=0;i<GPIOCNT;i++)
	{
		sprintf(gpioName, "gpio%d", gpioLed[i]);
		ret = gpio_request(gpioLed[i], gpioName);
		if(ret < 0) {
			printk("Failed request gpio%d error\n", gpioLed[i]);
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		ret = gpio_direction_output(gpioLed[i],OFF);
		if(ret < 0) {
			printk("Failed direction_output gpio%d error\n",gpioLed[i]);
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		gpio_set_value(gpioLed[i],(val>>i) & 0x1);
		// gpio_set_value(gpioLed[i],val & ( 0x1 << i));
	}

	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioLed[i]);
	}


	for(i=0;i<GPIOCNT;i++)
	{
		sprintf(gpioName, "gpio%d", gpioKey[i]);
		ret = gpio_request(gpioKey[i],gpioNmae);
		if(ret < 0) {
			printk("Failed request gpio%d error\n",gpioKey[i]);
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		ret = gpio_direction_input(gpioKey[i]);
		if(ret < 0) {
			printk("Failed direction_output gpio%d error\n",gpioKey[i]);
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		temp = gpio_get_value(gpioKey[i]) << i;
		keyData |= temp;
	}

	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioKey[i]);
	}


	return keyData;
}
