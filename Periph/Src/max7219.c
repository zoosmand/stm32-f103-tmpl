/**
  ******************************************************************************
  * @file           : max7219.c
  * @brief          : This file contains MAX 7219 display
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
#include "max7219.h"

/* Private variables ---------------------------------------------------------*/
static uint16_t maxData[(MAX7219_SEG_CNT * 8)];

/* Private function prototypes -----------------------------------------------*/
static int MAX7219_WriteBus(SPI_TypeDef*, const uint16_t);
// static int MAX7219_DrawLine(SPI_TypeDef*);








////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Transfesr SPI data via DMA
  * @param  cnt: Count of bytes to transfer
  * @param  dir: Direction - READ or WRITE EEPROM
  * @param  offset: Offset of a particular address of EEPROM
  * @retval None
  */
int MAX7219_Init(SPI_TypeDef* SPIx) {

  if (SPI_Enable(SPIx, SPIBufLen_16bit)) return (1);
  NSS_0_H;
  
  _delay_ms(5);
  
  // const uint16_t maxInit[1] = {
  //   0x0c00, // 0x0c - Shutdown,     0x00 - Shutdown, 0x01 - Run 
  // };
  const uint16_t maxInit[15] = {
    0x0c00, // 0x0c - Shutdown,     0x00 - Shutdown, 0x01 - Run 
    0x0f00, // 0x0f - DisplayTest,  0x00 - Off, 0x01 - On
    0x0900, // 0x09 - Decode Mode,  0x00 - No decode for digits 7�0, 0xff - Code B decode for digits 7�0
    0x0f00, // 0x0a - Intensity,    0x00 - lowest, 0x0f - highest
    0x0b07, // 0x0b - Scan-Limit,   0x07 - Display digits 0 1 2 3 4 5 6 7
    0x0100, // 0x01 - Line 1
    0x0200,
    0x0300,
    0x0400,
    0x0500,
    0x0600,
    0x0700,
    0x0800, // 0x08 - Line 8
    0x0c01, // 0x0c - Shutdown,     0x00 - Shutdown, 0x01 - Run 
    0x0000  // 0x00 - NOP
  };
  

  uint8_t segs = 0;
  
  for (uint8_t i = 0; i < sizeof(maxInit)/2; i++) {
    segs = MAX7219_SEG_CNT;
    NSS_0_L;
    while (segs-- > 0) {
      // if (SPI_Write_16b(SPIx, &maxInit[i], 1)) return (1);
      if (MAX7219_WriteBus(SPI1, maxInit[i])) return (1);
    }
    NSS_0_H;
  }
  

  
  
//  _delay_ms(10);
//  NSS_0_L;
//  SPI_Write_16b(SPIx, (uint16_t*)0x0102, 1);
//       _delay_ms(5);
//  SPI_Write_16b(SPIx, (uint16_t*)0x0103, 1);
//       _delay_ms(5);
//  SPI_Write_16b(SPIx, (uint16_t*)0x0104, 1);
//       _delay_ms(5);
//  SPI_Write_16b(SPIx, (uint16_t*)0x0104, 1);
//       _delay_ms(5);
//  NSS_0_H;

 _delay_ms(10);
 NSS_0_L;
//  MAX7219_WriteBus(SPIx, 0x0100);
//  MAX7219_WriteBus(SPIx, 0x0101);
//  MAX7219_WriteBus(SPIx, 0x0102);
//  MAX7219_WriteBus(SPIx, 0x0103);
//  MAX7219_WriteBus(SPIx, 0x0104);
 MAX7219_WriteBus(SPIx, 0x01cc);
 MAX7219_WriteBus(SPIx, 0x01cc);
 MAX7219_WriteBus(SPIx, 0x01cc);
 MAX7219_WriteBus(SPIx, 0x01cc);
 NSS_0_H;
//  
//  Delay_us(10);
//  _NSS_L;
//  Delay_us(1);
//  _NSS_H;

//  const char* txt = "QWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnm";
  // MAX7219_Draw("1234567890");
  // MAX7219_Draw("0987654321");

  if (SPI_Disable(SPIx)) return (1);


  return (0);

}



