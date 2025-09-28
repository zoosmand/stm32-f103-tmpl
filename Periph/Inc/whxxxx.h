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
int WHxxxx_Init(I2C_TypeDef*);


/* Private defines -----------------------------------------------------------*/
#define WHxxxx_I2C_ADDR       0x27 // WH1602 I2C Address
#define WHxxxx_8BIT_BUS       0x03 // 8-bit initial bus initialization
#define WHxxxx_CUR_UPLEFT     0x02 // Cursor positioin up an left
#define	WHxxxx_4BIT_BUS_2L    0x28 // 4-bit bus, LCD of 2 lines
#define	WHxxxx_4BIT_BUS_1L    0x20 // 4-bit bus, LCD of 1 line inverted
#define WHxxxx_DSPL_SW	      0x0c // Display on, cursor off, blink off
#define WHxxxx_CLR_DSLP       0x01 // Clear display
#define WHxxxx_POS_1LS        0x80 // Position at 1-st line, start 
#define WHxxxx_POS_2LS        0xc0 // Position at 2-nd line, start
#define WHxxxx_NOCMD          0x00 // No command


/* --- WHxxxx delays --- */
#define WHxxxx_SD             40
#define WHxxxx_LD             1640


/* --- WHxxxx control parameters --- */
#define WHxxxx_Bl             3
#define WHxxxx_E              2
#define WHxxxx_Rw 	          1
#define WHxxxx_Rs 	          0
#define WHxxxx_4BIT_BUS_MASK  0xf0


/* --- Display end of line parameters --- */
#define _0DCF_                0
#define _0ACF_                1

/* Extern functions prototypes ----------------------------------------------*/
int __attribute__((weak)) putc_dspl_wh1602(char);
int __attribute__((weak)) putc_dspl_wh2004(char);

#ifdef __cplusplus
}
#endif
#endif // _DISPLAY_H
