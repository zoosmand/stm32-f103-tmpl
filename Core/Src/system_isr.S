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

  ldr tmpa, =sysQuantCnt
  ldr tmpd, [tmpa]
  add tmpd, tmpd, #1 
  str tmpd, [tmpa]
  FLS _GEREG_, _SYSTICKF_

  pop {pc}


  .end