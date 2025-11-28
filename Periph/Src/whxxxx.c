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
static ErrorStatus WHxxxx_WriteChar(I2C_TypeDef*, uint8_t);
static ErrorStatus WHxxxx_WriteCommand(I2C_TypeDef*, uint8_t, uint32_t);





/**
 * @brief  Initializes WHxxxx display
 * @param  I2Cx: pointer to the I2C peripherals
 * @retval (int) status of operation
 */
ErrorStatus WHxxxx_Init(I2C_TypeDef* I2Cx) {
  
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
  
  uint8_t bytes[4];
  for(uint8_t i = 0; i < (sizeof(params) / 2); (i += 2)) {

    bytes[0] = _WR1NCMD((uint8_t)params[i]);
    bytes[1] = _WR2NCMD((uint8_t)params[i]);
    bytes[2] = _WR1NCMD(((uint8_t)params[i] << 4));
    bytes[3] = _WR2NCMD(((uint8_t)params[i] << 4));

    if (I2C_Master_Send(I2Cx, WHxxxx_I2C_ADDR, bytes, 4)) return (ERROR);
    _delay_us(params[i + 1]);
  }

  return (SUCCESS);
}


/**
 * @brief  Writes/Sends a charachter symbol to WHxxxx display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  ch: ACSII charachter
 * @retval status of operation
 */
static ErrorStatus WHxxxx_WriteChar(I2C_TypeDef* I2Cx, uint8_t ch) {

  uint8_t bytes[4];

  bytes[0] = _WR1NCHAR(ch);
  bytes[1] = _WR2NCHAR(ch);
  bytes[2] = _WR1NCHAR(ch << 4);
  bytes[3] = _WR2NCHAR(ch << 4);

  if (I2C_Master_Send(I2Cx, WHxxxx_I2C_ADDR, bytes, 4)) return (ERROR);

  _delay_us(WHxxxx_SD);
  return (SUCCESS);
}


/**
 * @brief  Writes/Sends a command to WHxxxx display
 * @param  I2Cx: pointer to the I2C peripherals
 * @param  cmd: 1602a command
 * @param  delay: command delay according documentation
 * @retval status of operation
 */
static ErrorStatus WHxxxx_WriteCommand(I2C_TypeDef* I2Cx, uint8_t cmd, uint32_t delay) {  

  uint8_t bytes[4];

  bytes[0] = _WR1NCMD(cmd);
  bytes[1] = _WR2NCMD(cmd);
  bytes[2] = _WR1NCMD(cmd << 4);
  bytes[3] = _WR2NCMD(cmd << 4);

  if (I2C_Master_Send(I2Cx, WHxxxx_I2C_ADDR, bytes, 4)) return (ERROR);

  _delay_us(delay);
  return (SUCCESS);
}



/**
 * @brief  Writes/Sends cgarachter to the given display
 * @param  ch: charachter to write
 * @param  dspl: display index #TODO define desctop selection
 * @retval status of operation
 */
int __attribute__((weak)) putc_dspl_wh1602(char ch) {
  if ((FLAG_CHECK(&_DSPLREG_, _0DCF_)) && (FLAG_CHECK(&_DSPLREG_, _0ACF_))) {
    FLAG_CLR(&_DSPLREG_, _0DCF_);
    FLAG_CLR(&_DSPLREG_, _0ACF_);

    if (WHxxxx_WriteCommand(I2C1, WHxxxx_CLR_DSLP, WHxxxx_LD)) return (1);
    if (WHxxxx_WriteCommand(I2C1, WHxxxx_POS_1LS, WHxxxx_SD)) return (1);
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    if (diplPrintPos > 15) {
      if (WHxxxx_WriteCommand(I2C1, WHxxxx_POS_2LS, WHxxxx_SD)) return (1);
      diplPrintPos = 0;
    }
    if (WHxxxx_WriteChar(I2C1, ch)) return (1);
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
 * @retval status of operation
 */
int __attribute__((weak)) putc_dspl_wh2004(char ch) {
  if ((FLAG_CHECK(&_DSPLREG_, _0DCF_)) && (FLAG_CHECK(&_DSPLREG_, _0ACF_))) {
    FLAG_CLR(&_DSPLREG_, _0DCF_);
    FLAG_CLR(&_DSPLREG_, _0ACF_);

    if (WHxxxx_WriteCommand(I2C1, WHxxxx_CLR_DSLP, WHxxxx_LD)) return (1);
    if (WHxxxx_WriteCommand(I2C1, WHxxxx_POS_1LS, WHxxxx_SD)) return (1);
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    if (diplPrintPos > 19) {
      if (WHxxxx_WriteCommand(I2C1, WHxxxx_POS_2LS, WHxxxx_SD)) return (1);
      diplPrintPos = 0;
    }
    if (WHxxxx_WriteChar(I2C1, ch)) return (1);
    diplPrintPos++;
  }
  
  if (ch == 0x0a) FLAG_SET(&_DSPLREG_, _0DCF_);
  if (ch == 0x0d) FLAG_SET(&_DSPLREG_, _0ACF_);
  return (0);
}