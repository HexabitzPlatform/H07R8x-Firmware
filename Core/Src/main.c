/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MAX9704_Amplifier.h"
#include "MAX9867_Codec.h"
#include "SFX_Tribal_Exhale_st_os_01.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 I2C_HandleTypeDef hi2c2;

I2S_HandleTypeDef hi2s1;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t data[4096]={0};
uint8_t tx[2048]={100};
uint8_t rx[4096]={0};
uint8_t dataReady = 1;
//uint16_t sineDigital[41] = { 0x8000, 0x89B1, 0x92F2, 0x9C14, 0xA40B, 0xAB66, 0xB1F1, 0xB7A7,
//		0xBB97, 0xBE3B, 0xBF8A, 0xBF5D, 0xBDB4, 0xBA99, 0xB6BD, 0xB0DA, 0xAA91, 0xA2A0, 0x9A24, 0x9124,
//		0x87AF, 0x7E1F, 0x748F, 0x6B3F, 0x622A, 0x59FA, 0x52DF, 0x4C57, 0x464D, 0x4360, 0x40EC, 0x3FFF,
//		0x4043, 0x424A, 0x44EB, 0x482D, 0x4C20, 0x509F, 0x5EBC, 0x6722, 0x708F };


//uint16_t sineDigital[41] = { 32768, 35233, 37642, 39940, 42075, 43998, 45665, 47039,
//		48087, 48787, 49122, 49085, 48676, 47905, 46789, 45354, 43633, 41664,
//		39492, 37168, 34743, 32273, 29815, 27423, 25154, 23058, 21183, 19571,
//		18261, 17280, 16652, 16391, 16503, 16986, 17827, 19009, 20504, 22279,
//		24292, 26498, 28847 };
uint16_t sineDigital[41] = { 32768, 35233, 37642, 39940, 42075, 43998, 45665, 47039,
		48087, 48787, 49122, 49085, 48676, 47905, 46789, 45354, 43633, 41664,
		39492, 37168, 34743, 32273, 29815, 27423, 25154, 23058, 21183, 19571,
		18261, 17280, 16652, 16391, 16503, 16986, 17827, 19009, 20504, 22279,
		24292, 26498, 28847 };
uint8_t flag = 2;
uint8_t UARTFLAG = 0;
//uint16_t sineDigital[41] = { 0x0080, 0xa189, 0x0a93, 39940, 0x049c, 0xdeab, 0x61b2, 0xbfb7,
//		0xd7bb, 0x93be, 0xe2bf, 0xbdbf, 0x24be, 0x21bb, 0xc5b6, 0x2ab1, 0x71aa, 0xc0a2,
//		0x449a, 0x3091, 34743, 0xb787, 0x7774, 0x1f6b, 0x4262, 0x125a, 21183, 0xbf52,
//		0x5547, 0x8043, 0x0c41, 0x0740, 0x7740, 0x5a42, 0xa345, 0x414a, 0x1850, 0x0757,
//		0xe45e, 0x8267, 0xaf70 };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_DMA_Init();
  MX_I2S1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  MAX9704_AmpInit(SWITCHING_MODE_670KHZ, GAIN_MODE_29dB);
//  MAX9867_CodecInit(DAC_LVL_GAIN_MINUS_10dB/*DAC_LVL_GAIN_MINUS_15dB*/,PLAYBACK_VOLUME_GAIN_MINUS_38dB,PLAYBACK_VOLUME_GAIN_MINUS_38dB);

  uint8_t DataCodec[2] = {0};
	DataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
//	DataCodec[1] = 0b00000000;
	DataCodec[1] = 0b10000000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_INT_ENA;
	DataCodec[1] = 0b11100010;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_SYSTEM_CLK;
	DataCodec[1] = 0b00010000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_H;
	DataCodec[1] = 0b01100000;//0X6000
//	DataCodec[1] = 0b00110000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_STEREO_AUD_CLK_CTRL_L;
	DataCodec[1] = 0b00000000;//0X6000
//	DataCodec[1] = 0b00000000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_INTERFACE_MODE1;
	DataCodec[1] = 0b00110000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_INTERFACE_MODE2;
	DataCodec[1] = 0b00000000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_CODEC_FILTERS;
//	DataCodec[1] = 0b10000001;
	DataCodec[1] = 0b10000001;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_SIDETONE;
	DataCodec[1] = 0b00000000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_DAC_LVL;
//	DataCodec[1] = 0b00000000;
	DataCodec[1] = 0b00001111;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_ADC_LVL;
	DataCodec[1] = 0b00000000;
//	DataCodec[1] = 0b00000000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;

	/*line input */

	DataCodec[0] = MAX9867_REG_L_LINE_INPUT_LVL;
	DataCodec[1] = 0b10000000;
