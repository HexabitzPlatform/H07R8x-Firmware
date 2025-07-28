/*
 BitzOS (BOS) V0.2.9 - Copyright (C) 2017-2023 Hexabitz
 All rights reserved

 File Name     : H07R8_gpio.h
 Description   : Header file contains all the functions prototypes for
 the GPIO .

 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"
typedef GPIO_TypeDef GPIO_HANDLE;

//typedef enum {
//	H07R8_OK =0,
//	H07R8_ERR_UnknownMessage,
//	H07R8_ERR_WrongParams,
//	H07R8_ERROR =255
//} Module_Status_t;

extern void GPIO_Init(void);
extern void IND_LED_Init(void);

//extern Module_Status_t SetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin);
//extern Module_Status_t ResetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin);
uint8_t IsFactoryReset(void);
BOS_Status GetPortGPIOs(uint8_t port,uint32_t *TX_Port,uint16_t *TX_Pin,uint32_t *RX_Port,uint16_t *RX_Pin);
Module_Status SetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin);
Module_Status ResetGPIOsPin(GPIO_HANDLE *GPIOx, uint16_t Pin);
void MX_GPIO_Init(void);
#ifdef __cplusplus
}
#endif
#endif /*__gpio_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
