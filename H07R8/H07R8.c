/*
 BitzOS (BOS) V0.3.1 - Copyright (C) 2017-2024 Hexabitz
 All rights reserved

 File Name     : H07R8.c
 Description   : Source code for module H07R8.
 	 	 	 	 (Description_of_module)

(Description of Special module peripheral configuration):
>>
>>
>>

 */

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"

/* Define UART variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;

/* Exported variables */
extern FLASH_ProcessTypeDef pFlash;
extern uint8_t numOfRecordedSnippets;

/* Module exported parameters ------------------------------------------------*/
module_param_t modParam[NUM_MODULE_PARAMS] ={{.paramPtr = NULL, .paramFormat =FMT_FLOAT, .paramName =""}};
#define MIN_PERIOD_MS				100


/* exported functions */

/* Private variables ---------------------------------------------------------*/
bool muteFlag;
uint8_t rx[BUFFER_FULL_SIZE]={0};
uint8_t dataFlag = 2;
/* Private function prototypes -----------------------------------------------*/
void ExecuteMonitor(void);

/* Create CLI commands --------------------------------------------------------*/

portBASE_TYPE CLI_CodecInitCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_CodecStreamStartCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_CodecStreamStopCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_CodecDAC_GainCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_CodecAudioLevelCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_CodecAudioMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_CodecShutdownCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_AmpGainCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_AmpMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_AmpShutdownCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );

const CLI_Command_Definition_t CLI_CodecInitDefinition =
{
	( const int8_t * ) "codecinit", /* The command string to type. */
	( const int8_t * ) "codecinit:\r\n initialize the Codec IC to receive stream digital audio data\r\n "
			"the parameters are :\r\n"
			"DAC level gain : set this parameter from 0(high) even 15 (low) if you listen saturation in the audio make this value low even audio be suitable.\r\n"
			"right audio level : set this value from 0 even 28.\r\n"
			"left audio level : set this value from 0 even 28.\r\n\r\n",
	CLI_CodecInitCommand, /* The function to run. */
	3 /* three parameters are expected. */
};

const CLI_Command_Definition_t CLI_CodecStreamStartDefinition =
{
	( const int8_t * ) "streamstart", /* The command string to type. */
	( const int8_t * ) "streamstart:\r\n digital audio data will be requested from SD Card module or another module has audio data. When the module starts up, start stream will be called automatically.\r\n ",
	CLI_CodecStreamStartCommand, /* The function to run. */
	0 /* three parameters are expected. */
};

const CLI_Command_Definition_t CLI_CodecStreamStopDefinition =
{
	( const int8_t * ) "streamstop", /* The command string to type. */
	( const int8_t * ) "streamstop:\r\n digital audio data will be stop from SD Card module or another module has audio data.\r\n ",
	CLI_CodecStreamStopCommand, /* The function to run. */
	0 /* three parameters are expected. */
};

const CLI_Command_Definition_t CLI_CodecDAC_GainDefinition =
{
	( const int8_t * ) "dacgain", /* The command string to type. */
	( const int8_t * ) "dacgain:\r\n control with dac gain \r\n"
			"the parameters is : DAC level gain : set this parameter from 0 (high) even 15 (low) if you listen saturation in the audio make this value low even audio be suitable.\r\n\r\n",
	CLI_CodecDAC_GainCommand, /* The function to run. */
	1 /* one parameter is expected. */
};

const CLI_Command_Definition_t CLI_CodecAudioLevelDefinition =
{
	( const int8_t * ) "audiolevel", /* The command string to type. */
	( const int8_t * ) "audiolevel:\r\n control with audio level to both left/right channels\r\n"
			"the parameters are :\r\n"
			"right audio level : set this value from 0 (high) even 28 (low).\r\n"
			"left audio level : set this value from 0 (high) even 28 (low).\r\n\r\n",
	CLI_CodecAudioLevelCommand, /* The function to run. */
	2 /* two parameters are expected. */
};

const CLI_Command_Definition_t CLI_CodecAudioMuteDefinition =
{
	( const int8_t * ) "audiomute", /* The command string to type. */
	( const int8_t * ) "audiomute:\r\n audio mute on the Codec's output where : 0 for disable mute and 1 for enable mute.\r\n\r\n",
	CLI_CodecAudioMuteCommand, /* The function to run. */
	1 /* one parameter is expected. */
};

