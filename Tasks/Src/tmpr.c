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


/* Private variables ---------------------------------------------------------*/

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

static uint8_t boschRawData_0[32];
static int32_t boschResults_0[3];

static BMxX80_TypeDef bosch_0 = {
  .DevID        = 0,
  .RawBufPtr    = boschRawData_0,
  .ResBufPtr    = boschResults_0,
  .Lock         = DISABLE,
  .I2Cx         = I2C1,
  .I2C_Address  = BMX280_I2C_ADDR,
  // .SPIx         = NULL,
  // .SPINssPort   = NULL,
  // .SPINssPin    = NULL,
  .DMAx         = DMA1,
  .DMAxTx       = DMA1_Channel6,
  .DMAxRx       = DMA1_Channel7,
};

static uint8_t boschRawData_1[42];
static int32_t boschResults_1[4];

static BMxX80_TypeDef bosch_1 = {
  .DevID        = 0,
  .RawBufPtr    = boschRawData_1,
  .ResBufPtr    = boschResults_1,
  .Lock         = DISABLE,
  // .I2Cx         = NULL,
  // .I2C_Address  = NULL,
  .SPIx         = SPI1,
  .SPINssPort   = GPIOA,
  .SPINssPin    = 2,
  .DMAx         = DMA1,
  .DMAxTx       = DMA1_Channel3,
  .DMAxRx       = DMA1_Channel2,
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
  
  uint8_t tmpBuf[4];

  if (FLAG_CHECK(boschScheduler.counterReg, boschScheduler.entranceFlag)) {

    FLAG_CLR(boschScheduler.counterReg, boschScheduler.entranceFlag);
    
    if (bosch_0.Lock == DISABLE) {
      if (BMx280_Measurment(&bosch_0)) {
        /* TODO reinitialize device overwise clear rediness flag */
        printf("Cannot collect Bosch device (BMx280) data\n");
        bosch_0.Lock = ENABLE;
      } else {
        sprintf(tmpBuf, "%i", boschResults_0[0]);
      }
    }

    if (bosch_1.Lock == DISABLE) {
      if (BMx680_Measurment(&bosch_1)) {
        /* TODO reinitialize device overwise clear rediness flag */
        printf("Cannot collect Bosch device (MBx680) data\n");
        bosch_1.Lock = ENABLE;
      }
    }

    /* TODO handle Bosch data usage */
    TM163x_TypeDef* tmDsplDev = Get_TmDiplayDevice();

    if (tmDsplDev->Lock == DISABLE) {
      
        tmDsplDev->Dig0 = tmpBuf[0];
        tmDsplDev->Dig1 = tmpBuf[1];
        tmDsplDev->Dig2 = tmpBuf[2];
        tmDsplDev->Dig3 = tmpBuf[3];

        if (TM163x_Print(tmDsplDev)) {
        printf("Cannot output data to TM display\n");
        tmDsplDev->Lock = ENABLE;
      }
    }

    Max72xx_TypeDef* maxDsplDev = Get_MaxDiplayDevice();
    
    if (maxDsplDev->Lock == DISABLE) {
      if (MAX72xx_Print(maxDsplDev, tmpBuf)) {
        printf("Cannot output data to MAX display\n");
        maxDsplDev->Lock = ENABLE;
      }
    }
  }
}




// ----------------------------------------------------------------------------

BMxX80_TypeDef* Get_BoschDevice(uint8_t devNum) {
  
  if (devNum == 0) return &bosch_0;
  if (devNum == 1) return &bosch_1;

  return NULL;
}



// ----------------------------------------------------------------------------

void DsMeasurment_CronHandler(void) {
  Scheduler_Handler(&dsScheduler);

  if (FLAG_CHECK(dsScheduler.counterReg, dsScheduler.entranceFlag)) {

    FLAG_CLR(dsScheduler.counterReg, dsScheduler.entranceFlag);

    /* TODO reinitialize device overwise clear rediness flag */
    
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