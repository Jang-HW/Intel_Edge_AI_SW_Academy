#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/moduleparam.h>

#define DEBUG 1

#define OFF 0
#define ON 1
#define GPIOCNT 8

long gpioLedInit(void);
void gpioLedSet(long);
void gpioLedFree(void);
long gpioKeyInit(void);
long gpioKeyGet(void);
void gpioKeyFree(void);

int gpioLed[] = {6,7,8,9,10,11,12,13};
int gpioKey[] = {16,17,18,19,20,21,22,23};


static int timerVal = 100;		// f=100Hz, t = 1/100=10ms, 100*10ms = 1sec
module_param(timerVal, int, 0);
static int ledVal = 0;
module_param(ledVal, int, 0);
struct timer_list timerLed;

void kerneltimer_func(struct timer_list *t);
void kerneltimer_registertimer(unsigned long timeover)
{
		timerLed.expires = get_jiffies_64 () + timeover;		// 10ms * 100 = 1sec
		timer_setup(&timerLed, kerneltimer_func, 0);
		add_timer(&timerLed);
}
void kerneltimer_func(struct timer_list *t)
{
#ifdef DEBUG
		printk("ledVal : %#04x\n", (unsigned int)(ledVal));
#endif
		gpioLedSet(ledVal);

		ledVal = ~ledVal & 0xff;
		mod_timer(t, get_jiffies_64() + timerVal);
}

int kerneltimer_init(void)
{
#ifdef DEBUG
		printk("timerVal : %d, sec : %d \n", timerVal, timerVal/HZ);
#endif
		kerneltimer_registertimer(timerVal);
		return 0;
}
void kerneltimer_exit(void)
{
		if(timer_pending(&timerLed))
				del_timer(&timerLed);
}

long gpioLedInit(void)
{
	long ret = 0;
	int i;
	char gpioName[10];
	for(i=0;i<GPIOCNT;i++)
	{
		sprintf(gpioName,"gpio%d",gpioLed[i]);
		ret = gpio_request(gpioLed[i],gpioName);
		if(ret < 0) {
			printk("Failed request gpio%d error\n",gpioLed[i]);
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
	return ret;
}

void gpioLedSet(long val)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
		gpio_set_value(gpioLed[i],val & (0x1 << i));
//		gpio_set_value(gpioLed[i],(val>>i) & 0x1);
	}

}

void gpioLedFree(void)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioLed[i]);
	}
}

long gpioKeyInit(void)
{
	long ret = 0;
	int i;
	char gpioName[10];
	for(i=0;i<GPIOCNT;i++)
	{
		sprintf(gpioName,"gpio%d",gpioKey[i]);
		ret = gpio_request(gpioKey[i],gpioName);
		if(ret < 0) {
			printk("Failed request gpio%d error\n",6);
			return ret;
		}
	}
	for(i=0;i<GPIOCNT;i++)
	{
		ret = gpio_direction_input(gpioKey[i]);
		if(ret < 0) {
			printk("Failed direction_output gpio%d error\n",6);
			return ret;
		}
	}
	return ret;
}

long gpioKeyGet(void)
{
	long keyData=0;
	long temp;
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
		temp = gpio_get_value(gpioKey[i]) << i;
		keyData |= temp;
	}
	return keyData;
}

void gpioKeyFree(void)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioKey[i]);
	}
}


module_init(kerneltimer_init);
module_exit(kerneltimer_exit);
MODULE_AUTHOR("KCCI-AIOT-JHW");
MODULE_DESCRIPTION("led key test module");
MODULE_LICENSE("Dual BSD/GPL");
