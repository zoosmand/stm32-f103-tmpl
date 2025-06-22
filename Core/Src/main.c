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
static __IO uint32_t myTaskCnt  = 0;
static __IO uint32_t myTaskReg  = 0;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static __I task_t myTask = {
  .counter = &myTaskCnt,
  .counterSrc = secCnt,
  .counterReg = &myTaskReg,
  .flag = 4,
  .period = 1
};

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  // __NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  
  if (CronSec) {
    // LedToggle();
    // printf("The long test message, that might stuck the program but now it does not at all...\n");

    Task_Handler(&myTask);
  }

  if (FLAG_CHECK(myTask.counterReg, myTask.flag)) LedToggle(GPIOB, GPIO_PIN_13_Pos, &myTask);


}



void LedToggle(GPIO_TypeDef* port, uint16_t pinSource, __I task_t* task) {
  (PIN_LEVEL(port, pinSource)) ? PIN_L(port, pinSource) : PIN_H(port, pinSource);
  FLAG_CLR(myTask.counterReg, task->flag);
}