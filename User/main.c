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

/*-----------------------------------------------------------*/
uint8_t f;
/* User Task */
void UserTask(void *argument){

	  AmpInit(AMP_SWITCHING_MODE_670KHZ, AMP_GAIN_MODE_29dB);
	  CodecInit(DAC_LVL_GAIN_MINUS_15dB, PLAYBACK_VOLUME_GAIN_PLUS_1dB, PLAYBACK_VOLUME_GAIN_PLUS_1dB);
//	  CodecStreamDataStart();
	// put your code here, to run repeatedly.
	while(1){
		if(f == 2)
		{
			CodecStreamDataStop();
			f=0;
		}
		if(f == 1)
		{
			CodecStreamDataStart();
			f=0;
		}
	}
}

/*-----------------------------------------------------------*/
