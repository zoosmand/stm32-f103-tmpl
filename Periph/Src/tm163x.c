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
const uint8_t symbols[] = {
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
  
  // if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);
  
  tm163x_Start(dev);
  if (tm163x_WriteByte(dev, 0x40)) return (ERROR);
  tm163x_Stop(dev);
  
  /* clear display from the 1-st address */
  /* TODO for TM1638 that have 16-bit output */
  tm163x_Start(dev);
  if (tm163x_WriteByte(dev, 0xc0)) return (ERROR);
  for(int i = 0; i < 4; i++) {
    if (tm163x_WriteByte(dev, 0xff)) return (ERROR);
  }
  tm163x_Stop(dev);
  
  tm163x_Start(dev);
  if (tm163x_WriteByte(dev, 0x8a)) return (ERROR);
  tm163x_Stop(dev);    

  // dev->Lock = ENABLE;
  return (SUCCESS);
}



// ----------------------------------------------------------------------------

ErrorStatus TM163x_Print(TM163x_TypeDef *dev) {

  tm163x_Start(dev);
  tm163x_WriteByte(dev, 0xc0);
  for (uint8_t i = 0; i < 4; i++) {
    // tm163x_WriteByte(dev, symbols[dev->BufPtr[i]]);
    tm163x_WriteByte(dev, symbols[2]);
  }
  tm163x_Stop(dev); 

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
  
  MODIFY_REG(
    TM_DIO_Port->CRH, 
    TM_DIO_Pin_Mask, 
    (GPIO_IN_PD << ((TM_DIO_Pin_Pos - 8) * 4))
  );
  
  _delay_us(5);

  while (TM_DIO_Level) { if (!(--tmout)) return (ERROR); }

  MODIFY_REG(
    TM_DIO_Port->CRH, 
    TM_DIO_Pin_Mask, 
    ((GPIO_GPO_PP | GPIO_IOS_10) << ((TM_DIO_Pin_Pos - 8) * 4))
  );

  return (SUCCESS);
}



// ----------------------------------------------------------------------------

static ErrorStatus tm163x_WriteByte(TM163x_TypeDef* dev, uint8_t byte) {

  // if (dev->Lock == ENABLE) dev->Lock = DISABLE; else return (ERROR);
  
  for (uint8_t i = 0; i < 8; i++) {
    TM_SCK_Low;
    if (byte & 0x01) {
      TM_DIO_High;
    } else {
      TM_DIO_Low;
    }
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


/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------- */


// -------------------------------------------------------------
void TM1637_Run(TM163x_TypeDef* dev, uint16_t __com_addr, uint8_t byte) {
  uint8_t __command = __com_addr & 0xff;
  uint8_t __address = (__com_addr >> 8) & 0xff;
  
  switch (__command) {
  	case 0x40:
  	case 0x44:
      goto TM1637_Run_Run;
  		break;
  	default:
      printf("Error!");
      Error_Handler();
  		break;
  }
  
  TM1637_Run_Run:
    tm163x_Start(dev);
    tm163x_WriteByte(dev, __command);
    tm163x_Stop(dev); 

    tm163x_Start(dev);
    tm163x_WriteByte(dev, __address);    

    if ((__com_addr & 0xff) == 0x40) {
      for(int i = 0; i < 4; i++) {
        tm163x_WriteByte(dev, byte);
      }
    } else {
      tm163x_WriteByte(dev, byte);
    }
    tm163x_Stop(dev);
}





// -------------------------------------------------------------
uint8_t Display(int32_t __data, DisplayInfo_TypeDef __info_type, ErrorLevel_TypeDev __er_state) {
  
  uint32_t displayData = 0;
  uint8_t tick = 0;

  if (__er_state == NO_ERR) {

    switch (__info_type) {
      case CLOCK:
        displayData |= 
            symbols[(__data/1000) ? __data/1000 : 0x0a]
          | ((symbols[__data%1000/100] | tick) << 8)
          | (symbols[__data%100/10] << 16)
          | (symbols[__data%10] << 24);
        break;
      case DIG_W_DOT_1:
      case DIG_W_DOT_2:
      case DIG_W_DOT_3:
        displayData |= (0x80 << (8*__info_type));
        if (__data >= 0) {
          if (__data >= 10000) {
            Display(0, DIGITS, ERR_1);
            break;
          }
          if (__data < 1000) {
            if (__info_type == DIG_W_DOT_3) {
              displayData |= symbols[0];
            }
          }
          if (__data < 100) {
            if ((__info_type == DIG_W_DOT_2) || (__info_type == DIG_W_DOT_3)) {
              displayData |= symbols[0] << 8; 
            }
          }
          if (__data < 10) {
            displayData |= symbols[0] << 16; 
          }

          displayData |= 
              symbols[(__data/1000) ? __data/1000 : 0x0a]
            | ((symbols[(__data%1000/100) ? __data%1000/100 : 0x0a]) << 8)
            | ((symbols[(__data%100/10) ? __data%100/10 : 0x0a]) << 16)
            | (symbols[__data%10] << 24);
        } else {
          __data *= -1;
          if ((__data >= 1000) || (__info_type == DIG_W_DOT_3)) {
            Display(0, DIGITS, ERR_2);
            break;
          }
          if (__info_type == DIG_W_DOT_2) {
            displayData |= 
                symbols[(__data < 1000) ? 0x0b : (__data < 100) ? 0x0b : 0x0a]
              | ((symbols[(__data < 100) ? 0 : (__data < 10) ? 0x0b : (__data%1000/100)]) << 8);
          }
          if (__info_type == DIG_W_DOT_1) {
            displayData |= 
                symbols[(__data > 99) ? 0x0b : 0x0a]
            | ((symbols[(__data < 100) ? 0x0b : (__data < 10) ? 0x0b : (__data%1000/100)]) << 8);
          }

          displayData |= 
              ((symbols[(__data < 10) ? 0 : (__data%100/10)]) << 16)
            | (symbols[__data%10] << 24);
        }
        break;
        
      default:
        if (__data >= 0) {
          if (__data >= 10000) {
            Display(0, DIGITS, ERR_1);
            break;
          }
          displayData |= 
              symbols[(__data/1000) ? __data/1000 : 0x0a]
            | ((symbols[(__data%1000/100) ? __data%1000/100 : 0x0a]) << 8)
            | ((symbols[(__data%100/10) ? __data%100/10 : 0x0a]) << 16)
            | (symbols[__data%10] << 24);
        } else {
          __data *= -1;
          if (__data >= 1000) {
            Display(0, DIGITS, ERR_2);
            break;
          }
          displayData |= 
              symbols[(__data > 99) ? 0x0b : 0x0a]
            | ((symbols[(__data < 100) ? (__data < 10) ? 0x0a : 0x0b : (__data%1000/100)]) << 8)
            | ((symbols[(__data < 10) ? (__data < 0) ? 0x0a : 0x0b : (__data%100/10)]) << 16)
            | (symbols[__data%10] << 24);
        }
        break;
    }
  }
  else {
    displayData = 
        symbols[0x0d]
      | (symbols[0x0e]<< 8)
      | (symbols[0x0e] << 16)
      | (symbols[__er_state] << 24);
  }
  return (tick) ? 0 : 0x80;
}
