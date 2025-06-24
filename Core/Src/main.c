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
static __IO uint32_t toggleTaskCnt  = 0;
static __IO uint32_t toggleTaskReg  = 0;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static __I task_scheduler_t toggleScheduler = {
  .counter    = &toggleTaskCnt,
  .counterSrc = sysQuantCnt,
  .period     = 50,
};

static __I task_led_toggle_t toggleTask = {
  .scheduler    = &toggleScheduler,
  .counterReg   = &toggleTaskReg,
  .entranceFlag = 31,
  .port         = GPIOB,
  .pin          = GPIO_PIN_13_Pos,
  .callback     = &LedToggle_Task,
};

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

  Scheduler_Handler((__I uint32_t*)&toggleTask);

  TASK_CTRL(toggleTask);

}



void LedToggle_Task(__I uint32_t *task) {
  __I task_led_toggle_t* tmpTask = (task_led_toggle_t*)task;
  (PIN_LEVEL(tmpTask->port, tmpTask->pin)) ? PIN_L(tmpTask->port, tmpTask->pin) : PIN_H(tmpTask->port, tmpTask->pin);
  // Turn off the task, aka clear task flag
  FLAG_CLR(tmpTask->counterReg, tmpTask->entranceFlag);
}