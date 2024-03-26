/*
 * MAX9704_Amplifier.c
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
Aux_L auxRegL;
Aux_H auxRegH;
Interrupt_Reg interruptEnReg;
System_Clock_Reg sysClkReg;
Stereo_Audio_Clock_Control_High_Reg stereoAudClkRegH;
Stereo_Audio_Clock_Control_Low_Reg stereoAudClkRegL;
Digital_Audio_Interface_Reg_1 digAudReg1;
Digital_Audio_Interface_Reg_2 digAudReg2;
Digital_Filtering_Reg codecFilterReg;
Sidetone_Reg sidetoneReg;
DAC_Level_Ctrl_Reg DAC_LevelReg;
ADC_Level_Ctrl_Reg ADCLvlReg;
Left_Line_input_Level_Reg lLineInReg;
Right_Line_input_Level_Reg rLineInReg;
Left_Volume_Ctrl_Reg  lVolumeCtrlReg;
Right_Volume_Ctrl_Reg rVolumeCtrlReg;
Left_Mic_Gain_Reg lMicGainReg;
Right_Mic_Gain_Reg rMicGainReg;
ADC_Input_Reg adcInputReg;
Digital_Microphone_Input_Reg digMicClkReg;
Mode_Configuration_Reg configModeReg;
Power_Management_Reg powerMangReg;

static uint8_t tDataCodec[2];



Status_TypeDef MAX9867_DigitalAudioInterfaceInit(MAX9867_Master_Slave_Mode mode,
	MAX9867_L_R_Clk_Invert lrclk, MAX9867_Bit_Clk_Invert bclkMode, MAX9867_SDOUT_Delay sdoutDelay,
	MAX9867_Delay_Mode delayMode, MAX9867_SDOUT_Mode_High_Impedance_Mode sdoutMode,
	MAX9867_TDM_Mode tdmMode, MAX9867_BCLK_Select bclkSelect, MAX9867_Mono_Playback_Mode monoMode,
	MAX9867_Fix_Line_Input_Volume fixLineVol)
{
	digAudReg1.MAS = mode;
	digAudReg1.WCI = lrclk;
	digAudReg1.BCI = bclkMode;
	digAudReg1.DLY = sdoutDelay;
	digAudReg1.HIZOFF = sdoutMode;
	digAudReg1.TDM = tdmMode;

	digAudReg2.BSEL = bclkSelect;
	digAudReg2.DMONO = monoMode;
	digAudReg2.LVOLFIX = fixLineVol;

	tDataCodec[0] = MAX9867_REG_INTERFACE_MODE1;
	tDataCodec[1] = digAudReg1.digAudReg1;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_INTERFACE_MODE2;
	tDataCodec[1] = digAudReg2.digAudReg2;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_DigitalFilterInit(Digital_Audio_Filter_Mode mode, ADC_DAC_Digital_Audio_Filter_Sٍpecifications ADC_Specifications,ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications)
{

	codecFilterReg.MODE = mode;
	codecFilterReg.AVFLT = ADC_Specifications;
	codecFilterReg.DVFLT = DAC_Specifications;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = codecFilterReg.digtlFltrReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

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
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_H;
	tDataCodec[1] = stereoAudClkRegH.stereoAudClkRegH;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_L;
	tDataCodec[1] = stereoAudClkRegL.stereoAudClkRegL;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;

	return STATUS_OK;
}

Status_TypeDef MAX9867_InterruptEnable(bool clipDetect,bool slewDetect,bool pllUnlock,bool headsetChange)
{
	interruptEnReg.ICLD = clipDetect;
	interruptEnReg.ISLD = slewDetect;
	interruptEnReg.IULK = pllUnlock;
	interruptEnReg.IJDET = headsetChange;

	tDataCodec[0] = MAX9867_REG_INT_ENA;
	tDataCodec[1] = interruptEnReg.interruptEnReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_Shoutdown(Shoutdown shtdown)
{
	powerMangReg.SHDN = shtdown;
	tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
	tDataCodec[1] = powerMangReg.pwrManagReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_HeadphoneAmpType(Headphone_Amp_Type ampMode)
{
	configModeReg.HPMODE = ampMode;
	tDataCodec[0] = MAX9867_REG_MODE;
	tDataCodec[1] = configModeReg.configModeReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_DAC_EnableDisable(DAC_En_Dis dac)
{
		powerMangReg.DALEN = dac;
		powerMangReg.DAREN = dac;
		tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
		tDataCodec[1] = powerMangReg.pwrManagReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_DAC_Gain(DAC_Gain firstAmp, DAC_Level_Ctrl progAmp)
{
	DAC_LevelReg.DACG = firstAmp;
	DAC_LevelReg.DACA = progAmp;
	tDataCodec[0] = MAX9867_REG_DAC_LVL;
	tDataCodec[1] = DAC_LevelReg.DACLvlReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_DAC_Mute(DAC_Mute_En_Dis dacMute)
{
	DAC_LevelReg.DACM = dacMute;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = DAC_LevelReg.DACLvlReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_ADC_EnableDisable(ADC_En_Dis adc)
{
	powerMangReg.ADLEN = adc;
	powerMangReg.ADREN = adc;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = powerMangReg.pwrManagReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_ADC_Gain(ADC_L_R adc, L_R_ADC_Level_Ctrl adcGain)
{
	if(adc == ADC_LEFT)
	{
		ADCLvlReg.AVL = adcGain;
		tDataCodec[0] = MAX9867_REG_DAC_LVL;
		tDataCodec[1] = ADCLvlReg.ADCLvlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(adc == ADC_RIGHT)
	{
		ADCLvlReg.AVR = adcGain;
		tDataCodec[0] = MAX9867_REG_DAC_LVL;
		tDataCodec[1] = ADCLvlReg.ADCLvlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}

	else if(adc == ADC_LEFT_RIGHT)
	{
		ADCLvlReg.AVL = adcGain;
		tDataCodec[0] = MAX9867_REG_DAC_LVL;
		tDataCodec[1] = ADCLvlReg.ADCLvlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;

		ADCLvlReg.AVR = adcGain;
		tDataCodec[0] = MAX9867_REG_DAC_LVL;
		tDataCodec[1] = ADCLvlReg.ADCLvlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_ADC_AduioInputMixer(ADC_L_R lrAdcInput, L_R_ADC_Audio_Input_Mixer mixer)
{
	if(lrAdcInput == ADC_LEFT)
	{
		adcInputReg.MXINL = mixer;
		tDataCodec[0] = MAX9867_REG_ADC_IN;
		tDataCodec[1] = adcInputReg.adcInputReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}

	else if(lrAdcInput == ADC_RIGHT)
	{
	adcInputReg.MXINR = mixer;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	}

	else if(lrAdcInput == ADC_LEFT_RIGHT)
	{
		adcInputReg.MXINL = mixer;
		tDataCodec[0] = MAX9867_REG_ADC_IN;
		tDataCodec[1] = adcInputReg.adcInputReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;

		adcInputReg.MXINR = mixer;
		tDataCodec[0] = MAX9867_REG_ADC_IN;
		tDataCodec[1] = adcInputReg.adcInputReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_LineInputEnableDisable(L_R_Line_Input_En_Dis lineInput, L_R_Line_Input lrLineInput)
{
	if(lrLineInput == LEFT_LINE_INPUT)
	{
		if(lineInput == LINE_INPUT_EN)
		{
			powerMangReg.LNLEN = LINE_INPUT_EN;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(lineInput == LINE_INPUT_DIS)
		{
			powerMangReg.LNLEN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
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
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(lineInput == LINE_INPUT_DIS)
		{
			powerMangReg.LNREN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
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
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;

			powerMangReg.LNREN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(lineInput == LINE_INPUT_DIS)
		{
			powerMangReg.LNLEN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;

			powerMangReg.LNREN = LINE_INPUT_DIS;
			tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
			tDataCodec[1] = powerMangReg.pwrManagReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;
		}
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_LineInputGain(L_R_Line_Input lineInput, L_R_Line_Input_Gain lineInputGain)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		lLineInReg.LIGL = lineInputGain;
		tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
		tDataCodec[1] = lLineInReg.lLineInReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == RIGHT_LINE_INPUT)
	{
		rLineInReg.LIGR = lineInputGain;
		tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
		tDataCodec[1] = rLineInReg.rLineInReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == LEFT_RIGHT_LINE_INPUT)
	{
		lLineInReg.LIGL = lineInputGain;
		rLineInReg.LIGR = lineInputGain;
		tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
		tDataCodec[1] = lLineInReg.lLineInReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
		tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
		tDataCodec[1] = rLineInReg.rLineInReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_LineInputMute(L_R_Line_Input lineInput, Line_Input_Mute_En_Dis mute)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		if(mute == LINE_INPUT_MUTE)
		{
			lLineInReg.LILM = LINE_INPUT_MUTE_EN;
			tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
			tDataCodec[1] = lLineInReg.lLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(mute == LINE_INPUT_UNMUTE)
		{
			lLineInReg.LILM = LINE_INPUT_MUTE_DIS;
			tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
			tDataCodec[1] = lLineInReg.lLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
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
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(mute == LINE_INPUT_UNMUTE)
		{
			rLineInReg.LIGR = LINE_INPUT_MUTE_DIS;
			tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
			tDataCodec[1] = rLineInReg.rLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
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
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;

			rLineInReg.LIGR = LINE_INPUT_MUTE_EN;
			tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
			tDataCodec[1] = rLineInReg.rLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;
		}
		else if(mute == LINE_INPUT_UNMUTE)
		{
			lLineInReg.LILM = LINE_INPUT_MUTE_DIS;
			tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
			tDataCodec[1] = lLineInReg.lLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;

			rLineInReg.LIGR = LINE_INPUT_MUTE_DIS;
			tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
			tDataCodec[1] = rLineInReg.rLineInReg;
			if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
					return STATUS_ERR;
		}
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_AudioLevel(L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol)
{
	if(channel == LEFT_VOLUME_CHA)
	{
		lVolumeCtrlReg.VOLL = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(channel == RIGHT_VOLUME_CHA)
	{
		rVolumeCtrlReg.VOLR = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(channel == LEFT_RIGHT_VOLUME_CHA)
	{
		rVolumeCtrlReg.VOLR = rPlaybackVol;
		lVolumeCtrlReg.VOLL = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_AudioMute(Audio_Mute audioMute)
{
	if(audioMute == AUDIO_MUTE_ENABLE)
	{
		rVolumeCtrlReg.VOLRM = AUDIO_MUTE_ENABLE;
		lVolumeCtrlReg.VOLLM = AUDIO_MUTE_ENABLE;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(audioMute == AUDIO_MUTE_DISABLE)
	{
		rVolumeCtrlReg.VOLRM = AUDIO_MUTE_DISABLE;
		lVolumeCtrlReg.VOLLM = AUDIO_MUTE_DISABLE;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_MicAmpGain(L_R_Mic mic, L_R_Mic_Preamp_Gain preAmpGain, L_R_Mic_Programble_Gain_Amp progGain)
{
	if(mic == MIC_LEFT)
	{
		lMicGainReg.PALEN = preAmpGain;
		lMicGainReg.PGAML = progGain;
		tDataCodec[0] = MAX9867_REG_L_MIC_GAIN;
		tDataCodec[1] = lMicGainReg.lMicGainReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}

	else if(mic == MIC_RIGHT)
	{
		rMicGainReg.PAREN = preAmpGain;
		rMicGainReg.PGAMR = progGain;
		tDataCodec[0] = MAX9867_REG_R_MIN_GAIN;
		tDataCodec[1] = rMicGainReg.rMicGainReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(mic == MIC_LEFT_RIGHT)
	{
		lMicGainReg.PALEN = preAmpGain;
		lMicGainReg.PGAML = progGain;
		tDataCodec[0] = MAX9867_REG_L_MIC_GAIN;
		tDataCodec[1] = lMicGainReg.lMicGainReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;

		rMicGainReg.PAREN = preAmpGain;
		rMicGainReg.PGAMR = progGain;
		tDataCodec[0] = MAX9867_REG_R_MIN_GAIN;
		tDataCodec[1] = rMicGainReg.rMicGainReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_MicSidetoneSourceAndGain(Digital_Sidetone_Source_Mixer sourceMixer, Sidetone_Gain_Diff_Headphone sidGainDiff
		, Sidetone_Gain_Capacitorless_Single_Ended_Headphone sidGainCapSinEnd, Amp_Type ampType)
{
	sidetoneReg.DSTS = sourceMixer;
	tDataCodec[0] = MAX9867_REG_SIDETONE;
	tDataCodec[1] = sidetoneReg.sidetoneReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	if(ampType == DIFFERENTIAL_AMP)
	{
		sidetoneReg.DVST = sidGainDiff;
		tDataCodec[0] = MAX9867_REG_SIDETONE;
		tDataCodec[1] = sidetoneReg.sidetoneReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(ampType == CAPACITORLESS_AMP || ampType == SINGLE_ENDED_AMP)
	{
		sidetoneReg.DVST = sidGainCapSinEnd;
		tDataCodec[0] = MAX9867_REG_SIDETONE;
		tDataCodec[1] = sidetoneReg.sidetoneReg;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_MicDigitalClock(Digital_Mic_Clk_Pre clock)
{
    digMicClkReg.MICCLK = clock;
	tDataCodec[0] = MAX9867_REG_MIC;
	tDataCodec[1] = digMicClkReg.digMicClkReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_MicDigitalLeftRightEnableDisable(Digital_Mic_Clk_Mode mode)
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
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_AuxiliaryInputCapture(Auxiliary_Input_Capture auxCapture)
{
	adcInputReg.AUXCAP = auxCapture;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_AuxiliaryInputGainCalibration(Auxiliary_Input_Gain_Calibration auxGain)
{
	adcInputReg.AUXGAIN = auxGain;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_AuxiliaryInputOffsetCalibration(Auxiliary_Input_Offset_Calibration auxOffset)
{
	adcInputReg.AUXCAL = auxOffset;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_AuxiliaryInputType(Auxiliary_Input_Type auxType)
{
	adcInputReg.AUXEN = auxType;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInputReg.adcInputReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_AuxiliaryRegRead(uint16_t *aux)
{
	tDataCodec[0] = MAX9867_REG_AUX_L;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 1) )
			return STATUS_ERR;
	if( STATUS_OK != ReadI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, &auxRegL.auxRegL, 1) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_AUX_H;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 1) )
			return STATUS_ERR;
	if( STATUS_OK != ReadI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, &auxRegH.auxRegH, 1) )
			return STATUS_ERR;

	*aux = ((auxRegH.auxRegH << 8) | auxRegL.auxRegL);
	return STATUS_OK;
}

Status_TypeDef MAX9867_JackSensEnableDisable(Jack_Sense_En_Dis jackSens)
{
	configModeReg.JDETEN = jackSens;
	tDataCodec[0] = MAX9867_REG_MODE;
	tDataCodec[1] = configModeReg.configModeReg;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

/********************************************************************************USER APIs*****************************************************************************************************************************/

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
		if( STATUS_OK != MAX9867_ADC_EnableDisable(ADC_ENABLE))
				return STATUS_ERR;
	}
	/* Enable left or right or both of them line-input */
	/* Note : if the right line-input is disabled and left line-input is enabled,
	 * the left line-input signal is routed to
	 * the left and right ADC input mixer and left and right headphone amplifier
	 */
	if( STATUS_OK != MAX9867_LineInputEnableDisable(LINE_INPUT_EN, lrLineInput))
			return STATUS_ERR;
	return STATUS_OK;
}

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
		if( STATUS_OK != MAX9867_DAC_EnableDisable(DAC_ENABLE))
				return STATUS_ERR;

	}
	/* Enable ADC */
	if( STATUS_OK != MAX9867_ADC_EnableDisable(ADC_ENABLE))
			return STATUS_ERR;
	return STATUS_OK;
}

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
	if( STATUS_OK != MAX9867_DAC_EnableDisable(DAC_ENABLE))
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef DcMeasurementInit(uint16_t *calibrationFactor)
{

	if( STATUS_OK != MAX9867_DigitalAudioInterfaceInit(MAX9867_SLAVE_MODE,
			LEFT_CHN_DATA_IN_OUT, SDIN_LATCHED_RISING_EDGE_BCLK, SDOUT_TRANS_AFTER_SDIN_LATCHED,
			SDIN_SDOUT_LATCHED_FIRST_BCLK_EDGE, SDOUT_HIGH_IMPEDANCE_AFTER_DATA_TRANS,
			LRCLK_INDICATE_L_R_AUDIO, OFF, SDIN_PROCESS_SEPARATELY,
			TRACKS_VOLL_VOLR_BITS))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_JackSensEnableDisable(JACKSNS_ENABLE))
			return STATUS_ERR;
	if( STATUS_OK != MAX9867_ADC_EnableDisable(ADC_ENABLE))
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