/*
 *
 */
static int MAX7219_WriteBus(SPI_TypeDef* SPIx, const uint16_t data) {
  // SPI_I2S_SendData(SPI_Entry, data);
  // while (!SPI_I2S_GetFlagStatus(SPI_Entry, SPI_I2S_FLAG_TXE));
  // return (0);
  
  
  if (SPI_Write_16b(SPIx, &data, 1)) return (1);
  // _delay_ms(5);
  
  return (0);

}




// /*
//  *
//  */
// static int MAX7219_PrintBus(SPI_TypeDef* SPIx) {
//   uint16_t cntr = 0;

//   for (uint8_t i = 0; i < 8; i++) {
//     // _NSS_L;
//     NSS_0_L;
//     for (uint8_t x = 0; x < MAX7219_SEG_CNT_; x++) {
//       MAX7219_WriteBus(SPIx, maxData[cntr++]);
//     }
//     // _NSS_H; 
//     NSS_0_H;
//   }

//   return (0);
// }




// /*
//  *
//  */
// int MAX7219_Print(SPI_TypeDef* SPIx, const char* buf) {

//   uint16_t len = 0;
//   const char *buf_;
//   buf_ = buf;
  
//   while (*buf_) {
//     len++;
//     buf_++;
//   }
  
//   uint8_t maxData[(len * 8) - 8];
//   //uint16_t tmpCompressedLine[((len / 4) * 3)];
//   uint16_t cntr = 0;

//   while (*buf) {
//     uint8_t pos = *buf;
//     if ((pos > 126) || (pos < 32)) {
//       if (pos == 176) pos = 95;
//       else return (1);
//     } else {
//       pos -= 32;
//     }
//     for (uint8_t i = 0; i < 8; i++) {
//       maxData[((cntr * 8) + i)] = font_dot_5x7_max[((pos * 8) + i)];
//     }
//     cntr++;
//     buf++;

//     return (0);
//   }




  
//   uint16_t z = 0;
//   uint16_t _in = 0;
//   uint16_t _out = 0;
//   uint16_t _cnt = 0;

//   for (uint8_t i = 0; i < 8; i++) {
//     for (uint16_t x = 0; x < MAX7219_SEG_CNT_; x++) {
//       maxData[z] = (((i + 1) << 8) & 0xff00);
// //      tmpCompressedLine[z] = (((i + 1) << 8) & 0xff00);
//      /*******/
//       if (x == 0) {
//         _in = 0;
//         _out = 0;
//         _cnt = 0;
//       }
//       if (_out == x) {
//         _in = _out;
//         _out = 3 + _in;
//         _cnt++;
//       }
//       if ((x >= _in) && (x < _out)) {
//         uint8_t k = x - _in;
//         maxData[z] |= (maxData[(((x + (_cnt - 1)) * 8) + i)] << (2 * k)) & 0x00ff;
//         maxData[z] |= (maxData[(((x + _cnt) * 8) + i)] >> (8 - (2 * (k + 1)))) & 0x00ff;
// //        tmpCompressedLine[z] |= (maxData[(((x + (_cnt - 1)) * 8) + i)] << (2 * k)) & 0x00ff;
// //        tmpCompressedLine[z] |= (maxData[(((x + _cnt) * 8) + i)] >> (8 - (2 * (k + 1)))) & 0x00ff;
//       } 
//       z++;
//     }
//   }
  
// //  for (uint16_t i = 0; i < (MAX7219_SEG_CNT_ * 8); i++) {
// //    maxData[i] = tmpCompressedLine[i];
// //  }

//   MAX7219_DrawLine(SPIx);

//   return (0);
// }
