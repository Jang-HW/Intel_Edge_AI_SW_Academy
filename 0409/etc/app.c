/*
 * app.c
 *
 *  Created on: Apr 8, 2024
 *      Author: IOT
 */
#include <stdio.h>
#include "main.h"
#include "app.h"
#include "button.h"


void button_callback(void * arg){
	static uint8_t count = 0;			// 코드 아님 : 값 유지, 유사 전역변수

	BUTTON_STS *pSts = (BUTTON_STS *)arg;

	if(pSts->edge)	printf("rising\r\n");
	if(!pSts->edge)	printf("falling : period = %d\r\n", pSts->pushed_count);

	count++;
	count %= 10;

	if(count == 0)	{
		button_regcbf(0, button_callback2);
		printf("Call back function is changed to callback_2\t\n");
	}
}

void button_callback2(void * arg){
	static uint8_t count = 0;			// 코드 아님 : 값 유지, 유사 전역변수

	count++;
	count %= 10;

	if(count == 0)	{
		button_regcbf(0, button_callback);
		printf("Call back function is changed to callback_1\t\n");
	}
}

void app(void){
	 uint32_t tick_prev, tick_curr;

	button_init();						// 버튼 초기화 함수
	button_regcbf(0, button_callback);

	tick_prev = tick_curr = HAL_GetTick();

	while(1){
		tick_curr = HAL_GetTick();

		if(tick_curr - tick_prev >= 1){
			button_check();
			tick_prev = tick_curr;
		}
	}
}



#if 0
void app(void)
{
	//uint16_t btn;
	//// Error!!!		GPIO_TypeDef *pPort = 0x40020800;
	//GPIO_TypeDef *pPort = GPIOC;	// (GPIO_TypeDef *)0x40020800;
	GPIO_PinState sts;

	printf("System start!\r\n");

	while(1){
		HAL_Delay(100);

		// btn = *(uint16_t *)0x40020810;		// Port C IDR
		//btn = pPort->IDR;

		//// 버튼 하나만 읽을
		//if(btn & (0x1 << 13)){
		//	printf("Blue button is pressed~\r\n");
		//}

		//sts = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
		sts = HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin);
		if(sts == GPIO_PIN_SET)
			printf("Blue button is pressed~\r\n");
	}
}
#endif
