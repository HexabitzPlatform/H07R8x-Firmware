/*
 * MAX9704_Amplifier.c
 * Description: MAX9704_Amplifier source file
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
#include "MAX9704_Amplifier.h"

/* GPIOs functions */
/*
 * @brief :set a GPIO pin form a port high
 */
Status_TypeDef SetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin)
{
	Status_TypeDef Status;

	if (NULL!=GPIOx)
	{
		HAL_GPIO_WritePin(GPIOx, Pin, GPIO_PIN_SET);
		Status=STATUS_OK;
	}
	else
		Status=STATUS_ERR;

	return Status;
}

/*
 * set a GPIO pin form a port low
 */
Status_TypeDef MAX9704_ResetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin)
{
	Status_TypeDef Status;

	if (NULL!=GPIOx)
	{
		HAL_GPIO_WritePin(GPIOx, Pin, GPIO_PIN_RESET);
		Status=STATUS_OK;
	}
	else
		Status=STATUS_ERR;

	return Status;
}

Status_TypeDef MAX9704_AmpSwitchingMode(Switching_Modes mode)
{
	switch(mode)
	{
		case SWITCHING_MODE_670KHZ:
			if( STATUS_OK != ResetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != ResetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return STATUS_ERR;
			break;
		case SWITCHING_MODE_940KHZ:
			if( STATUS_OK != ResetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != SetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return STATUS_ERR;
			break;
		case SWITCHING_MODE_470KHZ:
			if( STATUS_OK != SetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != ResetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return STATUS_ERR;
			break;
		case SWITCHING_MODE_AROUND_670KHZ:
			if( STATUS_OK != SetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != SetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return STATUS_ERR;
			break;
		/* default mode is SWITCHING_MODE_670KHZ */
		default:
			if( STATUS_OK != ResetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != ResetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9704_AmpGain(Gain_Modes mode)
{
	switch(mode)
	{
		case GAIN_MODE_29dB:
			if( STATUS_OK != ResetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != ResetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return STATUS_ERR;
			break;
		case GAIN_MODE_19dB:
			if( STATUS_OK != ResetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != SetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return STATUS_ERR;
			break;
		case GAIN_MODE_16dB:
			if( STATUS_OK != SetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != ResetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return STATUS_ERR;
			break;
		case GAIN_MODE_13dB:
			if( STATUS_OK != SetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != SetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return STATUS_ERR;
			break;
		/* default mode is GAIN_MODE_29dB */
		default:
			if( STATUS_OK != ResetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return STATUS_ERR;
			if( STATUS_OK != ResetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return STATUS_ERR;
	}
	return STATUS_OK;
}
Status_TypeDef MAX9704_AmpShutdown(Shutdown_Modes mode)
{
	switch(mode)
	{
		case SHUTDOWN_ENABLE:
			if( STATUS_OK != ResetGPIOsPin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin))
				return STATUS_ERR;
			break;
		case SHUTDOWN_DISABLE:
			if( STATUS_OK != SetGPIOsPin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin))
				return STATUS_ERR;
			break;
		/* default mode is SHUTDOWN_DISABLE */
		default:
			if( STATUS_OK != SetGPIOsPin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin))
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9704_AmpMute(Mute_Modes mode)
{
	switch(mode)
	{
		case MUTE_ENABLE:
			if( STATUS_OK != ResetGPIOsPin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin))
				return STATUS_ERR;
			break;
		case MUTE_DISABLE:
			if( STATUS_OK != SetGPIOsPin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin))
				return STATUS_ERR;
			break;
		/* default mode is MUTE_DISABLE */
		default:
			if( STATUS_OK != SetGPIOsPin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin))
				return STATUS_ERR;
	}
	return STATUS_OK;
}
