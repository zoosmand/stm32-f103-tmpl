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
  push {lr}

  FLS _GEREG_, _SYSTICKF_

  pop {pc}


/**
  * Window Watchdog interrupt handler.
  *
  */
  .section .text.WWDG_IRQHandler
  .global WWDG_IRQHandler
  .type WWDG_IRQHandler, %function
WWDG_IRQHandler:
  push {lr}

  pop {pc}

  .end