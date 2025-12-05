/**
  ******************************************************************************
  * @file           : ssd13xx.h
  * @brief          : This file contains definityion for code of SSD13xx 
  *                   display.
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

#ifndef __SSD13XX_DSPL_H
#define __SSD13XX_DSPL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"


/* Private defines -----------------------------------------------------------*/
/* --- Model relared difinitions --- */
#ifndef SSD_DSPL_MODEL
#define SSD_DSPL_MODEL 1315
#endif /* #ifndef SSD_DSPL_MODEL */

#ifndef SSD_DSPL_FONT
#define SSD_DSPL_FONT 1014
#endif /* #ifndef SSD_DSPL_FONT */

/* --- Bus related definitions --- */
#define SSD1306_I2C_ADDR    0x78 // SSD1306 I2C Address
#define SSD1315_I2C_ADDR    0x3c // SSD1315 I2C Address
#define SSD13xx_Co_BIT      7 // Co bit
#define SSD13xx_DC_BIT      6 // DC bit (1 - data, 0 - command)

#define SSD13xx_DATA_CTRL   (uint8_t)((1 << SSD13xx_DC_BIT) & ~(1 << SSD13xx_Co_BIT))
#define SSD13xx_CMD_CTRL    (uint8_t)(~(1 << SSD13xx_Co_BIT) & ~(1 << SSD13xx_DC_BIT))


/* Exported functions prototypes ---------------------------------------------*/

ErrorStatus SSD13xx_Init(SSD13xx_TypeDef*);
int __attribute__((weak)) putc_dspl_ssd(char);


#ifdef __cplusplus
}
#endif
#endif // __SSD13XX_DSPL_H
