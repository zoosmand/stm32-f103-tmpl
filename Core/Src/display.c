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
#include "display.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void WH1602_I2C_WriteByte(I2C_TypeDef*, uint8_t);
void WH1602_WriteChar(I2C_TypeDef*, uint8_t);
void WH1602_WriteCommand(I2C_TypeDef*, uint8_t, uint32_t);
void WH1602_I2C_Write(I2C_TypeDef* I2Cx, uint8_t, const char*);





/******************************************************************************/
void WH1602_I2C_Init(I2C_TypeDef* I2CPort){
  
  SimpleDelay(15000);
 
  uint16_t params[14] = {
    _8BBUS_,      4100,
    _8BBUS_,      100,
    _8BBUS_,      40,
    _CURUPLEFT_,  1640,
    _4BBUS2L_,    40,
    _DSPLSW_,     40,
    _CLRDSLP_,    1640
  }; 
  
  for(uint8_t i = 0; i < sizeof(params); i++){
    WH1602_I2C_WriteByte(I2CPort, _WR1NCMD((uint8_t)params[i]));
    WH1602_I2C_WriteByte(I2CPort, _WR2NCMD((uint8_t)params[i]));
    WH1602_I2C_WriteByte(I2CPort, _WR1NCMD(((uint8_t)params[i] << 4)));
    WH1602_I2C_WriteByte(I2CPort, _WR2NCMD(((uint8_t)params[i] << 4)));
    SimpleDelay(params[i++]);
  }

  WH1602_WriteCommand(I2CPort, _CLRDSLP_, 1640);
  WH1602_I2C_Write(I2CPort, 1, "QWErtyuiop");
  WH1602_I2C_Write(I2CPort, 2, "1234567890");

}








void WH1602_I2C_WriteByte(I2C_TypeDef* I2CPort, uint8_t RxByte){
  /* Stast I2C Peripherals enable */
  PREG_SET(I2CPort->CR1, I2C_CR1_PE_Pos);
  /* Generate start condition */
  PREG_SET(I2CPort->CR1, I2C_CR1_START_Pos);
  
  /* TODO Implement bus errors cycle */
  /* TODO Implement timeout in cycles */
  
  /* Wait until the start bit is set*/
  while(!(PREG_CHECK(I2CPort->SR1, I2C_SR1_SB_Pos)));
  /* Verify master mode*/
  while(!(PREG_CHECK(I2CPort->SR1, I2C_SR2_MSL_Pos)));
  
  /* Send the slave address into the bus */
  I2CPort->DR = _ADDR_<<1;
  
  /* Wait until address is sent */
  while(!(PREG_CHECK(I2CPort->SR1, I2C_SR1_ADDR_Pos)));
  /* Verify before transferring if trasmit buffer is empty */
  while(!(PREG_CHECK(I2CPort->SR1, I2C_SR1_TXE_Pos)));
  
  /* Clear status registers */
  (void)I2C1->SR1;
  (void)I2C1->SR2;
  
  /* Send data byte to the slave */
  I2CPort->DR = RxByte;
  /* Verify if byte transfer finished */
  while(!(PREG_CHECK(I2CPort->SR1, I2C_SR1_BTF_Pos)));
  /* Verify after transferring if trasmit buffer is empty */
  while(!(PREG_CHECK(I2CPort->SR1, I2C_SR1_TXE_Pos)));
  
  /* Generate stop condition */
  PREG_CLR(I2CPort->CR1, I2C_CR1_STOP_Pos);
  /* Stop the Peripherals */
  PREG_CLR(I2CPort->CR1, I2C_CR1_PE_Pos);
}







void WH1602_WriteChar(I2C_TypeDef* I2CPort, uint8_t ch){  
  WH1602_I2C_WriteByte(I2CPort, _WR1NCHAR(ch));
  WH1602_I2C_WriteByte(I2CPort, _WR2NCHAR(ch));
  WH1602_I2C_WriteByte(I2CPort, _WR1NCHAR(ch << 4));
  WH1602_I2C_WriteByte(I2CPort, _WR2NCHAR(ch << 4));
  SimpleDelay(40);
}






void WH1602_WriteCommand(I2C_TypeDef* I2CPort, uint8_t cmd, uint32_t delay){  
  WH1602_I2C_WriteByte(I2CPort, _WR1NCMD(cmd));
  WH1602_I2C_WriteByte(I2CPort, _WR2NCMD(cmd));
  WH1602_I2C_WriteByte(I2CPort, _WR1NCMD(cmd << 4));
  WH1602_I2C_WriteByte(I2CPort, _WR2NCMD(cmd << 4));
  SimpleDelay(delay);
}




uint32_t WH1602_I2C_LineLength(const char* buf){
  uint32_t len = 0;
  while(*buf){
    len++;
    buf++;
    if (*buf == 0x0a) break;
  }
  return len;
}




void WH1602_I2C_Write(I2C_TypeDef* I2Cx, uint8_t line, const char* buf){
  
  // printf("%s\n", buf);
  uint8_t dsplAddr = (line == 2) ? 0xc0 : 0x80;
  WH1602_WriteCommand(I2C1, dsplAddr, 40);
  
  uint32_t len = WH1602_I2C_LineLength(buf);
  
  for(uint32_t i = 0 ; i < len ; i++){
    WH1602_WriteChar(I2Cx, (*buf++));
  }
}



void WH1602_I2C_ReadByte(I2C_TypeDef* I2Cx, uint8_t RxByte){
}


void WH1602_I2C_ReadBuffer(I2C_TypeDef* I2Cx, uint16_t RxLength, uint8_t* RxBuffer){
}


