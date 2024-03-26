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
#include "Porting.h"
#include <stdbool.h>


#define MAX9867_SLAVE_ADDRESS_W            	0x30
#define MAX9867_SLAVE_ADDRESS_R				0x31

#define _DELAY_MS(TimeOut)       HAL_Delay(TimeOut)
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
	AUDIO_MUTE_DISABLE,
	AUDIO_MUTE_ENABLE
} Audio_Mute;

typedef enum
{
	DAC_DISABLE,
	DAC_ENABLE
} DAC_En_Dis;

typedef enum
{
	DAC_MUTE_DISABLE,
	DAC_MUTE_ENABLE
} DAC_Mute_En_Dis;

typedef enum
{
	ADC_DISABLE,
	ADC_ENABLE
} ADC_En_Dis;

typedef enum
{
	DAC_GAIN_0dB,
	DAC_GAIN_6dB,
	DAC_GAIN_12dB,
	DAC_GAIN_18dB
} DAC_Gain;

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
	MIC_LEFT,
	MIC_RIGHT,
	MIC_LEFT_RIGHT
} L_R_Mic;

typedef enum
{
	MIC_AMPLIFY,
	MIC_RECORDING,
	MIC_AMPLIFY_RECORDING
} Mic_Mode;

typedef enum
{
	ADC_LEFT,
	ADC_RIGHT,
	ADC_LEFT_RIGHT
} ADC_L_R;

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
	LINE_INPUT_EN,
	LINE_INPUT_DIS
} L_R_Line_Input_En_Dis;

typedef enum
{
	LEFT_LINE_INPUT,
	RIGHT_LINE_INPUT,
	LEFT_RIGHT_LINE_INPUT
} L_R_Line_Input;

typedef enum
{
	LINE_INPUT_AMPLIFY,
	LINE_INPUT_RECORDING,
	LINE_INPUT_AMPLIFY_RECORDING
} Line_Input_Mode;

typedef enum
{
	LINE_INPUT_MUTE,
	LINE_INPUT_UNMUTE
} Line_Input_Mute_En_Dis;

typedef enum
{
	SHOUTDOWN_ENABLE,
	SHOUTDOWN_DISABLE
} Shoutdown;

typedef enum
{
	PCLK_8,
	PCLK_6
} Digital_Mic_Clk_Pre;

typedef enum
{
	MAX9867_MASTER_MODE,
	MAX9867_SLAVE_MODE
} MAX9867_Master_Slave_Mode;

typedef enum
{
	LEFT_CHN_DATA_IN_OUT,
	RIGHT_CHN_DATA_IN_OUT
} MAX9867_L_R_Clk_Invert;

typedef enum
{
	SDIN_LATCHED_RISING_EDGE_BCLK,
	SDIN_LATCHED_FALLING_EDGE_BCLK
} MAX9867_Bit_Clk_Invert;

typedef enum
{
	SDOUT_TRANS_AFTER_SDIN_LATCHED,
	SDOUT_TRANS_WITH_SDIN_LATCHED
} MAX9867_SDOUT_Delay;

typedef enum
{
	SDIN_SDOUT_LATCHED_FIRST_BCLK_EDGE,
	SDIN_SDOUT_LATCHED_SECOND_BCLK_EDGE
} MAX9867_Delay_Mode;

typedef enum
{
	SDOUT_HIGH_IMPEDANCE_AFTER_DATA_TRANS,
	SDOUT_HIGH_OR_LOW_AFTER_DATA_TRANS
} MAX9867_SDOUT_Mode_High_Impedance_Mode;

typedef enum
{
	LRCLK_INDICATE_L_R_AUDIO,
	LRCLK_FRAMING_PULSE
} MAX9867_TDM_Mode;

typedef enum
{
	TRACKS_VOLL_VOLR_BITS,
	FIXED_VOLL_VOLR_BITS
} MAX9867_Fix_Line_Input_Volume;

typedef enum
{
	OFF,
	LRCLK_Multiply_64,
	LRCLK_Multiply_48,
	RESERVED,
	PCLK_DIVIDE_2,
	PCLK_DIVIDE_4,
	PCLK_DIVIDE_8,
	PCLK_DIVIDE_16,
} MAX9867_BCLK_Select;

typedef enum
{
	SDIN_PROCESS_SEPARATELY,
	SDIN_MIXED_SINGLE_CHA_ROUT_L_R_DAC
} MAX9867_Mono_Playback_Mode;

