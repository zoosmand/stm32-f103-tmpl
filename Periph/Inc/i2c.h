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
ErrorStatus I2C_Init(I2C_TypeDef*);
ErrorStatus I2C_Start(I2C_TypeDef*);
void I2C_Stop(I2C_TypeDef*);

/**
 * @brief  Transmits slave device address into the given I2C bus.
 * @param  I2Cx: pointer to the give I2C struct 
 * @param  addr: slave device address
 * @param  dir: data transmission direction (READ|WRITE, RX|TX)
 * @retval (int) status of operation
 */
ErrorStatus I2C_SendAddress(I2C_TypeDef*, uint8_t, DataTransmitDirection_TypeDef);

ErrorStatus I2C_WriteByte(I2C_TypeDef*, uint8_t);
uint8_t I2C_ReadByte(I2C_TypeDef*);







/**
  * @brief  Transmits in master mode an amount of data.
  * @param  I2Cx Pointer to a I2C_TypeDef I2Cx.
  * @param  slaveAddress Target I2Cx address: The I2Cx 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  buf Pointer to data buffer
  * @param  len Amount of data to be sent
  * @return transmit status
  */
ErrorStatus I2C_Master_Send(I2C_TypeDef *I2Cx, uint16_t slaveAddr, uint8_t *buf, uint32_t len);

/**
  * @brief  Receives in master mode an amount of data.
  * @param  I2Cx Pointer to a I2C_TypeDef I2Cx.
  * @param  slaveAddress Target I2Cx address: The I2Cx 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  buf Pointer to data buffer
  * @param  len Amount of data to be sent
  * @return receive status
  */
ErrorStatus I2C_Master_Receive(I2C_TypeDef *I2Cx, uint16_t slaveAddr, uint8_t *buf, uint16_t len);


#ifdef __cplusplus
}
#endif
#endif // __I2C_H
