/**
  ******************************************************************************
  * @file           : tm1803.c
  * @brief          : This file contains code of TM1803 3-channel LED
  *                   controller. 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 Askug Ltd.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "mled.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Locaal variables ---------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

__STATIC_INLINE void send_color(StripDevice_TypeDev*, uint32_t);

__STATIC_INLINE void send_bit(StripDevice_TypeDev*, uint8_t);





////////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------

ErrorStatus WS281x_Init(StripDevice_TypeDev* dev) {
  
  if (dev->Lock == DISABLE) dev->Lock = ENABLE; else return (ERROR);
  
  #ifdef STRIP_DEV

  TIM_TypeDef* TIMx = dev->Timer;
  uint8_t* buf = dev->BufPtr;

  if (TIMx == TIM1) {
    /* Init GPIO */
    /* Alternate on the highest speed, no need in remap */
    MODIFY_REG(
      STRIP_DATA_Port->CRH, 
      (0x0f << ((STRIP_DATA_Pin - 8) * 4)), 
      ((GPIO_AF_PP | GPIO_IOS_50) << ((STRIP_DATA_Pin - 8) * 4))
    );

    /* Set prescaler */
    TIM1->PSC = 0;
    /* Set autoreload value */
    TIM1->ARR = 90U - 1U;

    MODIFY_REG(TIMx->CR1, 0x1f, (
        (0 << TIM_CR1_CKD_Pos)    // No PSCK division
      | (1 << TIM_CR1_ARPE_Pos)   // Disable autoreload to be cached
      | (0 << TIM_CR1_CMS_Pos)    // Edge-aligned mode
      | (0 << TIM_CR1_DIR_Pos)    // Upconuter mode
      | (0 << TIM_CR1_OPM_Pos)    // One pulse mode off
      | (1 << TIM_CR1_URS_Pos)    // Only counter updates IRQ events or DMA requests
      | (0 << TIM_CR1_UDIS_Pos)   // Counter updates only by shadowed registers
      | (0 << TIM_CR1_CEN_Pos)    // Not now
    ));

    MODIFY_REG(TIMx->SMCR, (TIM_SMCR_SMS | TIM_SMCR_ECE), 
        (0 << TIM_SMCR_ECE_Pos)       // Use inbernal clock
      | (0b000 << TIM_SMCR_SMS_Pos)   // 
    );

    /* --- OS config --- */
    PREG_CLR(TIMx->CCER, TIM_CCER_CC1E_Pos);   // Disable Channel 1
    
    MODIFY_REG(TIMx->CCMR1, (TIM_CCMR1_OC1M | TIM_CCMR1_CC1S | TIM_CCMR1_OC1FE), 
        (0b110 << TIM_CCMR1_OC1M_Pos)       // Set the Output Compare PWM Mode 1
      | (0 << TIM_CCMR1_CC1S_Pos)           // Reset Capture/Compare selection Bits
      | (0 << TIM_CCMR1_OC1FE_Pos)          // Output compare 1 fast disable
      | (0 << TIM_CCMR1_OC1CE_Pos)          // Output compare 1 clear disable
      | (1 << TIM_CCMR1_OC1PE_Pos)          // Output compare 1 preload enable
    );

    MODIFY_REG(TIMx->CCER, (TIM_CCER_CC1NP | TIM_CCER_CC1NE), (
        (0 << TIM_CCER_CC1NP_Pos)     // Capture/Compare 1 complementary output polarity HIGH
      | (0 << TIM_CCER_CC1NE_Pos)     // Capture/Compare 1 complementary output disable
    ));
    
    MODIFY_REG(TIMx->CR2, (TIM_CR2_OIS1 | TIM_CR2_OIS1N | TIM_CR2_MMS), (
        (0 << TIM_CR2_OIS1_Pos)       // Output Idle state 1 (OC1 output)
      | (0 << TIM_CR2_OIS1N_Pos)      // Output Idle state 1 (OC1N output)
      | (0b000 << TIM_CR2_MMS_Pos)    // Master mode selection to reset
    ));

    TIMx->CCR1 = 0;

    PREG_CLR(TIMx->SMCR, TIM_SMCR_MSM_Pos);  // Disable Master/Slave mode

    MODIFY_REG(TIMx->BDTR, (TIM_BDTR_MOE | TIM_BDTR_DTG | TIM_BDTR_LOCK | TIM_BDTR_OSSI | TIM_BDTR_OSSR | TIM_BDTR_BKE | TIM_BDTR_BKP | TIM_BDTR_AOE), (
        (0 << TIM_BDTR_MOE_Pos)           // Not now
      | (0 << TIM_BDTR_AOE_Pos)           // Automatic output disable
      | (1 << TIM_BDTR_BKP_Pos)           // Break polarity active HIGH
      | (0 << TIM_BDTR_BKE_Pos)           // Break disable
      | (0 << TIM_BDTR_OSSR_Pos)          // Off-state selection for Run mode disabled
      | (0 << TIM_BDTR_OSSI_Pos)          // Off-state selection for Idle mode disables
      | (0b00 << TIM_BDTR_LOCK_Pos)       // Lock configuration OFF
      | (0b00000000 << TIM_BDTR_DTG_Pos)  // Dead-time generator setup
    ));

  }

  PREG_CLR(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);
  DMA1->IFCR = DMA_IFCR_CGIF2;        // clear all flags for CH2

  DMA1_Channel2->CPAR = (uint32_t)&TIM1->CCR1;

  MODIFY_REG(DMA1_Channel2->CCR, (
        DMA_CCR_MEM2MEM_Msk
      | DMA_CCR_PL_Msk
      | DMA_CCR_MSIZE_Msk
      | DMA_CCR_PSIZE_Msk
      | DMA_CCR_MINC_Msk
      | DMA_CCR_PINC_Msk
      | DMA_CCR_CIRC_Msk
      | DMA_CCR_DIR_Msk
    ), (
        (0 << DMA_CCR_MEM2MEM_Pos)    // Memory to memory disabled
      | (0b00 << DMA_CCR_PL_Pos)      // Priority LOW
      | (0b00 << DMA_CCR_MSIZE_Pos)   // 8-bit memory bandwidth
      | (0b01 << DMA_CCR_PSIZE_Pos)   // 16-bit peripheral bandwidth
      | (1 << DMA_CCR_MINC_Pos)       // Memory increment mode ON
      | (0 << DMA_CCR_PINC_Pos)       // Peripheral increment mode OFF
      | (0 << DMA_CCR_CIRC_Pos)       // Circular mode OFF
      | (1 << DMA_CCR_DIR_Pos)        // Direction from memory to peropheral
    )
  );
  
  PREG_SET(TIMx->DIER, TIM_DIER_CC1DE_Pos);
  PREG_SET(TIMx->DIER, TIM_DIER_UDE_Pos);
  PREG_SET(TIMx->EGR, TIM_EGR_UG_Pos);

  dev->Lock = DISABLE;
  return (SUCCESS);

  #else
    dev->Lock = ENABLE;
    return (ERROR);
  #endif /* ifdef STRIP_DEV */
}



