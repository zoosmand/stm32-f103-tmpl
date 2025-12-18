/**
  ******************************************************************************
  * @file           : max72xx.c
  * @brief          : This file contains MAX 72xx display code.
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
#include "max72xx.h"

/* Private variables ---------------------------------------------------------*/
#ifdef MAX_DSPL
static const uint16_t maxInit[15] = {
  0x0c00, // 0x0c - Shutdown,     0x00 - Shutdown
  0x0f00, // 0x0f - DisplayTest,  0x00 - Off, 0x01 - On
  0x0900, // 0x09 - Decode Mode,  0x00 - No decode for digits 7�0, 0xff - Code B decode for digits 7�0
  0x0a04, // 0x0a - Intensity,    0x00 - lowest, 0x0f - highest
  0x0b07, // 0x0b - Scan-Limit,   0x07 - Display digits 0 1 2 3 4 5 6 7
  0x0100, // 0x01 - Line 1
  0x0200,
  0x0300,
  0x0400,
  0x0500,
  0x0600,
  0x0700,
  0x0800, // 0x08 - Line 8
  0x0c01, // 0x0c - Run,          0x01 - Normal operation 
  0x0000  // 0x00 - NOP
};
#endif /* ifdef MAX_DSPL */


/* Private function prototypes -----------------------------------------------*/

/**
 * @brief   Prints the buffer from the given MAX72xx dev.
 * @param   dev: pointer to the MAX72xx device struct
 * @param   len: number of bytes to print at the buffer
 * @retval  status of operation
 */
static ErrorStatus mAX72xx_PrintBuf(Max72xx_TypeDef*, uint16_t);

/**
 * @brief   Compresses the buffer from the given MAX72xx dev.
 * @param   dev: pointer to the MAX72xx device struct
 * @param   len: number of bytes to print at the buffer
 * @param   step: number of offset steps
 * @retval  none
 */
static void mAX72xx_CompressBuf(Max72xx_TypeDef*, uint16_t, uint8_t);

/**
 * @brief   Adjusts the SPI bus according to MAX72xx parameters.
 * @param   dev: pointer to the MAX72xx device struct
 * @retval  none
 */
__STATIC_INLINE void SPI_Adjust(Max72xx_TypeDef*);

/**
  * @brief   Unconfigures the SPI bus.
  * @param   dev: pointer to the device struct
  * @retval  none
  */
__STATIC_INLINE ErrorStatus SPI_Unconfigure(Max72xx_TypeDef*);

/**
  * @brief   Writes/sends data to the SPI bus.
  * @param   dev: pointer to the device struct
  * @param   buf: pointer to the redefined buffer (struct buffre is also available)
  * @retval  none
  */
__STATIC_INLINE ErrorStatus SPI_Write(Max72xx_TypeDef*, uint16_t*);






// ----------------------------------------------------------------------------

ErrorStatus MAX72xx_Init(Max72xx_TypeDef* dev) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  #ifdef MAX_DSPL

    /* Initialize NSS Pin */
    if (dev->SPINssPin > 7) {
      MODIFY_REG(dev->SPINssPort->CRL, (0xf << ((dev->SPINssPin - 8) * 4)), ((GPIO_GPO_PP | GPIO_IOS_2) << ((dev->SPINssPin -8) * 4)));
    } else {
      MODIFY_REG(dev->SPINssPort->CRL, (0xf << (dev->SPINssPin * 4)), ((GPIO_GPO_PP | GPIO_IOS_2) << (dev->SPINssPin * 4)));
    }
    PIN_H(dev->SPINssPort, dev->SPINssPin);
    
    _delay_us(10);

    SPI_Adjust(dev);
    if (SPI_Enable(dev->SPIx)) return (SPI_Unconfigure(dev));
    
    _delay_ms(5); 
    
    for (uint8_t i = 0; i < sizeof(maxInit)/2; i++) {
      PIN_L(dev->SPINssPort, dev->SPINssPin);

      /* An alternative way to write data the the SPI bus */
      // if (SPI_Write_16b(dev->SPIx, &maxInit[i], dev->SegCnt)) return (SPI_Unconfigure(dev));

      SPI_Write(dev, &maxInit[i]);

      PIN_H(dev->SPINssPort, dev->SPINssPin);
    }

    SPI_Disable(dev->SPIx);
    dev->Lock = DISABLE;

  return (SUCCESS);
  
  #else
    dev->Lock = ENABLE;
    return (ERROR);
  #endif /* ifdef MAX_DSPL */
}