typedef enum
{
	MODE1,
	MODE2,
	MODE3,
	MODE4
} Digital_Mic_Clk_Mode;

typedef enum
{
	DISABLE_CLK,
	MCLK_BETWEEN_10_20_MHZ,
	MCLK_BETWEEN_20_40_MHZ,
	MCLK_BETWEEN_40_60_MHZ
} MCLK_Prescaler;

typedef enum
{
	PLL_DISABLE,
	PLL_ENABLE
} PLL_Mode_En_Dis;

typedef enum
{
	PLL_RAPID_LOCK_ENABLE,
	PLL_RAPID_LOCK_DISABLE
} PLL_Rapid_Lock_En_Dis;

typedef enum
{
	NORMAL_OR_PLL_MODE,
	PCLK_LRCK_1500_RATIO_MODE = 0x08,
	PCLK_LRCK_750_RATIO_MODE,
	PCLK_LRCK_1625_RATIO_MODE,
	PCLK_LRCK_812p5_RATIO_MODE,
	PCLK_LRCK_2000_RATIO_MODE,
	PCLK_LRCK_1000_RATIO_MODE,
	PCLK_LRCK_2400_RATIO_MODE,
	PCLK_LRCK_1200_RATIO_MODE
} Exact_Integer_Modes;

typedef enum
{
	UPDATE_AUX_WITH_VOLTAGE,
	HOLD_AUX_FOR_READING
} Auxiliary_Input_Capture;

typedef enum
{
	GAIN_NORMAL_OPERATION,
	CONNECT_INPUT_BUFFER_TO_INTERNAL_VOLTAGE
} Auxiliary_Input_Gain_Calibration;

typedef enum
{
	OFFSET_NORMAL_OPERATION,
	ADC_AUTO_CALIBRATE_ANY_OFFSET
} Auxiliary_Input_Offset_Calibration;

typedef enum
{
	JACKSNS_PIN_FOR_JACK_DETECTION,
	JACKSNS_PIN_FOR_DC_MEASUREMENT
} Auxiliary_Input_Type;

typedef enum
{
	JACKSNS_ENABLE,
	JACKSNS_DISABLE
} Jack_Sense_En_Dis;

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
} Headphone_Amp_Type;

typedef enum
{
	MIC_PREAMP_GAIN_DISABLED,
	MIC_PREAMP_GAIN_0dB,
	MIC_PREAMP_GAIN_PLUS_20dB,
	MIC_PREAMP_GAIN_PLUS_30dB
} L_R_Mic_Preamp_Gain;

typedef enum
{
	LEFT_VOLUME_CHA,
	RIGHT_VOLUME_CHA,
	LEFT_RIGHT_VOLUME_CHA
} L_R_Playback_Volume_Channel;

typedef enum
{
	AUDIO,
	VOICE
} Digital_Audio_Mode;

typedef enum
{
	GAIN_PLUS_24dB,
	GAIN_PLUS_22dB,
	GAIN_PLUS_20dB,
	GAIN_PLUS_18dB,
	GAIN_PLUS_16dB,
	GAIN_PLUS_14dB,
	GAIN_PLUS_12dB,
	GAIN_PLUS_10dB,
	GAIN_PLUS_8dB,
	GAIN_PLUS_6dB,
	GAIN_PLUS_4dB,
	GAIN_PLUS_2dB,
	GAIN_PLUS_0dB,
	GAIN_MINUS_2dB,
	GAIN_MINUS_4dB,
	GAIN_MINUS_6dB,
} L_R_Line_Input_Gain;

typedef enum
{
	ADC_GAIN_PLUS_0dB,
	ADC_GAIN_MINUS_1dB,
	ADC_GAIN_MINUS_2dB,
	ADC_GAIN_MINUS_3dB,
	ADC_GAIN_MINUS_4dB,
	ADC_GAIN_MINUS_5dB,
	ADC_GAIN_MINUS_6dB,
	ADC_GAIN_MINUS_7dB,
	ADC_GAIN_MINUS_8dB,
	ADC_GAIN_MINUS_9dB,
	ADC_GAIN_MINUS_10dB,
	ADC_GAIN_MINUS_11dB,
	ADC_GAIN_MINUS_12dB,
	ADC_GAIN_MINUS_13dB,
	ADC_GAIN_MINUS_14dB,
	ADC_GAIN_MINUS_15dB
} L_R_ADC_Level_Ctrl;

