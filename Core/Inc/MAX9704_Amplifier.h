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

#ifndef INC_MAX9704_AMPLIFIER_H_
#define INC_MAX9704_AMPLIFIER_H_

#include "main.h"
#include "Porting.h"


typedef enum{
	SWITCHING_MODE_670KHZ = 0,
	SWITCHING_MODE_940KHZ,
	SWITCHING_MODE_470KHZ,
	SWITCHING_MODE_AROUND_670KHZ
}Switching_Modes;

typedef enum{
	GAIN_MODE_29dB = 0,
	GAIN_MODE_19dB,
	GAIN_MODE_16dB,
	GAIN_MODE_13dB
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
