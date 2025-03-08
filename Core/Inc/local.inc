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



/** IO register details definitions */
.equ PIN0,	                  0x00000001
.equ PIN1,	                  0x00000002
.equ PIN2,	                  0x00000004
.equ PIN3,	                  0x00000008
.equ PIN4,	                  0x00000010
.equ PIN5,	                  0x00000020
.equ PIN6,	                  0x00000040
.equ PIN7,	                  0x00000080
.equ PIN8,	                  0x00000100
.equ PIN9,	                  0x00000200
.equ PIN10,	                  0x00000400
.equ PIN11,	                  0x00000800
.equ PIN12,	                  0x00001000
.equ PIN13,	                  0x00002000
.equ PIN14,	                  0x00004000
.equ PIN15,	                  0x00008000

.equ GPIO_MODE0_0,	          0
.equ GPIO_MODE0_1,	          1
.equ GPIO_CNF0_0,	            2
.equ GPIO_CNF0_1,	            3
.equ GPIO_MODE1_0,	          4
.equ GPIO_MODE1_1,	          5
.equ GPIO_CNF1_0,	            6
.equ GPIO_CNF1_1,	            7
.equ GPIO_MODE2_0,	          8
.equ GPIO_MODE2_1,	          9
.equ GPIO_CNF2_0,	            10
.equ GPIO_CNF2_1,	            11
.equ GPIO_MODE3_0,	          12
.equ GPIO_MODE3_1,	          13
.equ GPIO_CNF3_0,	            14
.equ GPIO_CNF3_1,	            15
.equ GPIO_MODE4_0,	          16
.equ GPIO_MODE4_1,	          17
.equ GPIO_CNF4_0,	            18
.equ GPIO_CNF4_1,	            19
.equ GPIO_MODE5_0,	          20
.equ GPIO_MODE5_1,	          21
.equ GPIO_CNF5_0,	            22
.equ GPIO_CNF5_1,	            23
.equ GPIO_MODE6_0,	          24
.equ GPIO_MODE6_1,	          25
.equ GPIO_CNF6_0,	            26
.equ GPIO_CNF6_1,	            27
.equ GPIO_MODE7_0,	          28
.equ GPIO_MODE7_1,	          29
.equ GPIO_CNF7_0,	            30
.equ GPIO_CNF7_1,	            31

.equ GPIO_MODE8_0,	          GPIO_MODE0_0
.equ GPIO_MODE8_1,	          GPIO_MODE0_1
.equ GPIO_CNF8_0,	            GPIO_CNF0_0
.equ GPIO_CNF8_1,	            GPIO_CNF0_1
.equ GPIO_MODE9_0,	          GPIO_MODE1_0
.equ GPIO_MODE9_1,	          GPIO_MODE1_1
.equ GPIO_CNF9_0,	            GPIO_CNF1_0
.equ GPIO_CNF9_1,	            GPIO_CNF1_1
.equ GPIO_MODE10_0,	          GPIO_MODE2_0
.equ GPIO_MODE10_1,	          GPIO_MODE2_1
.equ GPIO_CNF10_0,	          GPIO_CNF2_0
.equ GPIO_CNF10_1,	          GPIO_CNF2_1
.equ GPIO_MODE11_0,	          GPIO_MODE3_0
.equ GPIO_MODE11_1,	          GPIO_MODE3_1
.equ GPIO_CNF11_0,	          GPIO_CNF3_0
.equ GPIO_CNF11_1,	          GPIO_CNF3_1
.equ GPIO_MODE12_0,	          GPIO_MODE4_0
.equ GPIO_MODE12_1,	          GPIO_MODE4_1
.equ GPIO_CNF12_0,	          GPIO_CNF4_0
.equ GPIO_CNF12_1,	          GPIO_CNF4_1
.equ GPIO_MODE13_0,	          GPIO_MODE5_0
.equ GPIO_MODE13_1,	          GPIO_MODE5_1
.equ GPIO_CNF13_0,	          GPIO_CNF5_0
.equ GPIO_CNF13_1,	          GPIO_CNF5_1
.equ GPIO_MODE14_0,	          GPIO_MODE6_0
.equ GPIO_MODE14_1,	          GPIO_MODE6_1
.equ GPIO_CNF14_0,	          GPIO_CNF6_0
.equ GPIO_CNF14_1,	          GPIO_CNF6_1
.equ GPIO_MODE15_0,	          GPIO_MODE7_0
.equ GPIO_MODE15_1,	          GPIO_MODE7_1
.equ GPIO_CNF15_0,	          GPIO_CNF7_0
.equ GPIO_CNF15_1,	          GPIO_CNF7_1