// ----------------------------------------------------------------------------

ErrorStatus LedStrip_RunBus(StripDevice_TypeDev* dev) {
  uint32_t tmout = 10000;
  ErrorStatus status = SUCCESS;
  TIM_TypeDef* TIMx = dev->Timer;

  PREG_CLR(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);
  DMA1->IFCR = DMA_IFCR_CGIF2;        // clear all flags for CH2

  DMA1_Channel2->CMAR = (uint32_t)dev->BufPtr;
  DMA1_Channel2->CNDTR = dev->Count;

  PREG_SET(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);
  PREG_SET(TIMx->CCER, TIM_CCER_CC1E_Pos);
  PREG_SET(TIMx->BDTR, TIM_BDTR_MOE_Pos);
  PREG_SET(TIMx->CR1, TIM_CR1_CEN_Pos);

  while(!(PREG_CHECK(DMA1->ISR, DMA_ISR_TCIF2_Pos))) {
    if (!(--tmout)) { status = ERROR; }
  }

  PREG_CLR(TIMx->CR1, TIM_CR1_CEN_Pos);
  PREG_CLR(TIMx->BDTR, TIM_BDTR_MOE_Pos);
  PREG_CLR(TIMx->CCER, TIM_CCER_CC1E_Pos);
  PREG_CLR(DMA1_Channel2->CCR, DMA_CCR_EN_Pos);

  DMA1->IFCR = DMA_IFCR_CGIF2;

  return (status);
}



// ----------------------------------------------------------------------------

__STATIC_INLINE void send_bit(StripDevice_TypeDev* dev, uint8_t bit) {
  STRIP_DATA_High;
  __asm volatile(" \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
  ");
  if (bit) {
    __asm volatile(" \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
    ");
  }
  STRIP_DATA_Low;
  if (!bit) {
    __asm volatile(" \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
      nop\n\t \
    ");
  }
  __asm volatile(" \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
    nop\n\t \
  ");
}



// ----------------------------------------------------------------------------

__STATIC_INLINE void send_color(StripDevice_TypeDev* dev, uint32_t color) {
  
  for (int8_t i = 23; i >= 0; i--) {
    send_bit(dev, (uint8_t)((color >> i) & 1));
  }
}



// ----------------------------------------------------------------------------

ErrorStatus RunStrip(StripDevice_TypeDev* dev) {

  __disable_irq();
  for (uint16_t i = 0; i < dev->Count; i++) {
    send_color(dev, dev->BufPtr[i]);
  }
  _delay_us(500);
  __enable_irq();

  return (SUCCESS);
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
