/*
 * Porting.c
 * Description: Porting source file
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

#include "Porting.h"

Status_TypeDef SetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin)
{
	Status_TypeDef Status;

	if (NULL!=GPIOx)
	{
		HAL_GPIO_WritePin(GPIOx, Pin, GPIO_PIN_SET);
		Status=STATUS_OK;
	}
	else
		Status=STATUS_ERR;

	return Status;
}

/*
 * set a GPIO pin form a port low
 */
Status_TypeDef ResetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin)
{
	Status_TypeDef Status;

	if (NULL!=GPIOx)
	{
		HAL_GPIO_WritePin(GPIOx, Pin, GPIO_PIN_RESET);
		Status=STATUS_OK;
	}
	else
		Status=STATUS_ERR;

	return Status;
}

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

Status_TypeDef WriteI2S(I2S_HANDLE *xPort, uint16_t *pData, uint16_t Size)
{
  Status_TypeDef Status=STATUS_ERR;

  if (NULL!=xPort && NULL!=pData)
    {
      if (HAL_OK == HAL_I2S_Transmit(xPort, pData, Size, TIM_OUT_1MS))
    	  Status=STATUS_OK;

    }
  else
	  	  Status=STATUS_ERR;

  return Status;
}

Status_TypeDef ReadI2S(I2S_HANDLE *xPort, uint16_t *rBuffer, uint16_t Size)
{
	Status_TypeDef Status;

	if (NULL!=xPort && NULL!=rBuffer)
	{
	    if (HAL_OK == HAL_I2S_Receive(xPort, rBuffer, Size, TIM_OUT_1MS))
	    	Status=STATUS_OK;
	}
	else
			Status=STATUS_ERR;

	return Status;
}

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
