/*
 BitzOS (BOS) V0.2.9 - Copyright (C) 2017-2023 Hexabitz
 All rights reserved

 File Name     : main.c
 Description   : Main program body.
 */
/* Includes ------------------------------------------------------------------*/
#include "BOS.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Main function ------------------------------------------------------------*/
int main(void){


	Module_Init();		//Initialize Module &  BitzOS
	//Don't place your code here.
	for(;;){}
}
uint8_t pData[5] = "hello";
uint8_t pData1[5];
/*-----------------------------------------------------------*/
/* User Task */
void UserTask(void *argument){
//	int x;
//	AddButton(P1, SWITCH_NO, DBL_CLICKED);
//	AddButton(P2, SWITCH_NO, DBL_CLICKED);
//	AddButton(P3, SWITCH_NO, DBL_CLICKED);
//	AddButton(P4, SWITCH_NO, DBL_CLICKED);
//	AddButton(P5, SWITCH_NO, DBL_CLICKED);
//	AddButton(P6, SWITCH_NO, DBL_CLICKED);
	// put your code here, to run repeatedly.
	while(1){
//		HAL_UART_Transmit(&huart2, pData, 5, 1000);
		HAL_UART_Receive(&huart2, pData1, 5, 1000);
//		x++;
		HAL_Delay(10);
	}
}

void buttonDblClickedCallback(uint8_t port){
	IND_blink(100);
}
/*-----------------------------------------------------------*/