const CLI_Command_Definition_t CLI_CodecShutdownDefinition =
{
	( const int8_t * ) "codecshut", /* The command string to type. */
	( const int8_t * ) "codecshut:\r\n shutdown Codec where : 0 for disable shutdown and 1 for enable shutdown .:\r\n\r\n",
	CLI_CodecShutdownCommand, /* The function to run. */
	1 /* one parameter is expected. */
};

const CLI_Command_Definition_t CLI_AmpGainDefinition =
{
	( const int8_t * ) "ampgain", /* The command string to type. */
	( const int8_t * ) "ampgain:\r\n setting amplifier gain there is 5 levels start from 0 \r\n\r\n",
	CLI_AmpGainCommand, /* The function to run. */
	1 /* one parameter is expected. */
};

const CLI_Command_Definition_t CLI_AmpMuteDefinition =
{
	( const int8_t * ) "ampmute", /* The command string to type. */
	( const int8_t * ) "ampmute:\r\n Amplifier mute where : 0 for disable and 1 for enable \r\n\r\n",
	CLI_AmpMuteCommand, /* The function to run. */
	1 /* one parameter is expected. */
};

const CLI_Command_Definition_t CLI_AmpShoutdownDefinition =
{
	( const int8_t * ) "ampshut", /* The command string to type. */
	( const int8_t * ) "ampshut:\r\n Amplifier shutdown where : 0 for disable and 1 for enable \r\n\r\n",
	CLI_AmpShutdownCommand, /* The function to run. */
	1 /* one parameter is expected. */
};
/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
 |						    	 Private Functions						 |
 -------------------------------------------------------------------------
 */

///**
// * @brief  System Clock Configuration
// *         The system Clock is configured as follow :
// *            System Clock source            = PLL (HSE)
// *            SYSCLK(Hz)                     = 48000000
// *            HCLK(Hz)                       = 48000000
// *            AHB Prescaler                  = 1
// *            APB1 Prescaler                 = 1
// *            HSE Frequency(Hz)              = 8000000
// *            PREDIV                         = 1
// *            PLLMUL                         = 6
// *            Flash Latency(WS)              = 1
// * @param  None
// * @retval None
// */
//void SystemClock_Config(void){
//	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//	  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//
//	  /** Configure the main internal regulator output voltage
//	  */
//	  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
//	  /** Initializes the RCC Oscillators according to the specified parameters
//	  * in the RCC_OscInitTypeDef structure.
//	  */
//	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
//	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//	  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//	  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
//	  RCC_OscInitStruct.PLL.PLLN = 12;
//	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//	  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//	  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
//	  HAL_RCC_OscConfig(&RCC_OscInitStruct);
//
//	  /** Initializes the CPU, AHB and APB buses clocks
//	  */
//	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//	                              |RCC_CLOCKTYPE_PCLK1;
//	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//
//	  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
//
//	  /** Initializes the peripherals clocks
//	  */
//	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART2;
//	  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
//	  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
//	  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
//
//
//	  HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
//
//}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 45;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}


/*-----------------------------------------------------------*/


/* --- Save array topology and Command Snippets in Flash RO --- 
 */
