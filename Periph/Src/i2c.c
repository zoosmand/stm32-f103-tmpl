/**
  ******************************************************************************
  * @file           : display.c
  * @brief          : This file contains display code
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


void I2C_Start(I2C_TypeDef* I2Cx){
  /* Stast I2C Peripherals enable */
  PREG_SET(I2Cx->CR1, I2C_CR1_PE_Pos);
  /* Generate start condition */
  PREG_SET(I2Cx->CR1, I2C_CR1_START_Pos);

  /* Wait until the start bit is set*/
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_SB_Pos)));
  /* Verify master mode*/
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR2_MSL_Pos)));

}

void I2C_SendAddress(I2C_TypeDef* I2Cx, uint8_t addr){
 /* Send the slave address into the bus */
  I2Cx->DR = addr<<1;
  
  /* Wait until address is sent */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_ADDR_Pos)));
  /* Verify before transferring if trasmit buffer is empty */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_TXE_Pos)));
  
  /* Clear status registers */
  (void)I2C1->SR1;
  (void)I2C1->SR2;
 }


void I2C_Stop(I2C_TypeDef* I2Cx){
  /* Generate stop condition */
  PREG_CLR(I2Cx->CR1, I2C_CR1_STOP_Pos);
  /* Stop the Peripherals */
  PREG_CLR(I2Cx->CR1, I2C_CR1_PE_Pos);
  /* Clear status registers */
  (void)I2C1->SR1;
  (void)I2C1->SR2;
}


void I2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t RxByte){
  
  /* Send data byte to the couterpart */
  I2Cx->DR = RxByte;
  /* Verify if byte transfer finished */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_BTF_Pos)));
  /* Verify after transferring if trasmit buffer is empty */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_TXE_Pos)));
  
}

uint8_t I2C_ReadByte(I2C_TypeDef* I2Cx){
  
  /* Receive data byte from the couterpart */
  uint8_t TxByte = I2Cx->DR;
  /* Verify if byte transfer finished */
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_BTF_Pos)));
  /* Verify after transferring if trasmit buffer is empty */
  while(PREG_CHECK(I2Cx->SR1, I2C_SR1_RXNE_Pos));
  
}

