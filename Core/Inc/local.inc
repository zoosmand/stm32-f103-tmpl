/**
  * Local definitions.
  *
  */


/**
  * @brief Register definitions.
  *
  */
tmpa    .req r0
tmpd    .req r1
zero    .req r10
one     .req r11
_OFF_   .req r10
_ON_    .req r11
tmp     .req r12

/* ------------------------------------------------------------------------- */
/*                Definitions that are not exist in headers                  */
/* ------------------------------------------------------------------------- */
.equ PERIPH_BB,               0x42000000
.equ SRAM_BB,                 0x22000000
.equ SRAM_BASE,               0x20000000
.equ _SystemStackSize_,       128


/* ------------------------------------------------------------------------- */
/*                            Critical values                                */
/* ------------------------------------------------------------------------- */
/** Global system variables */
.equ _GEREG_,                 SRAM_BASE
.equ sysQuantCnt,             SRAM_BASE + 0x04
.equ secCnt,                  SRAM_BASE + 0x08
.equ secCntCache,             SRAM_BASE + 0x0c

/** Global Events Register Flags */
.equ _SYSTICKF_,              0 @ SysTick Interval Flag
.equ _SYSSECF_,               1 @ SysTick Interval Flag


/* ------------------------------------------------------------------------- */
/*                Definitions that are not exist in headers                  */
/* ------------------------------------------------------------------------- */

/** RCC PLL definitons. */
.equ RCC_CFGR_SW_PLL_Pos,     1
.equ RCC_CFGR_SWS_PLL_Pos,    3


/** Bit definition for SysTick_CTRL register */
.equ SysTick_CTRL_ENABLE,     0x00000001	@ Counter enable
.equ SysTick_CTRL_TICKINT,    0x00000002	@ Counting down to 0 pends the SysTick handler
.equ SysTick_CTRL_CLKSOURCE,  0x00000004	@ Clock source
.equ SysTick_CTRL_COUNTFLAG,  0x00010000	@ Count Flag

/** Bit definition for SysTick_LOAD register */
.equ SysTick_LOAD_RELOAD,     0x00ffffff	@ Value to load into the SysTick Current Value Register when the counter reaches 0

/** Bit definition for SysTick_VAL register */
.equ SysTick_VAL_CURRENT,     0x00ffffff	@ Current value at the time the register is accessed

/** Bit definition for SysTick_CALIB register */
.equ SysTick_CALIB_TENMS,     0x00ffffff	@ Reload value to use for 10ms timing
.equ SysTick_CALIB_SKEW,      0x40000000	@ Calibration value is not exactly 10 ms
.equ SysTick_CALIB_NOREF,     0x80000000	@ The reference clock is not provided