uint8_t SaveToRO(void){
	BOS_Status result =BOS_OK;
	HAL_StatusTypeDef FlashStatus =HAL_OK;
	uint16_t add =2, temp =0;
	uint8_t snipBuffer[sizeof(snippet_t) + 1] ={0};
	
	HAL_FLASH_Unlock();
	
	/* Erase RO area */
	FLASH_PageErase(FLASH_BANK_1,RO_START_ADDRESS);
	//TOBECHECKED
	FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
	if(FlashStatus != HAL_OK){
		return pFlash.ErrorCode;
	}
	else{
		/* Operation is completed, disable the PER Bit */
		CLEAR_BIT(FLASH->CR,FLASH_CR_PER);
	}
	
	/* Save number of modules and myID */
	if(myID){
		temp =(uint16_t )(N << 8) + myID;
		//HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,RO_START_ADDRESS,temp);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,RO_START_ADDRESS,temp);
		//TOBECHECKED
		FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
		if(FlashStatus != HAL_OK){
			return pFlash.ErrorCode;
		}
		else{
			/* If the program operation is completed, disable the PG Bit */
			CLEAR_BIT(FLASH->CR,FLASH_CR_PG);
		}
		
		/* Save topology */
		for(uint8_t i =1; i <= N; i++){
			for(uint8_t j =0; j <= MaxNumOfPorts; j++){
				if(array[i - 1][0]){
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,              //HALFWORD
						//TOBECHECKED
					RO_START_ADDRESS + add,array[i - 1][j]);
					add +=2;
					FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
					if(FlashStatus != HAL_OK){
						return pFlash.ErrorCode;
					}
					else{
						/* If the program operation is completed, disable the PG Bit */
						CLEAR_BIT(FLASH->CR,FLASH_CR_PG);
					}
				}
			}
		}
	}
	
	// Save Command Snippets
	int currentAdd = RO_MID_ADDRESS;
	for(uint8_t s =0; s < numOfRecordedSnippets; s++){
		if(snippets[s].cond.conditionType){
			snipBuffer[0] =0xFE;		// A marker to separate Snippets
			memcpy((uint8_t* )&snipBuffer[1],(uint8_t* )&snippets[s],sizeof(snippet_t));
			// Copy the snippet struct buffer (20 x numOfRecordedSnippets). Note this is assuming sizeof(snippet_t) is even.
			for(uint8_t j =0; j < (sizeof(snippet_t) / 2); j++){
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,currentAdd,*(uint16_t* )&snipBuffer[j * 2]);
				//HALFWORD
				//TOBECHECKED
				FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
				if(FlashStatus != HAL_OK){
					return pFlash.ErrorCode;
				}
				else{
					/* If the program operation is completed, disable the PG Bit */
					CLEAR_BIT(FLASH->CR,FLASH_CR_PG);
					currentAdd +=2;
				}
			}
			// Copy the snippet commands buffer. Always an even number. Note the string termination char might be skipped
			for(uint8_t j =0; j < ((strlen(snippets[s].cmd) + 1) / 2); j++){
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,currentAdd,*(uint16_t* )(snippets[s].cmd + j * 2));
				//HALFWORD
				//TOBECHECKED
				FlashStatus =FLASH_WaitForLastOperation((uint32_t ) HAL_FLASH_TIMEOUT_VALUE);
				if(FlashStatus != HAL_OK){
					return pFlash.ErrorCode;
				}
				else{
					/* If the program operation is completed, disable the PG Bit */
					CLEAR_BIT(FLASH->CR,FLASH_CR_PG);
					currentAdd +=2;
				}
			}
		}
	}
	
	HAL_FLASH_Lock();
	
	return result;
}

/* --- Clear array topology in SRAM and Flash RO --- 
 */
uint8_t ClearROtopology(void){
	// Clear the array 
	memset(array,0,sizeof(array));
	N =1;
	myID =0;
	
	return SaveToRO();
}
/*-----------------------------------------------------------*/

/* --- Trigger ST factory bootloader update for a remote module.
 */
void remoteBootloaderUpdate(uint8_t src,uint8_t dst,uint8_t inport,uint8_t outport){

	uint8_t myOutport =0, lastModule =0;
	int8_t *pcOutputString;

	/* 1. Get route to destination module */
	myOutport =FindRoute(myID,dst);
	if(outport && dst == myID){ /* This is a 'via port' update and I'm the last module */
		myOutport =outport;
		lastModule =myID;
	}
	else if(outport == 0){ /* This is a remote update */
		if(NumberOfHops(dst)== 1)
		lastModule = myID;
		else
		lastModule = route[NumberOfHops(dst)-1]; /* previous module = route[Number of hops - 1] */
	}

	/* 2. If this is the source of the message, show status on the CLI */
	if(src == myID){
		/* Obtain the address of the output buffer.  Note there is no mutual
		 exclusion on this buffer as it is assumed only one command console
		 interface will be used at any one time. */
		pcOutputString =FreeRTOS_CLIGetOutputBuffer();

		if(outport == 0)		// This is a remote module update
			sprintf((char* )pcOutputString,pcRemoteBootloaderUpdateMessage,dst);
		else
			// This is a 'via port' remote update
			sprintf((char* )pcOutputString,pcRemoteBootloaderUpdateViaPortMessage,dst,outport);

		strcat((char* )pcOutputString,pcRemoteBootloaderUpdateWarningMessage);
		writePxITMutex(inport,(char* )pcOutputString,strlen((char* )pcOutputString),cmd50ms);
		Delay_ms(100);
	}

	/* 3. Setup my inport and outport for bootloader update */
	SetupPortForRemoteBootloaderUpdate(inport);
	SetupPortForRemoteBootloaderUpdate(myOutport);


	/* 5. Build a DMA stream between my inport and outport */
	StartScastDMAStream(inport,myID,myOutport,myID,BIDIRECTIONAL,0xFFFFFFFF,0xFFFFFFFF,false);
}

