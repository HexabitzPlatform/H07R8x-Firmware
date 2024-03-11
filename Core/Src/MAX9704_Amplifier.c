/*
 * MAX9704_Amplifier.c
 * Description: MAX9704_Amplifier source file
 *  Created on: Mar 11, 2024
 *      Author: Abdulrahman Battikh @ Hexabitz
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Hexabitz.
 * All rights reserved.
 *
 ******************************************************************************
 */
#include "MAX9704_Amplifier.h"

void AmpSwitchingMode(Switching_Modes mode)
{
	switch(mode)
	{
		case SWITCHING_MODE_670KHZ:
			HAL_GPIO_WritePin(AMP_FS1_GPIO_Port, AMP_FS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AMP_FS2_GPIO_Port, AMP_FS2_Pin, GPIO_PIN_RESET);
			break;
		case SWITCHING_MODE_940KHZ:
			HAL_GPIO_WritePin(AMP_FS1_GPIO_Port, AMP_FS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AMP_FS2_GPIO_Port, AMP_FS2_Pin, GPIO_PIN_SET);
			break;
		case SWITCHING_MODE_470KHZ:
			HAL_GPIO_WritePin(AMP_FS1_GPIO_Port, AMP_FS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AMP_FS2_GPIO_Port, AMP_FS2_Pin, GPIO_PIN_RESET);
			break;
		case SWITCHING_MODE_AROUND_670KHZ:
			HAL_GPIO_WritePin(AMP_FS1_GPIO_Port, AMP_FS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AMP_FS2_GPIO_Port, AMP_FS2_Pin, GPIO_PIN_SET);
			break;
		/* default mode is SWITCHING_MODE_670KHZ */
		default:
			HAL_GPIO_WritePin(AMP_FS1_GPIO_Port, AMP_FS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AMP_FS2_GPIO_Port, AMP_FS2_Pin, GPIO_PIN_RESET);
	}
}

void AmpGain(Gain_Modes mode)
{
	switch(mode)
	{
		case GAIN_MODE_29dB:
			HAL_GPIO_WritePin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin, GPIO_PIN_RESET);
			break;
		case GAIN_MODE_19dB:
			HAL_GPIO_WritePin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin, GPIO_PIN_SET);
			break;
		case GAIN_MODE_16dB:
			HAL_GPIO_WritePin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin, GPIO_PIN_RESET);
			break;
		case GAIN_MODE_13dB:
			HAL_GPIO_WritePin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin, GPIO_PIN_SET);
			break;
		/* default mode is GAIN_MODE_29dB */
		default:
			HAL_GPIO_WritePin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin, GPIO_PIN_RESET);
	}
}
void AmpShutdown(Shutdown_Modes mode)
{
	switch(mode)
	{
		case SHUTDOWN_ENABLE:
			HAL_GPIO_WritePin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin, GPIO_PIN_RESET);
			break;
		case SHUTDOWN_DISABLE:
			HAL_GPIO_WritePin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin, GPIO_PIN_SET);
			break;
		/* default mode is SHUTDOWN_DISABLE */
		default:
			HAL_GPIO_WritePin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin, GPIO_PIN_SET);
	}
}

void AmpMute(Mute_Modes mode)
{
	switch(mode)
	{
		case MUTE_ENABLE:
			HAL_GPIO_WritePin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin, GPIO_PIN_RESET);
			break;
		case MUTE_DISABLE:
			HAL_GPIO_WritePin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin, GPIO_PIN_SET);
			break;
		/* default mode is MUTE_DISABLE */
		default:
			HAL_GPIO_WritePin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin, GPIO_PIN_SET);
	}
}
