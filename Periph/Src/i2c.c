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




int I2C_Init(I2C_TypeDef* I2Cx) {

  /* Condigure PB6 - SCL, PB7 - SDA, 2MHz frequency, alternative*/

  if (I2Cx == I2C1) {
    MODIFY_REG(I2C_Port->CRL,
      (SCL_Pin_Mask | SDA_Pin_Mask), (
        ((GPIO_AF_OD | GPIO_IOS_2) << (SCL_Pin_Pos * 4U))
      | ((GPIO_AF_OD | GPIO_IOS_2) << (SDA_Pin_Pos * 4U))
    ));
  }

  /* Set I2C frequency */
  MODIFY_REG(I2Cx->CR2, I2C_CR2_FREQ_Msk, APB1_FREQ/1000000);
  MODIFY_REG(I2Cx->CCR, I2C_CCR_CCR_Msk, (((I2C_TRISE_SM/2)/(1000/(APB1_FREQ/1000000)))+1));
  MODIFY_REG(I2Cx->TRISE, I2C_TRISE_TRISE_Msk, ((1000/(1000/(APB1_FREQ/1000000)))+1));
  
  
  if (I2Cx == I2C1) {
    // NVIC_SetPriority(I2C1_ER_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
    // NVIC_EnableIRQ(I2C1_ER_IRQn);
    return (0);
  }


  return (1);
}




/**
 * @brief  Starts (sends start condition) to the given I2C peripherals
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval (int) status of operation
 */
int I2C_Start(I2C_TypeDef* I2Cx){

  
  /* Stast I2C Peripherals enable */
  PREG_SET(I2Cx->CR1, I2C_CR1_PE_Pos);
  /* Generate start condition */
  PREG_SET(I2Cx->CR1, I2C_CR1_START_Pos);
  
  /* Wait until the start bit is set*/
  uint32_t tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_SB_Pos))) {
    if (!(--tmout)) {
      I2C_Stop(I2Cx);
      return (1);
    }
  }
  
  /* Verify master mode*/
  tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR2_MSL_Pos))) {
    if (!(--tmout)) {
      I2C_Stop(I2Cx);
      return (1);
    }
  }
  return (0);
}




/**
 * @brief  Sends slave's address to the given I2C bus
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  addr: slave address
 * @retval (int) status of operation
 */
int I2C_SendAddress(I2C_TypeDef* I2Cx, uint8_t addr){

  
  /* Send the slave address into the bus */
  I2Cx->DR = addr<<1;
  
  /* Wait until address is sent */
  uint32_t tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_ADDR_Pos))) {
    if (!(--tmout)) {
      I2C_Stop(I2Cx);
      return (1);
    }
  }

  /* Verify before transferring if trasmit buffer is empty */
  tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_TXE_Pos))) {
    if (!(--tmout)) {
      I2C_Stop(I2Cx);
      return (1);
    }
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
 * @retval (int) status of operation
 */
int I2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t txByte){

  
  /* Send data byte to the couterpart */
  I2Cx->DR = txByte;

  /* Verify if byte transfer finished */
  uint32_t tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_BTF_Pos))) {
    if (!(--tmout)) {
      I2C_Stop(I2Cx);
      return (1);
    }
  }
  
  /* Verify after transferring if trasmit buffer is empty */
  tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_TXE_Pos))) {
    if (!(--tmout)) {
      I2C_Stop(I2Cx);
      return (1);
    }
  }
  return (0);  
}




/**
 * @brief  Reads a byte value from the given I2C bus
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval A byte to receive from the given bus
 */
uint8_t I2C_ReadByte(I2C_TypeDef* I2Cx){

  
  /* Receive data byte from the couterpart */
  uint8_t rxByte = I2Cx->DR;
  
  /* Verify if byte transfer finished */
  uint32_t tmout = I2C_BUS_TMOUT;
  while(!(PREG_CHECK(I2Cx->SR1, I2C_SR1_BTF_Pos))) {
    if (!(--tmout)) {
      I2C_Stop(I2Cx);
      return (1);
    }
  }

  /* Verify after transferring if trasmit buffer is empty */
  tmout = I2C_BUS_TMOUT;
  while(PREG_CHECK(I2Cx->SR1, I2C_SR1_RXNE_Pos)) {
    if (!(--tmout)) {
      I2C_Stop(I2Cx);
      return (1);
    }
  }
  
  return rxByte;
}




/**
  * @brief  Writes 8 bit data via I2C
  * @param  I2Cx: pointer to an I2C instance
  * @param  data: a byte to send
  * @return None
  */
int I2C_Write(I2C_TypeDef *I2Cx, uint8_t slaveAddr, uint8_t *buf, uint16_t len) {

  return (0);
}






/**
  * @brief  Reads 8 bit data via I2C
  * @param  I2Cx: pointer to an I2C instance
  * @return  a received byte
  */
int I2C_Read(I2C_TypeDef *I2Cx, uint8_t slaveAddr, uint8_t reg, uint8_t *buf, uint16_t len) {

  return (0);
}



