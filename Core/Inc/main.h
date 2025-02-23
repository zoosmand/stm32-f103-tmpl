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

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define _GEREG_                       *(uint32_t*)(SRAM_BASE)
#define sysQuantCnt                   *(uint32_t*)(SRAM_BASE + 0x04)
#define secCnt                        *(uint32_t*)(SRAM_BASE + 0x08)
#define secCntCache                   *(uint32_t*)(SRAM_BASE + 0x0c)

/** Global Events Register Flags */
#define _SYSTICKF_                    0
#define _SYSSECF_                     1


/* Exported macro ------------------------------------------------------------*/
#define CronSec                       (_GEREG_ & (1<<_SYSSECF_))

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


/* Private definitions ---------------------------------------------------------*/
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0          ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1          ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2          ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3          ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4          ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
0 bit  for subpriority */
#endif



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

