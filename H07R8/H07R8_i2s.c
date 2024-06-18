/*
 BitzOS (BOS) V0.3.1 - Copyright (C) 2017-2024 Hexabitz
 All rights reserved

 File Name     : H05R0_i2c.c
 Description   : This file provides code for the configuration
 of the I2C instances.

 */
/* Includes ------------------------------------------------------------------*/
#include "BOS.h"
#include "H07R8_i2s.h"
/* USER CODE BEGIN 0 */
I2S_HandleTypeDef hi2s1;
DMA_HandleTypeDef hdma_spi1_tx;
/* USER CODE BEGIN 1 */


/**
  * @brief I2S1 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2S1_Init(void)
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
  hi2s1.Init.AudioFreq = I2S_AUDIOFREQ_44K;
  hi2s1.Init.CPOL = I2S_CPOL_LOW;
  if (HAL_I2S_Init(&hi2s1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S1_Init 2 */

  /* USER CODE END I2S1_Init 2 */

}

/**
* @brief I2S MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2s: I2S handle pointer
* @retval None
*/
void HAL_I2S_MspInit(I2S_HandleTypeDef* hi2s)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hi2s->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2S1;
    PeriphClkInit.I2s1ClockSelection = RCC_I2S1CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2S1 GPIO Configuration
    PA4     ------> I2S1_WS
    PA5     ------> I2S1_CK
    PA6     ------> I2S1_MCK
    PA7     ------> I2S1_SD
    */
    GPIO_InitStruct.Pin = I2S_WS_Pin|I2S_CK_Pin|I2S_MCK_Pin|I2S_SD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(I2S_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */
    /* I2S1 DMA Init */
    /* SPI1_TX Init */
    hdma_spi1_tx.Instance = DMA1_Channel1;
    hdma_spi1_tx.Init.Request = DMA_REQUEST_SPI1_TX;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi1_tx.Init.Mode = DMA_CIRCULAR;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hi2s,hdmatx,hdma_spi1_tx);

    /* I2S1 interrupt Init */
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
  /* USER CODE END SPI1_MspInit 1 */
  }

}

/**
* @brief I2S MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2s: I2S handle pointer
* @retval None
*/
void HAL_I2S_MspDeInit(I2S_HandleTypeDef* hi2s)
{
  if(hi2s->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**I2S1 GPIO Configuration
    PA4     ------> I2S1_WS
    PA5     ------> I2S1_CK
    PA6     ------> I2S1_MCK
    PA7     ------> I2S1_SD
    */
    HAL_GPIO_DeInit(I2S_GPIO_Port, I2S_WS_Pin|I2S_CK_Pin|I2S_MCK_Pin|I2S_SD_Pin);
    /* I2S1 DMA DeInit */
    HAL_DMA_DeInit(hi2s->hdmatx);

    /* I2S1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SPI1_IRQn);
  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }

}

/*
 * @brief: write n bytes to an I2S register
 * @param1: I2S port handle
 * @param2: Data to be sent
 * @param3: Data size
 * @retval: Status
 */
Module_Status WriteI2S(I2S_HANDLE *xPort, uint16_t *pData, uint16_t Size)
{
	Module_Status Status = H07R8_OK;

  if (NULL!=xPort && NULL!=pData)
    {
      if (HAL_OK == HAL_I2S_Transmit(xPort, pData, Size, TIM_OUT_1MS))
    	  Status = H07R8_OK;

    }
  else
	  	  Status = H07R8_ERROR;

  return Status;
}

/*
 * @brief: read n bytes from an I2S device
 * @param1: I2C port handle
 * @param3: Pointer to buffer to store received data in
 * @param4: Data size
 * @retval: Status
 */
Module_Status ReadI2S(I2S_HANDLE *xPort, uint16_t *rBuffer, uint16_t Size)
{
	Module_Status Status = H07R8_OK;

	if (NULL!=xPort && NULL!=rBuffer)
	{
	    if (HAL_OK == HAL_I2S_Receive(xPort, rBuffer, Size, TIM_OUT_1MS))
	    	Status = H07R8_OK;
	}
	else
			Status = H07R8_ERROR;

	return Status;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
