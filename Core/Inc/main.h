/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "stm32f1xx.h"

/* Private includes ----------------------------------------------------------*/
#include "common.h"
#include "gpio.h"
#include "monitor.h"
#include "hchk.h"
#include "i2c.h"
#include "spi.h"
#include "whxxxx.h"
#include "ssd13xx.h"
#include "fonts.h"
#include "w25qxx.h"
#include "eeprom.h"
#include "max72xx.h"
#include "bmx280.h"
#include "bmx680.h"
#include "tmpr.h"
#include "display.h"
#include "tm163x.h"
#include "ow.h"
#include "ds18b20.h"


#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Freq definitions */
#define APB2_FREQ   72000000U
#define APB1_FREQ   APB2_FREQ/2

/* Exported types ------------------------------------------------------------*/


/* Exported variables --------------------------------------------------------*/
extern uint32_t _GEREG_;
extern uint32_t _ASREG_;
extern uint32_t sysCnt;
extern uint32_t secCnt;


/* Exported constants --------------------------------------------------------*/


/** Global Events Register Flags */
#define _SYSTICKF_        0
#define _SYSSECF_         1


/** Rediness Services Register Flags */
#define GPIO_HB_RF        0
#define GPIO_TM_RF        1
#define GPIO_OW_RF        2
#define SPI1_RF           3
#define I2C1_RF           4
#define OW_BUS_RF         5
#define SSD_DSPL_RF       6
#define WH_DSPL_RF        7
#define MAX_DSPL_RF       8
#define TM_DSPL_RF        9
#define EEPROM_RF         10
#define BMX280_RF         11
#define BMX680_RF         12
#define HEARTBEAT_RF      13


/* Exported defines -----------------------------------------------------------*/
/* print standars output definitions */
#ifndef PRINTF_DSPL_RF
#define PRINTF_DSPL_RF    WH_DSPL_RF
#endif /* #ifndef PRINTF_DSPL_RF */

#define putc_dspl(ch)     DSPL_OUT(ch);

/* Exported macro ------------------------------------------------------------*/


/* Private definitions ---------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private structures -------------------------------------------------------------*/


/* Private includes ----------------------------------------------------------*/



/* Exported functions prototypes ---------------------------------------------*/
void __attribute__((weak)) Error_Handler(void);
int __attribute__((weak)) putc_dspl(char);
void Scheduler_Handler(task_scheduler_t*);
void _delay_us(uint32_t);
void _delay_ms(uint32_t);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
