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
void WH1602_WriteCommand(I2C_TypeDef*, uint8_t, uint16_t);
void WH1602_I2C_Write(I2C_TypeDef* I2Cx, uint8_t, const char*);





/******************************************************************************/
void WH1602_I2C_Init(I2C_TypeDef* I2CPort){
  
  SimpleDelay(50000);
 
  uint8_t params[14] = {
    0x03, 0x05,
    0x03, 0x05,
    0x03, 0x01,
    0x02, 0x02,
    0x28, 0x01,
    0x0c, 0x01,
    0x01, 0x00
  }; 
  
  for(uint8_t i = 0; i < sizeof(params); i++){
    WH1602_I2C_WriteByte(I2CPort, ((params[i] & 0xf0) | (_E + _Bl)));
    WH1602_I2C_WriteByte(I2CPort, ((params[i] & 0xf0) | _Bl));
    WH1602_I2C_WriteByte(I2CPort, (((params[i] << 4) & 0xf0) | (_E + _Bl)));
    WH1602_I2C_WriteByte(I2CPort, (((params[i] << 4) & 0xf0) | _Bl));
    SimpleDelay(params[i++]*1000);
  }

  SimpleDelay(1000);

  WH1602_WriteChar(I2CPort, 'K');
}








void WH1602_I2C_WriteByte(I2C_TypeDef* I2CPort, uint8_t RxByte){
  /* Stast I2C Peripherals enable */
  PREG_SET(I2CPort->CR1, I2C_CR1_PE_Pos);
  /* Generate start condition */
  PREG_SET(I2CPort->CR1, I2C_CR1_START_Pos);
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
  
  (void)I2C1->SR1;
  (void)I2C1->SR2;
  
  /* Send data byte to the slave */
  I2CPort->DR = RxByte;
  /* Verify if byte transfer finished */
  /* TODO Care of ACK NACK and bus errors  */
  while(!(PREG_CHECK(I2CPort->SR1, I2C_SR1_BTF_Pos)));
  /* Verify after transferring if trasmit buffer is empty */
  while(!(PREG_CHECK(I2CPort->SR1, I2C_SR1_TXE_Pos)));
  
  /* Generate stop condition */
  PREG_CLR(I2CPort->CR1, I2C_CR1_STOP_Pos);
  /* Stop the Peripherals */
  PREG_CLR(I2CPort->CR1, I2C_CR1_PE_Pos);
}







void WH1602_WriteChar(I2C_TypeDef* I2CPort, uint8_t charValue){  
  WH1602_I2C_WriteByte(I2CPort, ((charValue & 0xf0) | (_E + _Bl + _Rs)));
  WH1602_I2C_WriteByte(I2CPort, ((charValue & 0xf0) | (_Bl + _Rs)));
  WH1602_I2C_WriteByte(I2CPort, (((charValue << 4) & 0xf0) | (_E + _Bl + _Rs)));
  WH1602_I2C_WriteByte(I2CPort, (((charValue << 4) & 0xf0) | (_Bl + _Rs)));
  SimpleDelay(50);
}






void WH1602_WriteCommand(I2C_TypeDef* I2CPort, uint8_t commandValue, uint16_t delay){  
  WH1602_I2C_WriteByte(I2CPort, ((commandValue & 0xf0) | (_E + _Bl)));
  WH1602_I2C_WriteByte(I2CPort, ((commandValue & 0xf0) | _Bl));
  WH1602_I2C_WriteByte(I2CPort, (((commandValue << 4) & 0xf0) | (_E + _Bl)));
  WH1602_I2C_WriteByte(I2CPort, (((commandValue << 4) & 0xf0) | _Bl));
  SimpleDelay(delay);
}




uint32_t WH1602_I2C_LineLength(const char* charLine){
  uint32_t len = 0;
  while(*charLine){
    len++;
    charLine++;
    if (*charLine == 0x0a) break;
  }
  return len;
}




void WH1602_I2C_Write(I2C_TypeDef* I2Cx, uint8_t dsplAddress, const char* charLine){
  
  printf("%s\n", charLine);
  WH1602_WriteCommand(I2C1, dsplAddress, 50);
  
  uint32_t len = WH1602_I2C_LineLength(charLine);
  
  for(uint32_t i = 0 ; i < len ; i++){
    WH1602_WriteChar(I2Cx, (*charLine++));
  }
}



void WH1602_I2C_ReadByte(I2C_TypeDef* I2Cx, uint8_t RxByte){
}


void WH1602_I2C_ReadBuffer(I2C_TypeDef* I2Cx, uint16_t RxLength, uint8_t* RxBuffer){
}


