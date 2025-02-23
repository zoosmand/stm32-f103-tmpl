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
  * SysTick interrupt handler.
  *
  */
  .section .text.Cron_Handler
  .global Cron_Handler
  .type Cron_Handler, %function

Cron_Handler:

  /* Increase 1ms System tick. */
  FLT _GEREG_, _SYSTICKF_
	cbz tmpd, _CronCheckSecFlag_Handler
  
  ldr tmpa, =sysQuantCnt
  ldr tmpd, [tmpa]
  add tmpd, tmpd, #1 
  str tmpd, [tmpa]
  FLC _GEREG_, _SYSTICKF_

  /* Increase 1s counter. */
  ldr tmpa, =sysQuantCnt
  ldr r2, [tmpa]
  ldr tmpa, =secCntCache
  ldr r3, [tmpa]
  teq r2, r3
  bne _Cron_Handler_Exit

  add r3, r3, #0x03e8
  str r3, [tmpa]

  ldr tmpa, =secCnt
  ldr tmpd, [tmpa]
  add tmpd, tmpd, #1
  str tmpd, [tmpa]
  FLS _GEREG_, _SYSSECF_
  b _Cron_Handler_Exit

_CronCheckSecFlag_Handler:
  FLT _GEREG_, _SYSSECF_
	cbz tmpd, _Cron_Handler_Exit
  FLC _GEREG_, _SYSSECF_

_Cron_Handler_Exit:
  bl main
  b Cron_Handler


  .end