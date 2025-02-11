/**
  * System, clock, and peripheral initialization.
  *
  */

#include <stm32f103xb.h>
.include "stm32f1xx.inc"
.include "macroses.inc"


/**
  * System initialization.
  *
  */
  .section .text.SystemInit
  .global SystemInit
  .type SystemInit, %function

SystemInit:
  push {lr}
  STRW \
      RCC_APB1ENR_PWREN \
    , RCC_APB1ENR

	STRLHW \
      FLASH_ACR_LATENCY_2 \
    | FLASH_ACR_PRFTBE \
    , FLASH_ACR

  bl ClockInit
  bl PeripheralInit
  pop {pc}
  .size SystemInit, .-SystemInit


/**
  * Clock initialization.
  *
  */
ClockInit:
  push {lr}
  nop
  pop {pc}


/**
  * Peripheral initialization.
  *
  */
PeripheralInit:
  push {lr}
  STRW RCC_APB2ENR_IOPCEN, RCC_APB2ENR
  pop {pc}
