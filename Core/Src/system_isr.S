/**
  * System interrupt handlers.
  *
  */
  .syntax unified
  .thumb

#include <stm32f103xb.h>
.include "stm32f1xx.inc"
.include "macroses.inc"
.include "local.inc"



/**
  * SysTick interrupt handler.
  *
  */
  .section .text.SysTick_Handler
  .global SysTick_Handler
  .type SysTick_Handler, %function

SysTick_Handler:
  push {tmpa, tmpd, lr}

  /* Increase 1ms System tick. */
  ldr tmpa, =sysQuantCnt
  ldr tmpd, [tmpa]
  add tmpd, tmpd, 1 
  str tmpd, [tmpa]

  pop {tmpa, tmpd, pc}


/**
  * Window Watchdog interrupt handler.
  *
  */
  .section .text.WWDG_IRQHandler
  .global WWDG_IRQHandler
  .type WWDG_IRQHandler, %function
WWDG_IRQHandler:
  push {lr}

  STRBW 0, WWDG_SR
  STRLHW 0xff, WWDG_CR

  pop {pc}




/**
  * USART1 interrupt handler.
  *
  */
  .section .text.USART1_IRQHandler
  .global USART1_IRQHandler
  .type USART1_IRQHandler, %function
USART1_IRQHandler:
  push {tmpa, tmpd, tmp, lr}

  @ mov tmp, 0
  @ STRPB 0, USART1_SR, USART_SR_RXNE_Pos
  FLCP USART1_SR, USART_SR_RXNE_Pos
  ldr tmpd, =USART1_DR
  ldr tmp, [tmpd]
  
  pop {tmpa, tmpd, tmp, pc}

  .end