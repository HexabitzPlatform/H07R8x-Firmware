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
#include "H07R8.h"
#include "H07R8_i2c.h"
#include "H07R8_i2s.h"

Module_Status AmpGain(Amp_Gain gain);
Module_Status AmpMute(Mute_En_Dis mute);
Module_Status AmpShutdown(Shutdown_Modes mode);
/* Define UART variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
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

/* Private function prototypes -----------------------------------------------*/
void ExecuteMonitor(void);
static Module_Status AmpInit(void);

/* Create CLI commands --------------------------------------------------------*/

//portBASE_TYPE CLI_CodecInitCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE CLI_CodecStreamingDigitalAudioCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE CLI_CodecSoundLevelCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE CLI_CodecSoundMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE CLI_CodecSoundUnMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE CLI_CodecShutdownCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_AmpGainCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_AmpMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE CLI_AmpShutdownCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );

//const CLI_Command_Definition_t CLI_CodecInitDefinition =
//{
//	( const int8_t * ) "codecinit", /* The command string to type. */
//	( const int8_t * ) "streamAudio:\r\n stream audio from processor to MAX9867 codec ic by i2s \r\n\r\n",
//	CLI_CodecInitCommand, /* The function to run. */
//	0 /* zero parameters are expected. */
//};
//
//const CLI_Command_Definition_t CLI_CodecStreamingDigitalAudioDefinition =
//{
//	( const int8_t * ) "soundlevel", /* The command string to type. */
//	( const int8_t * ) "soundlevel:\r\n stream audio from processor to MAX9867 codec ic by i2s \r\n\r\n",
//	CLI_CodecStreamingDigitalAudioCommand, /* The function to run. */
//	0 /* zero parameters are expected. */
//};
//
//const CLI_Command_Definition_t CLI_CodecSoundLevelDefinition =
//{
//	( const int8_t * ) "soundmute", /* The command string to type. */
//	( const int8_t * ) "soundmute:\r\n stream audio from processor to MAX9867 codec ic by i2s \r\n\r\n",
//	CLI_CodecSoundLevelCommand, /* The function to run. */
//	0 /* zero parameters are expected. */
//};
//
//const CLI_Command_Definition_t CLI_CodecSoundMuteDefinition =
//{
//	( const int8_t * ) "soundunmute", /* The command string to type. */
//	( const int8_t * ) "soundunmute:\r\n stream audio from processor to MAX9867 codec ic by i2s \r\n\r\n",
//	CLI_CodecSoundMuteCommand, /* The function to run. */
//	0 /* zero parameters are expected. */
//};
//
//const CLI_Command_Definition_t CLI_CodecSoundUnMuteDefinition =
//{
//	( const int8_t * ) "codecshut", /* The command string to type. */
//	( const int8_t * ) "codecshut:\r\n stream audio from processor to MAX9867 codec ic by i2s \r\n\r\n",
//	CLI_CodecSoundUnMuteCommand, /* The function to run. */
//	0 /* zero parameters are expected. */
//};
//
//const CLI_Command_Definition_t CLI_CodecShoutdownDefinition =
//{
//	( const int8_t * ) "codecshut", /* The command string to type. */
//	( const int8_t * ) "codecshut:\r\n stream audio from processor to MAX9867 codec ic by i2s \r\n\r\n",
//	CLI_CodecShutdownCommand, /* The function to run. */
//	0 /* zero parameters are expected. */
//};

const CLI_Command_Definition_t CLI_AmpGainDefinition =
{
	( const int8_t * ) "ampgain", /* The command string to type. */
	( const int8_t * ) "ampgain:\r\n setting amplifier gain there is 5 levels start from 0 \r\n\r\n",
	CLI_AmpGainCommand, /* The function to run. */
	1 /* zero parameters are expected. */
};

const CLI_Command_Definition_t CLI_AmpMuteDefinition =
{
	( const int8_t * ) "ampmute", /* The command string to type. */
	( const int8_t * ) "ampmute:\r\n Amplifier mute where : 0 for disable and 1 for enable \r\n\r\n",
	CLI_AmpMuteCommand, /* The function to run. */
	1 /* zero parameters are expected. */
};

const CLI_Command_Definition_t CLI_AmpShoutdownDefinition =
{
	( const int8_t * ) "ampshut", /* The command string to type. */
	( const int8_t * ) "ampshut:\r\n Amplifier shutdown where : 0 for disable and 1 for enable \r\n\r\n",
	CLI_AmpShutdownCommand, /* The function to run. */
	1 /* zero parameters are expected. */
};
/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
 |						    	 Private Functions						 |
 -------------------------------------------------------------------------
 */

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow : 
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 48000000
 *            HCLK(Hz)                       = 48000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 1
 *            HSE Frequency(Hz)              = 8000000
 *            PREDIV                         = 1
 *            PLLMUL                         = 6
 *            Flash Latency(WS)              = 1
 * @param  None
 * @retval None
 */
