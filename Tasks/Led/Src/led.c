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
#include "led.h"

/* Private includes ----------------------------------------------------------*/
static __IO uint32_t toggleTaskCnt        = 0;
static __IO uint32_t toggleTaskReg        = 0;
static __IO uint32_t toggleTaskPauseCnt_1  = 0;
static __IO uint32_t toggleTaskPauseCnt_2  = 0;


/* Private variables ---------------------------------------------------------*/
static __I task_scheduler_t toggleScheduler = {
  .counter    = &toggleTaskCnt,
  .counterSrc = secCnt,
  .period     = 5,
};

static __I task_led_toggle_t toggleTask = {
  .scheduler      = &toggleScheduler,
  .counterReg     = &toggleTaskReg,
  .entranceFlag   = 31,
  .port           = GPIOA,
  .pin            = GPIO_PIN_8_Pos,
  .callback       = &LedToggle_Task,
  .pauseCnt_1     = &toggleTaskPauseCnt_1,
  .srcPauseCnt_1  = sysQuantCnt,
  .pauseValue_1   = 25,
  .pauseCnt_2     = &toggleTaskPauseCnt_2,
  .srcPauseCnt_2  = sysQuantCnt,
  .pauseValue_2   = 50,
};


void Led_Handler(void) {

  Scheduler_Handler((__I uint32_t*)&toggleTask);

  TASK_CTRL(toggleTask);

}


void LedToggle_Task(__I uint32_t *task) {
  __I task_led_toggle_t* tmpTask = (task_led_toggle_t*)task;
  // (PIN_LEVEL(tmpTask->port, tmpTask->pin)) ? PIN_L(tmpTask->port, tmpTask->pin) : PIN_H(tmpTask->port, tmpTask->pin);

  // Turn on the LED
  if (!FLAG_CHECK(tmpTask->counterReg, 1)) {
    FLAG_SET(tmpTask->counterReg, 1);
    PIN_H(tmpTask->port, tmpTask->pin);
    // Set up the first pause
    *tmpTask->pauseCnt_1 = *tmpTask->srcPauseCnt_1 + tmpTask->pauseValue_1;
    return;
  }

  // Handle the first pause
  if (!FLAG_CHECK(tmpTask->counterReg, 2)) {
    if (*tmpTask->pauseCnt_1 > *tmpTask->srcPauseCnt_1) return;
    FLAG_SET(tmpTask->counterReg, 2);
    return;
  }

  // Turn off the LED
  if (!FLAG_CHECK(tmpTask->counterReg, 3)) {
    FLAG_SET(tmpTask->counterReg, 3);
    PIN_L(tmpTask->port, tmpTask->pin);
    // Set up the first pause
    *tmpTask->pauseCnt_2 = *tmpTask->srcPauseCnt_2 + tmpTask->pauseValue_2;
    return;
  }

  // Handle the second pause
  if (!FLAG_CHECK(tmpTask->counterReg, 4)) {
    if (*tmpTask->pauseCnt_2 > *tmpTask->srcPauseCnt_1) return;
    FLAG_SET(tmpTask->counterReg, 4);
    return;
  }

  // Clear dedicated registry
  tmpTask->counterReg;
}