// ----------------------------------------------------------------------------

__STATIC_INLINE void SPI_Adjust(Max72xx_TypeDef* dev) {
  /* adjust frequency divider, 0b010 = 8, (PCLK)72/8 = 9MHz */
  /* set 16-bit data buffer length */ 
  MODIFY_REG(dev->SPIx->CR1, (SPI_CR1_BR_Msk | SPI_CR1_DFF_Msk), (SPI_CR1_BR_1 | SPI_CR1_DFF));
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_SSOE_Pos);

  /* Enable from memory to peripheral DMA transfer */
  PREG_SET(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);

  /* --- Configure DMA TX Channel3 --- */
  /* Cet priority high*/
  /* Cet memory to increment */
  /* Set direction from memory to peripheral */
  /* Set peripheral bandwidth 16-bit */
  /* Set memory bandwidth 16-bit */
  MODIFY_REG(dev->DMAxTx->CCR, (
        DMA_CCR_PL_Msk
      | DMA_CCR_MINC_Msk
      | DMA_CCR_DIR_Msk
      | DMA_CCR_PSIZE_Msk
      | DMA_CCR_MSIZE_Msk
    ), (
        DMA_CCR_PL_1
      | DMA_CCR_MINC
      | DMA_CCR_DIR
      | DMA_CCR_PSIZE_0
      | DMA_CCR_MSIZE_0
    )
  );
  
  uint32_t tmout = SPI_BUS_TMOUT;
  while(!(PREG_CHECK(dev->DMAxTx->CCR, DMA_CCR_MINC_Pos))) {
    if (!(--tmout)) return;
  }

  /* Set peripheral address */
  dev->DMAxTx->CPAR = (uint32_t)&dev->SPIx->DR;
}




// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus SPI_Unconfigure(Max72xx_TypeDef* dev) {
  
  /* Clear correspondents DMA flags */
  dev->DMAx->IFCR |= (
    DMA_IFCR_CGIF3_Msk
    | DMA_IFCR_CHTIF3_Msk
    | DMA_IFCR_CTCIF3_Msk
  );
  
  /* Disable SPI DMA transfer */
  PREG_CLR(dev->SPIx->CR2, SPI_CR2_TXDMAEN_Pos);
  /* Disable DMA transfer */
  PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);
  
  /* Stop SPI bus */
  PIN_H(dev->SPINssPort, dev->SPINssPin);
  SPI_Disable(dev->SPIx);
  
  /* Clear DMA registers */
  MODIFY_REG(dev->DMAxTx->CCR, (
    DMA_CCR_PL_Msk
    | DMA_CCR_MINC_Msk
    | DMA_CCR_DIR_Msk
    | DMA_CCR_PSIZE_Msk
    | DMA_CCR_MSIZE_Msk
  ), 0UL);
  
  return (ERROR);
}





// ----------------------------------------------------------------------------

__STATIC_INLINE ErrorStatus SPI_Write(Max72xx_TypeDef* dev, uint16_t* buf) {
    /* Set counter */
    dev->DMAxTx->CNDTR = dev->SegCnt;
    /* Set buffer address */
    dev->DMAxTx->CMAR = (uint32_t)buf;
    /* Enable DMA transfer */
    PREG_SET(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);

    /* Wait for transfer is compete */
    uint32_t tmout = SPI_BUS_TMOUT;
    while(!(PREG_CHECK(dev->DMAx->ISR, DMA_ISR_TCIF3_Pos))) {
      if (!(--tmout)) return (SPI_Unconfigure(dev));
    }

    /* Clear complete transger flag */
    SET_BIT(dev->DMAx->IFCR, DMA_IFCR_CTCIF3);
    /* Disable DMA transfer */
    PREG_CLR(dev->DMAxTx->CCR, DMA_CCR_EN_Pos);

    _delay_us(5);

    return (SUCCESS);
}



