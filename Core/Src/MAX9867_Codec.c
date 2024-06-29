/*
 * MAX9704_Codec.c
 * Description: MAX9704_Codec source file
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

#include "MAX9867_Codec.h"

/* MAX9867 Registers */
/* AUX High (Read Only) Register */
Aux_L auxRegL;
/* AUX Low (Read Only) Register */
Aux_H auxRegH;
/* Interrupt Enable Register */
Interrupt_Reg interruptEnReg;
/* System Clock Register */
System_Clock_Reg sysClkReg;
/* Stereo Audio Clock
Control High Register */
Stereo_Audio_Clock_Control_High_Reg stereoAudClkRegH;
/* Stereo Audio Clock
Control Low Register */
Stereo_Audio_Clock_Control_Low_Reg stereoAudClkRegL;
/* DIGITAL AUDIO INTERFACE Register1 */
Digital_Audio_Interface_Reg_1 digAudReg1;
/* DIGITAL AUDIO INTERFACE Register2 */
Digital_Audio_Interface_Reg_2 digAudReg2;
/* Codec Filters Register */
Digital_Filtering_Reg codecFilterReg;
/* Sidetone Register */
Sidetone_Reg sidetoneReg;
/* DAC Level Register */
DAC_Level_Ctrl_Reg DAC_LevelReg;
/* ADC Level Register */
ADC_Level_Ctrl_Reg ADCLvlReg;
/* Left-Line Input Level Register */
Left_Line_input_Level_Reg lLineInReg;
/* Right-Line Input Level Register */
Right_Line_input_Level_Reg rLineInReg;
/* Left Volume Control Register */
Left_Volume_Ctrl_Reg  lVolumeCtrlReg;
/* Right Volume Control Register */
Right_Volume_Ctrl_Reg rVolumeCtrlReg;
/* Left Microphone Gain Register */
Left_Mic_Gain_Reg lMicGainReg;
/* Right Microphone Gain Register */
Right_Mic_Gain_Reg rMicGainReg;
/* ADC Input Register */
ADC_Input_Reg adcInputReg;
/* Microphone Register */
Digital_Microphone_Input_Reg digMicClkReg;
/* Mode Register */
Mode_Configuration_Reg configModeReg;
/* POWER MANAGEMENT Register */
Power_Management_Reg powerMangReg;

static uint8_t tDataCodec[2];

/******************************************************************PRIVATE APIs**********************************************************/

/* Digital Audio Interface Unit Initialize */
/*
 * @brief   :Digital Audio Interface Unit Initialize.
 * @retval  :Status
 */

