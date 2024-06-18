/*
 * MAX9704_Codec.h
 * Description: MAX9704_Codec header file
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

#ifndef MAX9867_CODEC_H_
#define MAX9867_CODEC_H_

/******************************************************************INCLUDES**********************************************************/

#include "H07R8_i2c.h"
#include <stdbool.h>

//#endif /* INC_MAX9867_CODEC_H_ */
/******************************************************************MACROS**********************************************************/
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

/******************************************************************ENUMS**********************************************************/

typedef enum{
	CODEC_OK = 0,
	CODEC_INV,
	CODEC_TMOUT,
	CODEC_BUSY,
	CODEC_WRONG_ADDRESS,
	CODEC_ERR=255
}Codec_Status;

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
	MAX9867_SLAVE_MODE,
	MAX9867_MASTER_MODE
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
	BCLK_OFF,
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

/******************************************************************REGISTERS**********************************************************/
/* Status (Read Only) (0x00) */
typedef union{
	struct {
	uint8_t reserved1: 1;
    uint8_t JDET     : 1;
    uint8_t reserved2: 1;
    uint8_t reserved3: 1;
    uint8_t reserved4: 1;
    uint8_t ULK      : 1;
    uint8_t SLD      : 1;
    uint8_t CLD      : 1;
	};
	uint8_t auxRegH;
} Status_Reg;
/*********************************************************/
/* Jack Sense (Read Only) (0x01) */
typedef union{
	struct {
	uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t reserved3: 1;
    uint8_t reserved4: 1;
    uint8_t reserved5: 1;
    uint8_t JKMIC    : 1;
    uint8_t JKSNS    : 1;
    uint8_t LSNS     : 1;
	};
	uint8_t auxRegH;
} Jack_Sense_Reg;
/*********************************************************/
/* AUX High (Read Only) Register (0x02) */
typedef union{
	struct {
    uint8_t AUX: 8;
	};
	uint8_t auxRegH;
} Aux_H;
/*********************************************************/
/* AUX Low (Read Only) Register (0x03) */
typedef union{
	struct {
    uint8_t AUX: 8;
	};
	uint8_t auxRegL;
} Aux_L;
/*********************************************************/
/* Interrupt Enable Register (0x04) */
typedef union{
	struct {
    uint8_t reserved1: 1;
    uint8_t IJDET    : 1;
    uint8_t SDODLY   : 1;
    uint8_t reserved2: 1;
    uint8_t reserved3: 1;
    uint8_t IULK     : 1;
    uint8_t ISLD     : 1;
    uint8_t ICLD     : 1;
	};
	uint8_t interruptEnReg;
} Interrupt_Reg;
/*********************************************************/
/* System Clock Register (0x05) */
typedef union{
	struct {
    uint8_t FREQ     : 4;
    uint8_t PSCLK    : 2;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
	};
	uint8_t sysClkReg;
} System_Clock_Reg;
/*********************************************************/
/* Stereo Audio Clock Control High Register (0x06) */
typedef union{
	struct {
    uint8_t NI       : 7;
    uint8_t PLL      : 1;
	};
	uint8_t stereoAudClkRegH;
} Stereo_Audio_Clock_Control_High_Reg;
/*********************************************************/
/* Stereo Audio Clock Control Low Register (0x07) */
typedef union{
	struct {
    uint8_t NI0      : 1;
    uint8_t NI       : 7;
	};
	uint8_t stereoAudClkRegL;
} Stereo_Audio_Clock_Control_Low_Reg;
/*********************************************************/
/* DIGITAL AUDIO INTERFACE Register1 (0x08) */
typedef union{
	struct {
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t TDM 	 : 1;
    uint8_t HIZOFF   : 1;
    uint8_t DLY      : 1;
    uint8_t BCI      : 1;
    uint8_t WCI      : 1;
    uint8_t MAS      : 1;
	};
	uint8_t digAudReg1;
} Digital_Audio_Interface_Reg_1;
/*********************************************************/
/* DIGITAL AUDIO INTERFACE Register2 (0x09) */
typedef union{
	struct {
    uint8_t BSEL     : 3;
    uint8_t DMONO    : 1;
    uint8_t LVOLFIX  : 1;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t reserved3: 1;
	};
	uint8_t digAudReg2;
} Digital_Audio_Interface_Reg_2;
/*********************************************************/
/* Codec Filters Register (0x0a) */
typedef union{
	struct {
    uint8_t DVFLT	 : 3;
    uint8_t reserved : 1;
    uint8_t AVFLT    : 3;
    uint8_t MODE     : 1;
	};
	uint8_t digtlFltrReg;
} Digital_Filtering_Reg;
/*********************************************************/
/* Sidetone Register (0x0b) */
typedef union{
	struct {
    uint8_t DVST	 : 5;
    uint8_t reserved : 1;
    uint8_t DSTS     : 2;
	};
	uint8_t sidetoneReg;
} Sidetone_Reg;
/*********************************************************/
/* DAC Level Register (0x0c) */
typedef union{
	struct {
    uint8_t DACA	 : 4;
    uint8_t DACG 	 : 2;
    uint8_t DACM 	 : 1;
    uint8_t reserved : 1;
	};
	uint8_t DACLvlReg;
} DAC_Level_Ctrl_Reg;
/*********************************************************/
/* ADC Level Register (0x0d) */
typedef union{
	struct {
    uint8_t AVR	     : 4;
    uint8_t AVL 	 : 4;
	};
	uint8_t ADCLvlReg;
} ADC_Level_Ctrl_Reg;
/*********************************************************/
/* Left-Line Input Level Register (0x0e) */
typedef union{
	struct {
    uint8_t LIGL	 : 4;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t LILM     : 1;
    uint8_t reserved3: 1;
	};
	uint8_t lLineInReg;
} Left_Line_input_Level_Reg;
/*********************************************************/
/* Right-Line Input Level Register (0x0f) */
typedef union{
	struct {
    uint8_t LIGR	 : 4;
    uint8_t reserved1: 1;
    uint8_t reserved2: 1;
    uint8_t LIRM     : 1;
    uint8_t reserved3: 1;
	};
	uint8_t rLineInReg;
} Right_Line_input_Level_Reg;
/*********************************************************/
/* Left Volume Control Register (0x10) */
typedef union{
	struct {
    uint8_t VOLL	 : 6;
    uint8_t VOLLM 	 : 1;
    uint8_t reserved : 1;
	};
	uint8_t LVolCtrlReg;
} Left_Volume_Ctrl_Reg;
/*********************************************************/
/* Right Volume Control Register (0x11) */
typedef union{
	struct {
    uint8_t VOLR	 : 6;
    uint8_t VOLRM 	 : 1;
    uint8_t reserved : 1;
	};
	uint8_t RVolCtrlReg;
} Right_Volume_Ctrl_Reg;
/*********************************************************/
/* Left Microphone Gain Register (0x12) */
typedef union{
	struct {
    uint8_t PGAML	 : 5;
    uint8_t PALEN    : 2;
    uint8_t reserved : 1;
	};
	uint8_t lMicGainReg;
} Left_Mic_Gain_Reg;
/*********************************************************/
/* Right Microphone Gain Register (0x13) */
typedef union{
	struct {
    uint8_t PGAMR	 : 5;
    uint8_t PAREN    : 2;
    uint8_t reserved : 1;
	};
	uint8_t rMicGainReg;
} Right_Mic_Gain_Reg;
/*********************************************************/
/* ADC Input Register (0x14) */
typedef union{
	struct {
    uint8_t AUXEN	 : 1;
    uint8_t AUXCAL   : 1;
    uint8_t AUXGAIN  : 1;
    uint8_t AUXCAP	 : 1;
    uint8_t MXINR    : 2;
    uint8_t MXINL    : 2;
	};
	uint8_t adcInputReg;
} ADC_Input_Reg;
/*********************************************************/
/* Microphone Register (0x15) */
typedef union{
	struct {
    uint8_t reserved1 : 1;
    uint8_t reserved2 : 1;
    uint8_t reserved3 : 1;
    uint8_t reserved4 : 1;
    uint8_t DIGMICR   : 1;
    uint8_t DIGMICL   : 1;
    uint8_t MICCLK    : 2;
	};
	uint8_t digMicClkReg;
} Digital_Microphone_Input_Reg;
/*********************************************************/
/* Mode Register (0x16) */
typedef union{
	struct {
    uint8_t HPMODE	 : 3;
    uint8_t JDETEN   : 1;
    uint8_t reserved : 1;
    uint8_t ZDEN     : 1;
    uint8_t VSEN     : 1;
    uint8_t DSLEW    : 1;
	};
	uint8_t configModeReg;
} Mode_Configuration_Reg;
/*********************************************************/
/* POWER MANAGEMENT Register (0x17) */
typedef union{
	struct {
		uint8_t ADREN	 : 1;
		uint8_t ADLEN    : 1;
		uint8_t DAREN    : 1;
		uint8_t DALEN    : 1;
		uint8_t reserved : 1;
		uint8_t LNREN    : 1;
		uint8_t LNLEN    : 1;
		uint8_t SHDN     : 1;
	};
	uint8_t pwrManagReg;
}Power_Management_Reg;

