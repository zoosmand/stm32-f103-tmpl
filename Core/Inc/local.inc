/**
  * Local definitions.
  *
  */


.equ SystemCoreClock,         72000000


/**
  * @brief Register definitions.
  *
  */
tmpa    .req v1
tmpd    .req v2
tmp     .req v3
zero    .req v6
one     .req v7
_OFF_   .req v6
_ON_    .req v7

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
@ .equ _SYSTICKF_,              0 @ SysTick Interval Flag
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





/** Independent watchdog definitions */
.equ IWDG_KEY_RELOAD,         0x0000aaaa
.equ IWDG_KEY_ENABLE,         0x0000cccc
.equ IWDG_KEY_ACCESS,         0x00005555
.equ IWDG_RELOAD_COUNTER,     0x00000fff /* 4095 */


/** STM32 specific Interrupt Numbers */
.equ  WWDG_IRQn,              0      /* Window WatchDog Interrupt */
.equ  PVD_IRQn,               1      /* PVD through EXTI Line detection Interrupt            */
.equ  TAMPER_IRQn,            2      /* Tamper Interrupt */
.equ  RTC_IRQn,               3      /* RTC global Interrupt */
.equ  FLASH_IRQn,             4      /* FLASH global Interrupt */
.equ  RCC_IRQn,               5      /* RCC global Interrupt */
.equ  EXTI0_IRQn,             6      /* EXTI Line0 Interrupt */
.equ  EXTI1_IRQn,             7      /* EXTI Line1 Interrupt */
.equ  EXTI2_IRQn,             8      /* EXTI Line2 Interrupt */
.equ  EXTI3_IRQn,             9      /* EXTI Line3 Interrupt */
.equ  EXTI4_IRQn,             10     /* EXTI Line4 Interrupt */
.equ  DMA1_Channel1_IRQn,     11     /* DMA1 Channel 1 global Interrupt */
.equ  DMA1_Channel2_IRQn,     12     /* DMA1 Channel 2 global Interrupt */
.equ  DMA1_Channel3_IRQn,     13     /* DMA1 Channel 3 global Interrupt */
.equ  DMA1_Channel4_IRQn,     14     /* DMA1 Channel 4 global Interrupt */
.equ  DMA1_Channel5_IRQn,     15     /* DMA1 Channel 5 global Interrupt */
.equ  DMA1_Channel6_IRQn,     16     /* DMA1 Channel 6 global Interrupt */
.equ  DMA1_Channel7_IRQn,     17     /* DMA1 Channel 7 global Interrupt */
.equ  ADC1_2_IRQn,            18     /* ADC1 and ADC2 global Interrupt */
.equ  USB_HP_CAN1_TX_IRQn,    19     /* USB Device High Priority or CAN1 TX Interrupts */
.equ  USB_LP_CAN1_RX0_IRQn,   20     /* USB Device Low Priority or CAN1 RX0 Interrupts */
.equ  CAN1_RX1_IRQn,          21     /* CAN1 RX1 Interrupt */
.equ  CAN1_SCE_IRQn,          22     /* CAN1 SCE Interrupt */
.equ  EXTI9_5_IRQn,           23     /* External Line[9:5] Interrupts */
.equ  TIM1_BRK_IRQn,          24     /* TIM1 Break Interrupt */
.equ  TIM1_UP_IRQn,           25     /* TIM1 Update Interrupt */
.equ  TIM1_TRG_COM_IRQn,      26     /* TIM1 Trigger and Commutation Interrupt */
.equ  TIM1_CC_IRQn,           27     /* TIM1 Capture Compare Interrupt */
.equ  TIM2_IRQn,              28     /* TIM2 global Interrupt */
.equ  TIM3_IRQn,              29     /* TIM3 global Interrupt */
.equ  TIM4_IRQn,              30     /* TIM4 global Interrupt */
.equ  I2C1_EV_IRQn,           31     /* I2C1 Event Interrupt */
.equ  I2C1_ER_IRQn,           32     /* I2C1 Error Interrupt */
.equ  I2C2_EV_IRQn,           33     /* I2C2 Event Interrupt */
.equ  I2C2_ER_IRQn,           34     /* I2C2 Error Interrupt */
.equ  SPI1_IRQn,              35     /* SPI1 global Interrupt */
.equ  SPI2_IRQn,              36     /* SPI2 global Interrupt */
.equ  USART1_IRQn,            37     /* USART1 global Interrupt */
.equ  USART2_IRQn,            38     /* USART2 global Interrupt */
.equ  USART3_IRQn,            39     /* USART3 global Interrupt */
.equ  EXTI15_10_IRQn,         40     /* External Line[15:10] Interrupts */
.equ  RTC_Alarm_IRQn,         41     /* RTC Alarm through EXTI Line Interrupt */
.equ  USBWakeUp_IRQn,         42     /* USB Device WakeUp from suspend through EXTI Line Interrupt */


