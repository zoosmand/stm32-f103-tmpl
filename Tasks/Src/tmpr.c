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
__attribute__((section(".cron"))) static uint32_t boschTaskCnt           = 0;
__attribute__((section(".cron"))) static uint32_t boschTaskReg           = 0;

static task_scheduler_t boschScheduler = {
  .counter        = &boschTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 7,
  .counterReg     = &boschTaskReg,
  .entranceFlag   = 31,
};

static int32_t bosch_data[3];

static BMx280_ItemTypeDef bosch_0 = {
  .sensorType = BME280,
  .busType = BMx280_I2C,
  .bus = I2C1
};


/* Private function prototypes ----------------------------------------------*/






////////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------

void BoschMeasurment_CronHandler(void) {

  Scheduler_Handler(&boschScheduler);

  if (FLAG_CHECK(boschScheduler.counterReg, boschScheduler.entranceFlag)) {

    FLAG_CLR(boschScheduler.counterReg, boschScheduler.entranceFlag);
    
    if (BMx280_Measurment(&bosch_0, bosch_data)) printf("Cannot collect Bosch device data\n");

    /* TODO handle Bosch data usage */
  }
}




// ----------------------------------------------------------------------------

BMx280_ItemTypeDef* Get_BoschDevice(void) {

  return &bosch_0;
}