typedef enum
{
	DAC_LVL_GAIN_0dB,
	DAC_LVL_GAIN_MINUS_1dB,
	DAC_LVL_GAIN_MINUS_2dB,
	DAC_LVL_GAIN_MINUS_3dB,
	DAC_LVL_GAIN_MINUS_4dB,
	DAC_LVL_GAIN_MINUS_5dB,
	DAC_LVL_GAIN_MINUS_6dB,
	DAC_LVL_GAIN_MINUS_7dB,
	DAC_LVL_GAIN_MINUS_8dB,
	DAC_LVL_GAIN_MINUS_9dB,
	DAC_LVL_GAIN_MINUS_10dB,
	DAC_LVL_GAIN_MINUS_11dB,
	DAC_LVL_GAIN_MINUS_12dB,
	DAC_LVL_GAIN_MINUS_13dB,
	DAC_LVL_GAIN_MINUS_14dB,
	DAC_LVL_GAIN_MINUS_15dB
} DAC_Level_Ctrl;

typedef enum
{
	MIC_PROGRAMBLE_GAIN_PLUS_20dB,
	MIC_PROGRAMBLE_GAIN_PLUS_19dB,
	MIC_PROGRAMBLE_GAIN_PLUS_18dB,
	MIC_PROGRAMBLE_GAIN_PLUS_17dB,
	MIC_PROGRAMBLE_GAIN_PLUS_16dB,
	MIC_PROGRAMBLE_GAIN_PLUS_15dB,
	MIC_PROGRAMBLE_GAIN_PLUS_14dB,
	MIC_PROGRAMBLE_GAIN_PLUS_13dB,
	MIC_PROGRAMBLE_GAIN_PLUS_12dB,
	MIC_PROGRAMBLE_GAIN_PLUS_11dB,
	MIC_PROGRAMBLE_GAIN_PLUS_10dB,
	MIC_PROGRAMBLE_GAIN_PLUS_9dB,
	MIC_PROGRAMBLE_GAIN_PLUS_8dB,
	MIC_PROGRAMBLE_GAIN_PLUS_7dB,
	MIC_PROGRAMBLE_GAIN_PLUS_6dB,
	MIC_PROGRAMBLE_GAIN_PLUS_5dB,
	MIC_PROGRAMBLE_GAIN_PLUS_4dB,
	MIC_PROGRAMBLE_GAIN_PLUS_3dB,
	MIC_PROGRAMBLE_GAIN_PLUS_2dB,
	MIC_PROGRAMBLE_GAIN_PLUS_1dB,
	MIC_PROGRAMBLE_GAIN_PLUS_0dB
} L_R_Mic_Programble_Gain_Amp;

typedef enum
{
	PLAYBACK_VOLUME_GAIN_PLUS_6dB,
	PLAYBACK_VOLUME_GAIN_PLUS_5d5B,
	PLAYBACK_VOLUME_GAIN_PLUS_5dB,
	PLAYBACK_VOLUME_GAIN_PLUS_4d5B,
	PLAYBACK_VOLUME_GAIN_PLUS_4dB,
	PLAYBACK_VOLUME_GAIN_PLUS_3d5B,
	PLAYBACK_VOLUME_GAIN_PLUS_3dB,
	PLAYBACK_VOLUME_GAIN_PLUS_2dB,
	PLAYBACK_VOLUME_GAIN_PLUS_1dB,
	PLAYBACK_VOLUME_GAIN_0dB,
	PLAYBACK_VOLUME_GAIN_MINUS_1dB,
	PLAYBACK_VOLUME_GAIN_MINUS_2dB,
	PLAYBACK_VOLUME_GAIN_MINUS_3dB,
	PLAYBACK_VOLUME_GAIN_MINUS_4dB,
	PLAYBACK_VOLUME_GAIN_MINUS_5dB,
	PLAYBACK_VOLUME_GAIN_MINUS_6dB,
	PLAYBACK_VOLUME_GAIN_MINUS_8dB,
	PLAYBACK_VOLUME_GAIN_MINUS_10dB,
	PLAYBACK_VOLUME_GAIN_MINUS_12dB,
	PLAYBACK_VOLUME_GAIN_MINUS_14dB,
	PLAYBACK_VOLUME_GAIN_MINUS_16dB,
	PLAYBACK_VOLUME_GAIN_MINUS_18dB,
	PLAYBACK_VOLUME_GAIN_MINUS_20dB,
	PLAYBACK_VOLUME_GAIN_MINUS_22dB,
	PLAYBACK_VOLUME_GAIN_MINUS_24dB,
	PLAYBACK_VOLUME_GAIN_MINUS_26dB,
	PLAYBACK_VOLUME_GAIN_MINUS_30dB,
	PLAYBACK_VOLUME_GAIN_MINUS_34dB,
	PLAYBACK_VOLUME_GAIN_MINUS_38dB,
} L_R_Playback_Volume;