//	DataCodec[1] = 0b00000000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;

	DataCodec[0] = MAX9867_REG_R_LINE_INPUT_LVL;
	DataCodec[1] = 0b10000000;
//	DataCodec[1] = 0b00000000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;




	DataCodec[0] = MAX9867_REG_L_VOL_CTRL;
	DataCodec[1] = 0b00001001;
//	DataCodec[1] = 0b00011010;
//	DataCodec[1] = 0b00010111;
//	DataCodec[1] = 0b00001000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_R_VOL_CTRL;
	DataCodec[1] = 0b00001001;
//	DataCodec[1] = 0b00011010;
//	DataCodec[1] = 0b00011100;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_MODE;
	DataCodec[1] = 0b00000000;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;
	DataCodec[0] = MAX9867_REG_SYS_SHUTDOWN;
	DataCodec[1] = 0b10001100;
//	DataCodec[1] = 0b11111111;
	if( STATUS_OK != WriteI2C(MAX9867_I2C_HANDLE, MAX9867_SLAVE_ADDRESS_W, DataCodec, 2) )
			return STATUS_ERR;

//	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
	 HAL_UART_Receive_IT(&huart2, &rx[0], 4096);//4096
	 HAL_UART_Transmit(&huart2, &flag, 1, 2000);
	 flag=1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//		if(UARTFLAG == 109 || UARTFLAG == 1/* && dataReady == 1*/)
//		{
//			HAL_I2S_Transmit(&hi2s1, (uint16_t *)rx, 1024,2000);
//			HAL_UART_Receive_IT(&huart2, &rx[0], 2048);
//			HAL_UART_Transmit(&huart2, &flag, 1, 1000);
////			UARTFLAG = 0;
////		}
////
////		else if(UARTFLAG == 2/* && dataReady == 2*/)
////		{
//			HAL_I2S_Transmit(&hi2s1, (uint16_t *)&rx[2048], 1024,2000);
//			HAL_UART_Receive_IT(&huart2, &rx[2048], 2048);
//			HAL_UART_Transmit(&huart2, &flag, 1, 1000);
////			UARTFLAG = 0;
//		}
//	  for(int i=0;i<45319;i++)
//	  {
//		  HAL_I2S_Transmit(&hi2s1, &song[i], 1,1000);
//		  HAL_I2S_Transmit(&hi2s1, song, 45319,1000);
//	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 45;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0xC0000E12;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2S1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S1_Init(void)
{

  /* USER CODE BEGIN I2S1_Init 0 */

  /* USER CODE END I2S1_Init 0 */

  /* USER CODE BEGIN I2S1_Init 1 */

  /* USER CODE END I2S1_Init 1 */
  hi2s1.Instance = SPI1;
  hi2s1.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s1.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s1.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s1.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s1.Init.AudioFreq = 44100;
  hi2s1.Init.CPOL = I2S_CPOL_LOW;
  if (HAL_I2S_Init(&hi2s1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S1_Init 2 */

  /* USER CODE END I2S1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 2812000;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_EnableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, AMP_FS2_Pin|AMP_FS1_Pin|AMP_GAIN2_Pin|AMP_GAIN1_Pin
                          |AMP_SHUTDOWN_Pin|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : CODEC_IRQN_OUT_Pin */
  GPIO_InitStruct.Pin = CODEC_IRQN_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CODEC_IRQN_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : AMP_MUTE_Pin */
  GPIO_InitStruct.Pin = AMP_MUTE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AMP_MUTE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : AMP_FS2_Pin AMP_FS1_Pin AMP_GAIN2_Pin AMP_GAIN1_Pin
                           AMP_SHUTDOWN_Pin PB8 PB9 */
  GPIO_InitStruct.Pin = AMP_FS2_Pin|AMP_FS1_Pin|AMP_GAIN2_Pin|AMP_GAIN1_Pin
                          |AMP_SHUTDOWN_Pin|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Tx Half Transfer completed callbacks.
  * @param  hi2s: I2S handle
  */
//void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
//{
//
//  if(hi2s->Instance == SPI1)
//  {
//	  HAL_UART_Transmit(&huart2, &flag, 1, 1000);
//	  dataReady = 1;
//  }
//}
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);
  if(hi2s->Instance == SPI1)
  {
//	  HAL_I2S_DMAStop(&hi2s1);
//		if(dataReady == 1)
//		{
//			HAL_I2S_DMAStop(&hi2s1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
//			HAL_I2S_Transmit_DMA(&hi2s1, (uint16_t *)&rx[2048], 2048);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
//			memset(&rx[0],0,2048);
			HAL_UART_Receive_IT(&huart2, &rx[0], 2048);
			HAL_UART_Transmit(&huart2, &flag, 1, 1000);
//			dataReady = 2;
//		}
//		else if(dataReady == 2)
//		{
////			HAL_I2S_DMAStop(&hi2s1);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
////			HAL_I2S_Transmit_DMA(&hi2s1, (uint16_t *)rx, 2048);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
////			memset(&rx[2048],0,2048);
//			HAL_UART_Receive_IT(&huart2, &rx[2048], 2048);
//			HAL_UART_Transmit(&huart2, &flag, 1, 1000);
//			dataReady = 1;
//		}
  }
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);
  if(hi2s->Instance == SPI1)
  {
//	  HAL_I2S_DMAStop(&hi2s1);
//		if(dataReady == 1)
//		{
////			HAL_I2S_DMAStop(&hi2s1);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
////			HAL_I2S_Transmit_DMA(&hi2s1, (uint16_t *)&rx[2048], 2048);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
////			memset(&rx[0],0,2048);
//			HAL_UART_Receive_IT(&huart2, &rx[0], 2048);
//			HAL_UART_Transmit(&huart2, &flag, 1, 1000);
//			dataReady = 2;
//		}
//		else if(dataReady == 2)
//		{
//			HAL_I2S_DMAStop(&hi2s1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
//			HAL_I2S_Transmit_DMA(&hi2s1, (uint16_t *)rx, 2048);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
//			memset(&rx[2048],0,2048);
			HAL_UART_Receive_IT(&huart2, &rx[2048], 2048);
			HAL_UART_Transmit(&huart2, &flag, 1, 1000);
//			dataReady = 1;
//		}
  }
}
//void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
//{
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);
//  if(hi2s->Instance == SPI1)
//  {
////	  HAL_I2S_DMAStop(&hi2s1);
//		if(dataReady == 1)
//		{
////			HAL_I2S_DMAStop(&hi2s1);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
////			HAL_I2S_Transmit_DMA(&hi2s1, (uint16_t *)&rx[2048], 2048);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
////			memset(&rx[0],0,2048);
//			HAL_UART_Receive_IT(&huart2, &rx[0], 2048);
//			HAL_UART_Transmit(&huart2, &flag, 1, 1000);
//			dataReady = 2;
//		}
//		else if(dataReady == 2)
//		{
////			HAL_I2S_DMAStop(&hi2s1);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
////			HAL_I2S_Transmit_DMA(&hi2s1, (uint16_t *)rx, 2048);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
////			memset(&rx[2048],0,2048);
//			HAL_UART_Receive_IT(&huart2, &rx[2048], 2048);
//			HAL_UART_Transmit(&huart2, &flag, 1, 1000);
//			dataReady = 1;
//		}
//  }
//}

