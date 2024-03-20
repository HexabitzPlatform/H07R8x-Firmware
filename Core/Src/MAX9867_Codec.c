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


static uint8_t tDataCodec[2];
static uint8_t rDataCodec[2];

/* I2C functions */
/*
 * @brief: write n bytes to an I2C register
 * @param1: I2C port handle
 * @param2: Device address to be written
 * @param3: Data to be sent
 * @param4: Data size
 * @retval: Status
 */
Status_TypeDef WriteI2C(I2C_HANDLE *xPort, uint16_t sAddress, uint8_t *pData, uint16_t Size)
{
  Status_TypeDef Status=STATUS_ERR;

  if (NULL!=xPort && NULL!=pData)
    {
      if (HAL_OK == HAL_I2C_Master_Transmit(xPort, (uint16_t) sAddress, pData, Size, TIM_OUT_1MS))
      Status=STATUS_OK;

    }
  else
    Status=STATUS_ERR;

  return Status;
}

/*
 * @brief: read n bytes from an I2C device
 * @param1: I2C port handle
 * @param2: Device address to be read
 * @param3: Pointer to buffer to store received data in
 * @param4: Data size in bytes unit
 * @retval: Status
 */
Status_TypeDef ReadI2C(I2C_HANDLE *xPort, uint16_t sAddress, uint8_t *rBuffer, uint16_t Size)
{
	Status_TypeDef Status;

	if (NULL!=xPort && NULL!=rBuffer)
	{
	    if (HAL_OK == HAL_I2C_Master_Receive(xPort,  (uint16_t) sAddress, rBuffer, Size, TIM_OUT_1MS))
		Status=STATUS_OK;
	}
	else
		Status=STATUS_ERR;

	return Status;
}

Status_TypeDef MAX9867_Init(void)
{
	/* Enter codec in shutdown mode */
	tDataCodec[0] = MAX9867_REG_STATUS;
	tDataCodec[1] = 0x00;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	/* initialize digital audio interface */
	tDataCodec[0] = MAX9867_REG_STATUS;
	tDataCodec[1] = 0x10;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	return STATUS_OK;
}

Digital_Audio_Interface_Reg_1 digAudReg1;
Digital_Audio_Interface_Reg_2 digAudReg2;