void SystemClock_Config(void){
	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	  /** Configure the main internal regulator output voltage
	  */
	  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	  RCC_OscInitStruct.PLL.PLLN = 12;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	  HAL_RCC_OscConfig(&RCC_OscInitStruct);

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

	  /** Initializes the peripherals clocks
	  */
	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART2;
	  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);


	  HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
	
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

	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();

	/* Array ports */
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USART5_UART_Init();
	MX_USART6_UART_Init();
	MX_I2C2_Init();
	MX_I2S1_Init();
	AmpInit();
	 //Circulating DMA Channels ON All Module
	for (int i = 1; i <= NumOfPorts; i++) {
		if (GetUart(i) == &huart1) {
			index_dma[i - 1] = &(DMA1_Channel1->CNDTR);
		} else if (GetUart(i) == &huart2) {
			index_dma[i - 1] = &(DMA1_Channel2->CNDTR);
		} else if (GetUart(i) == &huart3) {
			index_dma[i - 1] = &(DMA1_Channel3->CNDTR);
		} else if (GetUart(i) == &huart5) {
			index_dma[i - 1] = &(DMA1_Channel4->CNDTR);
		} else if (GetUart(i) == &huart6) {
			index_dma[i - 1] = &(DMA1_Channel5->CNDTR);
		}
	}

}

/*-----------------------------------------------------------*/
/* --- H07R8 message processing task.
 */
Module_Status Module_MessagingTask(uint16_t code,uint8_t port,uint8_t src,uint8_t dst,uint8_t shift){
	Module_Status result =H07R8_OK;


	switch(code){
//		case(CODE_H07R8_CODEC_INIT):
//				MAX9867_Init();
//				break;
//		case(CODE_H07R8_CODEC_STREAM_AUDIO):
//				MAX9867_StreamingDigitalAudio();
//				break;
//		case(CODE_H07R8_CODEC_SOUND_LEVEL_CTRL):
//				MAX9867_SoundLevel();
//				break;
//		case(CODE_H07R8_CODEC_SOUND_MUTE):
//		case(CODE_H07R8_CODEC_SOUND_UNMUTE):
//				MAX9867_SoundMute();
//				break;
//		case(CODE_H07R8_CODEC_SHOUTDOWN):
//				MAX9867_Shutdown();
//				break;
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
			result =H07R8_ERR_UnknownMessage;
			break;
	}
	
	return result;
}
/* --- Get the port for a given UART. 
 */
uint8_t GetPort(UART_HandleTypeDef *huart){

	if(huart->Instance == USART6)
		return P1;
	else if(huart->Instance == USART2)
		return P2;
	else if(huart->Instance == USART3)
		return P3;
	else if(huart->Instance == USART1)
		return P4;
	else if(huart->Instance == USART5)
		return P5;
	
	return 0;
}

/*-----------------------------------------------------------*/

/* --- Register this module CLI Commands
 */