uint8_t once = 1;
uint8_t d1[10]={1,2,3,4,5,6,7,8,9,10};
/* for receive audio data on uart */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);
	if(huart->Instance == USART2)
	{
		if(once == 1)
		{

			HAL_I2S_Transmit_DMA(&hi2s1, (uint16_t *)rx, 2048);//1024
//			HAL_I2S_Transmit(&hi2s1, (uint16_t *)d1, 10,2000);
			once = 0;
		}
//		if(dataReady == 1)
//		{
//			dataReady = 2;
//			UARTFLAG = 1;
//		}
//
//		else if(dataReady == 2)
//		{
//			dataReady = 1;
//			UARTFLAG = 1;
//		}
//		if(UARTFLAG == 0)
//			UARTFLAG = 1;
//		else if(UARTFLAG == 1)
//			UARTFLAG = 2;
//		else if(UARTFLAG == 2)
//			UARTFLAG = 1;
	}
}


//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{
//    /* Check if a noise error occurred */
//    if(huart->ErrorCode & HAL_UART_ERROR_NE)
//    {
//        /* Handle noise error */
////        printf("UART noise error detected.\n");
//    }
//
//    /* Check if a framing error occurred */
//    if(huart->ErrorCode & HAL_UART_ERROR_FE)
//    {
//        /* Handle framing error */
////        printf("UART framing error detected.\n");
//    }
//
//    /* Check if an overrun error occurred */
//    if(huart->ErrorCode & HAL_UART_ERROR_ORE)
//    {
//        /* Handle overrun error */
////        printf("UART overrun error detected.\n");
//    }
//
//    /* Check if a parity error occurred */
//    if(huart->ErrorCode & HAL_UART_ERROR_PE)
//    {
//        /* Handle parity error */
////        printf("UART parity error detected.\n");
//    }
//
//    /* Check if a parity error occurred */
//    if(huart->ErrorCode & HAL_UART_ERROR_DMA)
//    {
//        /* Handle parity error */
////        printf("UART parity error detected.\n");
//    }
//
//    /* Check if a parity error occurred */
//    if(huart->ErrorCode & HAL_UART_ERROR_RTO)
//    {
//        /* Handle parity error */
////        printf("UART parity error detected.\n");
//    }
//
//    /* Clear the UART error flag */
//    huart->ErrorCode = HAL_UART_ERROR_NONE;
//}

void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