// ----------------------------------------------------------------------------

static ErrorStatus mAX72xx_PrintBuf(Max72xx_TypeDef* dev, uint16_t len) {

  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);

  SPI_Adjust(dev);
  if (SPI_Enable(dev->SPIx)) return (1);



  for (uint8_t i = 0; i < 8; i++) {
    PIN_L(dev->SPINssPort, dev->SPINssPin);
    
    /* An alternative way to write data the the SPI bus */
    // if (SPI_Write_16b(dev->SPIx, &(dev->BufPtr)[len * i], dev->SegCnt)) return (ERROR);

    SPI_Write(dev, &dev->BufPtr[len * i]);

    PIN_H(dev->SPINssPort, dev->SPINssPin);
  }

  /* Unconfigure and stop the SPI bus */
  SPI_Unconfigure(dev);

  dev->Lock = DISABLE;
  return (SUCCESS);
}





// ----------------------------------------------------------------------------

ErrorStatus MAX72xx_Print(Max72xx_TypeDef* dev, char* buf) {

  uint16_t len = 0;
  const char *buf_ = buf;

  while (*buf_++) len++;
  
  len = (len > dev->MaxSegCnt) ? dev->MaxSegCnt : len;

  for (uint16_t k = 0; k < len; k++) {
    
    uint8_t pos = buf[k];
    if ((pos > 126) || (pos < 32)) {
      if (pos == 176) pos = 95;
      else return (ERROR);
    } else pos -= 32;
    
    for (uint8_t i = 0; i < 8; i++) {
      dev->BufPtr[(k + (len * i))] = ((i + 1) << 8) | (uint8_t)font_dot_5x7_max[pos][i];
    }
  }

  /* TODO fix buffer overflow bug */
  // mAX72xx_CompressBuf(dev, len, 2);
  if (mAX72xx_PrintBuf(dev, len)) return (ERROR);

  return (SUCCESS);
}





// ----------------------------------------------------------------------------

static void mAX72xx_CompressBuf(Max72xx_TypeDef* dev, uint16_t len, uint8_t step) {

  if (step > 3) return;
  uint16_t cur_val, next_val, cur_ptr, next_ptr;
  uint8_t step_tmp, step_cnt, step_offset;
  
  for (uint8_t i = 0; i < (len * 8); i++) {
    cur_val     = 0;
    next_val    = 0;
    step_tmp    = 0;
    step_cnt    = 0;
    step_offset = 0;
    cur_ptr     = 0;
    next_ptr    = 0;
    
    for (uint8_t k = 0; k < len; k++) {

      if ((k + 2) >= len) {
        dev->BufPtr[k + 1] = (dev->BufPtr[k + 1] & 0xff00);
        break;
      }

      if (step_tmp >= 8) {
        step_offset++;
        k--;
      } 

      step_tmp  = ((step_cnt++) & 0x3) * step + step;

      cur_ptr   = len * i;
      next_ptr  = (len * i) + 1 + step_offset;
      
      cur_val   = dev->BufPtr[k + cur_ptr];
      next_val  = dev->BufPtr[k + next_ptr];

      dev->BufPtr[k + cur_ptr]                  = (cur_val & 0x00ff) | (((next_val & 0x00ff) << step_tmp) >> 8) | (cur_val & 0xff00);
      dev->BufPtr[k + next_ptr - step_offset]   = ((next_val << step_tmp) & 0x00ff) | (next_val & 0xff00);

    }
  }
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
