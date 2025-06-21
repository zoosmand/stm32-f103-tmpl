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
#define SWO_USART

/* Exported constants --------------------------------------------------------*/
#define _GEREG_                       *(uint32_t*)(SRAM_BASE)
#define sysQuantCnt                   *(uint32_t*)(SRAM_BASE + 0x04)
#define secCnt                        *(uint32_t*)(SRAM_BASE + 0x08)
#define secCntCache                   *(uint32_t*)(SRAM_BASE + 0x0c)

/** Global Events Register Flags */
#define _SYSTICKF_                    0
#define _SYSSECF_                     1


/* Exported macro ------------------------------------------------------------*/
// #define CronSec                       (_GEREG_ & (1<<_SYSSECF_))
#define CronSec                       FLAG_CHECK(_GEREG_, _SYSSECF_)

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void LedToggle(void); // localted in ./Periph/led.S


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



/* Private macro -------------------------------------------------------------*/
#define GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset)      (PERIPH_BB_BASE + ((addr + periphOffset) * 32U) + (wordOffset * 4U))
#define SET_PERIPH_BB_VAL(addr, periphOffset, wordOffset, key)  (*(uint32_t*)(GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset)) = key)
#define GET_PERIPH_BB_VAL(addr, periphOffset, wordOffset)       (*(__O uint32_t*)(GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset)))


#define GET_SRAM_BB_ADDR(addr, offset)                          (SRAM_BB_BASE + (addr * 32U) + (offset * 4U))
#define SET_SRAM_BB_VAL(addr, offset, key)                      (*(uint32_t*)(GET_SRAM_BB_ADDR(addr, offset)) = key)
#define GET_SRAM_BB_VAL(addr, offset)                           (*(__O uint32_t*)(GET_SRAM_BB_ADDR(addr, offset)))

/* Exported macro ------------------------------------------------------------*/
#define FLAG_SET(registry, flag)                                SET_SRAM_BB_VAL((uint32_t)&registry, flag, 1)
#define FLAG_CLR(registry, flag)                                SET_SRAM_BB_VAL((uint32_t)&registry, flag, 0)
#define FLAG_CHECK(registry, flag)                              (GET_SRAM_BB_VAL((uint32_t)&registry, flag))

#define PIN_H(port, pinSource)                                  SET_PERIPH_BB_VAL((uint32_t)port, GPIO_BSRR_Offset, pinSource, 1)
#define PIN_L(port, pinSource)                                  SET_PERIPH_BB_VAL((uint32_t)port, GPIO_BSRR_Offset, (pinSource + 16U), 1)
#define PIN_LEVEL(port, pinSource)                              (GET_PERIPH_BB_VAL((uint32_t)port, GPIO_IDR_Offset, pinSource))

#define PREG_SET(registry, key)                                 SET_PERIPH_BB_VAL((uint32_t)&registry, 0, key, 1)
#define PREG_CLR(registry, key)                                 SET_PERIPH_BB_VAL((uint32_t)&registry, 0, key, 0)
#define PREG_CHECK(registry, key)                               (GET_PERIPH_BB_VAL((uint32_t)&registry, 0, key))


#define BIT_2_0(per)        (per * 2U)
#define BIT_2_1(per)        (per * 2U + 1U)
#define BIT_4_0(per)        (per * 4U)
#define BIT_4_1(per)        (per * 4U + 1U)
#define BIT_4_2(per)        (per * 4U + 2U)
#define BIT_4_3(per)        (per * 4U + 3U)

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

