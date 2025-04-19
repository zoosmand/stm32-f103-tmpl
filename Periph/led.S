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



  .section .text.LedToggle
  .global LedToggle
  .type LedToggle, %function

LedToggle:
  push {lr}
  mov one, 1
	LDRPB tmpd, GPIOB_IDR, PIN_13_Pos
	cbz tmpd, _LedToggleDown
	STRPB one, GPIOB_BSRR, (PIN_13_Pos + 16)
	pop {pc}

_LedToggleDown:
	STRPB one, GPIOB_BSRR, PIN_13_Pos
  pop {pc}




  .end