/*-----------------------------------------------------------*/

/* --- Setup a port for remote ST factory bootloader update:
 - Set baudrate to 57600
 - Enable even parity
 - Set datasize to 9 bits
 */
void SetupPortForRemoteBootloaderUpdate(uint8_t port){
	UART_HandleTypeDef *huart =GetUart(port);

	huart->Init.BaudRate =57600;
	huart->Init.Parity = UART_PARITY_EVEN;
	huart->Init.WordLength = UART_WORDLENGTH_9B;
	HAL_UART_Init(huart);

	/* The CLI port RXNE interrupt might be disabled so enable here again to be sure */
	__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
}


/* --- H07R8 module initialization.
 */
void Module_Peripheral_Init(void){

//	 MX_GPIO_Init();
	/* Array ports */
//	MX_USART1_UART_Init();
//	MX_USART2_UART_Init();
//	MX_USART3_UART_Init();
//	MX_USART4_UART_Init();
//	MX_USART5_UART_Init();
	MX_USART6_UART_Init();
//	MX_I2C2_Init();
//	MX_I2S1_Init();
	 //Circulating DMA Channels ON All Module
//	for (int i = 1; i <= NumOfPorts; i++) {
//		if (GetUart(i) == &huart1) {
//			index_dma[i - 1] = &(DMA1_Channel1->CNDTR);}
//		else if (GetUart(i) == &huart2) {
//			index_dma[i - 1] = &(DMA1_Channel2->CNDTR);}
//		else if (GetUart(i) == &huart3) {
//			index_dma[i - 1] = &(DMA1_Channel3->CNDTR);}
//		else if (GetUart(i) == &huart4) {
//			index_dma[i - 1] = &(DMA1_Channel4->CNDTR);}
//		else if (GetUart(i) == &huart5) {
//			index_dma[i - 1] = &(DMA1_Channel5->CNDTR);}
//		else if (GetUart(i) == &huart6) {
//			index_dma[i - 1] = &(DMA1_Channel6->CNDTR);}
//	}


}

/*-----------------------------------------------------------*/
/* --- H07R8 message processing task.
 */
Module_Status Module_MessagingTask(uint16_t code,uint8_t port,uint8_t src,uint8_t dst,uint8_t shift){
	Module_Status result =H07R8_OK;


	switch(code){
		case(CODE_H07R8_CODEC_INIT):
				CodecInit(cMessage[port-1][shift],cMessage[port-1][shift+1],cMessage[port-1][shift+2]);
				break;
//		case(CODE_H07R8_CODEC_STREAM_START):
//				CodecStreamDataStart();
//				break;
//		case(CODE_H07R8_CODEC_STREAM_STOP):
//				CodecStreamDataStop();
//				break;
		case(CODE_H07R8_CODEC_DAC_GAIN):
				CodecDAC_Gain(cMessage[port-1][shift]);
				break;
		case(CODE_H07R8_CODEC_AUDIO_LEVEL_CTRL):
				CodecAudioLevel(cMessage[port-1][shift],cMessage[port-1][shift+1],cMessage[port-1][shift+1]);
				break;
		case(CODE_H07R8_CODEC_AUDIO_MUTE):
				CodecAudioMute(AUDIO_MUTE_ENABLE);
				break;
		case(CODE_H07R8_CODEC_AUDIO_UNMUTE):
				CodecAudioMute(AUDIO_MUTE_DISABLE);
				break;
		case(CODE_H07R8_CODEC_ENABLE_SHOUTDOWN):
				CodecShutdown(SHOUTDOWN_ENABLE);
				break;
		case(CODE_H07R8_CODEC_DISABLE_SHOUTDOWN):
				CodecShutdown(SHOUTDOWN_DISABLE);
				break;
		case(CODE_H07R8_AMP_GAIN):
				AmpGain(cMessage[port-1][shift]);
				break;
		case(CODE_H07R8_AMP_MUTE):
				AmpMute(MUTE_ENABLE);
				break;
		case(CODE_H07R8_AMP_UNMUTE):
				AmpMute(MUTE_DISABLE);
				break;
		case(CODE_H07R8_AMP_ENABLE_SHOUTDOWN):
				AmpShutdown(SHUTDOWN_ENABLE);
				break;
		case(CODE_H07R8_AMP_DISABLE_SHOUTDOWN):
				AmpShutdown(SHUTDOWN_DISABLE);
				break;
		default:
			result = H07R8_ERR_UnknownMessage;
			break;
	}
	
	return result;
}
/* --- Get the port for a given UART. 
 */
