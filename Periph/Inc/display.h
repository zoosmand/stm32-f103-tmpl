/**
  ******************************************************************************
  * @file           : display.h
  * @brief          : This file contains header definitions for display code
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
#ifndef _DISPLAY_H
#define _DISPLAY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
/* Private typedef -----------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void WH1602_I2C_Init(I2C_TypeDef*);
void WH1602_I2C_Write(I2C_TypeDef*, uint8_t, uint8_t, const char*);
void WH1602_I2C_Read(I2C_TypeDef*, uint16_t, uint8_t*);
void PrintCharDisplay(char, uint8_t);

/* Private defines -----------------------------------------------------------*/
#define _1602A_ADDR_        0x27 // WH1602 I2C Address
#define _1602A_8BBUS_       0x03 // 8-bit initial bus initialization
#define _1602A_CURUPLEFT_   0x02 // Cursor positioin up an left
#define	_1602A_4BBUS2L_     0x20 // 4-bit bus, LCD of 2 lines
#define _1602A_DSPLSW_	    0x0c // Display on, cursor off, blink off
#define _1602A_CLRDSLP_     0x01 // Clear display
#define _1602A_NOCMD_       0x00 // No command

/* --- WH0802A control parameters --- */
#define _1602A_Bl           3
#define _1602A_E            2
#define _1602A_Rw 	        1
#define _1602A_Rs 	        0
#define _1602A_4BMASK_      0xf0

/* --- WH0802A control mascroses --- */
#define _WR1NCHAR(ch) ((ch & _1602A_4BMASK_)|(1<<_1602A_E)|(1<<_1602A_Bl)|(1<<_1602A_Rs))
#define _WR2NCHAR(ch) ((ch & _1602A_4BMASK_)|(1<<_1602A_Bl)|(1<<_1602A_Rs))
#define _WR1NCMD(cmd) ((cmd & _1602A_4BMASK_)|(1<<_1602A_E)|(1<<_1602A_Bl))
#define _WR2NCMD(cmd) ((cmd & _1602A_4BMASK_)|(1<<_1602A_Bl))

/* --- Display end of line parameters --- */
#define _0DCF_              0
#define _0ACF_              1

/* Extern functions prototypes ----------------------------------------------*/


#ifdef __cplusplus
}
#endif
#endif // _DISPLAY_H
