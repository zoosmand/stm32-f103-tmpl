/**
  ******************************************************************************
  * @file           : tm163x.c
  * @brief          : This file contains code of TM163x Two Wire shift register 
  *                   for a digital display code.
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
#include "tm163x.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Locaal variables ---------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

/**
  * @brief  Sends a start command to the given shift register.
  * @param  dev pointer of the given device
  * @return status of operation
  */
static ErrorStatus tm163x_Start(TM163x_TypeDef*);

/**
  * @brief  Sends a stop command to the given shift register.
  * @param  dev pointer of the given device
  * @return status of operation
  */
static ErrorStatus tm163x_Stop(TM163x_TypeDef*);

/**
  * @brief  Writes the given byte to the given shift register.
  * @param  dev pointer of the given device
  * @param  byte byte to be sent
  * @return status of operation
  */
static ErrorStatus tm163x_WriteByte(TM163x_TypeDef*, uint8_t);





////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus TM163x_Init(TM163x_TypeDef* dev) {
  
  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);
  
  
  dev->Lock = ENABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

ErrorStatus TM163x_Print(TM163x_TypeDef *dev) {
  
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

static ErrorStatus tm163x_Start(TM163x_TypeDef* dev) {
  
  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);
  
  
  dev->Lock = ENABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

static ErrorStatus tm163x_Stop(TM163x_TypeDef* dev) {
  
  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);
  
  
  dev->Lock = ENABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

static ErrorStatus tm163x_WriteByte(TM163x_TypeDef* dev, uint8_t byte) {

  if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);
  
  
  dev->Lock = ENABLE;
  return (SUCCESS);
}


/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
