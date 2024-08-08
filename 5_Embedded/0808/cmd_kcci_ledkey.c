#include <vsprintf.h>
#include <command.h>
#include <asm/io.h>

#define BCM2711_GPIO_GPFSEL0 0xFE200000
#define BCM2711_GPIO_GPFSEL1 0xFE200004
#define BCM2711_GPIO_GPFSEL2 0xFE200008

#define BCM2711_GPIO_GPSET0 0xFE20001C
#define BCM2711_GPIO_GPCLR0 0xFE200028
#define BCM2711_GPIO_GPLEV0 0xFE200034

#define GPIO6_9_SIG_OUTPUT 0x09240000
#define GPIO10_13_SIG_OUTPUT 0x00012249  // rxd, txd
#define GPIO16_19_SIG_INPUT 0x00012249
#define GPIO20_23_SIG_INPUT 0x00000000

void led_init(void)
{
	writel(GPIO6_9_SIG_OUTPUT, BCM2711_GPIO_GPFSEL0);
	writel(GPIO10_13_SIG_OUTPUT, BCM2711_GPIO_GPFSEL1);
}

void key_init(void)
{
	writel(GPIO16_19_SIG_INPUT, BCM2711_GPIO_GPFSEL1);
	writel(GPIO20_23_SIG_INPUT, BCM2711_GPIO_GPFSEL2);
}
	
void led_write(unsigned long led_data)
{
	writel(0x3fc0, BCM2711_GPIO_GPCLR0);   // led all off
	led_data = led_data << 6;
	writel(led_data, BCM2711_GPIO_GPSET0);   // ledX on
}

void key_read(unsigned long *key_data)
{
	unsigned long read_key;

    read_key = readl(BCM2711_GPIO_GPLEV0);   // 키 상태 읽기
    *key_data = ((read_key << 8) >> 24);
//    printf("Key Read: %lu\n", *key_data); // 읽은 키 값 출력
}

static int do_KCCI_LED(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned long led_data;
	unsigned long key_data;
	unsigned long tmp_key = 0;

	if(argc != 2)
	{
		cmd_usage(cmdtp);
		return 1;
	}
	
	printf("*LED TEST START\n");
	led_init();
	key_init();
	led_data = simple_strtoul(argv[1], NULL, 16);
	led_write(led_data);

	do{
		key_read(&key_data);	// return : keydata - 0x00~0xff

		if((tmp_key == key_data) ||  key_data == 0){
			if(key_data ==0) tmp_key = 0;
			continue;
		}

		tmp_key = key_data;

		// o, x 출력 부분
		printf("0:1:2:3:4:5:6:7\n");

		
		for (int i = 0; i < 8; i++){
			if(key_data & (1 << i)) 
					printf("O");
			else
					printf("X");
		
			if(i != 7)	
					printf(":");
		}

		led_write(key_data);

		printf("\n\n");

	}while(key_data != 1 << 7);


	printf("*LED TEST END(%s : %#04x)\n\n ", argv[0], (unsigned int)led_data);
	
	return 0;
}

U_BOOT_CMD(
	led, 2, 0, do_KCCI_LED,
	"kcci LED Test.",
	"number - Input argument is only one. (led [0x00 ~ 0xff])\n");
