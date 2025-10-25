/**
  * System, clock, and peripheral initialization.
  *
  */
  .syntax unified
  .thumb

#include <stm32f103xb.h>
@ #include <core_cm3.h>
.include "stm32f1xx.inc"
.include "macroses.inc"
.include "local.inc"


.equ I2C_CCR_VAL, ((I2C_TRISE_SM/2)/(1000/(APB1_FREQ/1000000)))+1
.equ I2C1_TRISE_VAL, (1000/(1000/(APB1_FREQ/1000000)))+1


/**
  * I2c2 initialization.
  *
  */
  .section .text.I2c1Init
  .global I2c1Init
  .type I2c1Init, %function
I2c1Init:
  push {tmpa, tmpd, tmp, lr}
  
  /* Condigure PB6 - SCL, PB7 - SDA, 2MHz frequency, alternative*/
  ldr tmpa, =GPIOB_CRL
  ldr tmpd, [tmpa]
  mov tmp, ~(PIN_6_Mask | PIN_7_Mask)
  and tmpd, tmp
  mov tmp, \
      (_AF_OD | _IOS_2) << (PIN_6_Pos * 4) \
    | (_AF_OD | _IOS_2) << (PIN_7_Pos * 4)
  orr tmpd, tmp
  str tmpd, [tmpa]

  /* Peripheral frequency */
  ldr tmpa, =I2C1_CR2

  .equ I2C_BUS_FREQ, APB1_FREQ/1000000
  @ movs tmpd, #36
  movs tmpd, #I2C_BUS_FREQ
  str tmpd, [tmpa]

  /* CCR calculating */
  ldr tmpa, =I2C1_CCR
  ldr tmpd, =I2C_CCR_VAL
  @ movs tmpd, #180
  str tmpd, [tmpa]

  /* TRISE calculating */
  ldr tmpa, =I2C1_TRISE
  movs tmpd, #I2C1_TRISE_VAL
  @ movs tmpd, #37
  str tmpd, [tmpa]

  pop {tmpa, tmpd, tmp, pc}
  .size I2c1Init, .-I2c1Init




  .end
