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
static __attribute__((section(".cron"))) uint32_t _DSPLREG_ = 0;
static uint16_t diplPrintPos = 0;


/* Private function prototypes -----------------------------------------------*/
static int WHxxxx_WriteChar(I2C_TypeDef*, uint8_t);
static int WHxxxx_WriteCommand(I2C_TypeDef*, uint8_t, uint32_t);





/**
 * @brief  Initializes WHxxxx display
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval (int) status of operation
 */
int WHxxxx_Init(I2C_TypeDef* I2Cx) {
  
  /* Initial delay according WHxxxx documentation */
  _delay_ms(40);
 
  /* Initial parameter-delay pairs */
  uint16_t params[16] = {
    WHxxxx_8BIT_BUS,      4100,
    WHxxxx_8BIT_BUS,      100,
    WHxxxx_8BIT_BUS,      WHxxxx_SD,
    WHxxxx_CUR_UPLEFT,    WHxxxx_LD,
    WHxxxx_4BIT_BUS_1L,   WHxxxx_SD,
    WHxxxx_DSPL_SW,       WHxxxx_SD,
    WHxxxx_CLR_DSLP,      WHxxxx_LD,
    WHxxxx_POS_1LS,       WHxxxx_SD
  }; 
  
  if (I2C_Start(I2Cx)) return (1);
  if (I2C_SendAddress(I2Cx, WHxxxx_I2C_ADDR, TX)) return (1);
  for(uint8_t i = 0; i < sizeof(params); i++) {
    if (I2C_WriteByte(I2Cx, _WR1NCMD((uint8_t)params[i]))) return (1);
    if (I2C_WriteByte(I2Cx, _WR2NCMD((uint8_t)params[i]))) return (1);
    if (I2C_WriteByte(I2Cx, _WR1NCMD(((uint8_t)params[i] << 4)))) return (1);
    if (I2C_WriteByte(I2Cx, _WR2NCMD(((uint8_t)params[i] << 4)))) return (1);
    _delay_us(params[i++]);
  }
  I2C_Stop(I2Cx);

  return (0);
}


/**
 * @brief  Writes/Sends a charachter symbol to WHxxxx display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  ch: ACSII charachter
 * @retval (int) status of operation
 */
int WHxxxx_WriteChar(I2C_TypeDef* I2Cx, uint8_t ch) {
  if (I2C_WriteByte(I2Cx, _WR1NCHAR(ch))) return (1);
  if (I2C_WriteByte(I2Cx, _WR2NCHAR(ch))) return (1);
  if (I2C_WriteByte(I2Cx, _WR1NCHAR(ch << 4))) return (1);
  if (I2C_WriteByte(I2Cx, _WR2NCHAR(ch << 4))) return (1);
  _delay_us(WHxxxx_SD);
  return (0);
}


/**
 * @brief  Writes/Sends a command to WHxxxx display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  cmd: 1602a command
 * @param  delay: command delay according documentation
 * @retval (int) status of operation
 */
int WHxxxx_WriteCommand(I2C_TypeDef* I2Cx, uint8_t cmd, uint32_t delay) {  
  if (I2C_WriteByte(I2Cx, _WR1NCMD(cmd))) return (1);
  if (I2C_WriteByte(I2Cx, _WR2NCMD(cmd))) return (1);
  if (I2C_WriteByte(I2Cx, _WR1NCMD(cmd << 4))) return (1);
  if (I2C_WriteByte(I2Cx, _WR2NCMD(cmd << 4))) return (1);
  _delay_us(delay);
  return (0);
}



/**
 * @brief  Writes/Sends cgarachter to the given display
 * @param  ch: charachter to write
 * @param  dspl: display index #TODO define desctop selection
 * @retval (int) status of operation
 */
int __attribute__((weak)) putc_dspl_wh1602(char ch) {
  if ((FLAG_CHECK(&_DSPLREG_, _0DCF_)) && (FLAG_CHECK(&_DSPLREG_, _0ACF_))) {
    FLAG_CLR(&_DSPLREG_, _0DCF_);
    FLAG_CLR(&_DSPLREG_, _0ACF_);

    if (I2C_Start(I2C1)) return (1);
    if (I2C_SendAddress(I2C1, WHxxxx_I2C_ADDR, TX)) return (1);
    if (WHxxxx_WriteCommand(I2C1, WHxxxx_CLR_DSLP, WHxxxx_LD)) return (1);
    if (WHxxxx_WriteCommand(I2C1, WHxxxx_POS_1LS, WHxxxx_SD)) return (1);
    I2C_Stop(I2C1);
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    if (I2C_Start(I2C1)) return (1);
    if (I2C_SendAddress(I2C1, WHxxxx_I2C_ADDR, TX)) return (1);
    if (diplPrintPos > 15) {
      if (WHxxxx_WriteCommand(I2C1, WHxxxx_POS_2LS, WHxxxx_SD)) return (1);
      diplPrintPos = 0;
    }
    if (WHxxxx_WriteChar(I2C1, ch)) return (1);
    I2C_Stop(I2C1);
    diplPrintPos++;
  }
  
  if (ch == 0x0a) FLAG_SET(&_DSPLREG_, _0DCF_);
  if (ch == 0x0d) FLAG_SET(&_DSPLREG_, _0ACF_);
  return (0);
}



/**
 * @brief  Writes/Sends cgarachter to the given display
 * @param  ch: charachter to write
 * @param  dspl: display index #TODO define desctop selection
 * @retval (int) status of operation
 */
int __attribute__((weak)) putc_dspl_wh2004(char ch) {
  if ((FLAG_CHECK(&_DSPLREG_, _0DCF_)) && (FLAG_CHECK(&_DSPLREG_, _0ACF_))) {
    FLAG_CLR(&_DSPLREG_, _0DCF_);
    FLAG_CLR(&_DSPLREG_, _0ACF_);

    if (I2C_Start(I2C1)) return (1);
    if (I2C_SendAddress(I2C1, WHxxxx_I2C_ADDR, TX)) return (1);
    if (WHxxxx_WriteCommand(I2C1, WHxxxx_CLR_DSLP, WHxxxx_LD)) return (1);
    if (WHxxxx_WriteCommand(I2C1, WHxxxx_POS_1LS, WHxxxx_SD)) return (1);
    I2C_Stop(I2C1);
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    if (I2C_Start(I2C1)) return (1);
    if (I2C_SendAddress(I2C1, WHxxxx_I2C_ADDR, TX)) return (1);
    if (diplPrintPos > 19) {
      if (WHxxxx_WriteCommand(I2C1, WHxxxx_POS_2LS, WHxxxx_SD)) return (1);
      diplPrintPos = 0;
    }
    if (WHxxxx_WriteChar(I2C1, ch)) return (1);
    I2C_Stop(I2C1);
    diplPrintPos++;
  }
  
  if (ch == 0x0a) FLAG_SET(&_DSPLREG_, _0DCF_);
  if (ch == 0x0d) FLAG_SET(&_DSPLREG_, _0ACF_);
  return (0);
}