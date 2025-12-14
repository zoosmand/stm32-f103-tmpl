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
static const uint8_t symbols[] = {
  0x3f, // 0 - 0x3f
  0x06, // 1 - 0x06
  0x5b, // 2 - 0x5b
  0x4f, // 3 - 0x4f
  0x66, // 4 - 0x66
  0x6d, // 5 - 0x6d
  0x7d, // 6 - 0x7d
  0x07, // 7 - 0x07
  0x7f, // 8 - 0x7f
  0x6f, // 9 - 0x6f
  0x00, //   - 0x00
  0x40, // - - 0x40
  0x80, // . - 0x80
  0x79, // E - 0x79
  0x50, // r - 0x50
  0x78, // t - 0x78
  0x63, // o - 0x63
  0x4e, // o - 0x4e
  0x58  // c - 0x58
};


/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

/**
  * @brief  Sends a start command to the given shift register.
  * @param  dev pointer of the given device
  * @return none
  */
static void tm163x_Start(TM163x_TypeDef*);

/**
  * @brief  Sends a stop command to the given shift register.
  * @param  dev pointer of the given device
  * @return none
  */
static void tm163x_Stop(TM163x_TypeDef*);

/**
  * @brief  Reads an ACK from the bus.
  * @param  dev pointer of the given device
  * @return status of operation
  */
static ErrorStatus tm163x_ReadAck(TM163x_TypeDef*);

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
  
  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  /* Init GPIO */
  if (dev->PinDio > 7) {
    MODIFY_REG(
      dev->PortDio->CRH, 
      (0x0f << ((dev->PinDio - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_10) << ((dev->PinDio - 8) * 4))
    );
  } else {
    MODIFY_REG(
      dev->PortDio->CRL, 
      (0x0f << ((dev->PinDio - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_10) << (dev->PinDio * 4))
    );
  }

  if (dev->PinSck > 7) {
    MODIFY_REG(
      dev->PortSck->CRH, 
      (0x0f << ((dev->PinSck - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_10) << ((dev->PinSck - 8) * 4))
    );
  } else {
    MODIFY_REG(
      dev->PortSck->CRL, 
      (0x0f << ((dev->PinSck - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_10) << (dev->PinSck * 4))
    );
  }

  /* Clear diaplae segments */
  tm163x_Start(dev);
  if (tm163x_WriteByte(dev, 0x40)) return (ERROR);
  tm163x_Stop(dev);
  
  /* clear display from the 1-st address */
  /* TODO Modify the output for TM1638 */
  tm163x_Start(dev);
  if (tm163x_WriteByte(dev, 0xc0)) return (ERROR);
  for(int i = 0; i < 4; i++) {
    if (tm163x_WriteByte(dev, symbols[10])) return (ERROR);
  }
  tm163x_Stop(dev);
  
  tm163x_Start(dev);
  if (tm163x_WriteByte(dev, 0x8a)) return (ERROR);
  tm163x_Stop(dev);    

  dev->Lock = DISABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

ErrorStatus TM163x_Print(TM163x_TypeDef *dev) {

  dev->Lock = ENABLE;
  
  tm163x_Start(dev);
  if (tm163x_WriteByte(dev, 0xc0)) return (ERROR);

  uint8_t cop = 0;
  if (dev->Dig0 >= 0x30) cop = 0x30;

  /* TODO Modify the output for TM1638 */
  if (tm163x_WriteByte(dev, symbols[dev->Dig0 - cop])) return (ERROR);
  if (tm163x_WriteByte(dev, (symbols[dev->Dig1 - cop]) | 0x80)) return (ERROR);
  if (tm163x_WriteByte(dev, symbols[dev->Dig2 - cop])) return (ERROR);
  if (tm163x_WriteByte(dev, symbols[dev->Dig3 - cop])) return (ERROR);

  tm163x_Stop(dev); 

  dev->Lock = DISABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

static void tm163x_Start(TM163x_TypeDef* dev) {
  PIN_H(dev->PortSck, dev->PinSck);
  PIN_H(dev->PortDio, dev->PinDio);
  
  _delay_us(2);
  PIN_L(dev->PortDio, dev->PinDio);
}



// ----------------------------------------------------------------------------

static void tm163x_Stop(TM163x_TypeDef* dev) {
  PIN_L(dev->PortSck, dev->PinSck);
  _delay_us(2);
  
  PIN_L(dev->PortDio, dev->PinDio);
  _delay_us(2);
  
  PIN_H(dev->PortSck, dev->PinSck);
  _delay_us(2);
  
  PIN_H(dev->PortDio, dev->PinDio);
}



// ----------------------------------------------------------------------------

static ErrorStatus tm163x_ReadAck(TM163x_TypeDef* dev) {

  uint32_t tmout = TM_BUS_TMOUT;

  if (dev->PinDio > 7) {
    MODIFY_REG(
      dev->PortDio->CRH, 
      (0x0f << ((dev->PinDio - 8) * 4)), 
      (GPIO_IN_PD << ((dev->PinDio - 8) * 4))
    );
  } else {
    MODIFY_REG(
      dev->PortDio->CRL, 
      (0x0f << ((dev->PinDio - 8) * 4)), 
      (GPIO_IN_PD << (dev->PinDio * 4))
    );
  }
    
  _delay_us(5);

  while (TM_DIO_Level) { if (!(--tmout)) return (ERROR); }

  if (dev->PinDio > 7) {
    MODIFY_REG(
      dev->PortDio->CRH, 
      (0x0f << ((dev->PinDio - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_10) << ((dev->PinDio - 8) * 4))
    );
  } else {
    MODIFY_REG(
      dev->PortDio->CRL, 
      (0x0f << ((dev->PinDio - 8) * 4)), 
      ((GPIO_GPO_PP | GPIO_IOS_10) << (dev->PinDio * 4))
    );
  }

  return (SUCCESS);
}



// ----------------------------------------------------------------------------

static ErrorStatus tm163x_WriteByte(TM163x_TypeDef* dev, uint8_t byte) {

  for (uint8_t i = 0; i < 8; i++) {
    TM_SCK_Low;
    if (byte & 0x01) { TM_DIO_High; } else { TM_DIO_Low; }
    byte >>= 1;
    _delay_us(3);
    TM_SCK_High;
    _delay_us(3);
  }
  
  TM_SCK_Low;
  TM_DIO_Low;

  if (tm163x_ReadAck(dev)) return (ERROR);

  TM_SCK_High;
  _delay_us(2);
  TM_SCK_Low;
  return (SUCCESS);
}