/*********************************************************************FOR H07R8 MODULE****************************************************************/

Status_TypeDef MAX9867_CodecInit(DAC_Level_Ctrl dacGain,L_R_Playback_Volume rPlaybackVol,L_R_Playback_Volume lPlaybackVol)
{
	if( STATUS_OK != MAX9867_Shoutdown(SHOUTDOWN_ENABLE))
		return STATUS_ERR;
	if( STATUS_OK != MAX9867_ClockControlInit(MCLK_BETWEEN_10_20_MHZ, NORMAL_OR_PLL_MODE,
	  		PLL_DISABLE, 0, 0))
		return STATUS_ERR;
	if( STATUS_OK != MAX9867_DigitalAudioInterfaceInit(MAX9867_SLAVE_MODE,
			  LEFT_CHN_DATA_IN_OUT, SDIN_LATCHED_RISING_EDGE_BCLK, SDOUT_TRANS_AFTER_SDIN_LATCHED,
			  SDIN_SDOUT_LATCHED_SECOND_BCLK_EDGE, SDOUT_HIGH_IMPEDANCE_AFTER_DATA_TRANS,
			  LRCLK_INDICATE_L_R_AUDIO, OFF, SDIN_PROCESS_SEPARATELY,
			  TRACKS_VOLL_VOLR_BITS))
		return STATUS_ERR;
	ReadingDigitalAudioInit(AUDIO, LEFT_RIGHT_VOLUME_CHA, DAC_GAIN_0dB, dacGain,
			TYPE1, rPlaybackVol, lPlaybackVol);
	if( STATUS_OK != MAX9867_HeadphoneAmpType(STEREO_DIFF_CLICKLESS))
		return STATUS_ERR;
	if( STATUS_OK != MAX9867_Shoutdown(SHOUTDOWN_DISABLE))
		return STATUS_ERR;
	 return STATUS_OK;
}

Status_TypeDef SendingDigitalAudio(uint16_t *data,size_t size)
{
    for(int i=0; i<size; i++) {
    	if( STATUS_OK != WriteI2S(MAX9867_I2S_HANDLE, &data[i], 1))
    		return STATUS_ERR;
    }
    return STATUS_OK;
}

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
