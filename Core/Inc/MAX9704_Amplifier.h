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

/******************************************************************INCLUDE**********************************************************/

#include "Porting.h"

/******************************************************************ENUMS**********************************************************/
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

/******************************************************************PRIVATE APIs**********************************************************/

Status_TypeDef MAX9704_AmpSwitchingMode(Switching_Modes mode);
Status_TypeDef MAX9704_AmpGain(Amp_Gain mode);
Status_TypeDef MAX9704_AmpShutdown(Shutdown_Modes mode);
Status_TypeDef MAX9704_AmpMute(Mute_En_Dis mute);

/******************************************************************USER APIs**********************************************************/

Status_TypeDef MAX9704_AmpInit(Switching_Modes switchMode, Amp_Gain gain);
#endif /* INC_MAX9704_AMPLIFIER_H_ */

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