/********************************************************************************PRIVATE APIs*****************************************************************************************************************************/

Codec_Status MAX9867_ClockControlInit(MCLK_Prescaler mclkPresclr, Exact_Integer_Modes exactIntMode,
		PLL_Mode_En_Dis pllMode, uint32_t NI, PLL_Rapid_Lock_En_Dis pllRapidLock);
Codec_Status MAX9867_InterruptEnable(bool clipDetect,bool slewDetect,bool pllUnlock,bool headsetChange);
Codec_Status MAX9867_Shoutdown(Shoutdown shutdown);
Codec_Status MAX9867_HeadphoneAmpType(Headphone_Amp_Type ampType);
Codec_Status MAX9867_DAC_EnableDisable(DAC_En_Dis dac);
Codec_Status MAX9867_DAC_Gain(DAC_Gain firstAmp, DAC_Level_Ctrl progAmp);
Codec_Status MAX9867_DAC_Mute(DAC_Mute_En_Dis dacMute);
Codec_Status MAX9867_DigitalAudioInterfaceInit(void);
Codec_Status ReadingDigitalAudioInit(Digital_Audio_Mode audioMode, L_R_Playback_Volume_Channel channel, DAC_Gain firstAmp, DAC_Level_Ctrl progAmp,
		ADC_DAC_Digital_Audio_Filter_Sٍpecifications DAC_Specifications, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol);
/********************************************************************************USER APIs*****************************************************************************************************************************/

Codec_Status MAX9867_CodecInit(DAC_Level_Ctrl dacGain,L_R_Playback_Volume rPlaybackVol,L_R_Playback_Volume lPlaybackVol);
Codec_Status MAX9867_Codec_DAC_Gain(DAC_Level_Ctrl progAmp);
Codec_Status MAX9867_AudioLevel(L_R_Playback_Volume_Channel channel, L_R_Playback_Volume rPlaybackVol, L_R_Playback_Volume lPlaybackVol);
Codec_Status MAX9867_AudioMute(Audio_Mute audioMute);

#endif /* INC_MAX9867_CODEC_H_ */

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
