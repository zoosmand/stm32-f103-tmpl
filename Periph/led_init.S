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



@   .section .text.LedToggle
@   .global LedToggle
@   .type LedToggle, %function

@ LedToggle:
@   push {lr}
@   @ mov one, 1
@ 	@ LDRPB tmpd, GPIOB_IDR, PIN_13_Pos
@ 	TSTPB GPIOB_IDR, PIN_13_Pos
@ 	cbz tmpd, _LedToggleDown
@ 	STRPB 1, GPIOB_BSRR, (PIN_13_Pos + 16)
@ 	@ FLSP GPIOB_BSRR, (PIN_13_Pos + 16)
@ 	pop {pc}

@ _LedToggleDown:
@ 	STRPB 1, GPIOB_BSRR, PIN_13_Pos
@ 	@ FLSP GPIOB_BSRR, PIN_13_Pos
@   pop {pc}


/**
  * LED GPIO initialization.
  *
  */
  .section .text.LedInit
  .global LedInit
  .type LedInit, %function
LedInit:
  push {lr}

  /** PB13 Output LED, max 2 MHz, push-pull */
  ldr tmpa, =GPIOB_CRH
  ldr tmpd, [tmpa]
  mov tmp, ~PIN_13_Mask
  and tmpd, tmp
  mov tmp, _IOS_2 << ((PIN_13_Pos - 8) * 4)
  orr tmpd, tmp
  str tmpd, [tmpa]

	/** PB13 Output pull-up */
	@ STRPB 1, GPIOB_BSRR, PIN_13_Pos
  @ FLSP GPIOB_BSRR, PIN_13_Pos

  /** PB15 Output LED, max 2 MHz, push-pull */
  ldr tmpa, =GPIOB_CRH
  ldr tmpd, [tmpa]
  mov tmp, ~PIN_15_Mask
  and tmpd, tmp
  mov tmp, _IOS_2 << ((PIN_15_Pos - 8) * 4)
  orr tmpd, tmp
  str tmpd, [tmpa]

  /** PA8 Output LED, max 2 MHz, push-pull */
  ldr tmpa, =GPIOA_CRH
  ldr tmpd, [tmpa]
  mov tmp, ~PIN_8_Mask
  and tmpd, tmp
  mov tmp, _IOS_2 << ((PIN_8_Pos - 8) * 4)
  orr tmpd, tmp
  str tmpd, [tmpa]

  pop {pc}
    .size LedInit, .-LedInit

  .end