/**
  ******************************************************************************
  * @file           : i2c.h
  * @brief          : This file contains header definitions for I2C peripherals code
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Askug Ltd.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef _I2C_H
#define _I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private defines -----------------------------------------------------------*/
#define I2C_BUS_TMOUT   10000 /* cycles timeout on I2C bus operations */

/* Exported functions prototypes ---------------------------------------------*/
int I2C_Start(I2C_TypeDef*);
void I2C_Stop(I2C_TypeDef*);
int I2C_SendAddress(I2C_TypeDef*, uint8_t);
int I2C_WriteByte(I2C_TypeDef*, uint8_t);
uint8_t I2C_ReadByte(I2C_TypeDef*);



#ifdef __cplusplus
}
#endif
#endif // _I2C_H
