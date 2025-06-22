/**
  * System, clock, and peripheral initialization.
  *
  */
  .syntax unified
  .thumb

#include <stm32f103xb.h>
#include <core_cm3.h>
.include "stm32f1xx.inc"
.include "macroses.inc"
.include "local.inc"



/**
  * System initialization.
  *
  */
  .section .text.SystemInit
  .global SystemInit
  .type SystemInit, %function

SystemInit:
  push {lr}
  mov zero, 0
  mov one, 1
  
  /** Clear system stack */    
  ldr tmpa, =(SRAM_BASE - 4)
  mov tmp, (_SystemStackSize_ / 4)
_ClearSystemStack:
	str zero, [tmpa, #4]!
	sub tmp, 1
	teq tmp, 0
	bne _ClearSystemStack

  /** Init second cache with 1000 value */
  ldr tmpa, =secCntCache
  ldr tmpd, [tmpa]
  add tmpd, tmpd, #0x03e8
  str tmpd, [tmpa]

  /** Switch on MCU power */
  STRW \
      RCC_APB1ENR_PWREN \
    , RCC_APB1ENR

	/** Set Flash latency */
  STRLHW \
      FLASH_ACR_LATENCY_2 \
    | FLASH_ACR_PRFTBE \
    , FLASH_ACR

  bl ClockInit
  bl SysTickInit
  @ bl IWatchdogInit
  bl PeripheralInit
  @ bl WWatchdogInit
  bl LedInit
  bl Usart1Init

  pop {pc}
  .size SystemInit, .-SystemInit


/**
  * Clock initialization.
  *
  */
ClockInit:
  push {lr}

	MOVP RCC_CR
  /** Enable external High Speed clock. */
	STRP one, RCC_CR_HSEON_Pos
  /** Wait until External High-speed clock is ready. */
_Init_SYSTEM_CLOCK_hse_wake_up:
	LDRP tmpd, RCC_CR_HSERDY_Pos
	lsrs tmpd, tmpd, 1
	bcc _Init_SYSTEM_CLOCK_hse_wake_up

  STRW \
    RCC_CFGR_PLLMULL_2 \
  | RCC_CFGR_PLLMULL_1 \
  | RCC_CFGR_PLLMULL_0 \
  | RCC_CFGR_PLLSRC \
  | RCC_CFGR_PPRE1_DIV2 \
  , RCC_CFGR

	MOVP RCC_CR
  /**  Enable PLL. */
	STRP one, RCC_CR_PLLON_Pos
  /** Wait until PLL is ready. */
_Init_SYSTEM_CLOCK_pll_wake_up:
	LDRP tmpd, RCC_CR_PLLRDY_Pos
	lsrs tmpd, tmpd, 1
	bcc _Init_SYSTEM_CLOCK_pll_wake_up

	MOVP RCC_CFGR
  /**  Set PLL as the system clock. */
	STRP one, RCC_CFGR_SW_PLL_Pos
  /** Wait until PLL oscilator is ready. */
_Init_SYSTEM_CLOCK_sws_wake_up:
  LDRP tmpd, RCC_CFGR_SWS_PLL_Pos
	lsrs tmpd, tmpd, 1
	bcc _Init_SYSTEM_CLOCK_sws_wake_up

  pop {pc}



/**
  * SysTick initialization gives 1ms quant time.
  *
  */
SysTickInit:
  push {lr}

  STRW 0x0011940, SYSTICKRVR
  STRLHW \
    SysTick_CTRL_ENABLE \
  | SysTick_CTRL_CLKSOURCE \
  | SysTick_CTRL_TICKINT \
  , SYSTICKCSR

  pop {pc}



/**
  * Peripheral initialization.
  *
  */
PeripheralInit:
  push {lr}

  STRW \
    RCC_APB2ENR_AFIOEN \
  | RCC_APB2ENR_IOPAEN \
  | RCC_APB2ENR_IOPBEN \
  | RCC_APB2ENR_IOPCEN \
  | RCC_APB2ENR_USART1EN \
  , RCC_APB2ENR
  
  STRW \
    RCC_APB1ENR_WWDGEN \
  | RCC_APB1ENR_PWREN \
  , RCC_APB1ENR

  pop {pc}



/**
  * Independent watchdog (IWDG) initialization.
  *
  */
IWatchdogInit:
  push {lr}

  mov one, 1
  MOVP RCC_CSR
  /** Enable internal Low Speed clock. */
	STRP one, RCC_CSR_LSION_Pos

_IWatchdogInit_wake_up:
	LDRP tmpd, RCC_CSR_LSIRDY_Pos
	lsrs tmpd, tmpd, 1
	bcc _IWatchdogInit_wake_up

  /** Enable IWDG */
  STRW IWDG_KEY_ENABLE, IWDG_KR
  /** Enable IWDG write access */
  STRW IWDG_KEY_ACCESS, IWDG_KR
  /** Set IWDG prescaler: 64 */
  STRLHW 3, IWDG_PR
  /** Set IWDG reload counter */
  STRW IWDG_RELOAD_COUNTER, IWDG_RLR

  MOVP IWDG_SR
_IWatchdogInit_SR_wake_up:
	LDRP tmpd, IWDG_SR_RVU_Pos
	lsrs tmpd, tmpd, 1
	bcc _IWatchdogInit_SR_wake_up
	LDRP tmpd, IWDG_SR_PVU_Pos
	lsrs tmpd, tmpd, 1
	bcc _IWatchdogInit_SR_wake_up

  /** Reload IWDG key */
  STRW IWDG_KEY_RELOAD, IWDG_KR

  pop {pc}



/**
  * Window watchdog (WWDG) initialization.
  *
  */
WWatchdogInit:
  push {lr}

  MOVP WWDG_CR
	STRP one, WWDG_CR_WDGA_Pos

  MOVP WWDG_CFR
	STRP one, WWDG_CFR_EWI_Pos

  STRLHW \
    WWDG_CFR_WDGTB_0 \
  | WWDG_CFR_WDGTB_1 \
  | 0x7f \
  , WWDG_CFR

  MOVP WWDG_SR
  STRP zero, WWDG_SR_EWIF_Pos
  STRW 0, WWDG_SR

  STRLHW 0x7f, WWDG_CR

  MOVP WWDG_CR
  STRP one, WWDG_CR_WDGA_Pos

  pop {pc}



/**
  * LED GPIO initialization.
  *
  */
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
	STRPB 1, GPIOB_BSRR, PIN_13_Pos
  @ FLSP GPIOB_BSRR, PIN_13_Pos

  pop {pc}





  .end
