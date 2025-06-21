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
  * System Cron handler.
  *
  */
  .section .text.Cron_Handler
  .global Cron_Handler
  .type Cron_Handler, %function

Cron_Handler:
  /* Stop interrupts globally */
  cpsid i

  FLT _GEREG_, _SYSSECF_
	cbz tmpd, _Cron_Handler_Sec
  FLC _GEREG_, _SYSSECF_
  /** Reload IWDG key secondly */
  STRW IWDG_KEY_RELOAD, IWDG_KR

_Cron_Handler_Sec:
  /* Increase 1s counter. */
  ldr tmpa, =sysQuantCnt
  ldr tmp, [tmpa]
  ldr tmpa, =secCntCache
  ldr tmpd, [tmpa]

  cmp tmp, tmpd
  ite hi
  addhi tmpd, tmp, 0x03e8
  bls _Cron_Handler_Exit
  str tmpd, [tmpa]

  ldr tmpa, =secCnt
  ldr tmpd, [tmpa]
  add tmpd, tmpd, 1
  str tmpd, [tmpa]
  FLS _GEREG_, _SYSSECF_
  b _Cron_Handler_Exit

_Cron_Handler_Exit:
  /* Start interrupts globally */
  cpsie i
  
  /* Branch to the main */
  bl main
  b Cron_Handler


  .end