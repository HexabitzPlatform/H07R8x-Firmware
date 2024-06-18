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

Codec_Status MAX9867_DigitalAudioInterfaceInit(void)
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
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;

	tDataCodec[0] = MAX9867_REG_INTERFACE_MODE2;
	tDataCodec[1] = digAudReg2.digAudReg2;
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;
	return CODEC_OK;
}

/****************************************************************************************************/

Codec_Status MAX9867_DigitalFilterInit(Digital_Audio_Filter_Mode mode, ADC_DAC_Digital_Audio_Filter_Sٍpecifications ADC_Specifications,ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications)
{

	codecFilterReg.MODE = mode;
	codecFilterReg.AVFLT = ADC_Specifications;
	codecFilterReg.DVFLT = DAC_Specifications;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = codecFilterReg.digtlFltrReg;
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;
	return CODEC_OK;
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

Codec_Status MAX9867_ClockControlInit(MCLK_Prescaler mclkPresclr, Exact_Integer_Modes exactIntMode,
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
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;

	tDataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_H;
	tDataCodec[1] = stereoAudClkRegH.stereoAudClkRegH;
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;

	tDataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_L;
	tDataCodec[1] = stereoAudClkRegL.stereoAudClkRegL;
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;

	return CODEC_OK;
}

/****************************************************************************************************/

/* Shoutdown Enable-Disable */
/*
 * @brief  :Shoutdown Enable-Disable.
 * @param1 :Shoutdown Enable-Disable.
 * @retval :Status
 */

Codec_Status MAX9867_Shoutdown(Shoutdown shtdown)
{
	powerMangReg.SHDN = shtdown;
	tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
	tDataCodec[1] = powerMangReg.pwrManagReg;
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;
	return CODEC_OK;
}

/****************************************************************************************************/

/* Headphone Amplifier type */
/*
 * @brief  :Headphone has three types(differential,capacitorless,single-indedd)
 * 		    and these types either to be mono or stereo.
 * @param1 :Headphone Amplifier type.
 * @retval :Status
 */

Codec_Status MAX9867_HeadphoneAmpType(Headphone_Amp_Type ampMode)
{
	configModeReg.HPMODE = ampMode;
	tDataCodec[0] = MAX9867_REG_MODE;
	tDataCodec[1] = configModeReg.configModeReg;
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;
	return CODEC_OK;
}

/****************************************************************************************************/

/* DAC Enable-Disable */
/*
 * @brief  :DAC Enable-Disable to convert digital audio data to analog signal.
 * @param1 :DAC Enable-Disable.
 * @retval :Status
 */

Codec_Status MAX9867_DAC_EnableDisable(DAC_En_Dis dac)
{
		powerMangReg.DALEN = dac;
		powerMangReg.DAREN = dac;
		tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
		tDataCodec[1] = powerMangReg.pwrManagReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
	return CODEC_OK;
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

Codec_Status MAX9867_DAC_Gain(DAC_Gain firstAmp, DAC_Level_Ctrl progAmp)
{
	DAC_LevelReg.DACG = firstAmp;
	DAC_LevelReg.DACA = progAmp;
	tDataCodec[0] = MAX9867_REG_DAC_LVL;
	tDataCodec[1] = DAC_LevelReg.DACLvlReg;
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;
	return CODEC_OK;
}

/****************************************************************************************************/

/* DAC Mute */
/*
 * @brief  :DAC Mute.
 * @param1 :DAC Mute Enable-Disable.
 * @retval :Status
 */

Codec_Status MAX9867_DAC_Mute(DAC_Mute_En_Dis dacMute)
{
	DAC_LevelReg.DACM = dacMute;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = DAC_LevelReg.DACLvlReg;
	if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return CODEC_ERR;
	return CODEC_OK;
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

Codec_Status ReadingDigitalAudioInit(Digital_Audio_Mode audioMode, L_R_Playback_Volume_Channel channel, DAC_Gain firstAmp, DAC_Level_Ctrl progAmp,
		ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol)
{
	if(audioMode == AUDIO)
	{
		/* in audio application we must set only programmable amplifier, and set preamplifier to 0 because it
		 * especial with microphone voice gain */
		if( CODEC_OK != MAX9867_DAC_Gain(DAC_GAIN_0dB, progAmp))
				return CODEC_ERR;
		/* Enable DC-blocking just in Audio mode */
		/* Note : we can enable DC-blocking by set AVFLT and DVFLT with any value */
		if( CODEC_OK != MAX9867_DigitalFilterInit(FIR_AUDIO_FILTER,DISABLED,DAC_Specifications))
				return CODEC_ERR;
	}
	else if(audioMode == VOICE)
	{

		if( CODEC_OK != MAX9867_DAC_Gain(firstAmp, progAmp))
				return CODEC_ERR;
		/* Enable DC-blocking just in Audio mode */
		/* Note : we can enable DC-blocking by set AVFLT and DVFLT with any value */
		if( CODEC_OK != MAX9867_DigitalFilterInit(IIR_VOICE_FILTER,DISABLED,DAC_Specifications))
				return CODEC_ERR;
	}
	/* Audio level control */
	if( CODEC_OK != MAX9867_AudioLevel(channel, rPlaybackVol, lPlaybackVol))
			return CODEC_ERR;
	/* Enable DAC */
	if( CODEC_OK != MAX9867_DAC_EnableDisable(DAC_ENABLE))
			return CODEC_ERR;
	return CODEC_OK;
}

/********************************************************************************USER APIs*****************************************************************************************************************************/

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

Codec_Status MAX9867_CodecInit(DAC_Level_Ctrl dacGain,L_R_Playback_Volume rPlaybackVol,L_R_Playback_Volume lPlaybackVol)
{
	/* ENABLE Codec */
	if( CODEC_OK != MAX9867_Shoutdown(SHOUTDOWN_DISABLE))
		return CODEC_ERR;
	/* Setting the clock signals for Codec where:
	 * - MCLK_BETWEEN_10_20_MHZ : determine the master clock frequency coming from processor to codec.
	 * - NORMAL_OR_PLL_MODE : Normal mode.
	 * - PLL_DISABLE : see datasheet.
	 * - 0x6000 : determine the LRCLK frequency coming from processor to codec
	 * - 0 : disable rapid lock mode because of PLL is disable
	 * */
	if( CODEC_OK != MAX9867_ClockControlInit(MCLK_BETWEEN_10_20_MHZ, NORMAL_OR_PLL_MODE,
	  		PLL_DISABLE, 0x6000, 0))
		return CODEC_ERR;
	/* Setting the Digital Audio Interface which responsible for manage the codec */
	if( CODEC_OK != MAX9867_DigitalAudioInterfaceInit())
		return CODEC_ERR;
	/* setting the digital audio path in the codec where :
	 * - AUDIO : determine the sound is audio not voice.
	 * - LEFT_RIGHT_VOLUME_CHA : Enable left/right audio channel.
	 * - DAC_GAIN_0dB : disable preamplifier,this amplifier just use in status the sound is voice.
	 * - dacGain : setting the programmable amplifier by user.
	 * - TYPE1 : this filter just for voice,it isn't necessary in status the sound is audio.
	 * - rPlaybackVol : setting the level sound for the right channel by the user.
	 * - lPlaybackVol : setting the level sound for the left channel by the user.
	 * */
	if(CODEC_OK != ReadingDigitalAudioInit(AUDIO, LEFT_RIGHT_VOLUME_CHA, DAC_GAIN_0dB, dacGain,
			TYPE1, rPlaybackVol, lPlaybackVol))
		return CODEC_ERR;
	/* determine the headphone amplifier type in our status it is differential */
	if( CODEC_OK != MAX9867_HeadphoneAmpType(STEREO_DIFF_CLICKLESS))
		return CODEC_ERR;
	 return CODEC_OK;
}

/****************************************************************************************************/

/* Set DAC Gain for play digital audio */
/*
 * @brief  :Set DAC Gain for play digital audio.
 * @param2 :programmable amplifier gain(voice and audio stream).
 * @retval :Status
 */

Codec_Status MAX9867_Codec_DAC_Gain(DAC_Level_Ctrl progAmp)
{
	if( CODEC_OK != MAX9867_DAC_Gain(DAC_GAIN_0dB, progAmp))
			return CODEC_ERR;
	return CODEC_OK;
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

Codec_Status MAX9867_AudioLevel(L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol)
{
	if(channel == LEFT_VOLUME_CHA)
	{
		lVolumeCtrlReg.VOLL = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
	}
	else if(channel == RIGHT_VOLUME_CHA)
	{
		rVolumeCtrlReg.VOLR = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
	}
	else if(channel == LEFT_RIGHT_VOLUME_CHA)
	{
		rVolumeCtrlReg.VOLR = rPlaybackVol;
		lVolumeCtrlReg.VOLL = rPlaybackVol;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
	}
	return CODEC_OK;
}

/****************************************************************************************************/

/* Audio Mute Enable-Disable */
/*
 * @brief  :Audio Mute Enable-Disable.
 * @param1 :Audio Mute Enable-Disable.
 * @retval :Status
 */

Codec_Status MAX9867_AudioMute(Audio_Mute audioMute)
{
	if(audioMute == AUDIO_MUTE_ENABLE)
	{
		rVolumeCtrlReg.VOLRM = AUDIO_MUTE_ENABLE;
		lVolumeCtrlReg.VOLLM = AUDIO_MUTE_ENABLE;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
	}
	else if(audioMute == AUDIO_MUTE_DISABLE)
	{
		rVolumeCtrlReg.VOLRM = AUDIO_MUTE_DISABLE;
		lVolumeCtrlReg.VOLLM = AUDIO_MUTE_DISABLE;
		tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
		tDataCodec[1] = rVolumeCtrlReg.RVolCtrlReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
		tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
		tDataCodec[1] = lVolumeCtrlReg.LVolCtrlReg;
		if( CODEC_OK != WriteI2C(I2C_PORT, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return CODEC_ERR;
	}
	return CODEC_OK;
}

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
