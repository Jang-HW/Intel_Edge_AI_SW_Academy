/*
 * app.c
 *
 *  Created on: Apr 8, 2024
 *      Author: IOT
 */
#include <stdio.h>
#include "main.h"
#include "app2.h"
#include "button.h"
#include "temperate.h"

static uint16_t nowTemperature = 0;

void btn_callback(void * arg){

	BUTTON_STS *pSts = (BUTTON_STS *)arg;
	if(pSts->edge)	temperate_regBorder(0, nowTemperature);
}

void temperate_callback(void * arg){

	TEMPERATE_STS *pState = (TEMPERATE_STS *)arg;
	nowTemperature = pState->state;

	printf("- temperature: %d\r\n", (pState->state) - (pState->border));
	printf("%d\t %d\r\n\n", pState->state, pState->border);
}


void app2(void){

	 uint32_t tick_prev, tick_curr;

	temperate_init();						// 온도계 초기화 함수
	temperate_regcbf(0, temperate_callback);
	temperate_regBorder(0, 30000);

	button_init();							// 버튼 초기화 함수
	button_regcbf(0, btn_callback);

	tick_prev = tick_curr = HAL_GetTick();

	while(1){
		tick_curr = HAL_GetTick();

		if(tick_curr - tick_prev >= 1){
			button_check();
			temperate_check();
			tick_prev = tick_curr;
		}
	}
}
