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

#ifndef INC_MAX9867_CODEC_H_
#define INC_MAX9867_CODEC_H_

#include "main.h"

extern I2C_HandleTypeDef hi2c2;
typedef I2C_HandleTypeDef 			I2C_HANDLE;
#define TIM_OUT_1MS         50
#define MAX9867_I2C_HANDLE            		&hi2c2
#define MAX9867_SLAVE_ADDRESS_W            	0x30
#define MAX9867_SLAVE_ADDRESS_R				0x31
/* MAX9867 registers addresses */
#define MAX9867_REG_STATUS					0x00
#define MAX9867_REG_JACK_SENSE				0x01
#define MAX9867_REG_AUX_H					0x02
#define MAX9867_REG_AUX_L					0x03
#define MAX9867_REG_INT_ENA					0x04
#define MAX9867_REG_SYSTEM_CLK				0x05
#define MAX9867_REG_STEREO_AUD_CLK_CTRL_H	0x06
#define MAX9867_REG_STEREO_AUD_CLK_CTRL_L	0x07
#define MAX9867_REG_INTERFACE_MODE1			0x08
#define MAX9867_REG_INTERFACE_MODE2			0x09
#define MAX9867_REG_CODEC_FILTERS			0x0a
#define MAX9867_REG_SIDETONE				0x0b
#define MAX9867_REG_DAC_LVL					0x0c
#define MAX9867_REG_ADC_LVL					0x0d
#define MAX9867_REG_L_LINE_INPUT_LVL		0x0e
#define MAX9867_REG_R_LINE_INPUT_LVL		0x0f
#define MAX9867_REG_L_VOL_CTRL				0x10
#define MAX9867_REG_R_VOL_CTRL				0x11
#define MAX9867_REG_L_MIC_GAIN				0x12
#define MAX9867_REG_R_MIN_GAIN				0x13
#define MAX9867_REG_ADC_IN					0x14
#define MAX9867_REG_MIC						0x15
#define MAX9867_REG_MODE					0x16
#define MAX9867_REG_SYS_SHUTDOWN			0x17

typedef enum
{
	MUTE_DISABLE,
	MUTE_ENABLE
} Audio_Mute;

typedef enum
{
	DAC_DISABLE,
	DAC_ENABLE
} DAC_En_Dis;

typedef enum
{
	ADC_DISABLE,
	ADC_ENABLE
} ADC_En_Dis;

typedef enum
{
  GAIN_0dB,
  GAIN_6dB,
  GAIN_12dB,
  GAIN_18dB
} DAC_Gain;

typedef enum
{
  STEREO_DIFF_CLICKLESS,
  MONO_L_DIFF_CLICKLESS,
  STEREO_CAPLESS_CLICKLESS,
  MONO_L_CAPLESS_CLICKLESS,
  STEREO_SINGLE_ENDED_CLICKLESS,
  MONO_L_SINGLE_ENDED_CLICKLESS,
  STEREO_SINGLE_ENDED_FAST_TURN_ON,
  MONO_L_SINGLE_ENDED_FAST_TURN_ON
} Headphone_Amp_Mode;

typedef enum
{
  GAIN_PLUS_0dB,
  GAIN_MINUS_1dB,
  GAIN_MINUS_2dB,
  GAIN_MINUS_3dB,
  GAIN_MINUS_4dB,
  GAIN_MINUS_5dB,
  GAIN_MINUS_6dB,
  GAIN_MINUS_7dB,
  GAIN_MINUS_8dB,
  GAIN_MINUS_9dB,
  GAIN_MINUS_10dB,
  GAIN_MINUS_11dB,
  GAIN_MINUS_12dB,
  GAIN_MINUS_13dB,
  GAIN_MINUS_14dB,
  GAIN_MINUS_15dB
} DAC_Level_Ctrl;

typedef enum
{
  GAIN_PLUS_6dB,
  GAIN_PLUS_5d5B,
  GAIN_PLUS_5dB,
  GAIN_PLUS_4d5B,
  GAIN_PLUS_4dB,
  GAIN_PLUS_3d5B,
  GAIN_PLUS_3dB,
  GAIN_PLUS_2dB,
  GAIN_PLUS_1dB,
  GAIN_PLUS_0dB,
  GAIN_MINUS_1dB,
  GAIN_MINUS_2dB,
  GAIN_MINUS_3dB,
  GAIN_MINUS_4dB,
  GAIN_MINUS_5dB,
  GAIN_MINUS_6dB,
  GAIN_MINUS_8dB,
  GAIN_MINUS_10dB,
  GAIN_MINUS_12dB,
  GAIN_MINUS_14dB,
  GAIN_MINUS_16dB,
  GAIN_MINUS_18dB,
  GAIN_MINUS_20dB,
  GAIN_MINUS_22dB,
  GAIN_MINUS_24dB,
  GAIN_MINUS_26dB,
  GAIN_MINUS_30dB,
  GAIN_MINUS_34dB,
  GAIN_MINUS_38dB,
} L_R_Playback_Volume;