uint8_t GetPort(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1)
		return P1;
	else if(huart->Instance == USART2)
		return P2;
	else if(huart->Instance == USART3)
		return P3;
	else if(huart->Instance == USART4)
		return P4;
	else if(huart->Instance == USART5)
		return P5;
//	else if(huart->Instance == USART6)
//		return P6;
	return 0;
}

/*-----------------------------------------------------------*/

/* --- Register this module CLI Commands
 */
void RegisterModuleCLICommands(void){

	FreeRTOS_CLIRegisterCommand(&CLI_CodecInitDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_CodecStreamStartDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_CodecStreamStopDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_CodecDAC_GainDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_CodecAudioLevelDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_CodecAudioMuteDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_CodecShutdownDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_AmpGainDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_AmpMuteDefinition);
	FreeRTOS_CLIRegisterCommand(&CLI_AmpShoutdownDefinition);

}

/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
 ||							 	Local  APIs			    		          | 																 	|
 -----------------------------------------------------------------------
 */



/* -----------------------------------------------------------------------
 ||								  APIs							          |
 -----------------------------------------------------------------------
 */

/* Codec initialize */
/*
 * @brief  :Codec initialize for play audio file.
 * @param1 :set DAC gain.
 * @param2 :set the audio level for the right channel.
 * @param3 :set the audio level for the left channel.
 * @retval :Status
 */

Module_Status CodecInit(Codec_DAC_Gain dacGain, Left_Right_AUDIO_GAIN rPlaybackVol, Left_Right_AUDIO_GAIN lPlaybackVol)
{
	Module_Status Status = H07R8_OK;
	if(CODEC_OK != MAX9867_CodecInit(dacGain, rPlaybackVol, lPlaybackVol))
		return H07R8_ERROR;
	return Status;
}

/**********************************************************************************************/

/* Codec Stream Data Start */
/*
 * @brief  :when call this API the digital audio data will be requested from SD Card module or another module has audio data.
 * @retval :Status
 */

Module_Status CodecStreamDataStart(void)
{
	if(HAL_OK != HAL_UART_Receive_IT(&huart6, &rx[0], BUFFER_FULL_SIZE))
		return H07R8_ERROR;
	if(HAL_OK != HAL_UART_Transmit(&huart6, &dataFlag, 1, 2000))
		return H07R8_ERROR;
	dataFlag=1;
}
/**********************************************************************************************/

/* Codec Stream Data Stop */
/*
 * @brief  :when call this API the digital audio data will be stop from SD Card module or another module has audio data.
 * @retval :Status
 */

Module_Status CodecStreamDataStop(void)
{
	oneTime = 1;
	if(HAL_OK != HAL_I2S_DMAStop(&hi2s1))
		return H07R8_ERROR;
	if(HAL_OK != HAL_UART_AbortReceive(&huart6))
		return H07R8_ERROR;
}
/**********************************************************************************************/

/* DAC gain */
/*
 * @brief  :Set DAC gain.
 * @param1 :set DAC gain.
 * @retval :Status
 */

Module_Status CodecDAC_Gain(Codec_DAC_Gain gain)
{
	Module_Status Status = H07R8_OK;
	if( CODEC_OK != MAX9867_DAC_Gain(DAC_GAIN_0dB, gain))
			return H07R8_ERROR;
	return Status;
}

/**********************************************************************************************/

/* Set Audio Level */
/*
 * @brief  :Set Audio Level.
 * @param1 :Choose the channel for set the audio level, either left/right/both of channels.
 * @param2 :set the audio level for the right channel.
 * @param3 :set the audio level for the left channel.
 * @retval :Status
 */

