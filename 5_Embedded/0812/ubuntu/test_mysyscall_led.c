#include <linux/kernel.h>
#include <linux/gpio.h>

// now circit is 0 is off and 1 is on
#define OFF 0
#define ON 1
#define GPIO_CNT 8

int gpioLed[] = {6, 7, 8, 9, 10, 11, 12, 13};

long sys_mysyscall(long val)
{
	int ret;
	int i;
//	int gpioCnt = sizeof(gpioLed) / sizeof(gpioLed[0]);
	char gpioName[10];

	printk(KERN_INFO "Welcome to KCCI's Embedded System!! app value=%ld\n", val);

	// gpi  request
	for(i = 0; i < GPIO_CNT; i++)
	{
		sprintf(gpioName, "gpio%d", gpioLed[i]);

		ret = gpio_request(gpioLed[i], gpioName);
		if(ret < 0){
			printk("Failed Request GPIO %d error\n", gpioLed[i]);
			// changed auto to perror
			return ret;
		}
	}

	// gpio set default in OFF
	for(i = 0; i < GPIO_CNT; i++)
	{
		ret = gpio_direction_output(gpioLed[i], OFF);
		if(ret < 0) {
			printk("Failed Direction GPIO %d error\n", gpioLed[i]);
			// changed auto to perror
			return ret;
		}
	}

	// set gpio in val
		// val  = 0x00 ~ 0xff
	for(i = 0; i < GPIO_CNT; i++)
	{
		gpio_set_value(gpioLed[i], val & (0x01 << i));	
	}

	// gpio free 
	for(i = 0; i < GPIO_CNT; i++) 
	{
		gpio_free(gpioLed[i]);
	}

	return val*val;
}
