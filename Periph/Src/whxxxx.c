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
#include "whxxxx.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define _WR1NCHAR(ch) ((ch & WHxxxx_4BIT_BUS_MASK)|(1<<WHxxxx_E)|(1<<WHxxxx_Bl)|(1<<WHxxxx_Rs))
#define _WR2NCHAR(ch) ((ch & WHxxxx_4BIT_BUS_MASK)|(1<<WHxxxx_Bl)|(1<<WHxxxx_Rs))
#define _WR1NCMD(cmd) ((cmd & WHxxxx_4BIT_BUS_MASK)|(1<<WHxxxx_E)|(1<<WHxxxx_Bl))
#define _WR2NCMD(cmd) ((cmd & WHxxxx_4BIT_BUS_MASK)|(1<<WHxxxx_Bl))

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static __attribute__((section(".cron"))) uint32_t _DSPLREG_    = 0;
static uint16_t diplPrintPos = 0;


/* Private function prototypes -----------------------------------------------*/
static void WH1602_WriteChar(I2C_TypeDef*, uint8_t);
static void WH1602_WriteCommand(I2C_TypeDef*, uint8_t, uint32_t);
// uint32_t WH1602_I2C_BufferLength(const char*);
// void WH1602_I2C_ReadByte(I2C_TypeDef*, uint8_t);





/**
 * @brief  Initializes WH1602A display
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval None
 */
int WHxxxx_Init(I2C_TypeDef* I2Cx) {
  
  /* Initial delay according 1602a documentation */
  _delay_ms(40);
 
  /* Initial parameter-delay pairs */
  uint16_t params[16] = {
    WHxxxx_8BIT_BUS,      4100,
    WHxxxx_8BIT_BUS,      100,
    WHxxxx_8BIT_BUS,      53,
    WHxxxx_CUR_UPLEFT,  2160,
    WHxxxx_4BIT_BUS_1L,    53,
    WHxxxx_DSPL_SW,     53,
    WHxxxx_CLR_DSLP,    2160,
    WHxxxx_POS_1LS,        53
  }; 
  
  if (I2C_Start(I2Cx)) return (1);
  if (I2C_SendAddress(I2Cx, WHxxxx_I2C_ADDR)) return (1);
  for(uint8_t i = 0; i < sizeof(params); i++) {
    if (I2C_WriteByte(I2Cx, _WR1NCMD((uint8_t)params[i]))) return (1);
    if (I2C_WriteByte(I2Cx, _WR2NCMD((uint8_t)params[i]))) return (1);
    if (I2C_WriteByte(I2Cx, _WR1NCMD(((uint8_t)params[i] << 4)))) return (1);
    if (I2C_WriteByte(I2Cx, _WR2NCMD(((uint8_t)params[i] << 4)))) return (1);
    _delay_us(params[i++]);
  }
  I2C_Stop(I2Cx);

  // putc_dspl('\n');

  return (0);
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
  _delay_us(53);
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
  _delay_us(delay);
}


// /**
//  * @brief  Writes/Sends a command to WH1602A display
//  * @param  buf: pointer to the charachter/text buffer
//  * @retval The buffer length 
//  */
// uint32_t WH1602_I2C_BufferLength(const char* buf){
//   uint32_t len = 0;
//   while(*buf){
//     len++;
//     buf++;
//     if (*buf == 0x0a) break;
//   }
//   return len;
// }


// /**
//  * @brief  Writes/Sends a text buffer to WH1602A display
//  * @param  I2Cx: pointer to the I2C peripherals
//  * @param  line: 1602a display line [1,2]
//  * @param  extraCmd: extra 1602a command, e.g. clear display
//  * @param  buf: pointer to the charachter/text buffer
//  * @retval None
//  */
// void WH1602_I2C_Write(I2C_TypeDef* I2Cx, uint8_t line, uint8_t extraCmd, const char* buf) {
  
//   I2C_Start(I2Cx);
//   I2C_SendAddress(I2Cx, WHxxxx_I2C_ADDR);

//   if (extraCmd) {
//     WH1602_WriteCommand(I2Cx, extraCmd, 2160);
//   }

