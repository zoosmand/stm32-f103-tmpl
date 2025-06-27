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
#include "common.h"
#include "led.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */


/* Exported types ------------------------------------------------------------*/
#define SWO_USART

/* Exported constants --------------------------------------------------------*/
#define _GEREG_                       (uint32_t*)(SRAM_BASE)
#define sysQuantCnt                   (uint32_t*)(SRAM_BASE + 0x04)
#define secCnt                        (uint32_t*)(SRAM_BASE + 0x08)


/** Global Events Register Flags */
#define _SYSSECF_                     1


/* Exported macro ------------------------------------------------------------*/


/* Private definitions ---------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private structures -------------------------------------------------------------*/
// typedef struct {
//   uint32_t  *counter;
//   uint32_t  *counterSrc;
//  	uint32_t  period;
// } task_scheduler_t;



/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
// void LedToggle_Task(__I uint32_t*); // localted in ./Periph/led.S
void Scheduler_Handler(__I uint32_t*); // localted in ./Core/system_cron.S

// extern void Led_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