typedef enum
{
	SIDETONE_GAIN_DIFF_HEAD_OFF,
	SIDETONE_GAIN_DIFF_HEAD_0dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_2dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_4dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_6dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_8dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_10dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_12dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_14dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_16dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_18dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_20dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_22dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_24dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_26dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_28dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_30dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_32dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_34dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_36dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_38dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_40dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_42dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_44dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_46dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_48dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_50dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_52dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_54dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_56dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_58dB,
	SIDETONE_GAIN_DIFF_HEAD_MINUS_60dB
} Sidetone_Gain_Diff_Headphone;

typedef enum
{
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_OFF,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_5dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_7dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_9dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_11dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_13dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_15dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_17dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_19dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_21dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_23dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_25dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_27dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_29dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_31dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_33dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_35dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_37dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_39dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_41dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_43dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_45dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_47dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_49dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_51dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_53dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_55dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_57dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_59dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_61dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_63dB,
	SIDETONE_GAIN_DIFF_CAP_S_ENDED_MINUS_65dB
} Sidetone_Gain_Capacitorless_Single_Ended_Headphone;


/* 0x00 */
typedef struct {
        // Bit fields within the register
	uint8_t auxRegH;

    uint8_t AUX: 8;
    // Add more bit fields as needed
} Aux_H;
/* 0x01 */
typedef struct {
        // Bit fields within the register
	uint8_t auxRegH;
	uint8_t reserved1: 1;
    uint8_t JDET     : 1;
    uint8_t reserved2: 1;
    uint8_t reserved3: 1;
    uint8_t reserved4: 1;
    uint8_t ULK      : 1;
    uint8_t SLD      : 1;
    uint8_t CLD      : 1;
    // Add more bit fields as needed
} Status_Reg;
/* 0x02 */
typedef struct {
        // Bit fields within the register
	uint8_t auxRegH;
	uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t reserved3: 1;
    uint8_t reserved4: 1;
    uint8_t reserved5: 1;
    uint8_t JKMIC    : 1;
    uint8_t JKSNS    : 1;
    uint8_t LSNS     : 1;
    // Add more bit fields as needed
} Jack_Sense_Reg;

/* 0x03 */
typedef struct {
        // Bit fields within the register
	uint8_t auxRegL;

    uint8_t AUX: 8;
    // Add more bit fields as needed
} Aux_L;

/* 0x04 */
typedef struct {
        // Bit fields within the register
	uint8_t interruptEnReg;

    uint8_t reserved1: 1;
    uint8_t IJDET    : 1;
    uint8_t SDODLY   : 1;
    uint8_t reserved2: 1;
    uint8_t reserved3: 1;
    uint8_t IULK     : 1;
    uint8_t ISLD     : 1;
    uint8_t ICLD     : 1;
    // Add more bit fields as needed
} Interrupt_Reg;

/* 0x05 */
typedef struct {
        // Bit fields within the register
	uint8_t sysClkReg;
    uint8_t FREQ     : 3;
    uint8_t PSCLK    : 2;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    // Add more bit fields as needed
} System_Clock_Reg;

/* 0x06 */
typedef struct {
        // Bit fields within the register
	uint8_t stereoAudClkRegH;
    uint8_t NI       : 7;
    uint8_t PLL      : 1;
    // Add more bit fields as needed
} Stereo_Audio_Clock_Control_High_Reg;

/* 0x07 */
typedef struct {
        // Bit fields within the register
	uint8_t stereoAudClkRegL;
    uint8_t NI0      : 1;
    uint8_t NI       : 7;
    // Add more bit fields as needed
} Stereo_Audio_Clock_Control_Low_Reg;

/* 0x08 */
typedef struct {
        // Bit fields within the register
	uint8_t digAudReg1;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t TDM 	 : 1;
    uint8_t HIZOFF   : 1;
    uint8_t DLY      : 1;
    uint8_t BCI      : 1;
    uint8_t WCI      : 1;
    uint8_t MAS      : 1;
    // Add more bit fields as needed
} Digital_Audio_Interface_Reg_1;

/* 0x09 */
typedef struct {
        // Bit fields within the register
	uint8_t digAudReg2;
    uint8_t BSEL     : 3;
    uint8_t DMONO    : 1;
    uint8_t LVOLFIX  : 1;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t reserved3: 1;
    // Add more bit fields as needed
} Digital_Audio_Interface_Reg_2;