typedef enum
{
  IIR_VOICE_FILTER,
  FIR_AUDIO_FILTER
} Digital_Audio_Filter_Mode;

typedef enum
{
  IIR_VOICE_FILTER,
  FIR_AUDIO_FILTER
} Digital_Audio_Filter_Mode;

typedef enum
{
  DISABLED,
  TYPE1,
  TYPE2,
  TYPE3,
  TYPE4,
  TYPE5
} ADC_DAC_Digital_Audio_Filter_Sٍpecifications;

typedef enum
{
  DISABLED,
  GAIN_0dB,
  GAIN_PLUS_20dB,
  GAIN_PLUS_30dB
} L_R_Mic_Preamp_Gain;

typedef enum
{
  GAIN_PLUS_20dB,
  GAIN_PLUS_19dB,
  GAIN_PLUS_18dB,
  GAIN_PLUS_17dB,
  GAIN_PLUS_16dB,
  GAIN_PLUS_15dB,
  GAIN_PLUS_14dB,
  GAIN_PLUS_13dB,
  GAIN_PLUS_12dB,
  GAIN_PLUS_11dB,
  GAIN_PLUS_10dB,
  GAIN_PLUS_9dB,
  GAIN_PLUS_8dB,
  GAIN_PLUS_7dB,
  GAIN_PLUS_6dB,
  GAIN_PLUS_5dB,
  GAIN_PLUS_4dB,
  GAIN_PLUS_3dB,
  GAIN_PLUS_2dB,
  GAIN_PLUS_1dB,
  GAIN_PLUS_0dB
} L_R_Mic_Programble_Gain_Amp;

typedef enum
{
  GAIN_PLUS_0dB,
  GAIN_MINUS_1dB,
  GAIN_MINUS_2dB,
  GAIN_MINUS_3dB,
  GAIN_MINUS_4dB,
  GAIN_MINUS_5dB,
  GAIN_MINUS_6dB,
  GAIN_MINUS_7dB,
  GAIN_MINUS_8dB,
  GAIN_MINUS_9dB,
  GAIN_MINUS_10dB,
  GAIN_MINUS_11dB,
  GAIN_MINUS_12dB,
  GAIN_MINUS_13dB,
  GAIN_MINUS_14dB,
  GAIN_MINUS_15dB
} L_R_ADC_Level_Ctrl;

typedef enum
{
  OFF,
  GAIN_PLUS_0dB,
  GAIN_MINUS_2dB,
  GAIN_MINUS_4dB,
  GAIN_MINUS_6dB,
  GAIN_MINUS_8dB,
  GAIN_MINUS_10dB,
  GAIN_MINUS_12dB,
  GAIN_MINUS_14dB,
  GAIN_MINUS_16dB,
  GAIN_MINUS_18dB,
  GAIN_MINUS_20dB,
  GAIN_MINUS_22dB,
  GAIN_MINUS_24dB,
  GAIN_MINUS_26dB,
  GAIN_MINUS_28dB,
  GAIN_MINUS_30dB,
  GAIN_MINUS_32dB,
  GAIN_MINUS_34dB,
  GAIN_MINUS_36dB,
  GAIN_MINUS_38dB,
  GAIN_MINUS_40dB,
  GAIN_MINUS_42dB,
  GAIN_MINUS_44dB,
  GAIN_MINUS_46dB,
  GAIN_MINUS_48dB,
  GAIN_MINUS_50dB,
  GAIN_MINUS_52dB,
  GAIN_MINUS_54dB,
  GAIN_MINUS_56dB,
  GAIN_MINUS_58dB,
  GAIN_MINUS_60dB
} Sidetone_Gain_Diff_Headphone;

typedef enum
{
  OFF,
  GAIN_MINUS_5dB,
  GAIN_MINUS_7dB,
  GAIN_MINUS_9dB,
  GAIN_MINUS_11dB,
  GAIN_MINUS_13dB,
  GAIN_MINUS_15dB,
  GAIN_MINUS_17dB,
  GAIN_MINUS_19dB,
  GAIN_MINUS_21dB,
  GAIN_MINUS_23dB,
  GAIN_MINUS_25dB,
  GAIN_MINUS_27dB,
  GAIN_MINUS_29dB,
  GAIN_MINUS_31dB,
  GAIN_MINUS_33dB,
  GAIN_MINUS_35dB,
  GAIN_MINUS_37dB,
  GAIN_MINUS_39dB,
  GAIN_MINUS_41dB,
  GAIN_MINUS_43dB,
  GAIN_MINUS_45dB,
  GAIN_MINUS_47dB,
  GAIN_MINUS_49dB,
  GAIN_MINUS_51dB,
  GAIN_MINUS_53dB,
  GAIN_MINUS_55dB,
  GAIN_MINUS_57dB,
  GAIN_MINUS_59dB,
  GAIN_MINUS_61dB,
  GAIN_MINUS_63dB,
  GAIN_MINUS_65dB
} Sidetone_Gain_Capacitorless_Single_Ended_Headphone;

