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

/* GPIOs type define */
typedef GPIO_TypeDef GPIO_HANDLE;

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
}Gain_Modes;

typedef enum{
	SHUTDOWN_ENABLE = 0,
	SHUTDOWN_DISABLE
}Shutdown_Modes;

typedef enum{
	MUTE_ENABLE = 0,
	MUTE_DISABLE
}Mute_Modes;

/* Amplifier Switching Mode */
/*
 * @brief  :Audio switching frequency control.
 * @param1 :Switching Modes
 * @retval :Nothing
 */
Status_TypeDef MAX9704_AmpSwitchingMode(Switching_Modes mode);

/* Amplifier Gain */
/*
 * @brief  :Audio level control.
 * @param1 :Gain Modes
 * @retval :Nothing
 */
Status_TypeDef MAX9704_AmpGain(Gain_Modes mode);

/* Amplifier Shutdown */
/*
 * @brief  :Amplifier Shutdown.
 * @param1 :Shutdown Modes
 * @retval :Nothing
 */
Status_TypeDef MAX9704_AmpShutdown(Shutdown_Modes mode);

/* Amplifier Mute */
/*
 * @brief  :Aduio Mute.
 * @param1 :Mute Modes
 * @retval :Nothing
 */
Status_TypeDef MAX9704_AmpMute(Mute_Modes mode);

#endif /* INC_MAX9704_AMPLIFIER_H_ */
