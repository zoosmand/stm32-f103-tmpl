/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

  /* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Global variables ---------------------------------------------------------*/
__IO uint32_t _GEREG_       = 0;
__IO uint32_t sysCnt        = 0;
__IO uint32_t secCnt        = 0;
static __IO uint32_t secCntCache   = 1000;

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  // __NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  
  if (CRON_SEC_EVENT) {
    printf("The long test message, that might stuck the program but now it does not at all...\n");
  }

  Led_Handler();

}


/**
 * @brief  The application cron service.
 * @retval none
 */
void Cron_Handler(void) {
  while (1) {
    __disable_irq();
    
    if (FLAG_CHECK(_GEREG_, _SYSSECF_)) {
      FLAG_CLR(_GEREG_, _SYSSECF_);
      IWDG->KR = IWDG_KEY_RELOAD;
    }

    if (secCntCache <= sysCnt) {
      secCntCache = sysCnt + 1000;
      secCnt++;
      FLAG_SET(_GEREG_, _SYSSECF_);
    }

    __enable_irq();
    main();
  }
}



void Scheduler_Handler(__IO task_scheduler_t *scheduler) {
  __disable_irq();

  if (scheduler->counter <= scheduler->counterSrc) {
    scheduler->counter = scheduler->counterSrc + scheduler->period;
    FLAG_SET(scheduler->counterReg, scheduler->entranceFlag);
  }

  __enable_irq();
}
