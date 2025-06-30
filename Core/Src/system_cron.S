/**
  * System cron scheduler.
  *
  */
  .syntax unified
  .thumb

@ #include <stm32f103xb.h>
.include "stm32f1xx.inc"
.include "macroses.inc"
.include "local.inc"



@/**
@  * System Cron handler.
@  *
@  */
@  .section .text.Cron_Handler
@  .global Cron_Handler
@  .type Cron_Handler, %function
@
@Cron_Handler:
@  /* Stop interrupts globally */
@  cpsid i
@
@  FLT _GEREG_, _SYSSECF_
@	cbz tmpd, _Cron_Handler_Sec
@  FLC _GEREG_, _SYSSECF_
@  /** Reload IWDG key secondly */
@  STRW IWDG_KEY_RELOAD, IWDG_KR
@
@_Cron_Handler_Sec:
@  /* Increase 1s counter. */
@  ldr tmpa, =sysQuantCnt
@  ldr tmp, [tmpa]
@  ldr tmpa, =secCntCache
@  ldr tmpd, [tmpa]
@
@  cmp tmpd, tmp
@  ite ls
@  addls tmpd, tmp, 0x03e8
@  bhi _Cron_Handler_Exit
@  str tmpd, [tmpa]
@
@  ldr tmpa, =secCnt
@  ldr tmpd, [tmpa]
@  add tmpd, tmpd, 1
@  str tmpd, [tmpa]
@  FLS _GEREG_, _SYSSECF_
@  b _Cron_Handler_Exit
@
@_Cron_Handler_Exit:
@  /* Start interrupts globally */
@  cpsie i
@  
@  /* Branch to the main */
@  bl main
@  b Cron_Handler


@/**
@  * Scheduler handler.
@  *
@  */
@  .section .text.Scheduler_Handler
@  .global Scheduler_Handler
@  .type Scheduler_Handler, %function
@
@Scheduler_Handler:
@  push {a1-a4, v1-v5, lr}
@  /* Stop interrupts globally */
@  cpsid i
@
@  @ get scheduler struct address
@  ldr tmpa, [a1, #0x00]
@  
@  @ get source counter value
@  ldr v5, [tmpa, #0x04]   @ source counter address
@  ldr v2, [v5]            @ source counter value
@
@  @ get dedicated counter value
@  ldr v5, [tmpa, #0x00]
@  ldr v3, [v5]
@
@  @ get refresh period value
@  ldr v4, [tmpa, #0x08]
@
@  @ comparison and store the new counter value
@  cmp v3, v2
@  ite ls
@  addls v3, v2, v4
@  bhi _Scheduler_Handler_Exit
@  str v3, [v5]
@
@  @ get flag bit value
@  ldr v2, [a1, #0x08]
@  mov v3, 1
@  lsl v2, v3, v2
@
@  @ get BB register address and store the flag
@  ldr v5, [a1, #0x04]
@  orr v3, v3, v2
@  str v3, [v5]
@  
@  @ ldr tmpa, =((_GEREG_ & 0x0000ffff) * 0x20 + SRAM_BB)
@  @ @ set BB offset using the flag value
@  @ mov v3, 4
@  @ mul v2, v2, v3
@  @ @ set the new BB value 
@  @ mov v3, 1
@  @ str v3, [tmpa, v2] 
@
@_Scheduler_Handler_Exit:
@  cpsie i
@  pop {a1-a4, v1-v5, pc}


  .end