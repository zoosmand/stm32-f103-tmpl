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

/* Private includes ----------------------------------------------------------*/
// static __IO uint32_t toggleTaskCnt        = 0;
// static __IO uint32_t toggleTaskReg        = 0;
// static __IO uint32_t toggleTaskPauseCnt_1  = 0;
// static __IO uint32_t toggleTaskPauseCnt_2  = 0;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

// /* Private variables ---------------------------------------------------------*/
// static __I task_scheduler_t toggleScheduler = {
//   .counter    = &toggleTaskCnt,
//   .counterSrc = secCnt,
//   .period     = 5,
// };

// static __I task_led_toggle_t toggleTask = {
//   .scheduler      = &toggleScheduler,
//   .counterReg     = &toggleTaskReg,
//   .entranceFlag   = 31,
//   .port           = GPIOB,
//   .pin            = GPIO_PIN_13_Pos,
//   .callback       = &LedToggle_Task,
//   .pauseCnt_1     = &toggleTaskPauseCnt_1,
//   .srcPauseCnt_1  = sysQuantCnt,
//   .pauseValue_1   = 50,
//   .pauseCnt_2     = &toggleTaskPauseCnt_2,
//   .srcPauseCnt_2  = sysQuantCnt,
//   .pauseValue_2   = 50,
// };

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

  // Scheduler_Handler((__I uint32_t*)&toggleTask);

  // TASK_CTRL(toggleTask);
  Led_Handler();

}



// void LedToggle_Task(__I uint32_t *task) {
//   __I task_led_toggle_t* tmpTask = (task_led_toggle_t*)task;
//   // (PIN_LEVEL(tmpTask->port, tmpTask->pin)) ? PIN_L(tmpTask->port, tmpTask->pin) : PIN_H(tmpTask->port, tmpTask->pin);

//   // Turn on the LED
//   if (!FLAG_CHECK(tmpTask->counterReg, 1)) {
//     FLAG_SET(tmpTask->counterReg, 1);
//     PIN_L(tmpTask->port, tmpTask->pin);
//     // Set up the first pause
//     *tmpTask->srcPauseCnt_1 = *tmpTask->srcPauseCnt_1 + tmpTask->pauseValue_1;
//     return;
//   }

//   // Handle the first pause
//   if (!FLAG_CHECK(tmpTask->counterReg, 2)) {
//     if (*tmpTask->srcPauseCnt_1 <= *tmpTask->srcPauseCnt_1) return;
//     FLAG_SET(tmpTask->counterReg, 3);
//     return;
//   }

//   // Turn off the LED
//   if (!FLAG_CHECK(tmpTask->counterReg, 3)) {
//     FLAG_SET(tmpTask->counterReg, 3);
//     PIN_H(tmpTask->port, tmpTask->pin);
//     // Set up the first pause
//     *tmpTask->srcPauseCnt_1 = *tmpTask->srcPauseCnt_1 + tmpTask->pauseValue_1;
//     return;
//   }

//   // Handle the second pause
//   if (!FLAG_CHECK(tmpTask->counterReg, 4)) {
//     if (*tmpTask->srcPauseCnt_1 <= *tmpTask->srcPauseCnt_1) return;
//     FLAG_SET(tmpTask->counterReg, 4);
//     return;
//   }


//   // Turn off the task, aka clear task flag
//   // FLAG_CLR(tmpTask->counterReg, tmpTask->entranceFlag);
//   // Clear dedicated registry
//   tmpTask->counterReg;
// }