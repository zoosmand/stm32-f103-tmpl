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
static __attribute__((section(".cron"))) uint32_t _DSPLREG_    = 0;



/* Private function prototypes -----------------------------------------------*/
void WH1602_WriteChar(I2C_TypeDef*, uint8_t);
void WH1602_WriteCommand(I2C_TypeDef*, uint8_t, uint32_t);
uint32_t WH1602_I2C_BufferLength(const char*);
void WH1602_I2C_ReadByte(I2C_TypeDef*, uint8_t);





/**
 * @brief  Initializes WH1602A display
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval None
 */
void WH1602_I2C_Init(I2C_TypeDef* I2Cx) {
  
  /* Initial delay according 1602a documentation */
  SimpleDelay(15000);
 
  /* Initial parameter-delay pairs */
  uint16_t params[14] = {
    _1602A_8BBUS_,      4100,
    _1602A_8BBUS_,      100,
    _1602A_8BBUS_,      40,
    _1602A_CURUPLEFT_,  1640,
    _1602A_4BBUS2L_,    40,
    _1602A_DSPLSW_,     40,
    _1602A_CLRDSLP_,    1640
  }; 
  
  I2C_Start(I2Cx);
  I2C_SendAddress(I2Cx, _1602A_ADDR_);
  for(uint8_t i = 0; i < sizeof(params); i++) {
    I2C_WriteByte(I2Cx, _WR1NCMD((uint8_t)params[i]));
    I2C_WriteByte(I2Cx, _WR2NCMD((uint8_t)params[i]));
    I2C_WriteByte(I2Cx, _WR1NCMD(((uint8_t)params[i] << 4)));
    I2C_WriteByte(I2Cx, _WR2NCMD(((uint8_t)params[i] << 4)));
    SimpleDelay(params[i++]);
  }
  I2C_Stop(I2Cx);
}


/**
 * @brief  Writes/Sends a charachter symbol to WH1602A display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  ch: ACSII charachter
 * @retval None
 */
void WH1602_WriteChar(I2C_TypeDef* I2Cx, uint8_t ch) {
  I2C_WriteByte(I2Cx, _WR1NCHAR(ch));
  I2C_WriteByte(I2Cx, _WR2NCHAR(ch));
  I2C_WriteByte(I2Cx, _WR1NCHAR(ch << 4));
  I2C_WriteByte(I2Cx, _WR2NCHAR(ch << 4));
  SimpleDelay(40);
}


/**
 * @brief  Writes/Sends a command to WH1602A display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  cmd: 1602a command
 * @param  delay: command delay according documentation
 * @retval None
 */
void WH1602_WriteCommand(I2C_TypeDef* I2Cx, uint8_t cmd, uint32_t delay) {  
  I2C_WriteByte(I2Cx, _WR1NCMD(cmd));
  I2C_WriteByte(I2Cx, _WR2NCMD(cmd));
  I2C_WriteByte(I2Cx, _WR1NCMD(cmd << 4));
  I2C_WriteByte(I2Cx, _WR2NCMD(cmd << 4));
  SimpleDelay(delay);
}


/**
 * @brief  Writes/Sends a command to WH1602A display
 * @param  buf: pointer to the charachter/text buffer
 * @retval The buffer length 
 */
uint32_t WH1602_I2C_BufferLength(const char* buf){
  uint32_t len = 0;
  while(*buf){
    len++;
    buf++;
    if (*buf == 0x0a) break;
  }
  return len;
}


/**
 * @brief  Writes/Sends a text buffer to WH1602A display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  line: 1602a display line [1,2]
 * @param  extraCmd: extra 1602a command, e.g. clear display
 * @param  buf: pointer to the charachter/text buffer
 * @retval None
 */
void WH1602_I2C_Write(I2C_TypeDef* I2Cx, uint8_t line, uint8_t extraCmd, const char* buf) {
  
  I2C_Start(I2Cx);
  I2C_SendAddress(I2Cx, _1602A_ADDR_);

  if (extraCmd) {
    WH1602_WriteCommand(I2Cx, extraCmd, 1640);
  }

  uint8_t dsplAddr = (line == 2) ? 0xc0 : 0x80;
  WH1602_WriteCommand(I2C1, dsplAddr, 40);
  
  uint32_t len = WH1602_I2C_BufferLength(buf);
  
  for(uint32_t i = 0 ; i < len ; i++) {
    WH1602_WriteChar(I2Cx, (*buf++));
  }
  I2C_Stop(I2Cx);
}



/**
 * @brief  Reads a byte from WH1602A display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  rxByte: received byte
 * @retval None
 */
void WH1602_I2C_ReadByte(I2C_TypeDef* I2Cx, uint8_t rxByte) {

}


/**
 * @brief  Reads a text buffer from WH1602A display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  bufLen: received buffer length
 * @param  buf: pointer to the charachter/text buffer to receive
 * @retval None
 */
void WH1602_I2C_Read(I2C_TypeDef* I2Cx, uint16_t bufLen, uint8_t* buf) {

}


/**
 * @brief  Writes/Sends cgarachter to the given display
 * @param  ch: charachter to write
 * @param  dspl: display index #TODO define desctop selection
 * @retval None
 */
void PrintCharDisplay(char ch, uint8_t dspl){

  // WH1602_I2C_Write(I2C1, 1, _1602A_CLRDSLP_, &ch);
  // WH1602_I2C_Write(I2Cx, 2, _1602A_NOCMD_, "1234567890");
  if ((FLAG_CHECK(&_DSPLREG_, 0)) && (FLAG_CHECK(&_DSPLREG_, 1))) {
    FLAG_CLR(&_DSPLREG_, 0);
    FLAG_CLR(&_DSPLREG_, 1);

    I2C_Start(I2C1);
    I2C_SendAddress(I2C1, _1602A_ADDR_);
    WH1602_WriteCommand(I2C1, _1602A_CLRDSLP_, 1640);
    WH1602_WriteCommand(I2C1, 0x80, 1640);
    I2C_Stop(I2C1);
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    I2C_Start(I2C1);
    I2C_SendAddress(I2C1, _1602A_ADDR_);
    WH1602_WriteChar(I2C1, ch);
    I2C_Stop(I2C1);
  }
  
  if (ch == 0x0a) FLAG_SET(&_DSPLREG_, 0);
  if (ch == 0x0d) FLAG_SET(&_DSPLREG_, 1);

}