/* 0x0a */
typedef struct {
        // Bit fields within the register
	uint8_t digtlFltrReg;
    uint8_t DVFLT	 : 3;
    uint8_t reserved : 1;
    uint8_t AVFLT    : 3;
    uint8_t MODE     : 1;
    // Add more bit fields as needed
} Digital_Filtering_Reg;

/* 0x0b */
typedef struct {
        // Bit fields within the register
	uint8_t sidetoneReg;
    uint8_t DVST	 : 5;
    uint8_t reserved : 1;
    uint8_t DSTS     : 2;
    // Add more bit fields as needed
} Sidetone_Reg;

/* 0x0c */
typedef struct {
        // Bit fields within the register
	uint8_t DACLvlReg;
    uint8_t DACA	 : 4;
    uint8_t DACG 	 : 2;
    uint8_t DACM 	 : 1;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} DAC_Level_Ctrl_Reg;

/* 0x0d */
typedef struct {
        // Bit fields within the register
	uint8_t ADCLvlReg;
    uint8_t AVR	     : 4;
    uint8_t AVL 	 : 4;
    // Add more bit fields as needed
} ADC_Level_Ctrl_Reg;

/* 0x0e */
typedef struct {
        // Bit fields within the register
	uint8_t lLineInReg;
    uint8_t LIGL	 : 4;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t LILM     : 1;
    uint8_t reserved3: 1;
    // Add more bit fields as needed
} Left_Line_input_Level_Reg;

/* 0x0f */
typedef struct {
        // Bit fields within the register
	uint8_t rLineInReg;
    uint8_t LIGR	 : 4;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t LIRM     : 1;
    uint8_t reserved3: 1;
    // Add more bit fields as needed
} Right_Line_input_Level_Reg;

/* 0x10 */
typedef struct {
        // Bit fields within the register
	uint8_t LVolCtrlReg;
    uint8_t VOLL	 : 6;
    uint8_t VOLLM 	 : 1;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} Left_Volume_Ctrl_Reg;

/* 0x11 */
typedef struct {
        // Bit fields within the register
	uint8_t RVolCtrlReg;
    uint8_t VOLR	 : 6;
    uint8_t VOLRM 	 : 1;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} Right_Volume_Ctrl_Reg;

/* 0x12 */
typedef struct {
        // Bit fields within the register
	uint8_t lMicGainReg;
    uint8_t PGAML	 : 5;
    uint8_t PALEN    : 2;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} Left_Mic_Gain_Reg;

/* 0x13 */
typedef struct {
        // Bit fields within the register
	uint8_t rMicGainReg;
    uint8_t PGAMR	 : 5;
    uint8_t PAREN    : 2;
    uint8_t reserved : 1;
    // Add more bit fields as needed
} Right_Mic_Gain_Reg;

/* 0x14 */
typedef struct {
        // Bit fields within the register
	uint8_t adcInputReg;
    uint8_t AUXEN	 : 1;
    uint8_t AUXCAL   : 1;
    uint8_t AUXGAIN  : 1;
    uint8_t AUXCAP	 : 1;
    uint8_t MXINR    : 2;
    uint8_t MXINL    : 2;
    // Add more bit fields as needed
} ADC_Input_Reg;

/* 0x15 */
typedef struct {
        // Bit fields within the register
	uint8_t digMicClkReg;
    uint8_t reserved1 : 1;
    uint8_t reserved2 : 1;
    uint8_t reserved3 : 1;
    uint8_t reserved4 : 1;
    uint8_t DIGMICR   : 1;
    uint8_t DIGMICL   : 1;
    uint8_t MICCLK    : 2;
    // Add more bit fields as needed
} Digital_Microphone_Input_Reg;

/* 0x16 */
typedef struct {
        // Bit fields within the register
	uint8_t configModeReg;
    uint8_t HPMODE	 : 3;
    uint8_t JDETEN   : 1;
    uint8_t reserved : 1;
    uint8_t ZDEN     : 1;
    uint8_t VSEN     : 1;
    uint8_t DSLEW    : 1;
    // Add more bit fields as needed
} Mode_Configuration_Reg;

