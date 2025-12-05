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


static void LedToggle_Task(HearbeatDevice_TypeDev*);

/* Private includes ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

__attribute__((section(".cron"))) static uint32_t hbCounter   = 0;
__attribute__((section(".cron"))) static uint32_t hbRegister  = 0;
__attribute__((section(".cron"))) static uint32_t hbPause_1   = 0;
__attribute__((section(".cron"))) static uint32_t hbPause_2   = 0;



/* ----------------------------------------------------------------------------- */
static task_scheduler_t heartbeatScheduler = {
  .counter        = &hbCounter,
  .counterSrc     = &secCnt,
  .period         = 3,
  .counterReg     = &hbRegister,
  .entranceFlag   = 31,
};

static HearbeatDevice_TypeDev hearbeatLedDevice = {
  .Scheduler      = &heartbeatScheduler,
  .Port           = HEARTBEAT_LED_Port,
  .Pin            = HEARTBEAT_LED_Pin,
  // .Callback       = &LedToggle_Task,
  .PauseCnt_1     = &hbPause_1,
  .SrcPauseCnt_1  = &sysCnt,
  .PauseValue_1   = 5,
  .PauseCnt_2     = &hbPause_2,
  .SrcPauseCnt_2  = &sysCnt,
  .PauseValue_2   = 5,
};





////////////////////////////////////////////////////////////////////////////////





// ----------------------------------------------------------------------------

void Heartbeat_CronHandler(void) {

  Scheduler_Handler(&heartbeatScheduler);

  // TASK_CTRL(hearbeatLedDevice);
  if (FLAG_CHECK(heartbeatScheduler.counterReg, heartbeatScheduler.entranceFlag)) {
    LedToggle_Task(&hearbeatLedDevice);
  }

}


// ----------------------------------------------------------------------------

static void LedToggle_Task(HearbeatDevice_TypeDev* dev) {

  // Turn on the LED
  if (!FLAG_CHECK(dev->Scheduler->counterReg, 1)) {
    FLAG_SET(dev->Scheduler->counterReg, 1);
    PIN_L(dev->Port, dev->Pin);
    // Set up the first pause
    *dev->PauseCnt_1 = *dev->SrcPauseCnt_1 + dev->PauseValue_1;
    return;
  }

  // Handle the first pause
  if (!FLAG_CHECK(dev->Scheduler->counterReg, 2)) {
    if (*dev->PauseCnt_1 > *dev->SrcPauseCnt_1) return;
    FLAG_SET(dev->Scheduler->counterReg, 2);
    return;
  }

  // Turn off the LED
  if (!FLAG_CHECK(dev->Scheduler->counterReg, 3)) {
    FLAG_SET(dev->Scheduler->counterReg, 3);
    PIN_H(dev->Port, dev->Pin);
    // Set up the second pause
    *dev->PauseCnt_2 = *dev->SrcPauseCnt_2 + dev->PauseValue_2;
    return;
  }

  // Handle the second pause
  if (!FLAG_CHECK(dev->Scheduler->counterReg, 4)) {
    if (*dev->PauseCnt_2 > *dev->SrcPauseCnt_1) return;
    FLAG_SET(dev->Scheduler->counterReg, 4);
    return;
  }

  // Clear dedicated registry
  *dev->Scheduler->counterReg = 0;

}



// ----------------------------------------------------------------------------

HearbeatDevice_TypeDev* Get_HeartbeatDevice(void) {
  return &hearbeatLedDevice;
}