Module_Status CodecAudioLevel(Left_Right_Channel channel, Left_Right_AUDIO_GAIN rPlaybackVol, Left_Right_AUDIO_GAIN lPlaybackVol)
{
	Module_Status Status = H07R8_OK;
	if( CODEC_OK != MAX9867_AudioLevel(channel, rPlaybackVol, lPlaybackVol))
		return H07R8_ERROR;
	return Status;
}

/**********************************************************************************************/

/*  Codec Audio Mute */
/*
 * @brief  :Mute audio on the codec's output.
 * @param1 :Enable/Disable mute.
 * @retval :Status
 */

Module_Status CodecAudioMute(Codec_Audio_Mute audioMute)
{
	Module_Status Status = H07R8_OK;
	if( CODEC_OK != MAX9867_AudioMute(audioMute))
		return H07R8_ERROR;
	return Status;
}

/**********************************************************************************************/

/*  Codec Shutdown */
/*
 * @brief  :Codec Shutdown.
 * @param1 :Enable/Disable shutdown.
 * @retval :Status
 */

Module_Status CodecShutdown(Codec_Shutdown shtdown)
{
	Module_Status Status = H07R8_OK;
	if(CODEC_OK != MAX9867_Shoutdown(shtdown))
		return H07R8_ERROR;
	return Status;
}

/**********************************************************************************************/

/* Amplifier initialize */
/*
 * @brief  :Amplifier initialize.
 * @retval :Status
 */

Module_Status AmpInit(Amplifier_Switching_Modes switchMode, Amplifier_Gain gain)
{
	Module_Status Status = H07R8_OK;

	if(MAX9704_AmpInit(switchMode, gain) != AMP_OK)
		return H07R8_ERROR;

	return Status;
}

/**********************************************************************************************/

/* Amplifier Gain Settings */
/*
 * @brief  :Amplifier Gain Settings.
 * @param1 :Amplifier gain.
 * @retval :Status
 */
Module_Status AmpGain(Amplifier_Gain gain)
{
	Module_Status Status = H07R8_OK;
	if(gain == GAIN_MODE_0db)
	{
		if(AmpMute(MUTE_ENABLE) != H07R8_OK)
			return H07R8_ERROR;
		muteFlag = 1;
	}
	else
	{
		if(MAX9704_AmpGain(gain) != AMP_OK)
			return H07R8_ERROR;
		if(muteFlag == 1)
		{
			if(AmpMute(MUTE_DISABLE) != H07R8_OK)
				return H07R8_ERROR;
			muteFlag = 0;
		}
	}

	return Status;
}

/**********************************************************************************************/

/* Amplifier Mute */
/*
 * @brief  :Amplifier Mute.
 * @param1 :Enable/Disable mute.
 * @retval :Status
 */
Module_Status AmpMute(Amplifier_Mute mute)
{
	Module_Status Status = H07R8_OK;

	if(MAX9704_AmpMute(mute) != AMP_OK)
		return H07R8_ERROR;

	return Status;
}

/**********************************************************************************************/

/* Amplifier Shutdown */
/*
 * @brief  :Amplifier Shutdown.
 * @param1 :Enable/Disable shutdown.
 * @retval :Status
 */
Module_Status AmpShutdown(Amplifier_Shutdown mode)
{
	Module_Status Status = H07R8_OK;

	if(MAX9704_AmpShutdown(mode) != AMP_OK)
		return H07R8_ERROR;

	return Status;
}

/*-----------------------------------------------------------*/
/*  */



/* -----------------------------------------------------------------------
 |								Commands							      |
   -----------------------------------------------------------------------
 */

portBASE_TYPE CLI_CodecInitCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";

	const char *pDacGainStr = NULL;
	const char *pRightPlaybackVolStr = NULL;
	const char *pLeftPlaybackVolStr = NULL;

	uint8_t dacGain = 0;
	uint8_t rightPlaybackVol = 0;
	uint8_t leftPlaybackVol = 0;

	portBASE_TYPE dacGainStrLen = 0;
	portBASE_TYPE pRightPlaybackVolStrLen = 0;
	portBASE_TYPE pLeftPlaybackVolStrLen = 0;

		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		pDacGainStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 1, &dacGainStrLen);
		pRightPlaybackVolStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &pRightPlaybackVolStrLen);
		pLeftPlaybackVolStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 3, &pLeftPlaybackVolStrLen);

		dacGain = atoi(pDacGainStr);
		rightPlaybackVol = atoi(pRightPlaybackVolStr);
		leftPlaybackVol = atoi(pLeftPlaybackVolStr);

		status =  CodecInit(dacGain, rightPlaybackVol, leftPlaybackVol);

	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

