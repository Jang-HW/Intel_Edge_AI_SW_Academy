#include <linux/kernel.h>
#include <linux/gpio.h>

// now circit is 0 is off and 1 is on
#define OFF 0
#define ON 1
#define GPIO_CNT 8

int gpioLed[] = {6, 7, 8, 9, 10, 11, 12, 13};
int gpioBtn[] = {16, 17, 18, 19, 20, 21, 22, 23};

long sys_mysyscall(long val)
{
	int ret;
	int i;
	int input;
//	int gpioCnt = sizeof(gpioLed) / sizeof(gpioLed[0]);
	char gpioName[10];
	long var = 0;

	printk(KERN_INFO "Welcome to KCCI's Embedded System!! app value=%ld\n", val);

	// gpi  request
	for(i = 0; i < GPIO_CNT; i++)
	{
		sprintf(gpioName, "gpioBTN%d", gpioBtn[i]);
		ret = gpio_request(gpioBtn[i], gpioName);
		if(ret < 0){
			printk("Failed Request GPIO btn %d error\n", gpioBtn[i]);
			// changed auto to perror
			return ret;
		}
		
		sprintf(gpioName, "gpioLED%d", gpioBtn[i]);
		ret = gpio_request(gpioLed[i], gpioName);
		if(ret < 0){
			printk("Failed Request GPIO led %d error\n", gpioLed[i]);
			// changed auto to perror
			return ret;
		}
	}

	// gpio set default in OFF
	for(i = 0; i < GPIO_CNT; i++)
	{
		ret = gpio_direction_input(gpioBtn[i]);
		if(ret < 0) {
			printk("Failed Direction GPIO btn %d error\n", gpioBtn[i]);
			// changed auto to perror
			return ret;
		}

		ret = gpio_direction_output(gpioLed[i], OFF);
		if(ret < 0) {
			printk("Failed Direction GPIO led %d error\n", gpioLed[i]);
			// changed auto to perror
			return ret;
		}
	}

	// get gpio in val
	for(i = 0; i < GPIO_CNT; i++)
	{
		input = gpio_get_value(gpioBtn[i]);
		if(input != 0)
				var += (0x01 << i);

		gpio_set_value(gpioLed[i], input);	
	}

	// gpio free 
	for(i = 0; i < GPIO_CNT; i++) 
	{
		gpio_free(gpioBtn[i]);
		gpio_free(gpioLed[i]);
	}
	
	return var;
}
