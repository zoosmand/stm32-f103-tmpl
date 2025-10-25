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
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private defines -----------------------------------------------------------*/
#define I2C_BUS_TMOUT   10000 /* cycles timeout on I2C bus operations */

#define SCL_Pin        GPIO_PIN_6
#define SCL_Pin_Pos    GPIO_PIN_6_Pos
#define SCL_Pin_Mask   GPIO_PIN_6_Mask
#define SDA_Pin        GPIO_PIN_7
#define SDA_Pin_Pos    GPIO_PIN_7_Pos
#define SDA_Pin_Mask   GPIO_PIN_7_Mask
#define I2C_Port       GPIOB


#define I2C_SM_FREQ   100
#define I2C_FM_FREQ   400

#define I2C_TRISE_SM  1000000/I2C_SM_FREQ /* 10000ns */
#define I2C_TRISE_FM  1000000/I2C_FM_FREQ /* 2500ns */

/* Private defines -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
int I2C_Init(I2C_TypeDef*);
int I2C_Start(I2C_TypeDef*);
void I2C_Stop(I2C_TypeDef*);
int I2C_SendAddress(I2C_TypeDef*, uint8_t);
int I2C_WriteByte(I2C_TypeDef*, uint8_t);
uint8_t I2C_ReadByte(I2C_TypeDef*);


/**
  * @brief  Writes 8 bit data via I2C
  * @param  I2Cx: pointer to an I2C instance
  * @param  data: a byte to send
  * @return None
  */
int I2C_Write(I2C_TypeDef*, uint8_t, uint8_t*, uint16_t);

/**
  * @brief  Reads 8 bit data via I2C
  * @param  I2Cx: pointer to an I2C instance
  * @return  a received byte
  */
int I2C_Read(I2C_TypeDef*, uint8_t, uint8_t, uint8_t*, uint16_t);

#ifdef __cplusplus
}
#endif
#endif // __I2C_H
