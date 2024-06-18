/*
 BitzOS (BOS) V0.3.1 - Copyright (C) 2017-2024 Hexabitz
 All rights reserved
 
 File Name     : H07R8.h
 Description   : Header file for module H07R8.
 	 	 	 	 (Description_of_module)

(Description of Special module peripheral configuration):
>>
>>
>>

 */



/* H07R8 Module_Status Type Definition */
typedef enum {
	H07R8_OK =0,
	H07R8_ERR_UnknownMessage,
	H07R8_ERR_WrongParams,
	H07R8_ERROR =255
} Module_Status;

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef H07R8_H
#define H07R8_H

/* Includes ------------------------------------------------------------------*/

#include "H07R8_MemoryMap.h"
#include "H07R8_uart.h"
#include "H07R8_gpio.h"
#include "H07R8_dma.h"
#include "H07R8_adc.h"
#include "H07R8_timers.h"
#include "H07R8_inputs.h"
#include "H07R8_eeprom.h"
#include "H07R8_i2c.h"
#include "H07R8_i2s.h"
#include "MAX9704_Amplifier.h"
#include "MAX9867_Codec.h"

/* Macros ----------------------------------------------------------------------*/
#define BUFFER_FULL_SIZE	4096
#define BUFFER_HALF_SIZE	2048
/* Extern ----------------------------------------------------------------------*/
extern uint8_t rx[BUFFER_FULL_SIZE];
extern uint8_t dataFlag;
/* Enums ----------------------------------------------------------------------*/
typedef enum
{
	CODEC_DAC_GAIN_0dB,
	DAC_GAIN_MINUS_1dB,
	DAC_GAIN_MINUS_2dB,
	DAC_GAIN_MINUS_3dB,
	DAC_GAIN_MINUS_4dB,
	DAC_GAIN_MINUS_5dB,
	DAC_GAIN_MINUS_6dB,
	DAC_GAIN_MINUS_7dB,
	DAC_GAIN_MINUS_8dB,
	DAC_GAIN_MINUS_9dB,
	DAC_GAIN_MINUS_10dB,
	DAC_GAIN_MINUS_11dB,
	DAC_GAIN_MINUS_12dB,
	DAC_GAIN_MINUS_13dB,
	DAC_GAIN_MINUS_14dB,
	DAC_GAIN_MINUS_15dB
} Codec_DAC_Gain;

typedef enum
{
	AUDIO_GAIN_PLUS_6dB,
	AUDIO_GAIN_PLUS_5d5B,
	AUDIO_GAIN_PLUS_5dB,
	AUDIO_GAIN_PLUS_4d5B,
	AUDIO_GAIN_PLUS_4dB,
	AUDIO_GAIN_PLUS_3d5B,
	AUDIO_GAIN_PLUS_3dB,
	AUDIO_GAIN_PLUS_2dB,
	AUDIO_GAIN_PLUS_1dB,
	AUDIO_GAIN_0dB,
	AUDIO_GAIN_MINUS_1dB,
	AUDIO_GAIN_MINUS_2dB,
	AUDIO_GAIN_MINUS_3dB,
	AUDIO_GAIN_MINUS_4dB,
	AUDIO_GAIN_MINUS_5dB,
	AUDIO_GAIN_MINUS_6dB,
	AUDIO_GAIN_MINUS_8dB,
	AUDIO_GAIN_MINUS_10dB,
	AUDIO_GAIN_MINUS_12dB,
	AUDIO_GAIN_MINUS_14dB,
	AUDIO_GAIN_MINUS_16dB,
	AUDIO_GAIN_MINUS_18dB,
	AUDIO_GAIN_MINUS_20dB,
	AUDIO_GAIN_MINUS_22dB,
	AUDIO_GAIN_MINUS_24dB,
	AUDIO_GAIN_MINUS_26dB,
	AUDIO_GAIN_MINUS_30dB,
	AUDIO_GAIN_MINUS_34dB,
	AUDIO_GAIN_MINUS_38dB,
} Left_Right_AUDIO_GAIN;

typedef enum
{
	LEFT_CHA,
	RIGHT_CHA,
	LEFT_RIGHT_CHA
} Left_Right_Channel;

typedef enum
{
	CODEC_SHUTDOWN_ENABLE,
	CODEC_SHUTDOWN_DISABLE
} Codec_Shutdown;

typedef enum
{
	CODDEC_AUDIO_MUTE_DISABLE,
	CODEC_AUDIO_MUTE_ENABLE
} Codec_Audio_Mute;

typedef enum{
	AMP_GAIN_MODE_0db = 0,
	AMP_GAIN_MODE_13dB,
	AMP_GAIN_MODE_16dB,
	AMP_GAIN_MODE_19dB,
	AMP_GAIN_MODE_29dB
}Amplifier_Gain;

typedef enum{
	AMP_MUTE_ENABLE = 0,
	AMP_MUTE_DISABLE
}Amplifier_Mute;

typedef enum{
	AMP_SHUTDOWN_ENABLE = 0,
	AMP_SHUTDOWN_DISABLE
}Amplifier_Shutdown;
/* Exported definitions -------------------------------------------------------*/

#define	modulePN		_H07R8


/* Port-related definitions */
#define	NumOfPorts			5

#define P_PROG 				P2						/* ST factory bootloader UART */

/* Define available ports */
#define _P1 
#define _P2 
#define _P3 
#define _P4 
#define _P5 

/* Define available USARTs */
#define _Usart1 1
#define _Usart2 1
#define _Usart3 1
#define _Usart5 1
#define _Usart6	1


/* Port-UART mapping */
#define P1uart &huart6
#define P2uart &huart2
#define P3uart &huart3
#define P4uart &huart1
#define P5uart &huart5



