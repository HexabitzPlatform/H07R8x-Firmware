/*
 * MAX9704_Amplifier.h
 * Description: MAX9704_Amplifier header file
 *  Created on: Mar 11, 2024
 *      Author: Abdulrahman Battikh @ Hexabitz
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Hexabitz.
 * All rights reserved.
 *
 ******************************************************************************
 */

#ifndef MAX9704_AMPLIFIER_H_
#define MAX9704_AMPLIFIER_H_


#include "H07R8_gpio.h"

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

typedef enum{
	STATUS_OK = 0,
	STATUS_INV,
	STATUS_TMOUT,
	STATUS_BUSY,
	STC3117_WRONG_ADDRESS,
	STATUS_ERR=255
}Status_TypeDef;

typedef enum{
	SWITCHING_MODE_670KHZ = 0,
	SWITCHING_MODE_940KHZ,
	SWITCHING_MODE_470KHZ,
	SWITCHING_MODE_AROUND_670KHZ
}Switching_Modes;

typedef enum{
	GAIN_MODE_0db = 0,
	GAIN_MODE_13dB,
	GAIN_MODE_16dB,
	GAIN_MODE_19dB,
	GAIN_MODE_29dB
}Amp_Gain;

typedef enum{
	SHUTDOWN_ENABLE = 0,
	SHUTDOWN_DISABLE
}Shutdown_Modes;

typedef enum{
	MUTE_ENABLE = 0,
	MUTE_DISABLE
}Mute_En_Dis;

/* Amplifier Switching Mode */
/*
 * @brief  :Audio switching frequency control.
 * @param1 :Switching Modes
 * @retval :Status
 */
Status_TypeDef MAX9704_AmpSwitchingMode(Switching_Modes mode);

/* Amplifier Gain */
/*
 * @brief  :Audio level control.
 * @param1 :Gain Modes
 * @retval :Status
 */
Status_TypeDef MAX9704_AmpGain(Amp_Gain mode);

/* Amplifier Shutdown */
/*
 * @brief  :Amplifier Shutdown.
 * @param1 :Shutdown Modes
 * @retval :Status
 */
Status_TypeDef MAX9704_AmpShutdown(Shutdown_Modes mode);

/* Amplifier Mute */
/*
 * @brief  :Aduio Mute.
 * @param1 :Mute Modes
 * @retval :Status
 */
Status_TypeDef MAX9704_AmpMute(Mute_En_Dis mute);

/******************************************************************USER APIs**********************************************************/

/* Amplifier Initialize */
/*
 * @brief  :Amplifier Initialize.
 * @param1 :switching mode.
 * @param2 :amplifier gain.
 * @retval :Status
 */
Status_TypeDef MAX9704_AmpInit(Switching_Modes switchMode, Amp_Gain gain);
#endif /* INC_MAX9704_AMPLIFIER_H_ */

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