Status_TypeDef MAX9867_DigitalAudioInterfaceInit(void)
{
	digAudReg1.MAS = 0;
	digAudReg1.WCI = 0;
	digAudReg1.BCI = 1;
	digAudReg1.DLY = 1;
	digAudReg1.HIZOFF = 0;
	digAudReg1.TDM = 0;

	digAudReg2.BSEL = 0;
	digAudReg2.DMONO = 0;
	digAudReg2.LVOLFIX = 0;

	tDataCodec[0] = MAX9867_REG_INTERFACE_MODE1;
	tDataCodec[1] = digAudReg1.digAudReg1;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_INTERFACE_MODE2;
	tDataCodec[1] = digAudReg2.digAudReg2;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

Status_TypeDef MAX9867_DigitalFilterInit(Digital_Audio_Filter_Mode mode, ADC_DAC_Digital_Audio_Filter_Sٍpecifications ADC_Specifications,ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications)
{

	codecFilterReg.MODE = mode;
	codecFilterReg.AVFLT = ADC_Specifications;
	codecFilterReg.DVFLT = DAC_Specifications;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = codecFilterReg.digtlFltrReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Clock Control Init */
/*
 * @brief  :ClockControlInit.
 * @param1 :Master clock Prescaler on MCLK pin.
 * @param2 :Exact integer modes.
 * @Note   :In both master and slave mode, common MCLK frequencies (12MHz, 13MHz, 16MHz,
 *          and 19.2MHz) can be programmed to operate in exact integer mode
 *          for both 8kHz and 16kHz sample rates. In these modes, the MCLK and LRCLK rates
 *          are selected by using the FREQ bits instead of the NI and PLL control bits.
 * @param3 :PLL mode enable.
 * @Note   :When operating in slave mode, a PLL can be
 *			enabled to lock onto externally generated LRCLK
 *			signals that are not integer related to PCLK.
 * @param4 :ratio PCLK/LRCLK to set LRCLK.
 * @Note   :when enable PLL mode program NI to the nearest desired ratio.
 * @param5 :PLL rapid lock mode enable.
 * @Note   :when enable PLL mode set the NI[0] = 1
 *          to enable the PLL’s rapid lock mode. If NI[0] = 0, then NI is ignored and
 *          PLL lock time is slower.
 * @retval :Status
 */

Status_TypeDef MAX9867_ClockControlInit(MCLK_Prescaler mclkPresclr, Exact_Integer_Modes exactIntMode,
		PLL_Mode_En_Dis pllMode, uint32_t NI, PLL_Rapid_Lock_En_Dis pllRapidLock)
{
	sysClkReg.PSCLK = mclkPresclr;
	sysClkReg.FREQ = exactIntMode;

	stereoAudClkRegH.PLL = pllMode;
	stereoAudClkRegH.NI = NI >> 8;

	stereoAudClkRegL.NI0 = pllRapidLock;
	stereoAudClkRegL.NI = NI;

	tDataCodec[0] = MAX9867_REG_SYSTEM_CLK;
	tDataCodec[1] = sysClkReg.sysClkReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_H;
	tDataCodec[1] = stereoAudClkRegH.stereoAudClkRegH;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_L;
	tDataCodec[1] = stereoAudClkRegL.stereoAudClkRegL;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	return STATUS_OK;
}

/****************************************************************************************************/

/* Interrupt Enable */
/*
 * @brief  :Interrupt Enable.
 * @param1 :Clip Detect Flag (Indicates that a signal has reached or exceeded full scale in the ADC or DAC).
 * @param2 :Slew Level Detect Flag.
 * @param3 :Digital PLL Unlock Flag.
 * @param4 :Headset Configuration Change Flag.
 * @retval :Status
 */

Status_TypeDef MAX9867_InterruptEnable(bool clipDetect,bool slewDetect,bool pllUnlock,bool headsetChange)
{
	interruptEnReg.ICLD = clipDetect;
	interruptEnReg.ISLD = slewDetect;
	interruptEnReg.IULK = pllUnlock;
	interruptEnReg.IJDET = headsetChange;

	tDataCodec[0] = MAX9867_REG_INT_ENA;
	tDataCodec[1] = interruptEnReg.interruptEnReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Headphone Amplifier type */
/*
 * @brief  :Headphone has three types(differential,capacitorless,single-indedd)
 * 		    and these types either to be mono or stereo.
 * @param1 :Headphone Amplifier type.
 * @retval :Status
 */

Status_TypeDef MAX9867_HeadphoneAmpType(Headphone_Amp_Type ampMode)
{
	configModeReg.HPMODE = ampMode;
	tDataCodec[0] = MAX9867_REG_MODE;
	tDataCodec[1] = configModeReg.configModeReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* DAC Enable-Disable */
/*
 * @brief  :DAC Enable-Disable to convert digital audio data to analog signal.
 * @param1 :DAC Enable-Disable.
 * @retval :Status
 */

Status_TypeDef MAX9867_DAC_EnDis(DAC_En_Dis dac)
{
		powerMangReg.DALEN = dac;
		powerMangReg.DAREN = dac;
		tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
		tDataCodec[1] = powerMangReg.pwrManagReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Set DAC Gain */
/*
 * @brief  :Set DAC Gain.
 * @Note   : MAX9867 has two amplifiers(DACG,DACA) to set the DAC gain
 * 			 we set DACG amplifier just in status voice stream,in status audio stream we set it (DAC_GAIN_0dB)
 * 			 DACA amplifier we set it in status voice or audio stream.
 * @param1 :preamplifier gain(just voice stream).
 * @param2 :programmable amplifier gain(voice and audio stream).
 * @retval :Status
 */

Status_TypeDef MAX9867_DAC_Gain(DAC_Gain firstAmp, DAC_Level_Ctrl progAmp)
{
	DAC_LevelReg.DACG = firstAmp;
	DAC_LevelReg.DACA = progAmp;
	tDataCodec[0] = MAX9867_REG_DAC_LVL;
	tDataCodec[1] = DAC_LevelReg.DACLvlReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* DAC Mute */
/*
 * @brief  :DAC Mute.
 * @param1 :DAC Mute Enable-Disable.
 * @retval :Status
 */

Status_TypeDef MAX9867_DAC_Mute(DAC_Mute_En_Dis dacMute)
{
	DAC_LevelReg.DACM = dacMute;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = DAC_LevelReg.DACLvlReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* ADC Enable-Disable */
/*
 * @brief  :ADC Enable-Disable to convert analog audio or voice signals to digital data stream.
 * @param1 :ADC Enable-Disable
 * @retval :Status
 */

Status_TypeDef MAX9867_ADC_EnDis(ADC_En_Dis adc)
{
	powerMangReg.ADLEN = adc;
	powerMangReg.ADREN = adc;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = powerMangReg.pwrManagReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Set ADC Gain */
/*
 * @brief  :Set ADC Gain.
 * @param1 :choose left/right ADC channels or both of them.
 * @param2 :Set ADC Gain.
 * @retval :Status
 */

Status_TypeDef MAX9867_ADC_Gain(ADC_L_R adc, L_R_ADC_Level_Ctrl adcGain)
{
	if(adc == ADC_LEFT)
	{
		ADCLvlReg.AVL = adcGain;
		tDataCodec[0] = MAX9867_REG_DAC_LVL;
		tDataCodec[1] = ADCLvlReg.ADCLvlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(adc == ADC_RIGHT)
	{
		ADCLvlReg.AVR = adcGain;
		tDataCodec[0] = MAX9867_REG_DAC_LVL;
		tDataCodec[1] = ADCLvlReg.ADCLvlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}

	else if(adc == ADC_LEFT_RIGHT)
	{
		ADCLvlReg.AVL = adcGain;
		tDataCodec[0] = MAX9867_REG_DAC_LVL;
		tDataCodec[1] = ADCLvlReg.ADCLvlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;

		ADCLvlReg.AVR = adcGain;
		tDataCodec[0] = MAX9867_REG_DAC_LVL;
		tDataCodec[1] = ADCLvlReg.ADCLvlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

/****************************************************************************************************/

Status_TypeDef MAX9867_ADC_AduioInputMixer(ADC_L_R lrAdcInput, L_R_ADC_Audio_Input_Mixer mixer)
{
	if(lrAdcInput == ADC_LEFT)
	{
		adcInputReg.MXINL = mixer;
		tDataCodec[0] = MAX9867_REG_ADC_IN;
		tDataCodec[1] = adcInputReg.adcInputReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}

	else if(lrAdcInput == ADC_RIGHT)
	{
	adcInputReg.MXINR = mixer;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	}

	else if(lrAdcInput == ADC_LEFT_RIGHT)
	{
		adcInputReg.MXINL = mixer;
		tDataCodec[0] = MAX9867_REG_ADC_IN;
		tDataCodec[1] = adcInputReg.adcInputReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;

		adcInputReg.MXINR = mixer;
		tDataCodec[0] = MAX9867_REG_ADC_IN;
		tDataCodec[1] = adcInputReg.adcInputReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

/****************************************************************************************************/

/* LineInput Enable-Disable */
/*
 * @brief  :LineInput  Enable-Disable(input for analog audio stream).
 * @Note   :MAX9867 has two line inputs(L/R).
 * @param1 :LineInput  Enable-Disable.
 * @param2 :choose line input(L/R/both of them).
 * @retval :Status
 */

Status_TypeDef MAX9867_LineInputEnDis(L_R_Line_Input_En_Dis lineInput, L_R_Line_Input lrLineInput)
{
	if(lrLineInput == LEFT_LINE_INPUT)
	{
		if(lineInput == LINE_INPUT_EN)
		{
			powerMangReg.LNLEN = LINE_INPUT_EN;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(lineInput == LINE_INPUT_DIS)
		{
			powerMangReg.LNLEN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
	}
	else if(lrLineInput == RIGHT_LINE_INPUT)
	{
		if(lineInput == LINE_INPUT_EN)
		{
			powerMangReg.LNREN = LINE_INPUT_EN;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(lineInput == LINE_INPUT_DIS)
		{
			powerMangReg.LNREN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
	}
	else if(lrLineInput == LEFT_RIGHT_LINE_INPUT)
	{
		if(lineInput == LINE_INPUT_EN)
		{
			powerMangReg.LNLEN = LINE_INPUT_EN;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;

			powerMangReg.LNREN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(lineInput == LINE_INPUT_DIS)
		{
			powerMangReg.LNLEN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;

			powerMangReg.LNREN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
	}
	return STATUS_OK;
}

/****************************************************************************************************/

/* Line-Input Gain */
/*
 * @brief  :Line-Input Gain.
 * @param1 :choose line input(L/R/both of them).
*  @param2  :Line-Input Gain.
 * @retval :Status
 */

Status_TypeDef MAX9867_LineInputGain(L_R_Line_Input lineInput, L_R_Line_Input_Gain lineInputGain)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		lLineInReg.LIGL = lineInputGain;
		tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
		tDataCodec[1] = lLineInReg.lLineInReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == RIGHT_LINE_INPUT)
	{
		rLineInReg.LIGR = lineInputGain;
		tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
		tDataCodec[1] = rLineInReg.rLineInReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == LEFT_RIGHT_LINE_INPUT)
	{
		lLineInReg.LIGL = lineInputGain;
		rLineInReg.LIGR = lineInputGain;
		tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
		tDataCodec[1] = lLineInReg.lLineInReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
		tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
		tDataCodec[1] = rLineInReg.rLineInReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

/****************************************************************************************************/

/* Line-Input Mute Enable-Disable */
/*
 * @brief  :Line-Input Mute Enable-Disable.
 * @param1 :mute line-input(L/R/both of them).
 * @param2 :mute line-input Enable-Disable.
 * @retval :Status
 */

Status_TypeDef MAX9867_LineInputMute(L_R_Line_Input lineInput, Line_Input_Mute_En_Dis mute)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		if(mute == LINE_INPUT_MUTE)
		{
			lLineInReg.LILM = LINE_INPUT_MUTE_EN;
			tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
			tDataCodec[1] = lLineInReg.lLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(mute == LINE_INPUT_UNMUTE)
		{
			lLineInReg.LILM = LINE_INPUT_MUTE_DIS;
			tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
			tDataCodec[1] = lLineInReg.lLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
	}
	else if(lineInput == RIGHT_LINE_INPUT)
	{
		if(mute == LINE_INPUT_MUTE)
		{
			rLineInReg.LIGR = LINE_INPUT_MUTE_EN;
			tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
			tDataCodec[1] = rLineInReg.rLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(mute == LINE_INPUT_UNMUTE)
		{
			rLineInReg.LIGR = LINE_INPUT_MUTE_DIS;
			tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
			tDataCodec[1] = rLineInReg.rLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
	}
	else if(lineInput == LEFT_RIGHT_LINE_INPUT)
	{
		if(mute == LINE_INPUT_MUTE)
		{
			lLineInReg.LILM = LINE_INPUT_MUTE_EN;
			tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
			tDataCodec[1] = lLineInReg.lLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;

			rLineInReg.LIGR = LINE_INPUT_MUTE_EN;
			tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
			tDataCodec[1] = rLineInReg.rLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(mute == LINE_INPUT_UNMUTE)
		{
			lLineInReg.LILM = LINE_INPUT_MUTE_DIS;
			tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
			tDataCodec[1] = lLineInReg.lLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;

			rLineInReg.LIGR = LINE_INPUT_MUTE_DIS;
			tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
			tDataCodec[1] = rLineInReg.rLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
					return STATUS_ERR;
		}
	}
	return STATUS_OK;
}

/****************************************************************************************************/

/* Microphone Amplifier Gain */
/*
 * @brief  :Set Microphone Amplifier Gain.
 * @Note   :MAX867 has two amplifiers to set the microphone gain where:
 * 			PALEN(pre-amplifier),PGAML(programmable amplifier).
 * @param1 :choose microphone(L/R/both of them).
 * @param2 :pre-amplifier gain (PALEN).
 * @param3 :programmable gain (PGAML).
 * @retval :Status
 */

Status_TypeDef MAX9867_MicAmpGain(L_R_Mic mic, L_R_Mic_Preamp_Gain preAmpGain, L_R_Mic_Programble_Gain_Amp progGain)
{
	if(mic == MIC_LEFT)
	{
		lMicGainReg.PALEN = preAmpGain;
		lMicGainReg.PGAML = progGain;
		tDataCodec[0] = MAX9867_REG_L_MIC_GAIN;
		tDataCodec[1] = lMicGainReg.lMicGainReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}

	else if(mic == MIC_RIGHT)
	{
		rMicGainReg.PAREN = preAmpGain;
		rMicGainReg.PGAMR = progGain;
		tDataCodec[0] = MAX9867_REG_R_MIN_GAIN;
		tDataCodec[1] = rMicGainReg.rMicGainReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(mic == MIC_LEFT_RIGHT)
	{
		lMicGainReg.PALEN = preAmpGain;
		lMicGainReg.PGAML = progGain;
		tDataCodec[0] = MAX9867_REG_L_MIC_GAIN;
		tDataCodec[1] = lMicGainReg.lMicGainReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;

		rMicGainReg.PAREN = preAmpGain;
		rMicGainReg.PGAMR = progGain;
		tDataCodec[0] = MAX9867_REG_R_MIN_GAIN;
		tDataCodec[1] = rMicGainReg.rMicGainReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

/****************************************************************************************************/

/* Set Microphone Sidetone Source And Gain */
/*
 * @brief  :Set Microphone Sidetone Source And Gain.
 * @Note   :Sidetone is the sound of your own voice that you hear in a telephone receiver or headset
 * 			while speaking on the phone. It allows you to hear yourself speaking, which can help
 * 			regulate your voice volume and pitch during a conversation.
 * @param1 :Sidtone source is from (L/R/Both of them) ADC.
 * @param2 :Sidtone Gain.
 * @Note   :Sidtone Gain depend on type of headphone amplifier where:
 * 			- differential amplifier has gain.
 * 			- capacitorless and single-endded amplifier has gain.
 * @param3 :Amplifier type.
 * @retval :Status
 */

Status_TypeDef MAX9867_MicSidetoneSourceAndGain(Digital_Sidetone_Source_Mixer sourceMixer, Sidetone_Gain_Diff_Headphone sidGainDiff
		, Sidetone_Gain_Capacitorless_Single_Ended_Headphone sidGainCapSinEnd, Amp_Type ampType)
{
	sidetoneReg.DSTS = sourceMixer;
	tDataCodec[0] = MAX9867_REG_SIDETONE;
	tDataCodec[1] = sidetoneReg.sidetoneReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	if(ampType == DIFFERENTIAL_AMP)
	{
		sidetoneReg.DVST = sidGainDiff;
		tDataCodec[0] = MAX9867_REG_SIDETONE;
		tDataCodec[1] = sidetoneReg.sidetoneReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(ampType == CAPACITORLESS_AMP || ampType == SINGLE_ENDED_AMP)
	{
		sidetoneReg.DVST = sidGainCapSinEnd;
		tDataCodec[0] = MAX9867_REG_SIDETONE;
		tDataCodec[1] = sidetoneReg.sidetoneReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

/****************************************************************************************************/

/* Set Digital Microphone Clock */
/*
 * @brief  :Set Digital Microphone Clock.
 * @param1 :Set Digital Microphone Clock.
 * @retval :Status
 */

Status_TypeDef MAX9867_MicDigitalClk(Digital_Mic_Clk_Pre clock)
{
    digMicClkReg.MICCLK = clock;
	tDataCodec[0] = MAX9867_REG_MIC;
	tDataCodec[1] = digMicClkReg.digMicClkReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Digital Microphone Enable-Disable */
/*
 * @brief  :Digital Microphone Enable-Disable.
 * @Note   : this table describe how enable digital microphone
 	 	 	 ________________________________________________________________________
 			|DIGMICL | DIGMICR | Left ADC Input  		  | Right ADC Input          |
 			|________|_________|__________________________|__________________________|
			|   0    |    0    | ADC input mixer 		  | ADC input mixer			 |
			|________|_________|__________________________|__________________________|
            |        |         | Line input (left analog  | Right digital microphone |
            |   0    |    1    | microphone unavailable)  |							 |
            |________|_________|__________________________|__________________________|
            |   1    |    0    | Left digital microphone  | ADC input mixer			 |
            |________|_________|__________________________|__________________________|
            |   1    |    1    | Left digital microphone  | Right digital microphone |
            |________|_________|__________________________|__________________________|

 * @Note   :The left analog microphone input is never available when DIGMICL or DIGMICR = 1.
 * @param1 :Digital Microphone Enable-Disable.
 * @retval :Status
 */

Status_TypeDef MAX9867_MicDigitalLeftRightEnDis(Digital_Mic_Clk_Mode mode)
{
	switch(mode)
	{
		case 0:
			digMicClkReg.DIGMICL = 0;
			digMicClkReg.DIGMICR = 0;
			break;
		case 1:
			digMicClkReg.DIGMICL = 0;
			digMicClkReg.DIGMICR = 1;
			break;
		case 2:
			digMicClkReg.DIGMICL = 1;
			digMicClkReg.DIGMICR = 0;
			break;
		case 3:
			digMicClkReg.DIGMICL = 1;
			digMicClkReg.DIGMICR = 1;
			break;
		default:
			digMicClkReg.DIGMICL = 0;
			digMicClkReg.DIGMICR = 0;
	}
	tDataCodec[0] = MAX9867_REG_MIC;
	tDataCodec[1] = digMicClkReg.digMicClkReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Auxiliary Input Capture */
/*
 * @brief  :Update AUX register on MAX9867 JACKSNS/AUX pin with the DC voltage
 * 			or holding AUX register for reading.
 * @param1 :Update or holding AUX register
 * @retval :Status
 */

Status_TypeDef MAX9867_AuxiliaryInputCapture(Auxiliary_Input_Capture auxCapture)
{
	adcInputReg.AUXCAP = auxCapture;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Auxiliary Input Gain Calibration */
/*
 * @brief  :Auxiliary Input Gain Calibration.
 * @Note   :When set this bit The input buffer (AUX register) is disconnected from JACKSNS/AUX and connected to an internal voltage reference.
			While in this mode, read the AUX register and store the value. Use the stored value as a gain
			calibration factor, K, on subsequent readings.
 * @param1 :Auxiliary Input Gain Calibration.
 * @retval :Status
 */

Status_TypeDef MAX9867_AuxiliaryInputGainCalibration(Auxiliary_Input_Gain_Calibration auxGain)
{
	adcInputReg.AUXGAIN = auxGain;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Auxiliary Input Offset Calibration */
/*
 * @brief  :Auxiliary Input Offset Calibration.
 * @Note   :When set this bit JACKSNS/AUX pin is disconnected from the input
 * 		    and the ADC automatically calibrates out any internal offsets.
 * @param1 :Auxiliary Input Offset Calibration.
 * @retval :Status
 */

Status_TypeDef MAX9867_AuxiliaryInputOffsetCalibration(Auxiliary_Input_Offset_Calibration auxOffset)
{
	adcInputReg.AUXCAL = auxOffset;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Auxiliary Input Type */
/*
 * @brief  :Auxiliary Input Type.
 * @Note   :When set this bit:
 * 			0 = Use JACKSNS/AUX for jack detection.
 * 			1 = Use JACKSNS/AUX for DC measurements.
 * @param1 :Auxiliary Input Type.
 * @retval :Status
 */

Status_TypeDef MAX9867_AuxiliaryInputType(Auxiliary_Input_Type auxType)
{
	adcInputReg.AUXEN = auxType;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

Status_TypeDef MAX9867_AuxiliaryRegRead(uint16_t *aux)
{
	tDataCodec[0] = MAX9867_REG_AUX_L;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 1) )
			return STATUS_ERR;
	if( STATUS_OK != ReadI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_R, &auxRegL.auxRegL, 1) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_AUX_H;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 1) )
			return STATUS_ERR;
	if( STATUS_OK != ReadI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_R, &auxRegH.auxRegH, 1) )
			return STATUS_ERR;

	*aux = ((auxRegH.auxRegH << 8) | auxRegL.auxRegL);
	return STATUS_OK;
}

/****************************************************************************************************/

/* JackSens Enable Disable */
/*
 * @brief  :JackSens Enable Disable.
 * @param1 :JackSens Enable Disable.
 * @Note   :0 = Enables pullups on LOUTP and JACKSNS/AUX to detect jack insertion.
 * 			1 = Enables the comparator circuitry on JACKSNS/AUX to detect voltage changes.
 * @retval :Status
 */

Status_TypeDef MAX9867_JackSensEnDis(Jack_Sense_En_Dis jackSens)
{
	configModeReg.JDETEN = jackSens;
	tDataCodec[0] = MAX9867_REG_MODE;
	tDataCodec[1] = configModeReg.configModeReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/********************************************************************************USER APIs*****************************************************************************************************************************/

/* Audio Amplify Recording */
/*
 * @brief   :Amplify/recording the audio which comes on line-inputs pins(LINL,LINR).
 * @Note    :some of parameters used in (Amplify mode) and other in (Recording mode) and other (Amplify and Recording modes).
 * @Note    :in status Amplify mode ignore Recording mode parameters,vice versa.
 * @param1  :line-input mode(Amplify/recording/both of them).
 * @param2  :line-input channel(L/R/both of them) (Amplify and Recording).
 * @param3  :line-input gain (Amplify and recording).
 * @param4  :choose Audio channel(L/R/both of them) (Amplify mode).
 * @param5  :right Audio amplifier level control (Amplify mode).
 * @param6  :left Audio amplifier level control (Amplify mode).
 * @param7  :choose ADC channel(L/R/both of them) (Recording).
 * @param8  :Set ADC gain (Recording).
 * @param9  :choose Audio ADC filter type (Recording).
 * @Note1   :MAX9867 uses (FIR filter) for Audio, so use configure in @Note2.
 * @Note2   :0 = DC-blocking filter is disabled.
 * 			 Any other setting = DC-blocking filter is enabled.
 * @retval  :Status
 */

Status_TypeDef AudioAmplifyRecordingInit(Line_Input_Mode lineInputMode, L_R_Line_Input lrLineInput,L_R_Line_Input_Gain gain, L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol,
		L_R_Playback_Volume lPlaybackVol, ADC_L_R adc, L_R_ADC_Level_Ctrl adcGain, ADC_DAC_Digital_Audio_Filter_Sٍpecifications ADC_Specifications)
{
	/* Line-input gain */
	if( STATUS_OK != MAX9867_LineInputGain(lrLineInput, gain))
			return STATUS_ERR;
	if(lineInputMode == LINE_INPUT_AMPLIFY || lineInputMode == LINE_INPUT_AMPLIFY_RECORDING)
	{
		/* Audio level control */
		if( STATUS_OK != MAX9867_AudioLevel(channel, rPlaybackVol, lPlaybackVol))
				return STATUS_ERR;
		/* when enable line-input, automatically enables the left and right headphone */
	}
	else if(lineInputMode == LINE_INPUT_RECORDING || lineInputMode == LINE_INPUT_AMPLIFY_RECORDING)
	{
		/* choose input ADC */
		if( STATUS_OK != MAX9867_ADC_AduioInputMixer(adc, L_R_LINE_IN))
				return STATUS_ERR;
		/* set digital filter */
		if( STATUS_OK != MAX9867_DigitalFilterInit(FIR_AUDIO_FILTER,ADC_Specifications,DISABLED))
				return STATUS_ERR;
		/* set ADC gain */
		if( STATUS_OK != MAX9867_ADC_Gain(adc, adcGain))
				return STATUS_ERR;
		/* Enable ADC */
		if( STATUS_OK != MAX9867_ADC_EnDis(ADC_ENABLE))
				return STATUS_ERR;
	}
	/* Enable left or right or both of them line-input */
	/* Note : if the right line-input is disabled and left line-input is enabled,
	 * the left line-input signal is routed to
	 * the left and right ADC input mixer and left and right headphone amplifier
	 */
	if( STATUS_OK != MAX9867_LineInputEnDis(LINE_INPUT_EN, lrLineInput))
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Voice Amplify Recording */
/*
 * @brief   :Amplify/recording the voice which comes on microphones inputs pins(MICLP,MICLN,MICRP,MICRN).
 * @Note    :some of parameters used in (Amplify mode) and other in (Recording mode) and other (Amplify and Recording modes).
 * @Note    :in status Amplify mode ignore Recording mode parameters,vice versa.
 * @param1  :microphones mode(Amplify/recording/both of them).
 * @param2  :microphones channel(L/R/both of them) (Amplify and Recording modes)
 * @param3  :Preamplifier gain (Amplify and Recording modes).
 * @param4  :programmable gain (Amplify and Recording modes).
 * @param5  :choose Voice ADC filter type (Amplify and Recording modes).
 * @Note    :choose the Voice ADC filter type from the table below.
 * @param6  :choose Voice DAC filter type (Amplify mode).
 * @Note    :choose the Voice DAC filter type from the table below.
 * @Note    :MAX9867 uses (IIR filter) for voice, so use configure in the table below.
  	     _________________________________________________________________________________________________________
  		|	  CODE  |	 FILTER TYPE  |	INTENDED SAMPLE RATE | (kHz) HIGHPASS CORNER FREQUENCY (Hz) | 217Hz NOTCH |
  		|___________|_________________|______________________|______________________________________|_____________|
        |     0x0   |                                     Disabled												  |
        |___________|_____________________________________________________________________________________________|
        |     0x1  	|    Elliptical   |       	  16         |                  256                 |     Yes     |
        |___________|_________________|______________________|______________________________________|_____________|
		|     0x2  	|    Butterworth  |       	  16         |                  500                 |     No      |
		|___________|_________________|______________________|______________________________________|_____________|
        |     0x3  	|    Elliptical   |            8         |                  256                 |     Yes     |
        |___________|_________________|______________________|______________________________________|_____________|
        |     0x4  	|    Butterworth  |            8         |                  500                 |     No      |
        |___________|_________________|______________________|______________________________________|_____________|
        |     0x5  	|    Butterworth  |          8 to 24     |                 fS/240               |     No      |
        |___________|_________________|______________________|______________________________________|_____________|
        | 0x6 to 0x7|                 |                   Reserved                                                |
        |___________|_________________|___________________________________________________________________________|
 * @param7  :choose ADC channel (L/R/both of them) (Amplify and Recording modes).
 * @param8  :Set ADC gain (Amplify and Recording).
 * @param9  :choose voice source (L/R/both of them) ADC channel (Amplify mode).
 * @param10 :sidetone gain in status differential headphone type (Amplify mode).
 * @Note    :ignore it in status capacitorless or single-endded headphone type
 * @param11 :sidetone gain in status capacitorless or single-endded headphone type (Amplify mode).
 * @Note    :ignore it in status differential headphone type
 * @param12 :amplifier type (differential/capacitorless/single-endded) (Amplify mode).
 * @param13 :choose Audio channel(L/R/both of them) (Amplify mode).
 * @param14 :right Audio amplifier level control (Amplify mode).
 * @param15 :left Audio amplifier level control (Amplify mode).
 * @retval  :Status
 */

Status_TypeDef VoiceAmplifyRecordingInit(Mic_Mode micMode, L_R_Mic mic,
		L_R_Mic_Preamp_Gain preAmpGain, L_R_Mic_Programble_Gain_Amp progGain,
		ADC_DAC_Digital_Audio_Filter_Sٍpecifications ADC_Specifications,
		ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications,
		ADC_L_R adc, L_R_ADC_Level_Ctrl adcGain,
		Digital_Sidetone_Source_Mixer sourceMixer, Sidetone_Gain_Diff_Headphone sidGainDiff,
		Sidetone_Gain_Capacitorless_Single_Ended_Headphone sidGainCapSinEnd, Amp_Type ampType,
		L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol)
{
	/* set microphone amplifier gain */
	if( STATUS_OK != MAX9867_MicAmpGain(mic, preAmpGain, progGain))
			return STATUS_ERR;
	/* choose input ADC */
	if( STATUS_OK != MAX9867_ADC_AduioInputMixer(adc, L_R_ANALOG_MIC))
			return STATUS_ERR;
	/* set digital filter */
	if( STATUS_OK != MAX9867_DigitalFilterInit(IIR_VOICE_FILTER,ADC_Specifications,DAC_Specifications))
			return STATUS_ERR;
	/* set ADC gain */
	if( STATUS_OK != MAX9867_ADC_Gain(adc, adcGain))
			return STATUS_ERR;

	if(micMode == MIC_AMPLIFY || micMode == MIC_AMPLIFY_RECORDING)
	{
		/* choose mic source from left or right ADC or both of them
		 * then set microphone gain.
		 */
		if( STATUS_OK != MAX9867_MicSidetoneSourceAndGain(sourceMixer, sidGainDiff, sidGainCapSinEnd, ampType))
				return STATUS_ERR;
		/* set audio level */
		if( STATUS_OK != MAX9867_AudioLevel(channel, rPlaybackVol, lPlaybackVol))
				return STATUS_ERR;
		/* Enable DAC */
		if( STATUS_OK != MAX9867_DAC_EnDis(DAC_ENABLE))
				return STATUS_ERR;

	}
	/* Enable ADC */
	if( STATUS_OK != MAX9867_ADC_EnDis(ADC_ENABLE))
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Reading Digital Audio */
/*
 * @brief  :Reading Digital Audio from (RAM/SDcard/...etc).
 * @param1 :choose digital audio mode (Audio,Voice).
 * @Note   :Audio like(song/speech/...etc from line-input) Voice like (sound produced by vocal cords from microphone).
 * @param2 :choose Audio channel(L/R/both of them).
 * @param3 :preamplifier gain(just voice stream).
 * @param4 :programmable amplifier gain(voice and audio stream).
 * @Note   :MAX9867 has two amplifiers(DACG,DACA) to set the DAC gain
 * 			 we set DACG amplifier (preamplifier) just in status voice stream,in status audio stream we set it (DAC_GAIN_0dB)
 * 			 DACA amplifier (programmable amplifier) we set it in status voice or audio stream.
 * @param5 :choose DAC filter type.
 * @Note   :in status digital voice data (we use IIR filter) set DAC filter type from the table below:
  		 _________________________________________________________________________________________________________
  		|	  CODE  |	 FILTER TYPE  |	INTENDED SAMPLE RATE | (kHz) HIGHPASS CORNER FREQUENCY (Hz) | 217Hz NOTCH |
  		|___________|_________________|______________________|______________________________________|_____________|
        |     0x0   |                                     Disabled												  |
        |___________|_____________________________________________________________________________________________|
        |     0x1  	|    Elliptical   |       	  16         |                  256                 |     Yes     |
        |___________|_________________|______________________|______________________________________|_____________|
		|     0x2  	|    Butterworth  |       	  16         |                  500                 |     No      |
		|___________|_________________|______________________|______________________________________|_____________|
        |     0x3  	|    Elliptical   |            8         |                  256                 |     Yes     |
        |___________|_________________|______________________|______________________________________|_____________|
        |     0x4  	|    Butterworth  |            8         |                  500                 |     No      |
        |___________|_________________|______________________|______________________________________|_____________|
        |     0x5  	|    Butterworth  |          8 to 24     |                 fS/240               |     No      |
        |___________|_________________|______________________|______________________________________|_____________|
        | 0x6 to 0x7|                 |                   Reserved                                                |
        |___________|_________________|___________________________________________________________________________|
 * @Note   :in status digital audio data (we use FIR filter) set DAC filter type from the configure below:
 * 		    0 = DC-blocking filter is disabled.
 * 			Any other setting = DC-blocking filter is enabled.
 * @param6 :right Audio amplifier level control.
 * @param7 :left Audio amplifier level control.
 * @retval :Status
 */

Status_TypeDef ReadingDigitalAudioInit(Digital_Audio_Mode audioMode, L_R_Playback_Volume_Channel channel, DAC_Gain firstAmp, DAC_Level_Ctrl progAmp,
		ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol)
{
	if(audioMode == AUDIO)
	{
		/* in audio application we must set only programmable amplifier, and set preamplifier to 0 because it
		 * especial with microphone voice gain */
		if( STATUS_OK != MAX9867_DAC_Gain(DAC_GAIN_0dB, progAmp))
				return STATUS_ERR;
		/* Enable DC-blocking just in Audio mode */
		/* Note : we can enable DC-blocking by set AVFLT and DVFLT with any value */
		if( STATUS_OK != MAX9867_DigitalFilterInit(FIR_AUDIO_FILTER,DISABLED,DAC_Specifications))
				return STATUS_ERR;
	}
	else if(audioMode == VOICE)
	{

		if( STATUS_OK != MAX9867_DAC_Gain(firstAmp, progAmp))
				return STATUS_ERR;
		/* Enable DC-blocking just in Audio mode */
		/* Note : we can enable DC-blocking by set AVFLT and DVFLT with any value */
		if( STATUS_OK != MAX9867_DigitalFilterInit(IIR_VOICE_FILTER,DISABLED,DAC_Specifications))
				return STATUS_ERR;
	}
	/* Audio level control */
	if( STATUS_OK != MAX9867_AudioLevel(channel, rPlaybackVol, lPlaybackVol))
			return STATUS_ERR;
	/* Enable DAC */
	if( STATUS_OK != MAX9867_DAC_EnDis(DAC_ENABLE))
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* DC measurement Initialize */
/*
 * @brief  :DC Measurement Initialize.
 * @param1 :pointer to calibration factor.
 * @Note   :use this factor to calculate DC measurement.
 * @retval :Status
 */

Status_TypeDef DcMeasurementInit(uint16_t *calibrationFactor)
{

	if( STATUS_OK != MAX9867_DigitalAudioInterfaceInit())
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_JackSensEnDis(JACKSNS_ENABLE))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_ADC_EnDis(ADC_ENABLE))
			return STATUS_ERR;
	/* Calibrate the offset */
	if( STATUS_OK != MAX9867_AuxiliaryInputType(JACKSNS_PIN_FOR_DC_MEASUREMENT))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_AuxiliaryInputOffsetCalibration(ADC_AUTO_CALIBRATE_ANY_OFFSET))
		    return STATUS_ERR;
	_DELAY_MS(40);
	if( STATUS_OK != MAX9867_AuxiliaryInputOffsetCalibration(OFFSET_NORMAL_OPERATION))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_AuxiliaryInputGainCalibration(CONNECT_INPUT_BUFFER_TO_INTERNAL_VOLTAGE))
			return STATUS_ERR;
	_DELAY_MS(40);
	if( STATUS_OK != MAX9867_AuxiliaryInputCapture(CONNECT_INPUT_BUFFER_TO_INTERNAL_VOLTAGE))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_AuxiliaryRegRead(calibrationFactor))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_AuxiliaryInputCapture(GAIN_NORMAL_OPERATION))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_AuxiliaryInputGainCalibration(GAIN_NORMAL_OPERATION))
			return STATUS_ERR;
		if( STATUS_OK != MAX9867_AuxiliaryInputType(JACKSNS_PIN_FOR_JACK_DETECTION))
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Reading DC measurement */
/*
 * @brief  :reading DC Measurement on JACKSNS/AUX pin.
 * @param1 :DC Measurement value.
 * @param2 :calibration factor value.
 * @retval :Status
 */

Status_TypeDef ReadingDcMeasurement(uint32_t *dcMeasurement, uint16_t calibrationFactor)
{
	uint16_t auxReg;
	/* Measure the voltage on JACKSNS/AUX */
	if( STATUS_OK != MAX9867_AuxiliaryInputType(JACKSNS_PIN_FOR_DC_MEASUREMENT))
			return STATUS_ERR;
	_DELAY_MS(40);
	if( STATUS_OK != MAX9867_AuxiliaryInputCapture(CONNECT_INPUT_BUFFER_TO_INTERNAL_VOLTAGE))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_AuxiliaryRegRead(&auxReg))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_AuxiliaryInputCapture(GAIN_NORMAL_OPERATION))
			return STATUS_ERR;
	/* DC measurement complete */
	*dcMeasurement = 0.738 * (auxReg/calibrationFactor);
	return STATUS_OK;
}

/*********************************************************************FOR H07R8 MODULE AND USER****************************************************************/

/* MAX9867 Init Application */
/*
 * @brief  :this API contain:
 * 		    - initializing digital audio interface unit.
 * 		    - initializing Reading Digital Audio.
 * 		    - choosing headphone amplifier type.
 * @param1 :control DAC gain.
 * @param2 :right Audio amplifier level control.
 * @param3 :left Audio amplifier level control.
 * @retval :Status
 */

Status_TypeDef MAX9867_CodecInit(DAC_Level_Ctrl dacGain,L_R_Playback_Volume rPlaybackVol,L_R_Playback_Volume lPlaybackVol)
{
	/* ENABLE Codec */
	if( STATUS_OK != MAX9867_Shoutdown(SHOUTDOWN_DISABLE))
		return STATUS_ERR;
	/* Setting the clock signals for Codec where:
	 * - MCLK_BETWEEN_10_20_MHZ : determine the master clock frequency coming from processor to codec.
	 * - NORMAL_OR_PLL_MODE : Normal mode.
	 * - PLL_DISABLE : see datasheet.
	 * - 0x6000 : determine the LRCLK frequency coming from processor to codec
	 * - 0 : disable rapid lock mode because of PLL is disable
	 * */
	if( STATUS_OK != MAX9867_ClockControlInit(MCLK_BETWEEN_10_20_MHZ, NORMAL_OR_PLL_MODE,
	  		PLL_DISABLE, 0x6000, 0))
		return STATUS_ERR;
	/* Setting the Digital Audio Interface which responsible for manage the codec */
	if( STATUS_OK != MAX9867_DigitalAudioInterfaceInit())
		return STATUS_ERR;
	/* setting the digital audio path in the codec where :
	 * - AUDIO : determine the sound is audio not voice.
	 * - LEFT_RIGHT_VOLUME_CHA : Enable left/right audio channel.
	 * - DAC_GAIN_0dB : disable preamplifier,this amplifier just use in status the sound is voice.
	 * - dacGain : setting the programmable amplifier by user.
	 * - TYPE1 : this filter just for voice,it isn't necessary in status the sound is audio.
	 * - rPlaybackVol : setting the level sound for the right channel by the user.
	 * - lPlaybackVol : setting the level sound for the left channel by the user.
	 * */
	if(STATUS_OK != ReadingDigitalAudioInit(AUDIO, LEFT_RIGHT_VOLUME_CHA, DAC_GAIN_0dB, dacGain,
			TYPE1, rPlaybackVol, lPlaybackVol))
		return STATUS_ERR;
	/* determine the headphone amplifier type in our status it is differential */
	if( STATUS_OK != MAX9867_HeadphoneAmpType(STEREO_DIFF_CLICKLESS))
		return STATUS_ERR;
	 return STATUS_OK;
}

/****************************************************************************************************/

/* Set Audio Volume Level */
/*
 * @brief  :Set Audio Volume Level.
 * @param1 :choose (L/R/both of them) channels
 * @param2 :left channel audio volume level.
 * @param3 :right channel audio volume level.
 * @retval :Status
 */

Status_TypeDef MAX9867_AudioLevel(L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol)
{
	if(channel == LEFT_VOLUME_CHA)
	{
		lVolumeCtrlReg.VOLL = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(channel == RIGHT_VOLUME_CHA)
	{
		rVolumeCtrlReg.VOLR = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(channel == LEFT_RIGHT_VOLUME_CHA)
	{
		rVolumeCtrlReg.VOLR = rPlaybackVol;
		lVolumeCtrlReg.VOLL = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

/****************************************************************************************************/

/* Set DAC Gain for play digital audio */
/*
 * @brief  :Set DAC Gain for play digital audio.
 * @param2 :programmable amplifier gain(voice and audio stream).
 * @retval :Status
 */

Status_TypeDef Codec_DAC_Gain(DAC_Level_Ctrl progAmp)
{
	if( STATUS_OK != MAX9867_DAC_Gain(DAC_GAIN_0dB, progAmp))
			return STATUS_ERR;
	return STATUS_OK;
}

/****************************************************************************************************/

/* Audio Mute Enable-Disable */
/*
 * @brief  :Audio Mute Enable-Disable.
 * @param1 :Audio Mute Enable-Disable.
 * @retval :Status
 */

Status_TypeDef MAX9867_AudioMute(Audio_Mute audioMute)
{
	if(audioMute == AUDIO_MUTE_ENABLE)
	{
		rVolumeCtrlReg.VOLRM = AUDIO_MUTE_ENABLE;
		lVolumeCtrlReg.VOLLM = AUDIO_MUTE_ENABLE;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(audioMute == AUDIO_MUTE_DISABLE)
	{
		rVolumeCtrlReg.VOLRM = AUDIO_MUTE_DISABLE;
		lVolumeCtrlReg.VOLLM = AUDIO_MUTE_DISABLE;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

/****************************************************************************************************/

/* Shoutdown Enable-Disable */
/*
 * @brief  :Shoutdown Enable-Disable.
 * @param1 :Shoutdown Enable-Disable.
 * @retval :Status
 */

Status_TypeDef MAX9867_Shoutdown(Shoutdown shtdown)
{
	powerMangReg.SHDN = shtdown;
	tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
	tDataCodec[1] = powerMangReg.pwrManagReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