/* Port Definitions */
#define	USART1_TX_PIN		GPIO_PIN_9
#define	USART1_RX_PIN		GPIO_PIN_10
#define	USART1_TX_PORT		GPIOA
#define	USART1_RX_PORT		GPIOA
#define	USART1_AF			GPIO_AF1_USART1

#define	USART2_TX_PIN		GPIO_PIN_2
#define	USART2_RX_PIN		GPIO_PIN_3
#define	USART2_TX_PORT		GPIOA
#define	USART2_RX_PORT		GPIOA
#define	USART2_AF			GPIO_AF1_USART2

#define	USART3_TX_PIN		GPIO_PIN_10
#define	USART3_RX_PIN		GPIO_PIN_11
#define	USART3_TX_PORT		GPIOB
#define	USART3_RX_PORT		GPIOB
#define	USART3_AF			GPIO_AF4_USART3

#define	USART5_TX_PIN		GPIO_PIN_3
#define	USART5_RX_PIN		GPIO_PIN_2
#define	USART5_TX_PORT		GPIOD
#define	USART5_RX_PORT		GPIOD
#define	USART5_AF			GPIO_AF3_USART5

#define	USART6_TX_PIN		GPIO_PIN_8
#define	USART6_RX_PIN		GPIO_PIN_9
#define	USART6_TX_PORT		GPIOB
#define	USART6_RX_PORT		GPIOB
#define	USART6_AF			GPIO_AF8_USART6


/* Module-specific Definitions */

/* Indicator LED */
#define _IND_LED_PORT			GPIOA
#define _IND_LED_PIN			GPIO_PIN_12

/* Module GPIO Pinout */
#define CODEC_IRQN_OUT_Pin GPIO_PIN_1
#define CODEC_IRQN_OUT_GPIO_Port GPIOB
#define AMP_MUTE_Pin GPIO_PIN_1
#define AMP_MUTE_GPIO_Port GPIOD
#define AMP_FS2_Pin GPIO_PIN_3
#define AMP_FS2_GPIO_Port GPIOB
#define AMP_FS1_Pin GPIO_PIN_4
#define AMP_FS1_GPIO_Port GPIOB
#define AMP_GAIN2_Pin GPIO_PIN_5
#define AMP_GAIN2_GPIO_Port GPIOB
#define AMP_GAIN1_Pin GPIO_PIN_6
#define AMP_GAIN1_GPIO_Port GPIOB
#define AMP_SHUTDOWN_Pin GPIO_PIN_7
#define AMP_SHUTDOWN_GPIO_Port GPIOB

/* Module I2C Pinout */
#define I2C_SDA_Pin          GPIO_PIN_14
#define I2C_SDA_GPIO_Port    GPIOB
#define I2C_SCL_Pin          GPIO_PIN_13
#define I2C_SCL_GPIO_Port    GPIOB
#define I2C_GPIO_Port    	 GPIOB

/* Module I2S Pinout */
#define I2S_WS_Pin          GPIO_PIN_4
#define I2S_WS_GPIO_Port    GPIOA
#define I2S_CK_Pin          GPIO_PIN_5
#define I2S_CK_GPIO_Port    GPIOA
#define I2S_MCK_Pin         GPIO_PIN_6
#define I2S_MCK_GPIO_Port   GPIOA
#define I2S_SD_Pin          GPIO_PIN_7
#define I2S_SD_GPIO_Port    GPIOA
#define I2S_GPIO_Port    	GPIOA

#define NUM_MODULE_PARAMS		1

/* Audio Module GPIO Pinout */


/* H07R8 Module Special Timer */


/* H07R8 Module Special ADC */


/* H07R8 Module special parameters */


/* Module EEPROM Variables */
// Module Addressing Space 500 - 599
#define _EE_MODULE							500		

///* H07R8 Module_Status Type Definition */
//typedef enum {
//	H07R8_OK =0,
//	H07R8_ERR_UnknownMessage,
//	H07R8_ERR_WrongParams,
//	H07R8_ERROR =255
//} Module_Status;



#define Module_Status1 Module_Status
/* Export Module typedef structure */


/* Export UART variables */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;

/* Define UART Init prototypes */
extern void MX_USART1_UART_Init(void);
extern void MX_USART2_UART_Init(void);
extern void MX_USART3_UART_Init(void);
extern void MX_USART5_UART_Init(void);
extern void MX_USART6_UART_Init(void);
extern void SystemClock_Config(void);
extern void ExecuteMonitor(void);

/* -----------------------------------------------------------------------
||								  APIs							          ||
 -----------------------------------------------------------------------
 */

void SetupPortForRemoteBootloaderUpdate(uint8_t port);
void remoteBootloaderUpdate(uint8_t src,uint8_t dst,uint8_t inport,uint8_t outport);
Module_Status CodecInit(Codec_DAC_Gain dacGain,Left_Right_AUDIO_GAIN rPlaybackVol, Left_Right_AUDIO_GAIN lPlaybackVol);
Module_Status CodecStreamDataStart(void);
Module_Status CodecDAC_Gain(Codec_DAC_Gain gain);
Module_Status CodecAudioLevel(Left_Right_Channel channel, Left_Right_AUDIO_GAIN rPlaybackVol, Left_Right_AUDIO_GAIN lPlaybackVol);
Module_Status CodecAudioMute(Codec_Audio_Mute audioMute);
Module_Status CodecShutdown(Codec_Shutdown shtdown);
Module_Status AmpGain(Amplifier_Gain gain);
Module_Status AmpMute(Amplifier_Mute mute);
Module_Status AmpShutdown(Amplifier_Shutdown mode);

/* -----------------------------------------------------------------------
 ||								Commands							      ||
 -----------------------------------------------------------------------
 */


#endif /* H07R8_H */

/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