void RegisterModuleCLICommands(void){

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
/*       */
//Module_Status CodecInit()
//{
//	Module_Status Status = H07R8_OK;
//	return Status;
//}
//
//Module_Status CodecStreamingDigitalAudio(uint16_t *data,size_t size)
//{
//	Module_Status Status = H07R8_OK;
//    for(int i=0; i<size; i++) {
//    	if( H07R8_OK != WriteI2S(I2S_PORT, &data[i], 1))
//    		return H07R8_ERROR;
//    }
//	return Status;
//}
//
//Module_Status CodecSoundLevel()
//{
//	Module_Status Status = H07R8_OK;
//	return Status;
//}
//
//Module_Status CodecSoundMute()
//{
//	Module_Status Status = H07R8_OK;
//	return Status;
//}
//
//Module_Status CodecShutdown()
//{
//	Module_Status Status = H07R8_OK;
//	return Status;
//}

static Module_Status AmpInit(void)
{
	Module_Status Status = H07R8_OK;

	if(MAX9704_AmpInit(SWITCHING_MODE_670KHZ, GAIN_MODE_13dB) != STATUS_OK)
		return H07R8_ERROR;

	return Status;
}

Module_Status AmpGain(Amp_Gain gain)
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
		if(MAX9704_AmpGain(gain) != STATUS_OK)
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

Module_Status AmpMute(Mute_En_Dis mute)
{
	Module_Status Status = H07R8_OK;

	if(MAX9704_AmpMute(mute) != STATUS_OK)
		return H07R8_ERROR;

	return Status;
}

Module_Status AmpShutdown(Shutdown_Modes mode)
{
	Module_Status Status = H07R8_OK;

	if(MAX9704_AmpShutdown(mode) != STATUS_OK)
		return H07R8_ERROR;

	return Status;
}


/*-----------------------------------------------------------*/
/*  */



/* -----------------------------------------------------------------------
 |								Commands							      |
   -----------------------------------------------------------------------
 */
//portBASE_TYPE CLI_CodecInitCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
//	Module_Status status = H07R8_OK;
//	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
//	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
//
//	const char *pDacGainStr = NULL;
//	uint8_t dacGain = 0;
//	portBASE_TYPE dacGainStrLen = 0;
//
//		(void )xWriteBufferLen;
//		configASSERT(pcWriteBuffer);
//
//		pDacGainStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &dacGainStrLen);
//
//		dacGain = atoi(pDacGainStr);
//		status = MAX9867_Init(/*dacGain*/);
//
//	 if(status == H07R8_OK)
//	 {
//			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);
//
//	 }
//
//	 else if(status == H07R8_ERROR)
//			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);
//
//
//	return pdFALSE;
//
//}
//
//
//portBASE_TYPE CLI_CodecStreamingDigitalAudioCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
//	Module_Status status = H07R8_OK;
//	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
//	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
//
//		(void )xWriteBufferLen;
//		configASSERT(pcWriteBuffer);
//
////		status = MAX9867_StreamingDigitalAudio();
//	 if(status == H07R8_OK)
//	 {
//			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);
//
//	 }
//
//	 else if(status == H07R8_ERROR)
//			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);
//
//
//	return pdFALSE;
//
//}
//
//portBASE_TYPE CLI_CodecSoundLevelCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
//	Module_Status status = H07R8_OK;
//	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
//	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
//	const char *pLeftSoundLvlStr = NULL;
//	const char *pRightSoundLvlStr = NULL;
//	uint8_t leftSoundLvl = 0,rightSoundLvl = 0;
//	portBASE_TYPE leftSoundLvlStrLen = 0;
//	portBASE_TYPE rightSoundLvlStrLen = 0;
//
//		(void )xWriteBufferLen;
//		configASSERT(pcWriteBuffer);
//
//		pLeftSoundLvlStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &leftSoundLvlStrLen);
//		pRightSoundLvlStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 3, &rightSoundLvlStrLen);
//
//		leftSoundLvl = atoi(pLeftSoundLvlStr);
//		rightSoundLvl = atoi(pRightSoundLvlStr);
//
//		MAX9867_SoundLevel();
//	 if(status == H07R8_OK)
//	 {
//			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);
//
//	 }
//
//	 else if(status == H07R8_ERROR)
//			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);
//
//
//	return pdFALSE;
//
//}
//
//portBASE_TYPE CLI_CodecSoundMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
//	Module_Status status = H07R8_OK;
//	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
//	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
//	const char *pMuteStr = NULL;
//	uint8_t mute = 0;
//	portBASE_TYPE muteStrLen = 0;
//
//		(void )xWriteBufferLen;
//		configASSERT(pcWriteBuffer);
//
//		pMuteStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &muteStrLen);
//
//		mute = atoi(pMuteStr);
//
//		status = MAX9867_SoundMute();
//
//	 if(status == H07R8_OK)
//	 {
//			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);
//
//	 }
//
//	 else if(status == H07R8_ERROR)
//			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);
//
//
//	return pdFALSE;
//
//}
//
//portBASE_TYPE CLI_CodecSoundUnMuteCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
//	Module_Status status = H07R8_OK;
//	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
//	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
//	const char *pMuteStr = NULL;
//	uint8_t mute = 0;
//	portBASE_TYPE muteStrLen = 0;
//
//		(void )xWriteBufferLen;
//		configASSERT(pcWriteBuffer);
//
//		pMuteStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &muteStrLen);
//
//		mute = atoi(pMuteStr);
//		status = MAX9867_SoundMute();
//
//	 if(status == H07R8_OK)
//	 {
//			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);
//
//	 }
//
//	 else if(status == H07R8_ERROR)
//			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);
//
//
//	return pdFALSE;
//
//}
//
//portBASE_TYPE CLI_CodecShutdownCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString ){
//	Module_Status status = H07R8_OK;
//	static const int8_t *pcOKMessage=(int8_t* )"Ok\n\r";
//	static const int8_t *pcErrorsMessage =(int8_t* )"Error!\n\r";
//	const char *pShutdownStr = NULL;
//	uint8_t shutdown = 0;
//	portBASE_TYPE shutdownStrLen = 0;
//
//		(void )xWriteBufferLen;
//		configASSERT(pcWriteBuffer);
//
//		pShutdownStr = (const char *)FreeRTOS_CLIGetParameter(pcWriteBuffer, 2, &shutdownStrLen);
//
//		shutdown = atoi(pShutdownStr);
//		status = MAX9867_Shutdown();
//	 if(status == H07R8_OK)
//	 {
//			 sprintf((char* )pcWriteBuffer,(char* )pcOKMessage);
//
//	 }
//
//	 else if(status == H07R8_ERROR)
//			strcpy((char* )pcWriteBuffer,(char* )pcErrorsMessage);
//
//
//	return pdFALSE;
//
//}

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