Status_TypeDef DigitalAudioInterfaceInit(MAX9867_Master_Slave_Mode mode,
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
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_INTERFACE_MODE2;
	tDataCodec[1] = digAudReg2.digAudReg2;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

System_Clock_Reg sysClk;
Stereo_Audio_Clock_Control_High_Reg audioClkH;
Stereo_Audio_Clock_Control_Low_Reg audioClkL;

Status_TypeDef ClockControlInit(MCLK_Prescaler mclkPresclr, Exact_Integer_Modes exactIntMode,
		PLL_Mode_En_Dis pllMode, uint32_t NI, bool NI0)
{
	sysClk.PSCLK = mclkPresclr;
	sysClk.FREQ = exactIntMode;

	audioClkH.PLL = pllMode;
	audioClkH.NI = NI >> 8;

	audioClkL.NI0 = NI0;
	audioClkL.NI = NI;

	tDataCodec[0] = MAX9867_REG_SYSTEM_CLK;
	tDataCodec[1] = sysClk.sysClk;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_H;
	tDataCodec[1] = stereoAudClkH.stereoAudClkH;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	tDataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_L;
	tDataCodec[1] = stereoAudClkL.stereoAudClkL;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;

	return STATUS_OK;
}

Interrupt_Reg interruptEn;
Status_TypeDef InterruptEnable(bool clipDetect,bool slewDetect,bool pllUnlock,bool headsetChange)
{
	interruptEn.ICLD = clipDetect;
	interruptEn.ISLD = slewDetect;
	interruptEn.IULK = pllUnlock;
	interruptEn.IJDET = headsetChange;

	tDataCodec[0] = MAX9867_REG_INT_ENA;
	tDataCodec[1] = interruptEn.interruptEn;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_DAC_Gain(DAC_Gain firstAmp, DAC_Level_Ctrl progAmp)
{
	DAC_Level_Ctrl_Reg DAC_Level = {.DACG = firstAmp,.DACA = progAmp};
	tDataCodec[0] = MAX9867_REG_DAC_LVL;
	tDataCodec[1] = DAC_Level.DACLvl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef MAX9867_AudioLevel(L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol)
{
	Right_Volume_Ctrl_Reg rVolumeCtrl = {.VOLR = rPlaybackVol};
	Left_Volume_Ctrl_Reg  lVolumeCtrl = {.VOLL = rPlaybackVol};
	tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
	tDataCodec[1] = rVolumeCtrl.RVolCtrl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
	tDataCodec[1] = lVolumeCtrl.LVolCtrl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Right_Volume_Ctrl_Reg rVolumeCtrl;
Left_Volume_Ctrl_Reg  lVolumeCtrl;
Status_TypeDef AudioMute(void)
{
	rVolumeCtrl.VOLRM = MUTE_ENABLE;
	lVolumeCtrl.VOLLM = MUTE_ENABLE;
	tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
	tDataCodec[1] = rVolumeCtrl.RVolCtrl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
	tDataCodec[1] = lVolumeCtrl.LVolCtrl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}
Status_TypeDef AudioUnmute(void)
{
	rVolumeCtrl.VOLRM = MUTE_DISABLE;
	lVolumeCtrl.VOLLM = MUTE_DISABLE;
	tDataCodec[0] = MAX9867_REG_L_VOL_CTRL;
	tDataCodec[1] = rVolumeCtrl.RVolCtrl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	tDataCodec[0] = MAX9867_REG_R_VOL_CTRL;
	tDataCodec[1] = lVolumeCtrl.LVolCtrl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

static Status_TypeDef DigitalFilterInit(Digital_Audio_Filter_Mode mode, ADC_DAC_Digital_Audio_Filter_Sٍpecifications ADC_Specifications,ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications)
{

	Digital_Filtering_Reg codecFilter = {.MODE = mode, .AVFLT = ADC_Specifications, .DVFLT = DAC_Specifications};
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = codecFilter.digtlFltr;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef DAC_Enable(void)
{
	Power_Management_Reg powerMang = {.DALEN = DAC_ENABLE, .DAREN = DAC_ENABLE};
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = powerMang.pwrManag;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef DAC_Disable(void)
{
	Power_Management_Reg powerMang = {.DALEN = DAC_DISABLE, .DAREN = DAC_DISABLE};
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = powerMang.pwrManag;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}
DAC_Level_Ctrl_Reg DACLvl;
Status_TypeDef DAC_Mute(DAC_Mute_En_Dis dacMute)
{
	DACLvl.DACM = dacMute;
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = DACLvl.DACLvl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef HeadphoneAmpMode(Headphone_Amp_Mode ampMode)
{
	Mode_Configuration_Reg configMode = {.HPMODE = ampMode};
	tDataCodec[0] = MAX9867_REG_MODE;
	tDataCodec[1] = configMode.confMode;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef LeftMicAmpGain(L_R_Mic_Preamp_Gain preAmpGain, L_R_Mic_Programble_Gain_Amp progGain)
{
	Left_Mic_Gain_Reg lMicGain = {.PALEN = preAmpGain, .PGAML = progGain};
	tDataCodec[0] = MAX9867_REG_L_MIC_GAIN;
	tDataCodec[1] = lMicGain.lMicGain;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef RightMicAmpGain(L_R_Mic_Preamp_Gain preAmpGain, L_R_Mic_Programble_Gain_Amp progGain)
{
	Right_Mic_Gain_Reg rMicGain = {.PAREN = preAmpGain, .PGAMR = progGain};
	tDataCodec[0] = MAX9867_REG_R_MIN_GAIN;
	tDataCodec[1] = rMicGain.rMicGain;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

static Status_TypeDef LADC_AduioInputMixer(L_R_ADC_Audio_Input_Mixer lMixer)
{
	ADC_Input_Reg adcInput = {.MXINL = lMixer};
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInput.adcInput;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

static Status_TypeDef RADC_AduioInputMixer(L_R_ADC_Audio_Input_Mixer rMixer)
{
	ADC_Input_Reg adcInput = {.MXINR = rMixer};
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInput.adcInput;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef ADC_Enable()
{
	Power_Management_Reg powerMang = {.ADLEN = ADC_ENABLE, .ADREN = ADC_ENABLE};
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = powerMang.pwrManag;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef ADC_Disable()
{
	Power_Management_Reg powerMang = {.ADLEN = ADC_DISABLE, .ADREN = ADC_DISABLE};
	tDataCodec[0] = MAX9867_REG_CODEC_FILTERS;
	tDataCodec[1] = powerMang.pwrManag;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef LADC_Gain(L_R_ADC_Level_Ctrl ladcGain)
{
	DAC_Level_Ctrl_Reg ADCLvl = {.AVL = ladcGain};
	tDataCodec[0] = MAX9867_REG_DAC_LVL;
	tDataCodec[1] = DAC_Level.DACLvl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef RADC_Gain(L_R_ADC_Level_Ctrl radcGain)
{
	DAC_Level_Ctrl_Reg ADCLvl = {.AVR = radcGain};
	tDataCodec[0] = MAX9867_REG_DAC_LVL;
	tDataCodec[1] = DAC_Level.DACLvl;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef SidetoneSourceAndGain(Digital_Sidetone_Source_Mixer sourceMixer, Sidetone_Gain_Diff_Headphone sidGainDiff
		, Sidetone_Gain_Capacitorless_Single_Ended_Headphone sidGainCapSinEnd, Amp_Type ampType)
{
	Sidetone_Reg sidetone = {.DSTS = sourceMixer};
	tDataCodec[0] = MAX9867_REG_SIDETONE;
	tDataCodec[1] = sidetone.sidetone;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	if(ampType == DIFFERENTIAL_AMP)
	{
		Sidetone_Reg sidetone = {.DVST = sidGainDiff};
		tDataCodec[0] = MAX9867_REG_SIDETONE;
		tDataCodec[1] = sidetone.sidetone;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(ampType == CAPACITORLESS_AMP || ampType == SINGLE_ENDED_AMP)
	{
		Sidetone_Reg sidetone = {.DVST = sidGainCapSinEnd};
		tDataCodec[0] = MAX9867_REG_SIDETONE;
		tDataCodec[1] = sidetone.sidetone;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef LineInputMute(L_R_Line_Input lineInput)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		Left_Line_input_Level_Reg lLineIn = {.LILM = LINE_INPUT_MUTE_EN};
		tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
		tDataCodec[1] = lLineIn.lLineIn;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == RIGHT_LINE_INPUT)
	{
		Right_Line_input_Level_Reg rLineIn = {.LIGR = LINE_INPUT_MUTE_EN};
		tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
		tDataCodec[1] = rLineIn.rLineIn;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef LineInputUnmute(L_R_Line_Input lineInput)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		Left_Line_input_Level_Reg lLineIn = {.LILM = LINE_INPUT_MUTE_DIS};
		tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
		tDataCodec[1] = lLineIn.lLineIn;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == RIGHT_LINE_INPUT)
	{
		Right_Line_input_Level_Reg rLineIn = {.LIGR = LINE_INPUT_MUTE_DIS};
		tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
		tDataCodec[1] = rLineIn.rLineIn;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef LineInputGain(L_R_Line_Input lineInput, L_R_Line_Input_Gain lineInputGain)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		Left_Line_input_Level_Reg lLineIn = {.LIGL = lineInputGain};
		tDataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
		tDataCodec[1] = lLineIn.lLineIn;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == RIGHT_LINE_INPUT)
	{
		Right_Line_input_Level_Reg rLineIn = {.LIGR = lineInputGain};
		tDataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
		tDataCodec[1] = rLineIn.rLineIn;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef LineIputEnable(L_R_Line_Input lineInput)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		Power_Management_Reg powerMang = {.LNLEN = LINE_INPUT_EN};
		tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
		tDataCodec[1] = powerMang.pwrManag;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == RIGHT_LINE_INPUT)
	{
		Power_Management_Reg powerMang = {.LNREN = LINE_INPUT_EN};
		tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
		tDataCodec[1] = powerMang.pwrManag;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef LineIputDisable(L_R_Line_Input lineInput)
{
	if(lineInput == LEFT_LINE_INPUT)
	{
		Power_Management_Reg powerMang = {.LNLEN = LINE_INPUT_DIS};
		tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
		tDataCodec[1] = powerMang.pwrManag;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	else if(lineInput == RIGHT_LINE_INPUT)
	{
		Power_Management_Reg powerMang = {.LNREN = LINE_INPUT_DIS};
		tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
		tDataCodec[1] = powerMang.pwrManag;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef MAX9867_Shoutdown(Shoutdown shtdown)
{
	Power_Management_Reg powerMang = {.SHDN = shtdown};
	tDataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
	tDataCodec[1] = powerMang.pwrManag;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef DigitalMicClock(Digital_Mic_Clk clock)
{
	Digital_Microphone_Input_Reg digMicClk = {.MICCLK = clock};
	tDataCodec[0] = MAX9867_REG_MIC;
	tDataCodec[1] = digMicClk.digMicClk;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef LR_DigitalMicMode(Digital_Mic_Clk mode)
{
	Digital_Microphone_Input_Reg digMicClk;
	switch(mode)
	{
		case 0:
			digMicClk.DIGMICL = 0;
			digMicClk.DIGMICR = 0;
			break;
		case 1:
			digMicClk.DIGMICL = 0;
			digMicClk.DIGMICR = 1;
			break;
		case 2:
			digMicClk.DIGMICL = 1;
			digMicClk.DIGMICR = 0;
			break;
		case 3:
			digMicClk.DIGMICL = 1;
			digMicClk.DIGMICR = 1;
			break;
		default:
			digMicClk.DIGMICL = 0;
			digMicClk.DIGMICR = 0;
	}
	tDataCodec[0] = MAX9867_REG_MIC;
	tDataCodec[1] = digMicClk.digMicClk;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

ADC_Input_Reg adcInput;

Status_TypeDef AuxiliaryInputCapture(Auxiliary_Input_Capture auxCapture)
{
	adcInput.AUXCAP = auxCapture;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInput.adcInput;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef AuxiliaryInputGainCalibration(Auxiliary_Input_Gain_Calibration auxGain)
{
	adcInput.AUXGAIN = auxGain;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInput.adcInput;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef AuxiliaryInputOffsetCalibration(Auxiliary_Input_Offset_Calibration auxOffset)
{
	adcInput.AUXCAL = auxOffset;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInput.adcInput;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef AuxiliaryInputEnable(Auxiliary_Input_En_Dis auxEna)
{
	adcInput.AUXEN = auxEna;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInput.adcInput;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}

Status_TypeDef AuxiliaryInputDisable(Auxiliary_Input_En_Dis auxDis)
{
	adcInput.AUXEN = auxDis;
	tDataCodec[0] = MAX9867_REG_ADC_IN;
	tDataCodec[1] = adcInput.adcInput;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
			return STATUS_ERR;
	return STATUS_OK;
}
