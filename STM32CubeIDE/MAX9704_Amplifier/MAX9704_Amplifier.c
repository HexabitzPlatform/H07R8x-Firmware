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

/*********************************************************INCLUDES***************************************************************/
#include "MAX9704_Amplifier.h"

/*********************************************************PRIVATE APIS***********************************************************/
/* Amplifier Switching Mode */
/*
 * @brief  :Audio switching frequency control.
 * @param1 :Switching Modes
 * @retval :Status
 */

Amp_Status MAX9704_AmpSwitchingMode(Switching_Modes mode)
{
	switch(mode)
	{
		case SWITCHING_MODE_670KHZ:
			if( H07R8_OK != ResetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return AMP_ERR;
			if( H07R8_OK != ResetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return AMP_ERR;
			break;
		case SWITCHING_MODE_940KHZ:
			if( H07R8_OK != ResetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return AMP_ERR;
			if( H07R8_OK != SetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return AMP_ERR;
			break;
		case SWITCHING_MODE_470KHZ:
			if( H07R8_OK != SetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return AMP_ERR;
			if( H07R8_OK != ResetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return AMP_ERR;
			break;
		case SWITCHING_MODE_AROUND_670KHZ:
			if( H07R8_OK != SetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return AMP_ERR;
			if( H07R8_OK != SetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return AMP_ERR;
			break;
		/* default mode is SWITCHING_MODE_670KHZ */
		default:
			if( H07R8_OK != ResetGPIOsPin(AMP_FS1_GPIO_Port, AMP_FS1_Pin))
				return AMP_ERR;
			if( H07R8_OK != ResetGPIOsPin(AMP_FS2_GPIO_Port, AMP_FS2_Pin))
				return AMP_ERR;
	}
	return H07R8_OK;
}

/**************************************************************************************************************************/

/* Amplifier Gain */
/*
 * @brief  :Audio level control.
 * @param1 :Gain Modes
 * @retval :Status
 */

Amp_Status MAX9704_AmpGain(Amp_Gain gain)
{
	switch(gain)
	{
		case GAIN_MODE_29dB:
			if( H07R8_OK != ResetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return AMP_ERR;
			if( H07R8_OK != ResetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return AMP_ERR;
			break;
		case GAIN_MODE_19dB:
			if( H07R8_OK != ResetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return AMP_ERR;
			if( H07R8_OK != SetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return AMP_ERR;
			break;
		case GAIN_MODE_13dB:
			if( H07R8_OK != SetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return AMP_ERR;
			if( H07R8_OK != ResetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return AMP_ERR;
			break;
		case GAIN_MODE_16dB:
			if( H07R8_OK != SetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return AMP_ERR;
			if( H07R8_OK != SetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return AMP_ERR;
			break;
		/* default mode is GAIN_MODE_29dB */
		default:
			if( H07R8_OK != ResetGPIOsPin(AMP_GAIN1_GPIO_Port, AMP_GAIN1_Pin))
				return AMP_ERR;
			if( H07R8_OK != ResetGPIOsPin(AMP_GAIN2_GPIO_Port, AMP_GAIN2_Pin))
				return AMP_ERR;
	}
	return H07R8_OK;
}

/**************************************************************************************************************************/

/* Amplifier Shutdown */
/*
 * @brief  :Amplifier Shutdown.
 * @param1 :Shutdown Modes
 * @retval :Status
 */

Amp_Status MAX9704_AmpShutdown(Shutdown_Modes mode)
{
	switch(mode)
	{
		case SHUTDOWN_ENABLE:
			if( H07R8_OK != ResetGPIOsPin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin))
				return AMP_ERR;
			break;
		case SHUTDOWN_DISABLE:
			if( H07R8_OK != SetGPIOsPin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin))
				return AMP_ERR;
			break;
		/* default mode is SHUTDOWN_DISABLE */
		default:
			if( H07R8_OK != SetGPIOsPin(AMP_SHUTDOWN_GPIO_Port, AMP_SHUTDOWN_Pin))
				return AMP_ERR;
	}
	return H07R8_OK;
}

/**************************************************************************************************************************/

/* Amplifier Mute */
/*
 * @brief  :Aduio Mute.
 * @param1 :Mute Modes
 * @retval :Status
 */

Amp_Status MAX9704_AmpMute(Mute_En_Dis mute)
{
	switch(mute)
	{
		case MUTE_ENABLE:
			if( H07R8_OK != ResetGPIOsPin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin))
				return AMP_ERR;
			break;
		case MUTE_DISABLE:
			if( H07R8_OK != SetGPIOsPin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin))
				return AMP_ERR;
			break;
		/* default mode is MUTE_DISABLE */
		default:
			if( H07R8_OK != SetGPIOsPin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin))
				return AMP_ERR;
	}
	return H07R8_OK;
}

/******************************************************************USER APIs**********************************************************/

/* Amplifier Initialize */
/*
 * @brief  :Amplifier Initialize.
 * @param1 :switching mode.
 * @param2 :amplifier gain.
 * @retval :Status
 */

Amp_Status MAX9704_AmpInit(Switching_Modes switchMode, Amp_Gain gain)
{
	if( H07R8_OK != MAX9704_AmpShutdown(SHUTDOWN_ENABLE))
		return AMP_ERR;
	if( H07R8_OK != MAX9704_AmpSwitchingMode(switchMode))
	    return AMP_ERR;
	if( H07R8_OK != MAX9704_AmpGain(gain))
		return AMP_ERR;
	if( H07R8_OK != MAX9704_AmpShutdown(SHUTDOWN_DISABLE))
		return AMP_ERR;
	return H07R8_OK;
}

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
