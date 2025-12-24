/**
  ******************************************************************************
  * @file           : gpio.c
  * @brief          : This file contains code services that use standart GPIO
  *                   pin functions;
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
#include "gpio.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Local variables -----------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/






////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus GPIO_Heartbeat_Init(void) {

  if (PREG_CHECK(HEARTBEAT_LED_Port->LCKR, HEARTBEAT_LED_Pin)) return (ERROR);

  return (SUCCESS);
}





// ----------------------------------------------------------------------------

ErrorStatus GPIO_TM163x_Init(void) {

  /* Check if pins are not locked */
  if (PREG_CHECK(TM_SCK_Port->LCKR, TM_DIO_Pin)) return (ERROR);
  if (PREG_CHECK(TM_DIO_Port->LCKR, TM_DIO_Pin)) return (ERROR);

  return (SUCCESS);
}




// ----------------------------------------------------------------------------

ErrorStatus GPIO_OneWire_Init(void) {

  /* Check if pins are not locked */
  if (PREG_CHECK(OneWire_PORT->LCKR, OneWire_PIN)) return (ERROR);

  return (SUCCESS);
}



// ----------------------------------------------------------------------------

ErrorStatus GPIO_Strip_Init(void) {

  /* Check if pins are not locked */
  if (PREG_CHECK(STRIP_DATA_Port->LCKR, STRIP_DATA_Pin)) return (ERROR);

  return (SUCCESS);
}




/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