portBASE_TYPE CLI_CodecStreamStartCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";


		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		status =  CodecStreamDataStart();

	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

portBASE_TYPE CLI_CodecStreamStopCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";


		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		status =  CodecStreamDataStop();

	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

portBASE_TYPE CLI_CodecDAC_GainCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";

	uint8_t dacGain = 0;
	portBASE_TYPE dacGainStrLen = 0;
	const char *pDacGainStr = NULL;

		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		pDacGainStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 1, &dacGainStrLen);

		dacGain = atoi(pDacGainStr);

		status = CodecDAC_Gain(dacGain);

	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

portBASE_TYPE CLI_CodecAudioLevelCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
	const char *pchannelStr = NULL;
	const char *pLeftSoundLvlStr = NULL;
	const char *pRightSoundLvlStr = NULL;
	uint8_t channel, leftSoundLvl = 0, rightSoundLvl = 0;
	portBASE_TYPE channelStrLen = 0;
	portBASE_TYPE leftSoundLvlStrLen = 0;
	portBASE_TYPE rightSoundLvlStrLen = 0;

		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		pchannelStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &channelStrLen);
		pLeftSoundLvlStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &leftSoundLvlStrLen);
		pRightSoundLvlStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 3, &rightSoundLvlStrLen);

		channel = atoi(pchannelStr);
		leftSoundLvl = atoi(pLeftSoundLvlStr);
		rightSoundLvl = atoi(pRightSoundLvlStr);

		status = CodecAudioLevel(channel, leftSoundLvl, rightSoundLvl);
	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

portBASE_TYPE CLI_CodecAudioMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
	const char *pMuteStr = NULL;
	uint8_t mute = 0;
	portBASE_TYPE muteStrLen = 0;

		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		pMuteStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 1, &muteStrLen);

		mute = atoi(pMuteStr);

		status = CodecAudioMute(mute);

	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

portBASE_TYPE CLI_CodecShutdownCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
	const char *pShutdownStr = NULL;
	uint8_t shutdown = 0;
	portBASE_TYPE shutdownStrLen = 0;

		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		pShutdownStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &shutdownStrLen);

		shutdown = atoi(pShutdownStr);
		status = CodecShutdown(shutdown);
	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

portBASE_TYPE CLI_AmpGainCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
	const char *pGainStr = NULL;
	uint8_t gain = 0;
	portBASE_TYPE gainStrLen = 0;

		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		pGainStr = (const char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &gainStrLen);

		gain = atoi(pGainStr);
		status = AmpGain(gain);

	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

portBASE_TYPE CLI_AmpMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
	const char *pMuteStr = NULL;
	uint8_t mute = 0;
	portBASE_TYPE muteStrLen = 0;

		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		pMuteStr = (const char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &muteStrLen);

		mute = atoi(pMuteStr);
		if(mute == 0)
			status = AmpMute(MUTE_DISABLE);
		else if(mute == 1)
			status = AmpMute(MUTE_ENABLE);

	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}


portBASE_TYPE CLI_AmpShutdownCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
	Module_Status status = H07R8_OK;
	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
	const char *pShutdownStr = NULL;
	uint8_t shutdown = 0;
	portBASE_TYPE shutdownStrLen = 0;

		(void )xWriteBufferLen;
		configASSERT(pcWriteBuffer);

		pShutdownStr = (const char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &shutdownStrLen);

		shutdown = atoi(pShutdownStr);
		if(shutdown == 0)
			status = AmpShutdown(SHUTDOWN_DISABLE);
		else if(shutdown == 1)
			status = AmpShutdown(SHUTDOWN_ENABLE);

	 if(status == H07R8_OK)
	 {
			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);

	 }

	 else if(status == H07R8_ERROR)
			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);


	return pdFALSE;

}

/*-----------------------------------------------------------*/

/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
