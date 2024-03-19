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
	else if(ampType == CAPACITORLESS_AMP || Amp_Type == SINGLE_ENDED_AMP)
	{
		Sidetone_Reg sidetone = {.DVST = sidGainCapSinEnd};
		tDataCodec[0] = MAX9867_REG_SIDETONE;
		tDataCodec[1] = sidetone.sidetone;
		if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, tDataCodec, 2) )
				return STATUS_ERR;
	}
	return STATUS_OK;
}

Status_TypeDef LineInputMute(L_R_Line_Input lineInput, L_R_Line_Input_Mute_En_Dis mute)
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

Status_TypeDef LineInputUnmute(L_R_Line_Input lineInput, L_R_Line_Input_Mute_En_Dis mute)
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

Status_TypeDef