typedef enum
{
	DIFFERENTIAL_AMP,
	CAPACITORLESS_AMP,
	SINGLE_ENDED_AMP
}Amp_Type;

typedef enum
{
	NO_SIDETONE_SELECTED,
	L_ADC,
	R_ADC,
	L_R_ADC
} Digital_Sidetone_Source_Mixer;

typedef enum
{
  No_INPUT,
  L_R_ANALOG_MIC,
  L_R_LINE_IN,
  L_R_ANALOG_MIC_PLUS_LINE_IN
} L_R_ADC_Audio_Input_Mixer;

typedef enum
{
	LINE_INPUT_MUTE_EN,
	LINE_INPUT_MUTE_DIS
} L_R_Line_Input_Mute_En_Dis;
typedef enum
{
	LEFT_LINE_INPUT,
	RIGHT_LINE_INPUT,
} L_R_Line_Input;


typedef struct {
        // Bit fields within the register
	uint8_t DACLvl;
    uint8_t DACA	 : 4;
    uint8_t DACG 	 : 2;
    uint8_t DACM 	 : 1;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} DAC_Level_Ctrl_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t ADCLvl;
    uint8_t AVR	     : 4;
    uint8_t AVL 	 : 4;
    // Add more bit fields as needed
} ADC_Level_Ctrl_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t LVolCtrl;
    uint8_t VOLL	 : 6;
    uint8_t VOLLM 	 : 1;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} Left_Volume_Ctrl_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t RVolCtrl;
    uint8_t VOLR	 : 6;
    uint8_t VOLRM 	 : 1;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} Right_Volume_Ctrl_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t digtlFltr;
    uint8_t DVFLT	 : 3;
    uint8_t reserved : 1;
    uint8_t AVFLT    : 3;
    uint8_t MODE     : 1;
    // Add more bit fields as needed
} Digital_Filtering_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t pwrManag;
    uint8_t ADREN	 : 1;
    uint8_t ADLEN    : 1;
    uint8_t DAREN    : 1;
    uint8_t DALEN    : 1;
    uint8_t reserved : 1;
    uint8_t LNREN    : 1;
    uint8_t LNLEN    : 1;
    uint8_t SHDN     : 1;
    // Add more bit fields as needed
} Power_Management_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t confMode;
    uint8_t HPMODE	 : 3;
    uint8_t JDETEN   : 1;
    uint8_t reserved : 1;
    uint8_t ZDEN     : 1;
    uint8_t VSEN     : 1;
    uint8_t DSLEW    : 1;
    // Add more bit fields as needed
} Mode_Configuration_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t lMicGain;
    uint8_t PGAML	 : 5;
    uint8_t PALEN    : 2;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} Left_Mic_Gain_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t rMicGain;
    uint8_t PGAMR	 : 5;
    uint8_t PAREN    : 2;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} Right_Mic_Gain_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t adcInput;
    uint8_t AUXEN	 : 1;
    uint8_t AUXCAL   : 1;
    uint8_t AUXGAIN  : 1;
    uint8_t AUXCAP	 : 1;
    uint8_t MXINR    : 2;
    uint8_t MXINL    : 2;
    // Add more bit fields as needed
} ADC_Input_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t sidetone;
    uint8_t DVST	 : 5;
    uint8_t reserved : 1;
    uint8_t DSTS     : 2;
    // Add more bit fields as needed
} Sidetone_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t lLineIn;
    uint8_t LIGL	 : 4;
    uint8_t reserved : 1;
    uint8_t reserved1 : 1;
    uint8_t LILM     : 1;
    uint8_t reserved2 : 1;
    // Add more bit fields as needed
} Left_Line_input_Level_Reg;

typedef struct {
        // Bit fields within the register
	uint8_t rLineIn;
    uint8_t LIGR	 : 4;
    uint8_t reserved : 1;
    uint8_t reserved1 : 1;
    uint8_t LIRM     : 1;
    uint8_t reserved2 : 1;
    // Add more bit fields as needed
} Right_Line_input_Level_Reg;
#endif /* INC_MAX9867_CODEC_H_ */
