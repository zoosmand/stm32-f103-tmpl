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
static const uint16_t initParams[16] = {
  WHxxxx_8BIT_BUS,      4100,
  WHxxxx_8BIT_BUS,      100,
  WHxxxx_8BIT_BUS,      WHxxxx_SD,
  WHxxxx_CUR_UPLEFT,    WHxxxx_LD,
  WHxxxx_4BIT_BUS_1L,   WHxxxx_SD,
  WHxxxx_DSPL_SW,       WHxxxx_SD,
  WHxxxx_CLR_DSLP,      WHxxxx_LD,
  WHxxxx_POS_1LS,       WHxxxx_SD
}; 




/* Private function prototypes -----------------------------------------------*/
/**
 * @brief  Writes/Sends a charachter symbol to the WHxxxx display.
 * @param  dev: pointer to the particular device structure
 * @param  ch: ACSII charachter
 * @retval status of operation
 */
static ErrorStatus WHxxxx_WriteChar(WHxxxx_TypeDef*, uint8_t);

/**
 * @brief  Writes/Sends a command to the WHxxxx display.
 * @param  dev: pointer to the particular device structure
 * @param  cmd: WH display command
 * @param  delay: command delay according documentation
 * @retval status of operation
 */
static ErrorStatus WHxxxx_WriteCommand(WHxxxx_TypeDef*, uint8_t, uint32_t);




// ----------------------------------------------------------------------------

ErrorStatus WHxxxx_Init(WHxxxx_TypeDef* dev) {
  
  /* Initial delay according WHxxxx documentation */
  _delay_ms(40);
 
  /* Initialize the device wuth parameter/delay pairs */
  uint8_t bytes[4];
  for(uint8_t i = 0; i < (sizeof(initParams) / 2); (i += 2)) {

    bytes[0] = _WR1NCMD((uint8_t)initParams[i]);
    bytes[1] = _WR2NCMD((uint8_t)initParams[i]);
    bytes[2] = _WR1NCMD(((uint8_t)initParams[i] << 4));
    bytes[3] = _WR2NCMD(((uint8_t)initParams[i] << 4));
    
    if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, bytes, 4)) return (ERROR);
    _delay_us(initParams[i + 1]);
  }
  
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

static ErrorStatus WHxxxx_WriteChar(WHxxxx_TypeDef* dev, uint8_t ch) {
  
  uint8_t bytes[4];
  
  bytes[0] = _WR1NCHAR(ch);
  bytes[1] = _WR2NCHAR(ch);
  bytes[2] = _WR1NCHAR(ch << 4);
  bytes[3] = _WR2NCHAR(ch << 4);
  
  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, bytes, 4)) return (ERROR);
  
  _delay_us(WHxxxx_SD);
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

static ErrorStatus WHxxxx_WriteCommand(WHxxxx_TypeDef* dev, uint8_t cmd, uint32_t delay) {  
  
  uint8_t bytes[4];
  
  bytes[0] = _WR1NCMD(cmd);
  bytes[1] = _WR2NCMD(cmd);
  bytes[2] = _WR1NCMD(cmd << 4);
  bytes[3] = _WR2NCMD(cmd << 4);
  
  if (I2C_Master_Send(dev->I2Cx, dev->I2C_Address, bytes, 4)) return (ERROR);
  
  _delay_us(delay);
  return (SUCCESS);
}


// ----------------------------------------------------------------------------

int __attribute__((weak)) putc_dspl_wh(char ch) {
  WHxxxx_TypeDef* dev = Get_WhDiplayDevice(WH_MODEL);

  if ((FLAG_CHECK(&_DSPLREG_, _0DCF_)) && (FLAG_CHECK(&_DSPLREG_, _0ACF_))) {
    FLAG_CLR(&_DSPLREG_, _0DCF_);
    FLAG_CLR(&_DSPLREG_, _0ACF_);

    if (WHxxxx_WriteCommand(dev, WHxxxx_CLR_DSLP, WHxxxx_LD)) return (1);
    if (WHxxxx_WriteCommand(dev, WHxxxx_POS_1LS, WHxxxx_SD)) return (1);
    diplPrintPos = 0;
  }
  if ((ch != 0x0a) && (ch != 0x0d)) {
    if (diplPrintPos > 15) {
      if (WHxxxx_WriteCommand(dev, WHxxxx_POS_2LS, WHxxxx_SD)) return (1);
      diplPrintPos = 0;
    }
    if (WHxxxx_WriteChar(dev, ch)) return (1);
    diplPrintPos++;
  }
  
  if (ch == 0x0a) FLAG_SET(&_DSPLREG_, _0DCF_);
  if (ch == 0x0d) FLAG_SET(&_DSPLREG_, _0ACF_);
  return (SUCCESS);
}

