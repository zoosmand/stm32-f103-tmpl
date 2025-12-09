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
__attribute__((section(".cron"))) static uint32_t boschTaskCnt = 0;
__attribute__((section(".cron"))) static uint32_t boschTaskReg = 0;

static task_scheduler_t boschScheduler = {
  .counter        = &boschTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 7,
  .counterReg     = &boschTaskReg,
  .entranceFlag   = 31,
};

static uint8_t boschRawData_280[32];
static int32_t boschResults_280[3];

static BMxX80_TypeDef bosch_280 = {
  .DevID        = 0,
  .RawBufPtr    = boschRawData_280,
  .ResBufPtr    = boschResults_280,
  .Lock         = DISABLE,
  .I2Cx         = I2C1,
  .I2C_Address  = BMX280_I2C_ADDR,
  // .SPIx         = NULL,
  // .SPINssPort   = NULL,
  // .SPINssPin    = NULL,
  .DMAx         = DMA1,
  .DMAxRx       = DMA1_Channel7,
};

static uint8_t  boschRawData_680[24];
static uint16_t boschCalibData_680[24];
static int32_t  boschResults_680[4];

static BMxX80_TypeDef bosch_680 = {
  .DevID        = 0,
  .RawBufPtr    = boschRawData_680,
  .ResBufPtr    = boschResults_680,
  .CalibBufPtr  = boschCalibData_680,
  .Lock         = DISABLE,
  .I2Cx         = I2C1,
  .I2C_Address  = BMX680_I2C_ADDR,
  // .SPIx         = SPI1,
  // .SPINssPort   = SPI1_NSS_2_Port,
  // .SPINssPin    = SPI1_NSS_2_Pin,
  // .DMAx         = DMA1,
  // .DMAxTx       = DMA1_Channel3,
  // .DMAxRx       = DMA1_Channel2,
  .DMAx         = DMA1,
  .DMAxRx       = DMA1_Channel7,
};

/* Dallas DS related data */
__attribute__((section(".cron"))) static uint32_t dsTaskCnt = 0;
__attribute__((section(".cron"))) static uint32_t dsTaskReg = 0;

static task_scheduler_t dsScheduler = {
  .counter        = &dsTaskCnt,
  .counterSrc     = &secCnt,
  .period         = 5,
  .counterReg     = &dsTaskReg,
  .entranceFlag   = 31,
};

static OneWireDevice_t ow_pack[16];

static OneWireBus_TypeDef ow_set = {
  .Count    = 16,
  .Pin      = OneWire_PIN,
  .Port     = OneWire_PORT,
  .Devs     = ow_pack,
  .Lock     = DISABLE,
};



/* Private function prototypes ----------------------------------------------*/






////////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------

void BoschMeasurment_CronHandler(void) {

  Scheduler_Handler(&boschScheduler);
  
  uint8_t tmpBuf[4];

  if (FLAG_CHECK(boschScheduler.counterReg, boschScheduler.entranceFlag)) {

    FLAG_CLR(boschScheduler.counterReg, boschScheduler.entranceFlag);
    
    if (bosch_280.Lock == DISABLE) {
      if (BMx280_Measurement(&bosch_280)) {
        /* TODO reinitialize device overwise clear rediness flag */
        printf("Cannot collect Bosch device (BMx280) data\n");
        bosch_280.Lock = ENABLE;
      } else {
        sprintf(tmpBuf, "%i", boschResults_280[0]);
      }
    }

    if (bosch_680.Lock == DISABLE) {
      if (BMx680_Measurement(&bosch_680)) {
        /* TODO reinitialize device overwise clear rediness flag */
        printf("Cannot collect Bosch device (MBx680) data\n");
        bosch_680.Lock = ENABLE;
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

    WHxxxx_TypeDef* whDsplDev = Get_WhDiplayDevice(1602);
    
    if (whDsplDev->Lock == DISABLE) {
      uint8_t tmpBuf2[5];
      tmpBuf2[0] = tmpBuf[0];
      tmpBuf2[1] = tmpBuf[1];
      tmpBuf2[2] = '.';
      tmpBuf2[3] = tmpBuf[2];
      tmpBuf2[4] = tmpBuf[3];
      if (WHxxxx_Print(whDsplDev, tmpBuf2, 5)) {
        printf("Cannot output data to HW display\n");
        whDsplDev->Lock = ENABLE;
      }
    }
  }
}




// ----------------------------------------------------------------------------

BMxX80_TypeDef* Get_BoschDevice(uint16_t model) {
  
  if (model == 280) return &bosch_280;
  if (model == 680) return &bosch_680;

  return NULL;
}



// ----------------------------------------------------------------------------

void DsMeasurment_CronHandler(void) {
  Scheduler_Handler(&dsScheduler);

  if (FLAG_CHECK(dsScheduler.counterReg, dsScheduler.entranceFlag)) {

    FLAG_CLR(dsScheduler.counterReg, dsScheduler.entranceFlag);

    /* TODO realize heath check */
    
    OneWireDevice_t* owDev = Get_OneWireDevice(0);

    if (owDev->Lock == DISABLE) {
      if (DS18B20_GetTemperatureMeasurment(owDev)) {
        /* --- on error, set up -128.00 C --- */
        owDev->Spad[0] = 0x00;
      }
      uint32_t* t1 = (int32_t*)&owDev->Spad;
      printf("%d.%02d\n", 
        (int8_t)((*t1 & 0x0000fff0) >> 4), (uint8_t)(((*t1 & 0x0000000f) * 100) >> 4)
      );

      /* TODO handle DS data usage */
      __NOP();
    }
  }
}



// ----------------------------------------------------------------------------

OneWireBus_TypeDef* Get_OneWireBusDevice(void) {

  return &ow_set;
}


// ----------------------------------------------------------------------------

OneWireDevice_t* Get_OneWireDevice(uint8_t denNum) {

  ow_set.Devs[denNum].ParentBusPtr = (uint32_t*)&ow_set;
  ow_set.Devs[denNum].Lock = DISABLE;

  return &ow_set.Devs[denNum];
}