//   uint8_t dsplAddr = (line == 2) ? WHxxxx_POS_2LS : WHxxxx_POS_1LS;
//   WH1602_WriteCommand(I2C1, dsplAddr, 53);
  
//   uint32_t len = WH1602_I2C_BufferLength(buf);
  
//   for(uint32_t i = 0 ; i < len ; i++) {
//     WH1602_WriteChar(I2Cx, (*buf++));
//   }
//   I2C_Stop(I2Cx);
// }



// /**
//  * @brief  Reads a byte from WH1602A display
//  * @param  I2Cx: pointer to the I2C peripherals
//  * @param  rxByte: received byte
//  * @retval None
//  */
// void WH1602_I2C_ReadByte(I2C_TypeDef* I2Cx, uint8_t rxByte) {

// }


// /**
//  * @brief  Reads a text buffer from WH1602A display
//  * @param  I2Cx: pointer to the I2C peripherals
//  * @param  bufLen: received buffer length
//  * @param  buf: pointer to the charachter/text buffer to receive
//  * @retval None
//  */
// void WH1602_I2C_Read(I2C_TypeDef* I2Cx, uint16_t bufLen, uint8_t* buf) {

// }


/**
 * @brief  Writes/Sends cgarachter to the given display
 * @param  ch: charachter to write
 * @param  dspl: display index #TODO define desctop selection
 * @retval None
 */
void __attribute__((weak)) putc_dspl_dh1602(char ch){
  if ((FLAG_CHECK(&_DSPLREG_, _0DCF_)) && (FLAG_CHECK(&_DSPLREG_, _0ACF_))) {
    FLAG_CLR(&_DSPLREG_, _0DCF_);
    FLAG_CLR(&_DSPLREG_, _0ACF_);

    I2C_Start(I2C1);
    I2C_SendAddress(I2C1, WHxxxx_I2C_ADDR);
    WH1602_WriteCommand(I2C1, WHxxxx_CLR_DSLP, 1640);
    WH1602_WriteCommand(I2C1, WHxxxx_POS_1LS, 40);
    I2C_Stop(I2C1);
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    I2C_Start(I2C1);
    I2C_SendAddress(I2C1, WHxxxx_I2C_ADDR);
    if (diplPrintPos > 15) {
      WH1602_WriteCommand(I2C1, WHxxxx_POS_2LS, 40);
      diplPrintPos = 0;
    }
    WH1602_WriteChar(I2C1, ch);
    I2C_Stop(I2C1);
    diplPrintPos++;
  }
  
  if (ch == 0x0a) FLAG_SET(&_DSPLREG_, _0DCF_);
  if (ch == 0x0d) FLAG_SET(&_DSPLREG_, _0ACF_);

}



/**
 * @brief  Writes/Sends cgarachter to the given display
 * @param  ch: charachter to write
 * @param  dspl: display index #TODO define desctop selection
 * @retval None
 */
void __attribute__((weak)) putc_dspl_dh2004(char ch) {
  if ((FLAG_CHECK(&_DSPLREG_, _0DCF_)) && (FLAG_CHECK(&_DSPLREG_, _0ACF_))) {
    FLAG_CLR(&_DSPLREG_, _0DCF_);
    FLAG_CLR(&_DSPLREG_, _0ACF_);

    I2C_Start(I2C1);
    I2C_SendAddress(I2C1, WHxxxx_I2C_ADDR);
    WH1602_WriteCommand(I2C1, WHxxxx_CLR_DSLP, 2160);
    WH1602_WriteCommand(I2C1, WHxxxx_POS_1LS, 53);
    I2C_Stop(I2C1);
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    I2C_Start(I2C1);
    I2C_SendAddress(I2C1, WHxxxx_I2C_ADDR);
    if (diplPrintPos > 19) {
      WH1602_WriteCommand(I2C1, WHxxxx_POS_2LS, 53);
      diplPrintPos = 0;
    }
    WH1602_WriteChar(I2C1, ch);
    I2C_Stop(I2C1);
    diplPrintPos++;
  }
  
  if (ch == 0x0a) FLAG_SET(&_DSPLREG_, _0DCF_);
  if (ch == 0x0d) FLAG_SET(&_DSPLREG_, _0ACF_);

}