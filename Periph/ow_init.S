/**
  * System cron scheduler.
  *
  */
  .syntax unified
  .thumb

#include <stm32f103xb.h>
.include "stm32f1xx.inc"
.include "macroses.inc"
.include "local.inc"



/**
  * LED GPIO initialization.
  *
  */
  .section .text.OwInit
  .global OwInit
  .type OwInit, %function
OwInit:
  push {lr}

  /** PB12 Output max 50 MHz, open-drain */
  ldr tmpa, =GPIOB_CRH
  ldr tmpd, [tmpa]
  ldr tmp, =(~PIN_12_Mask)
  and tmpd, tmp
  ldr tmp, =((_IOS_50 | _GPO_OD) << ((PIN_12_Pos - 8) * 4))
  orr tmpd, tmp
  str tmpd, [tmpa]

	STRPB 1, GPIOB_BSRR, PIN_12_Pos

  pop {pc}
    .size OwInit, .-OwInit

  .end