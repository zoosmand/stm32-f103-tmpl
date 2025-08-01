/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

  /* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Global variables ---------------------------------------------------------*/
__attribute__((section(".cron"))) uint32_t _GEREG_               = 0;
__attribute__((section(".cron"))) uint32_t sysCnt                = 0;
__attribute__((section(".cron"))) uint32_t secCnt                = 0;
static __attribute__((section(".cron"))) uint32_t secCntCache    = 0;

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

  static uint32_t tmpCnt = 0;
  
  if (CRON_SEC_EVENT) {
    // printf("The long test message, that might stuck the program but now it does not at all...\n");
    // printf("second:%li\n", secCnt);

    
    if (tmpCnt <= secCnt ) {
      DS18B20_ConvertT();
      tmpCnt = secCnt + 4;
    }
  }

  Led_Handler();

}


/**
 * @brief  The application cron service.
 * @retval none
 */
void Cron_Handler(void) {
  WH1602_I2C_Init(I2C1);
  OW_Search();
  // DS18B20_ConvertT();

  // while (1) {
  //   PIN_H(GPIOB, GPIO_PIN_12_Pos);
  //   _delay_us(750);
  //   PIN_L(GPIOB, GPIO_PIN_12_Pos);
  //   _delay_us(1000);
  // }
  
  while (1) {
    __disable_irq();
    
    if (FLAG_CHECK(&_GEREG_, _SYSSECF_)) {
      FLAG_CLR(&_GEREG_, _SYSSECF_);
      IWDG->KR = IWDG_KEY_RELOAD;
    }

    if (secCntCache <= sysCnt) {
      secCntCache = sysCnt + 1000;
      secCnt++;
      FLAG_SET(&_GEREG_, _SYSSECF_);
    }

    __enable_irq();
    main();
  }
}



void Scheduler_Handler(task_scheduler_t *scheduler) {
  __disable_irq();

  if (*scheduler->counter <= *scheduler->counterSrc) {
    *scheduler->counter = *scheduler->counterSrc + scheduler->period;
    FLAG_SET(scheduler->counterReg, scheduler->entranceFlag);
  }

  __enable_irq();
}



// void _delay_us(uint32_t __us_delay){
//   __asm__ __volatile__ (
//     // "ldr r0, #%__us_delay\n"
//     "mul r0, r0, 1\n"
//     "_LOOP_: \n"
//     "subs r0, r0, 1\n"
//     "bpl _LOOP_\n"
//     // : //"=r" (__us_delay)
//     // :
//     // : "r0"
//   );
// }

