/**
  ******************************************************************************
  * @file           : i2c.c
  * @brief          : This file contains I2C peripherals code
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

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"


/**
 * @brief  Starts (sends start condition) to the given I2C peripherals
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval None
 */
int I2C_Start(I2C_TypeDef* I2Cx){

  uint32_t timeout_threshold = sysCnt + I2C_BUS_TMOUT;

  /* Stast I2C Peripherals enable */
  PREG_SET(I2Cx->CR1, I2C_CR1_PE_Pos);
  /* Generate start condition */
  PREG_SET(I2Cx->CR1, I2C_CR1_START_Pos);

  /* Wait until the start bit is set*/
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_SB_Pos))) {
    if (sysCnt >= timeout_threshold) {
      PREG_CLR(I2Cx->CR1, I2C_CR1_PE_Pos);
      return (1);
    }
  }
  
  timeout_threshold = sysCnt + I2C_BUS_TMOUT;
  /* Verify master mode*/
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR2_MSL_Pos))) {
    if (sysCnt >= timeout_threshold) {
      PREG_CLR(I2Cx->CR1, I2C_CR1_PE_Pos);
      return (1);
    }
  }
  return (0);
}


/**
 * @brief  Sends slave's address to the given I2C bus
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  addr: slave address
 * @retval None
 */
int I2C_SendAddress(I2C_TypeDef* I2Cx, uint8_t addr){

  uint32_t timeout_threshold = sysCnt + I2C_BUS_TMOUT;

  /* Send the slave address into the bus */
  I2Cx->DR = addr<<1;
  
  /* Wait until address is sent */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_ADDR_Pos))) {
    if (sysCnt >= timeout_threshold) return (1);
  }
  /* Verify before transferring if trasmit buffer is empty */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_TXE_Pos))) {
    if (sysCnt >= timeout_threshold) return (1);
  }
  
  /* Clear status registers */
  (void)I2C1->SR1;
  (void)I2C1->SR2;

  return (0);
 }


/**
 * @brief  Starts (sends stop condition) to the given I2C bus
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval None
 */
void I2C_Stop(I2C_TypeDef* I2Cx){

  /* Generate stop condition */
  PREG_CLR(I2Cx->CR1, I2C_CR1_STOP_Pos);
  /* Stop the Peripherals */
  PREG_CLR(I2Cx->CR1, I2C_CR1_PE_Pos);
  /* Clear status registers */
  (void)I2C1->SR1;
  (void)I2C1->SR2;
}


/**
 * @brief  Writes a byte value to the given I2C bus
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  txByte: byte to send to the given bus
 * @retval None
 */
int I2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t txByte){

  uint32_t timeout_threshold = sysCnt + I2C_BUS_TMOUT;

  /* Send data byte to the couterpart */
  I2Cx->DR = txByte;
  /* Verify if byte transfer finished */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_BTF_Pos))) {
    if (sysCnt >= timeout_threshold) return (1);
  }
  /* Verify after transferring if trasmit buffer is empty */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_TXE_Pos))) {
    if (sysCnt >= timeout_threshold) return (1);
  }
  return (0);  
}


/**
 * @brief  Reads a byte value from the given I2C bus
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval A byte to receive from the given bus
 */
uint8_t I2C_ReadByte(I2C_TypeDef* I2Cx){

  uint32_t timeout_threshold = sysCnt + I2C_BUS_TMOUT;

  /* Receive data byte from the couterpart */
  uint8_t rxByte = I2Cx->DR;
  /* Verify if byte transfer finished */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_BTF_Pos))) {
    if (sysCnt >= timeout_threshold) return (0xff);
  }
  /* Verify after transferring if trasmit buffer is empty */
  while(PREG_CHECK(I2Cx->SR1, I2C_SR1_RXNE_Pos)) {
    if (sysCnt >= timeout_threshold) return (0xff);
  }
  
  return rxByte;
}