/* 0x17 */
typedef struct {
        // Bit fields within the register
	uint8_t pwrManagReg;
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

Status_TypeDef WriteI2C(I2C_HANDLE *xPort, uint16_t sAddress, uint8_t *pData, uint16_t Size);
Status_TypeDef ReadI2C(I2C_HANDLE *xPort, uint16_t sAddress, uint8_t *rBuffer, uint16_t Size);
Status_TypeDef WriteI2S(I2S_HANDLE *xPort, uint16_t *pData, uint16_t Size);
Status_TypeDef ReadI2S(I2S_HANDLE *xPort, uint16_t *rBuffer, uint16_t Size);

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
		PLL_Mode_En_Dis pllMode, uint32_t NI, PLL_Rapid_Lock_En_Dis pllRapidLock);

/* Interrupt Enable */
/*
 * @brief  :Interrupt Enable.
 * @param1 :Clip Detect Flag (Indicates that a signal has reached or exceeded full scale in the ADC or DAC).
 * @param2 :Slew Level Detect Flag.
 * @param3 :Digital PLL Unlock Flag.
 * @param4 :Headset Configuration Change Flag.
 * @retval :Status
 */
Status_TypeDef MAX9867_InterruptEnable(bool clipDetect,bool slewDetect,bool pllUnlock,bool headsetChange);

/* Shoutdown Enable-Disable */
/*
 * @brief  :Shoutdown Enable-Disable.
 * @param1 :Shoutdown Enable-Disable.
 * @retval :Status
 */
Status_TypeDef MAX9867_Shoutdown(Shoutdown shutdown);

/* Headphone Amplifier type */
/*
 * @brief  :Headphone has three types(differential,capacitorless,single-indedd)
 * 		    and these types either to be mono or stereo.
 * @param1 :Headphone Amplifier type.
 * @retval :Status
 */
Status_TypeDef MAX9867_HeadphoneAmpType(Headphone_Amp_Type ampType);

/* DAC Enable-Disable */
/*
 * @brief  :DAC Enable-Disable to convert digital audio data to analog signal.
 * @param1 :DAC Enable-Disable.
 * @retval :Status
 */
Status_TypeDef MAX9867_DAC_EnableDisable(DAC_En_Dis dac);

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
Status_TypeDef MAX9867_DAC_Gain(DAC_Gain firstAmp, DAC_Level_Ctrl progAmp);

/* DAC Mute */
/*
 * @brief  :DAC Mute.
 * @param1 :DAC Mute Enable-Disable.
 * @retval :Status
 */
Status_TypeDef MAX9867_DAC_Mute(DAC_Mute_En_Dis dacMute);

/* ADC Enable-Disable */
/*
 * @brief  :ADC Enable-Disable to convert analog audio or voice signals to digital data stream.
 * @param1 :ADC Enable-Disable
 * @retval :Status
 */
Status_TypeDef MAX9867_ADC_EnableDisable(ADC_En_Dis adc);

/* Set ADC Gain */
/*
 * @brief  :Set ADC Gain.
 * @param1 :choose left/right ADC channels or both of them.
 * @param2 :Set ADC Gain.
 * @retval :Status
 */
Status_TypeDef MAX9867_ADC_Gain(ADC_L_R adc, L_R_ADC_Level_Ctrl adcGain);

/* LineInput  Enable-Disable */
/*
 * @brief  :LineInput  Enable-Disable(input for analog audio stream).
 * @Note   :MAX9867 has two line inputs(L/R).
 * @param1 :LineInput  Enable-Disable.
 * @param2 :choose line input(L/R/both of them).
 * @retval :Status
 */
Status_TypeDef MAX9867_LineInputEnableDisable(L_R_Line_Input_En_Dis lineInput, L_R_Line_Input lrLineInput);

/* Line-Input Gain */
/*
 * @brief  :Line-Input Gain.
 * @param1 :choose line input(L/R/both of them).
*  @param2  :Line-Input Gain.
 * @retval :Status
 */
Status_TypeDef MAX9867_LineInputGain(L_R_Line_Input lineInput, L_R_Line_Input_Gain lineInputGain);

/* Line-Input Mute Enable-Disable */
/*
 * @brief  :Line-Input Mute Enable-Disable.
 * @param1 :mute line-input(L/R/both of them).
 * @param2 :mute line-input Enable-Disable.
 * @retval :Status
 */
Status_TypeDef MAX9867_LineInputMute(L_R_Line_Input lineInput, Line_Input_Mute_En_Dis mute);

/* Set Audio Volume Level */
/*
 * @brief  :Set Audio Volume Level.
 * @param1 :choose (L/R/both of them) channels
 * @param2 :left channel audio volume level.
 * @param3 :right channel audio volume level.
 * @retval :Status
 */
