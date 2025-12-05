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
#include "hchk.h"


static void LedToggle_Task(uint32_t *task);

/* Private includes ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

__attribute__((section(".cron"))) static uint32_t ledGreenTaskCnt         = 0;
__attribute__((section(".cron"))) static uint32_t ledGreenTaskReg         = 0;
__attribute__((section(".cron"))) static uint32_t ledGreenTaskPauseCnt_1  = 0;
__attribute__((section(".cron"))) static uint32_t ledGreenTaskPauseCnt_2  = 0;



/* ----------------------------------------------------------------------------- */
static task_scheduler_t ledGreenScheduler = {
  .counter        = &ledGreenTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 3,
  .counterReg     = &ledGreenTaskReg,
  .entranceFlag   = 31,
};

static HearbeatDevice_TypeDev ledGreenHearbeatDevice = {
  .Scheduler      = &ledGreenScheduler,
  .Port           = HEARTBEAT_LED_Port,
  .Pin            = HEARTBEAT_LED_Pin,
  .Callback       = &LedToggle_Task,
  .PauseCnt_1     = &ledGreenTaskPauseCnt_1,
  .SrcPauseCnt_1  = &sysCnt,
  .PauseValue_1   = 1,
  .PauseCnt_2     = &ledGreenTaskPauseCnt_2,
  .SrcPauseCnt_2  = &sysCnt,
  .PauseValue_2   = 50,
};





////////////////////////////////////////////////////////////////////////////////





// ----------------------------------------------------------------------------

void Heartbeat_CronHandler(void) {

  Scheduler_Handler(&ledGreenScheduler);

  TASK_CTRL(ledGreenHearbeatDevice);

}


// ----------------------------------------------------------------------------

static void LedToggle_Task(uint32_t *task) {
  __I HearbeatDevice_TypeDev* tmpTask = (HearbeatDevice_TypeDev*)task;

  // Turn on the LED
  if (!FLAG_CHECK(tmpTask->Scheduler->counterReg, 1)) {
    FLAG_SET(tmpTask->Scheduler->counterReg, 1);
    PIN_L(tmpTask->Port, tmpTask->Pin);
    // Set up the first pause
    *tmpTask->PauseCnt_1 = *tmpTask->SrcPauseCnt_1 + tmpTask->PauseValue_1;
    return;
  }

  // Handle the first pause
  if (!FLAG_CHECK(tmpTask->Scheduler->counterReg, 2)) {
    if (*tmpTask->PauseCnt_1 > *tmpTask->SrcPauseCnt_1) return;
    FLAG_SET(tmpTask->Scheduler->counterReg, 2);
    return;
  }

  // Turn off the LED
  if (!FLAG_CHECK(tmpTask->Scheduler->counterReg, 3)) {
    FLAG_SET(tmpTask->Scheduler->counterReg, 3);
    PIN_H(tmpTask->Port, tmpTask->Pin);
    // Set up the second pause
    *tmpTask->PauseCnt_2 = *tmpTask->SrcPauseCnt_2 + tmpTask->PauseValue_2;
    return;
  }

  // Handle the second pause
  if (!FLAG_CHECK(tmpTask->Scheduler->counterReg, 4)) {
    if (*tmpTask->PauseCnt_2 > *tmpTask->SrcPauseCnt_1) return;
    FLAG_SET(tmpTask->Scheduler->counterReg, 4);
    return;
  }

  // Clear dedicated registry
  *tmpTask->Scheduler->counterReg = 0;

}



// ----------------------------------------------------------------------------

HearbeatDevice_TypeDev* Get_HeartbeatDevice(void) {
  return &ledGreenHearbeatDevice;
}