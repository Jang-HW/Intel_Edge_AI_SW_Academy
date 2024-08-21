#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/gpio.h>

#define LEDKEY_DEV_NAME "calldev"
#define LEDKEY_DEV_MAJOR 230

#define OFF 0
#define ON 1
#define GPIOCNT 8

static long gpioLedInit(void);
static void gpioLedSet(long);
static void gpioLedFree(void);
static long gpioKeyInit(void);
static long gpioKeyGet(void);
static void gpioKeyFree(void);

static int gpioLed[] = {6,7,8,9,10,11,12,13 };
static int gpioKey[] = {16,17,18,19,20,21,22,23};

static int ledkey_open(struct inode *inode, struct file *filp)
{
		int num = MINOR(inode->i_rdev);
		printk("call open -> minor : %d \n", num);
		num = MAJOR(inode->i_rdev);
		printk("call open -> major : %d \n", num);

		return 0;
}

static loff_t ledkey_llseek(struct file *filp, loff_t off, int whence)
{
		printk("call llseek -> off : %08X, whence : %08X \n", (unsigned int)off, whence);
		return 0x23;
}

static ssize_t ledkey_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
		char kbuf;
		int count_re;

		kbuf = (char)gpioKeyGet();
//		*buf = kbuf;
//		put_user(kbuf, buf);
		count_re = copy_to_user(buf, &kbuf, count);

		printk("key : %#04lX\n", (long)kbuf);
		printk("call read -> buf : %08X, count : %08X \n", (unsigned int)buf, count);

		if (count_re < 0) return -EFAULT;

		return count;
}

static ssize_t ledkey_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
		char kbuf;
		int count_re;

//		get_user(kbuf, buf);
		count_re = copy_from_user(&kbuf, buf, count);

		gpioLedSet(*(long *)buf);
		printk("call write -> buf : %08X, count : %08X \n", (unsigned int)buf, count);

		if (count_re < 0) return -EFAULT;

		return count;
}

static long ledkey_iotctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
		printk("call iotctl -> cmd : %08X, arg: %08X \n", cmd, (unsigned int)arg);
		return 0x53;
}

static int ledkey_release(struct inode *inode, struct file *filp)
{
		printk("call release \n");
		return 0;
}

/////////////////////

static long gpioLedInit(void)
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
static void gpioLedSet(long val)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
		gpio_set_value(gpioLed[i], val & (0x1 << i));
//		gpio_set_value(gpioLed[i],(val>>i) & 0x1);
	}

}
static void gpioLedFree(void)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioLed[i]);
	}
}
static long gpioKeyInit(void)
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
static long gpioKeyGet(void)
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
static void gpioKeyFree(void)
{
	int i;
	for(i=0;i<GPIOCNT;i++)
	{
  		gpio_free(gpioKey[i]);
	}
}

//////////////////////////////////////

struct file_operations ledkey_fops = 
{
	.owner	 	= THIS_MODULE, 
	.llseek		= ledkey_llseek, 
	.read		= ledkey_read, 
	.write		= ledkey_write, 
	.unlocked_ioctl = ledkey_iotctl, 
	.open		= ledkey_open, 
	.release	= ledkey_release, 
};

static int ledkey_init(void)
{
	int result;

	result = gpioKeyInit();
	if(result < 0)
		return result;

	result = gpioLedInit();
	if(result < 0)
		return result;

	printk("call ledkey_init \n");
	result = register_chrdev(LEDKEY_DEV_MAJOR, LEDKEY_DEV_NAME, &ledkey_fops);
	if(result < 0) return result;

	return 0;
}

static void ledkey_exit(void)
{
	    gpioLedSet(0x00);
	    gpioLedFree();
		gpioKeyFree();
		printk("call ledkey_exit \n");
		unregister_chrdev(LEDKEY_DEV_MAJOR, LEDKEY_DEV_NAME);
}

module_init(ledkey_init);
module_exit(ledkey_exit);

MODULE_AUTHOR("Jang-HW");
MODULE_DESCRIPTION("test module");
MODULE_LICENSE("Dual BSD/GPL");