Status_TypeDef MAX9867_AudioLevel(L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol);

/* Audio Mute Enable-Disable */
/*
 * @brief  :Audio Mute Enable-Disable.
 * @param1 :Audio Mute Enable-Disable.
 * @retval :Status
 */
Status_TypeDef MAX9867_AudioMute(Audio_Mute audioMute);

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
Status_TypeDef MAX9867_MicAmpGain(L_R_Mic mic, L_R_Mic_Preamp_Gain preAmpGain, L_R_Mic_Programble_Gain_Amp progGain);

/* Set Digital Microphone Clock */
/*
 * @brief  :Set Digital Microphone Clock.
 * @param1 :Set Digital Microphone Clock.
 * @retval :Status
 */
Status_TypeDef MAX9867_MicDigitalClock(Digital_Mic_Clk_Pre clock);

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
Status_TypeDef MAX9867_MicDigitalLeftRightEnableDisable(Digital_Mic_Clk_Mode mode);

/* Auxiliary Input Capture */
/*
 * @brief  :Update AUX register on MAX9867 JACKSNS/AUX pin with the DC voltage
 * 			or holding AUX register for reading.
 * @param1 :Update or holding AUX register
 * @retval :Status
 */
Status_TypeDef MAX9867_AuxiliaryInputCapture(Auxiliary_Input_Capture auxCapture);

/* Auxiliary Input Gain Calibration */
/*
 * @brief  :Auxiliary Input Gain Calibration.
 * @Note   :When set this bit The input buffer (AUX register) is disconnected from JACKSNS/AUX and connected to an internal voltage reference.
			While in this mode, read the AUX register and store the value. Use the stored value as a gain
			calibration factor, K, on subsequent readings.
 * @param1 :Auxiliary Input Gain Calibration.
 * @retval :Status
 */
Status_TypeDef MAX9867_AuxiliaryInputGainCalibration(Auxiliary_Input_Gain_Calibration auxGain);

/* Auxiliary Input Offset Calibration */
/*
 * @brief  :Auxiliary Input Offset Calibration.
 * @Note   :When set this bit JACKSNS/AUX pin is disconnected from the input
 * 		    and the ADC automatically calibrates out any internal offsets.
 * @param1 :Auxiliary Input Offset Calibration.
 * @retval :Status
 */
Status_TypeDef MAX9867_AuxiliaryInputOffsetCalibration(Auxiliary_Input_Offset_Calibration auxOffset);

/* Auxiliary Input Type */
/*
 * @brief  :Auxiliary Input Type.
 * @Note   :When set this bit:
 * 			0 = Use JACKSNS/AUX for jack detection.
 * 			1 = Use JACKSNS/AUX for DC measurements.
 * @param1 :Auxiliary Input Type.
 * @retval :Status
 */
Status_TypeDef MAX9867_AuxiliaryInputType(Auxiliary_Input_Type auxType);

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
Status_TypeDef MAX9867_MicSidetoneSourceAndGain(Digital_Sidetone_Source_Mixer sourceMixer, Sidetone_Gain_Diff_Headphone sidGainDiff,
    Sidetone_Gain_Capacitorless_Single_Ended_Headphone sidGainCapSinEnd, Amp_Type ampType);

/* Digital Audio Interface Unit Initialize */
/*
 * @brief   :Digital Audio Interface Unit Initialize.
 * @param1  :master slave mode.
 * @param2  :choose L/R channel.
 * @Note    :0 = Left-channel data is input and output while LRCLK is low.
 * 			 1 = Right-channel data is input and output while LRCLK is low.
 * @Note    :WCI is ignored when TDM = 1.
 * @param3  :latching data on SDIN when rising or falling edge from BCLK and latching SDOUT
 * 			 after delay depend on param3.
 * @param4  :delay latching data on SDOUT after latching data on SDIN.
 * @param5  :latching data on SDOUT/SDIN when first BCLK edge or second BCLK edge.
 * @param6  :SDOUT High-Impedance Mode.
 * @param7  :TDM mode select.
 * @Note    :TDM mode stands for Time-Division Multiplexing mode.
 * 			 Time-Division Multiplexing is a method of transmitting multiple signals over a single communication channel
 * 			 by interleaving the data in time slots.
 * @param8  :BCLK Select prescaler.
 * @param9  :Mono Playback Mode.
 * @Note    :0 = Stereo data input on SDIN is processed separately.
 * 			 1 = Stereo data input on SDIN is mixed to a single channel and routed to both the left and right DAC.
 * @param10 :fix the line input playback volume.
 * @retval  :Status
 */