.equ GPIO_LCKR_KEY,           0x00010000

.equ PIN_0,                   0x00000001 /*!< Select pin 0 */
.equ PIN_0_Pos,               0
.equ PIN_0_Mask,              0x0000000f
.equ PIN_1,                   0x00000002 /*!< Select pin 1 */
.equ PIN_1_Pos,               1
.equ PIN_1_Mask,              0x000000f0
.equ PIN_2,                   0x00000004 /*!< Select pin 2 */
.equ PIN_2_Pos,               2
.equ PIN_2_Mask,              0x00000f00
.equ PIN_3,                   0x00000008 /*!< Select pin 3 */
.equ PIN_3_Pos,               3
.equ PIN_3_Mask,              0x0000f000
.equ PIN_4,                   0x00000010 /*!< Select pin 4 */
.equ PIN_4_Pos,               4
.equ PIN_4_Mask,              0x000f0000
.equ PIN_5,                   0x00000020 /*!< Select pin 5 */
.equ PIN_5_Pos,               5
.equ PIN_5_Mask,              0x00f00000
.equ PIN_6,                   0x00000040 /*!< Select pin 6 */
.equ PIN_6_Pos,               6
.equ PIN_6_Mask,              0x0f000000
.equ PIN_7,                   0x00000080 /*!< Select pin 7 */
.equ PIN_7_Pos,               7
.equ PIN_7_Mask,              0xf0000000
.equ PIN_8,                   0x00000100 /*!< Select pin 8 */
.equ PIN_8_Pos,               8
.equ PIN_8_Mask,              0x0000000f
.equ PIN_9,                   0x00000200 /*!< Select pin 9 */
.equ PIN_9_Pos,               9
.equ PIN_9_Mask,              0x000000f0
.equ PIN_10,                  0x00000400 /*!< Select pin 10 */
.equ PIN_10_Pos,              10
.equ PIN_10_Mask,             0x00000f00
.equ PIN_11,                  0x00000800 /*!< Select pin 11 */
.equ PIN_11_Pos,              11
.equ PIN_11_Mask,             0x0000f000
.equ PIN_12,                  0x00001000 /*!< Select pin 12 */
.equ PIN_12_Pos,              12
.equ PIN_12_Mask,             0x000f0000
.equ PIN_13,                  0x00002000 /*!< Select pin 13 */
.equ PIN_13_Pos,              13
.equ PIN_13_Mask,             0x00f00000
.equ PIN_14,                  0x00004000 /*!< Select pin 14 */
.equ PIN_14_Pos,              14
.equ PIN_14_Mask,             0x0f000000
.equ PIN_15,                  0x00008000 /*!< Select pin 15 */
.equ PIN_15_Pos,              15
.equ PIN_15_Mask,             0xf0000000
.equ PIN_ALL,                 0x0000ffff /*!< Select all pins */



.equ _GPO_PP,                 0b0000 /* General purpose output push-pull */
.equ _GPO_OD,                 0b0100 /* General purpose output open drain */
.equ _IOS_2,                  0b0010 /* I/O speed is 2 MHz */
.equ _IOS_10,                 0b0001 /* I/O speed is 10 MHz */
.equ _IOS_50,                 0b0011 /* I/O speed is 50 MHz */
.equ _AF_PP,                  0b1000 /* Alterhative function push-pull */
.equ _AF_OD,                  0b1100 /* Alterhative function open drain */
.equ _IN_AN,                  0b0000 /* Input analog */
.equ _IN_FL,                  0b0100 /* Input floating point */
.equ _IN_PD,                  0b1000 /* correspondent ODR register pin has to be 0 */
.equ _IN_PU,                  0b1000 /* correspondent ODR register pin has to be 1 */
