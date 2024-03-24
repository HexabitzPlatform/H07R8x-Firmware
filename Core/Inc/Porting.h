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

Status_TypeDef WriteI2C(I2C_HANDLE *xPort, uint16_t sAddress, uint8_t *pData, uint16_t Size);
Status_TypeDef ReadI2C(I2C_HANDLE *xPort, uint16_t sAddress, uint8_t *rBuffer, uint16_t Size);
Status_TypeDef WriteI2S(I2S_HANDLE *xPort, uint16_t *pData, uint16_t Size);
Status_TypeDef ReadI2S(I2S_HANDLE *xPort, uint16_t *rBuffer, uint16_t Size);

#endif /* INC_PORTING_H_ */

/************************ (C) COPYRIGHT Hexabitz *****END OF FILE****/
