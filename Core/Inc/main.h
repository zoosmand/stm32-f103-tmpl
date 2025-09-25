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

#include "stm32f1xx.h"

/* Private includes ----------------------------------------------------------*/
#include "i2c.h"
#include "display.h"
#include "ssd13xx.h"
#include "fonts.h"


#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Freq definitions */
#define APB2_FREQ   72000000U
#define APB1_FREQ   APB2_FREQ/2

/* Exported types ------------------------------------------------------------*/


/* Exported variables --------------------------------------------------------*/
extern uint32_t _GEREG_;
extern uint32_t sysCnt;
extern uint32_t secCnt;


/* Exported constants --------------------------------------------------------*/
#define SWO_DSPL

/** Global Events Register Flags */
#define _SYSSECF_                     1


/* Exported macro ------------------------------------------------------------*/


/* Private definitions ---------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private structures -------------------------------------------------------------*/
typedef struct {
  uint32_t  *counter;
  uint32_t  *counterSrc;
  uint32_t  period;
  uint32_t  *counterReg;
  uint32_t  entranceFlag;
} task_scheduler_t;


typedef struct {
  uint8_t   addr[8];
  uint8_t   spad[9];
} OneWireDevice_t;


/* Private includes ----------------------------------------------------------*/
#include "common.h"
#include "led.h"
#include "ow.h"
#include "ds18b20.h"



/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void Scheduler_Handler(task_scheduler_t*);
void _delay_us(uint32_t);
void _delay_ms(uint32_t);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
