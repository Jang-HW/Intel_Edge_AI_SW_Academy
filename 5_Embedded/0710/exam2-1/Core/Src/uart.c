/*
 * uart.h
 *
 *  Created on: Jul 10, 2024
 *      Author: IOT
 */

#include "uart.h"


UART_HandleTypeDef *uartHandle;

// 수신한 data를 저장하기 위한 공간
uint8_t rxChar;

// 수신한 글자의 조합 상식은 여러 통신방식에서 circular buffer에 저장하도록홤
	// 원형 버퍼 / 원형 큐
	// 저장을 위한 원형 버퍼
		// 일반적으로 패킷의 2배~3배 정도로 지정하기도
		// 통신으로 큰 용량을 보내다 보면 데이터가 0.2% 확률로 깨지는 현상이 발생하기도 함
		// 클럭 차이로 발생하기도
		// 크리스탈을 끼워주면 더 정확하게 송수신함
		// 소켓 통신 속도를 2KB 이상 쓰지 않는 이유임
#define rxBufferMax 255
int rxReadPointer;
int rxWritePointer;
uint8_t rxBuffer[rxBufferMax];

void initUart(UART_HandleTypeDef *inHandle){
	// 내부적으로 다른 uart 저장 가능
	uartHandle = inHandle;

	// 대기 시켜놨다가 1글자를 수신하면, interrupt 발생
		// 이걸 여러 글자를 하지 않는 이유는 수신 오류로
		// 못 받는 경우를 대비하기 위해서, 그냥 하나씩 처리하도록 함
	HAL_UART_Receive_IT(uartHandle, &rxChar, 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart){
	rxBuffer[rxWritePointer] = rxChar;
	rxWritePointer++;
	rxWritePointer %= rxBufferMax;

	// 문자열을 계속해서 수신하기 위해서 다시 준비 시킴
	HAL_UART_Receive_IT(uartHandle, &rxChar, 1);
}


// 문자열 하나를 버퍼에서 꺼내오는 함수
uint16_t getRxBuffer(){
	uint16_t result;

	// 수신한 data가 없을 때를 처리하기 위한 부분
	// binary data 수신할 경우,
		// 수신한 data가 0인 경우와 헷갈리기 때문에
			// -1으로 지정하거나 함수의 출력형을 큰거로 사용함
	if(rxWritePointer == rxReadPointer)	return -1;

	result = rxBuffer[rxReadPointer];
	rxReadPointer++;
	rxReadPointer %= rxBufferMax;

	return result;
}

char *getString(){
	static char str[10];
	static uint8_t pos = 0;
	int16_t ch = getRxBuffer();

	// 통신 시작 문자가 처리되고 나서야 문자열 수신을 시작
	if (ch != -1){
		if (ch == '\n'){
			// 배열을 0으로 10칸 초기화
			memset(str, 0, 10);
			pos = 0;
		}
		else{
			str[pos] = ch;
			pos++;
		}
	}
	return str;
}

void putSerial(int inValue){
	// 지난 시간에 파이썬에서 쓸 통신 프로토콜을 동일하게 사용할 것
	char txBuffer[5] = { 0, };
	txBuffer[0] = STX;

	txBuffer[1] = (inValue & 0x7f) | 0x80;
	txBuffer[2] = ((inValue >> 7) & 0x7f) | 0x80;
	// 00{01 1010 0}[110 1100]이라는 data를 보낼려함
		// 110 1100 | 0b1000 0000 = 0b1110 1100
		// 011 0100 | 0b1000 0000 = 0b1011 0100

	// over flow가 나면 알아서 버려지므로 걱정하지말고 편하게 짜면됨
	txBuffer[3] = txBuffer[0] + txBuffer[1] + txBuffer[2];
	txBuffer[4] = ETX;

	HAL_UART_Transmit(uartHandle, txBuffer, 5, 1);
}

void putSerial2(int inValue){
	char txBuffer[4] = { 0, };
	txBuffer[0] = 0x11;
	txBuffer[1] = 0x22;
	// 16 bit 데이터를 8bit로 쪼개기 위한 방법
	// 정석
		// txBuffer[2] = inValue;
		// txBuffer[3] = inValue >> 8;
		// 설명
			// inValue = 0b 1010 1010 0011 1100;
			// txBuffer[2] = 0b 1010 1010 0011 1100;
			//													(리틀엔디안이라 뒤에 8자리만 담김)
			// txBuffer[3] = 0b 1010 1010 0011 1100 >> 8;
			//						 = 0b 1010 1010;
	memcpy(&txBuffer[2], &inValue, 2);

	HAL_UART_Transmit(uartHandle, txBuffer, 5, 1);
}
