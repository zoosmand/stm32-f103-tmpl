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
#include "heartbeat.h"


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
  .PauseValue_1   = 15,
  .PauseCnt_2     = &ledGreenTaskPauseCnt_2,
  .SrcPauseCnt_2  = &sysCnt,
  .PauseValue_2   = 50,
};





////////////////////////////////////////////////////////////////////////////////





// ----------------------------------------------------------------------------

ErrorStatus Heartbeat_Init(HearbeatDevice_TypeDev* dev) {

  /* Init GPIO */
  if (dev->Pin > 7) {
    MODIFY_REG(
      dev->Port->CRH, 
      (0x0f << ((dev->Pin - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_2) << ((dev->Pin - 8) * 4))
    );
  } else {
    MODIFY_REG(
      dev->Port->CRL, 
      (0x0f << ((dev->Pin - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_2) << (dev->Pin * 4))
    );
  }

  PIN_H(dev->Port, dev->Pin);

  return (SUCCESS);
}







void Heartbeat_CronHandler(void) {

  // Scheduler_Handler(&ledRedScheduler);
  // Scheduler_Handler(&ledBlueScheduler);
  Scheduler_Handler(&ledGreenScheduler);

  // TASK_CTRL(ledRedTask);
  // TASK_CTRL(ledBlueTask);
  TASK_CTRL(ledGreenHearbeatDevice);

}


static void LedToggle_Task(uint32_t *task) {
//   __I task_led_toggle_t* tmpTask = (task_led_toggle_t*)task;

//   // Turn on the LED
//   if (!FLAG_CHECK(tmpTask->scheduler->counterReg, 1)) {
//     FLAG_SET(tmpTask->scheduler->counterReg, 1);
//     PIN_L(tmpTask->port, tmpTask->pin);
//     // Set up the first pause
//     *tmpTask->pauseCnt_1 = *tmpTask->srcPauseCnt_1 + tmpTask->pauseValue_1;
//     return;
//   }

//   // Handle the first pause
//   if (!FLAG_CHECK(tmpTask->scheduler->counterReg, 2)) {
//     if (*tmpTask->pauseCnt_1 > *tmpTask->srcPauseCnt_1) return;
//     FLAG_SET(tmpTask->scheduler->counterReg, 2);
//     return;
//   }

//   // Turn off the LED
//   if (!FLAG_CHECK(tmpTask->scheduler->counterReg, 3)) {
//     FLAG_SET(tmpTask->scheduler->counterReg, 3);
//     PIN_H(tmpTask->port, tmpTask->pin);
//     // Set up the second pause
//     *tmpTask->pauseCnt_2 = *tmpTask->srcPauseCnt_2 + tmpTask->pauseValue_2;
//     return;
//   }

//   // Handle the second pause
//   if (!FLAG_CHECK(tmpTask->scheduler->counterReg, 4)) {
//     if (*tmpTask->pauseCnt_2 > *tmpTask->srcPauseCnt_1) return;
//     FLAG_SET(tmpTask->scheduler->counterReg, 4);
//     return;
//   }

//   // Clear dedicated registry
//   *tmpTask->scheduler->counterReg = 0;

}




HearbeatDevice_TypeDev* Get_HeartbeatDevice(void) {
  return &ledGreenHearbeatDevice;
}