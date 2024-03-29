/*
 BitzOS (BOS) V0.3.1 - Copyright (C) 2017-2024 Hexabitz
 All rights reserved

 File Name     : H05R0_i2c.h
 Description   :This file contains all the functions prototypes for
 the i2c

 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2S_H__
#define __I2S_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
extern I2S_HandleTypeDef   hi2s1;

/* USER CODE BEGIN Private defines */
/* I2C Typedef */
typedef I2S_HandleTypeDef 			I2S_HANDLE;

/* I2C port definitions */
#define I2S_PORT					&hi2s1
#define TIM_OUT_1MS         		1u


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
void MX_I2S1_Init(void);
Module_Status WriteI2S(I2S_HANDLE *xPort, uint16_t *pData, uint16_t Size);
Module_Status ReadI2S(I2S_HANDLE *xPort, uint16_t *rBuffer, uint16_t Size);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
