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
// void WH1602_WriteChar(I2C_TypeDef*, uint8_t);
// void WH1602_WriteCommand(I2C_TypeDef*, uint8_t, uint32_t);
void WH1602_I2C_Write(I2C_TypeDef* I2Cx, uint8_t, const char*);
   
/* Private defines -----------------------------------------------------------*/
#define _ADDR_        0x27 // WH1602 I2C Address
#define _8BBUS_       0x03 // 8-bit initial bus initialization
#define _CURUPLEFT_   0x02 // Cursor positioin up an left
#define	_4BBUS2L_     0x28 // 4-bit bus, LCD of 2 lines
#define _DSPLSW_	    0x0c // Display on, cursor off, blink off
#define _CLRDSLP_     0x01 // Clear display
// #define _CLRDSP2_     0x80 // Clear 2-nd line

/***************************** WH0802A Control Bits *****************************/
#define _Bl           3
#define _E            2
#define _Rw 	        1
#define _Rs 	        0
#define _4BMASK_      0xf0

/***************************** WH0802A Control masks *****************************/
#define _WR1NCHAR(ch) ((ch & _4BMASK_)|(1<<_E)|(1<<_Bl)|(1<<_Rs))
#define _WR2NCHAR(ch) ((ch & _4BMASK_)|(1<<_Bl)|(1<<_Rs))
#define _WR1NCMD(cmd) ((cmd & _4BMASK_)|(1<<_E)|(1<<_Bl))
#define _WR2NCMD(cmd) ((cmd & _4BMASK_)|(1<<_Bl))


/* Extern functions prototypes ----------------------------------------------*/


#ifdef __cplusplus
}
#endif
#endif // _DISPLAY_H
