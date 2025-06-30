/**
  ******************************************************************************
  * @file           : led.c
  * @brief          : LED functions code
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

  /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "led.h"

/* Private includes ----------------------------------------------------------*/
static __IO uint32_t ledRedTaskCnt           = 0;
static __IO uint32_t ledRedTaskReg           = 0;
static __IO uint32_t ledRedTaskPauseCnt_1    = 0;
static __IO uint32_t ledRedTaskPauseCnt_2    = 0;

static __IO uint32_t ledBlueTaskCnt          = 0;
static __IO uint32_t ledBlueTaskReg          = 0;
static __IO uint32_t ledBlueTaskPauseCnt_1   = 0;
static __IO uint32_t ledBlueTaskPauseCnt_2   = 0;

static __IO uint32_t ledGreenTaskCnt         = 0;
static __IO uint32_t ledGreenTaskReg         = 0;
static __IO uint32_t ledGreenTaskPauseCnt_1  = 0;
static __IO uint32_t ledGreenTaskPauseCnt_2  = 0;


/* Private variables ---------------------------------------------------------*/
static __I task_scheduler_t ledRedScheduler = {
  .counter    = &ledRedTaskCnt,
  .counterSrc = &secCnt,
  .period     = 7,
  .counterReg     = &ledRedTaskReg,
  .entranceFlag   = 31,
};

static __I task_led_toggle_t ledRedTask = {
  .scheduler      = &ledRedScheduler,
  // .taskReg        = &ledRedTaskReg,
  .port           = GPIOA,
  .pin            = GPIO_PIN_8_Pos,
  .callback       = &LedToggle_Task,
  .pauseCnt_1     = &ledRedTaskPauseCnt_1,
  .srcPauseCnt_1  = &sysCnt,
  .pauseValue_1   = 25,
  .pauseCnt_2     = &ledRedTaskPauseCnt_2,
  .srcPauseCnt_2  = &sysCnt,
  .pauseValue_2   = 50,
};


/* ----------------------------------------------------------------------------- */
static __I task_scheduler_t ledBlueScheduler = {
  .counter    = &ledBlueTaskCnt,
  .counterSrc = &secCnt,
  .period     = 5,
  .counterReg     = &ledBlueTaskReg,
  .entranceFlag   = 31,
};

static __I task_led_toggle_t ledBlueTask = {
  .scheduler      = &ledBlueScheduler,
  // .taskReg        = &ledBlueTaskReg,
  .port           = GPIOB,
  .pin            = GPIO_PIN_15_Pos,
  .callback       = &LedToggle_Task,
  .pauseCnt_1     = &ledBlueTaskPauseCnt_1,
  .srcPauseCnt_1  = &sysCnt,
  .pauseValue_1   = 14,
  .pauseCnt_2     = &ledBlueTaskPauseCnt_2,
  .srcPauseCnt_2  = &sysCnt,
  .pauseValue_2   = 50,
};

/* ----------------------------------------------------------------------------- */


static __I task_scheduler_t ledGreenScheduler = {
  .counter    = &ledGreenTaskCnt,
  .counterSrc = &secCnt,
  .period     = 3,
  .counterReg     = &ledGreenTaskReg,
  .entranceFlag   = 31,
};

static __I task_led_toggle_t ledGreenTask = {
  .scheduler      = &ledGreenScheduler,
  // .taskReg        = &ledGreenTaskReg,
  .port           = GPIOB,
  .pin            = GPIO_PIN_13_Pos,
  .callback       = &LedToggle_Task,
  .pauseCnt_1     = &ledGreenTaskPauseCnt_1,
  .srcPauseCnt_1  = &sysCnt,
  .pauseValue_1   = 25,
  .pauseCnt_2     = &ledGreenTaskPauseCnt_2,
  .srcPauseCnt_2  = &sysCnt,
  .pauseValue_2   = 50,
};

/* ----------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------- */

void Led_Handler(void) {

  Scheduler_Handler(&ledRedTask);
  Scheduler_Handler(&ledBlueTask);
  Scheduler_Handler(&ledGreenTask);

  TASK_CTRL(ledRedTask);
  TASK_CTRL(ledBlueTask);
  TASK_CTRL(ledGreenTask);

}


void LedToggle_Task(__I uint32_t *task) {
  __I task_led_toggle_t* tmpTask = (task_led_toggle_t*)task;
  // (PIN_LEVEL(tmpTask->port, tmpTask->pin)) ? PIN_L(tmpTask->port, tmpTask->pin) : PIN_H(tmpTask->port, tmpTask->pin);

  // Turn on the LED
  if (!FLAG_CHECK(tmpTask->scheduler->counterReg, 1)) {
    FLAG_SET(tmpTask->scheduler->counterReg, 1);
    PIN_H(tmpTask->port, tmpTask->pin);
    // Set up the first pause
    *tmpTask->pauseCnt_1 = *tmpTask->srcPauseCnt_1 + tmpTask->pauseValue_1;
    return;
  }

  // Handle the first pause
  if (!FLAG_CHECK(tmpTask->scheduler->counterReg, 2)) {
    if (*tmpTask->pauseCnt_1 > *tmpTask->srcPauseCnt_1) return;
    FLAG_SET(tmpTask->scheduler->counterReg, 2);
    return;
  }

  // Turn off the LED
  if (!FLAG_CHECK(tmpTask->scheduler->counterReg, 3)) {
    FLAG_SET(tmpTask->scheduler->counterReg, 3);
    PIN_L(tmpTask->port, tmpTask->pin);
    // Set up the first pause
    *tmpTask->pauseCnt_2 = *tmpTask->srcPauseCnt_2 + tmpTask->pauseValue_2;
    return;
  }

  // Handle the second pause
  if (!FLAG_CHECK(tmpTask->scheduler->counterReg, 4)) {
    if (*tmpTask->pauseCnt_2 > *tmpTask->srcPauseCnt_1) return;
    FLAG_SET(tmpTask->scheduler->counterReg, 4);
    return;
  }

  // Clear dedicated registry
  tmpTask->scheduler->counterReg;
}