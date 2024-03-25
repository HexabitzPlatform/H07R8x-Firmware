/*
 * Porting.h
 * Description: Porting header file
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
#ifndef INC_PORTING_H_
#define INC_PORTING_H_

#include "main.h"
/* GPIOs type define */
typedef GPIO_TypeDef GPIO_HANDLE;

extern  I2C_HandleTypeDef                   hi2c2;
typedef I2C_HandleTypeDef 					I2C_HANDLE;
typedef I2S_HandleTypeDef 					I2S_HANDLE;
#define TIM_OUT_1MS         				50
#define MAX9867_I2C_HANDLE            		&hi2c2
#define MAX9867_I2S_HANDLE            		&hi2s1

Status_TypeDef SetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin);
Status_TypeDef ResetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin);
Status_TypeDef WriteI2C(I2C_HANDLE *xPort, uint16_t sAddress, uint8_t *pData, uint16_t Size);
Status_TypeDef ReadI2C(I2C_HANDLE *xPort, uint16_t sAddress, uint8_t *rBuffer, uint16_t Size);
Status_TypeDef WriteI2S(I2S_HANDLE *xPort, uint16_t *pData, uint16_t Size);
Status_TypeDef ReadI2S(I2S_HANDLE *xPort, uint16_t *rBuffer, uint16_t Size);

#endif /* INC_PORTING_H_ */

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
