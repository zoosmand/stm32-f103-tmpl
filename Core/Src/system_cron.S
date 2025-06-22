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


/**
  * Scheduler handler.
  *
  */
  .section .text.Task_Handler
  .global Task_Handler
  .type Task_Handler, %function

Task_Handler:
  push {a1-a4, v1-v5, lr}
  /* Stop interrupts globally */
  cpsid i

  @ counter source address
  ldr tmpa, [a1, #0x04]
  ldr v2, [tmpa]

  @ counter address
  ldr tmpa, [a1, #0x00]
  ldr v3, [tmpa]

  @ period address
  ldr v4, [a1, #0x10]

  @ comparison
  cmp v2, v3
  ite hi
  addhi v3, v2, v4
  bls _Task_Handler_Exit
  str v3, [tmpa]

  @ get flag bit value
  ldr v2, [a1, #0x0c]
  mov v3, 1
  lsl v2, v3, v2

  @ get BB register address
  ldr tmpa, [a1, #0x08]
  ldr v3, [tmpa]
  orr v3, v3, v2
  str v3, [tmpa]
  
  @ ldr tmpa, =((_GEREG_ & 0x0000ffff) * 0x20 + SRAM_BB)
  @ @ set BB offset using the flag value
  @ mov v3, 4
  @ mul v2, v2, v3
  @ @ set the new BB value 
  @ mov v3, 1
  @ str v3, [tmpa, v2] 

_Task_Handler_Exit:
  cpsie i
  pop {a1-a4, v1-v5, pc}


  .end