.equ GPIO_IDR0,	              0
.equ GPIO_IDR1,	              1
.equ GPIO_IDR2,	              2
.equ GPIO_IDR3,	              3
.equ GPIO_IDR4,	              4
.equ GPIO_IDR5,	              5
.equ GPIO_IDR6,	              6
.equ GPIO_IDR7,	              7
.equ GPIO_IDR8,	              8
.equ GPIO_IDR9,	              9
.equ GPIO_IDR10,              10
.equ GPIO_IDR11,              11
.equ GPIO_IDR12,              12
.equ GPIO_IDR13,              13
.equ GPIO_IDR14,              14
.equ GPIO_IDR15,              15

.equ GPIO_ODR0,	              0
.equ GPIO_ODR1,	              1
.equ GPIO_ODR2,	              2
.equ GPIO_ODR3,	              3
.equ GPIO_ODR4,	              4
.equ GPIO_ODR5,	              5
.equ GPIO_ODR6,	              6
.equ GPIO_ODR7,	              7
.equ GPIO_ODR8,	              8
.equ GPIO_ODR9,	              9
.equ GPIO_ODR10,              10
.equ GPIO_ODR11,              11
.equ GPIO_ODR12,              12
.equ GPIO_ODR13,              13
.equ GPIO_ODR14,              14
.equ GPIO_ODR15,              15

.equ GPIO_BS0,	              0
.equ GPIO_BS1,	              1
.equ GPIO_BS2,	              2
.equ GPIO_BS3,	              3
.equ GPIO_BS4,	              4
.equ GPIO_BS5,	              5
.equ GPIO_BS6,	              6
.equ GPIO_BS7,	              7
.equ GPIO_BS8,	              8
.equ GPIO_BS9,	              9
.equ GPIO_BS10,               10
.equ GPIO_BS11,               11
.equ GPIO_BS12,               12
.equ GPIO_BS13,               13
.equ GPIO_BS14,               14
.equ GPIO_BS15,               15

.equ GPIO_BR0,	              16
.equ GPIO_BR1,	              17
.equ GPIO_BR2,	              18
.equ GPIO_BR3,	              19
.equ GPIO_BR4,	              30
.equ GPIO_BR5,	              21
.equ GPIO_BR6,	              22
.equ GPIO_BR7,	              23
.equ GPIO_BR8,	              24
.equ GPIO_BR9,	              25
.equ GPIO_BR10,               26
.equ GPIO_BR11,               27
.equ GPIO_BR12,               28
.equ GPIO_BR13,               29
.equ GPIO_BR14,               30
.equ GPIO_BR15,               31

.equ GPIO_BRR0,	              0
.equ GPIO_BRR1,	              1
.equ GPIO_BRR2,	              2
.equ GPIO_BRR3,	              3
.equ GPIO_BRR4,	              4
.equ GPIO_BRR5,	              5
.equ GPIO_BRR6,	              6
.equ GPIO_BRR7,	              7
.equ GPIO_BRR8,	              8
.equ GPIO_BRR9,	              9
.equ GPIO_BRR10,              10
.equ GPIO_BRR11,              11
.equ GPIO_BRR12,              12
.equ GPIO_BRR13,              13
.equ GPIO_BRR14,              14
.equ GPIO_BRR15,              15

.equ GPIO_LCKR0,	            0
.equ GPIO_LCKR1,	            1
.equ GPIO_LCKR2,	            2
.equ GPIO_LCKR3,	            3
.equ GPIO_LCKR4,	            4
.equ GPIO_LCKR5,	            5
.equ GPIO_LCKR6,	            6
.equ GPIO_LCKR7,	            7
.equ GPIO_LCKR8,	            8
.equ GPIO_LCKR9,	            9
.equ GPIO_LCKR10,             10
.equ GPIO_LCKR11,             11
.equ GPIO_LCKR12,             12
.equ GPIO_LCKR13,             13
.equ GPIO_LCKR14,             14
.equ GPIO_LCKR15,             15
.equ GPIO_LCKR_KEY,           0x00010000