Status_TypeDef MAX9867_DigitalAudioInterfaceInit(MAX9867_Master_Slave_Mode mode,
	MAX9867_L_R_Clk_Invert lrclk, MAX9867_Bit_Clk_Invert bclkMode, MAX9867_SDOUT_Delay sdoutDelay,
	MAX9867_Delay_Mode delayMode, MAX9867_SDOUT_Mode_High_Impedance_Mode sdoutMode,
	MAX9867_TDM_Mode tdmMode, MAX9867_BCLK_Select bclkSelect, MAX9867_Mono_Playback_Mode monoMode,
	MAX9867_Fix_Line_Input_Volume fixLineVol);

/* JackSens Enable Disable */
/*
 * @brief  :JackSens Enable Disable.
 * @param1 :JackSens Enable Disable.
 * @Note   :0 = Enables pullups on LOUTP and JACKSNS/AUX to detect jack insertion.
 * 			1 = Enables the comparator circuitry on JACKSNS/AUX to detect voltage changes.
 * @retval :Status
 */
Status_TypeDef MAX9867_JackSensEnableDisable(Jack_Sense_En_Dis jackSens);

/* Audio Amplify Recording */
/*
 * @brief   :Amplify/recording the audio which comes on line-inputs pins(LINL,LINR).
 * @Note    :some of parameters used in (Amplify mode) and other in (Recording mode) and other (Amplify and Recording modes).
 * @Note    :in status Amplify mode ignore Recording mode parameters,vice versa.
 * @param1  :line-input mode(Amplify/recording/both of them).
 * @param2  :line-input channel(L/R/both of them) (Amplify and Recording).
 * @param3  :line-input gain (Amplify and recording).
 * @param4  :choose Audio channel(L/R/both of them) (Amplify mode).
 * @param5  :right Audio level control (Amplify mode).
 * @param6  :left Audio level control (Amplify mode).
 * @param7  :choose ADC channel(L/R/both of them) (Recording).
 * @param8  :Set ADC gain (Recording).
 * @param9  :choose Audio ADC filter type (Recording).
 * @Note1   :MAX9867 uses (FIR filter) for Audio, so use configure in @Note2.
 * @Note2   :0 = DC-blocking filter is disabled.
 * 			 Any other setting = DC-blocking filter is enabled.
 * @retval  :Status
 */

Status_TypeDef AudioAmplifyRecordingInit(Line_Input_Mode lineInputMode, L_R_Line_Input lrLineInput,L_R_Line_Input_Gain gain, L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol,
		L_R_Playback_Volume lPlaybackVol, ADC_L_R adc, L_R_ADC_Level_Ctrl adcGain,
		ADC_DAC_Digital_Audio_Filter_Sٍpecifications ADC_Specifications);

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
 * @param14 :right Audio level control (Amplify mode).
 * @param15 :left Audio level control (Amplify mode).
 * @retval  :Status
 */
Status_TypeDef VoiceAmplifyRecordingInit(Mic_Mode micMode, L_R_Mic mic,
		L_R_Mic_Preamp_Gain preAmpGain, L_R_Mic_Programble_Gain_Amp progGain,
		ADC_DAC_Digital_Audio_Filter_Sٍpecifications ADC_Specifications,
		ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications,
		ADC_L_R adc, L_R_ADC_Level_Ctrl adcGain,
		Digital_Sidetone_Source_Mixer sourceMixer, Sidetone_Gain_Diff_Headphone sidGainDiff,
		Sidetone_Gain_Capacitorless_Single_Ended_Headphone sidGainCapSinEnd, Amp_Type ampType,
		L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol);

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
 * @param6 :right Audio level control.
 * @param7 :left Audio level control.
 * @retval :Status
 */
Status_TypeDef ReadingDigitalAudioInit(Digital_Audio_Mode audioMode, L_R_Playback_Volume_Channel channel, DAC_Gain firstAmp,
		DAC_Level_Ctrl progAmp, ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications, L_R_Playback_Volume rPlaybackVol,
		L_R_Playback_Volume lPlaybackVol);

/* DC measurement */
/*
 * @brief  :dc Measurement on JACKSNS/AUX pin.
 * @param1 :return a value with dc measurement.
 * @retval :Status
 */
Status_TypeDef DcMeasurement(uint32_t *dcMeasurement);
#endif /* INC_MAX9867_CODEC_H_ */

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
