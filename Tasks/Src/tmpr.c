/**
  ******************************************************************************
  * @file           : tmpr.c
  * @brief          : This file contains code for measurment of tempetature,
  *                   humidity, and atmospheric pressure.
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
#include "tmpr.h"


/* Private includes ----------------------------------------------------------*/

/* Bosch related data */
__attribute__((section(".cron"))) static uint32_t boschTaskCnt    = 0;
__attribute__((section(".cron"))) static uint32_t boschTaskReg    = 0;

static task_scheduler_t boschScheduler = {
  .counter        = &boschTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 7,
  .counterReg     = &boschTaskReg,
  .entranceFlag   = 31,
};

static uint8_t boschRawData[32];
static int32_t boschResults[3];

static BMxX80_TypeDef bosch_0 = {
  .DevID      = 0,
  .RawBufPtr  = boschRawData,
  .ResBufPtr  = boschResults,
  .Lock       = ENABLE,
  .I2CBus     = I2C1,
  .SPIBus     = NONE,
};

/* Dallas DS related data */
__attribute__((section(".cron"))) static uint32_t dsTaskCnt       = 0;
__attribute__((section(".cron"))) static uint32_t dsTaskReg       = 0;

static task_scheduler_t dsScheduler = {
  .counter        = &dsTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 5,
  .counterReg     = &dsTaskReg,
  .entranceFlag   = 31,
};



/* Private function prototypes ----------------------------------------------*/






////////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------

void BoschMeasurment_CronHandler(void) {

  Scheduler_Handler(&boschScheduler);

  if (FLAG_CHECK(boschScheduler.counterReg, boschScheduler.entranceFlag)) {

    FLAG_CLR(boschScheduler.counterReg, boschScheduler.entranceFlag);
    
    if (BMx280_Measurment(&bosch_0)) printf("Cannot collect Bosch device data\n");

    /* TODO handle Bosch data usage */
    __NOP();
  }
}




// ----------------------------------------------------------------------------

BMxX80_TypeDef* Get_BoschDevice(void) {
  
  return &bosch_0;
}



// ----------------------------------------------------------------------------

void DsMeasurment_CronHandler(void) {
  Scheduler_Handler(&dsScheduler);

  if (FLAG_CHECK(dsScheduler.counterReg, dsScheduler.entranceFlag)) {

    FLAG_CLR(dsScheduler.counterReg, dsScheduler.entranceFlag);
    
    OneWireDevice_t* devs = Get_OwDevices();

    for (uint8_t i = 0; i < 2; i++) {
      if (DS18B20_GetTemperatureMeasurment(&devs[i])) {
        /* --- on error, set up -128.00 C --- */
        devs[i].spad[0] = 0x00;
        devs[i].spad[1] = 0x08;
      }
    }

    uint32_t* t1 = (int32_t*)&devs[0].spad;
    uint32_t* t2 = (int32_t*)&devs[1].spad;
    printf("%d.%02d %d.%02d\n", 
      (int8_t)((*t1 & 0x0000fff0) >> 4), (uint8_t)(((*t1 & 0x0000000f) * 100) >> 4),
      (int8_t)((*t2 & 0x0000fff0) >> 4), (uint8_t)(((*t2 & 0x0000000f) * 100) >> 4)
    );

    /* TODO handle DS data usage */
    __NOP();
  }

}