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



@ .equ I2c2_BAUDRATE,         115200
@ .equ I2c2_FRACTION,         0x0001 /* Baudrate = 9600 -> Fractual = 0x0120 */
@ .equ I2c2_BAUDRATE_CALC,    ((SystemCoreClock / (I2c2_BAUDRATE * 16)) << 4) | I2c2_FRACTION



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
  ldr tmpd, =I2C_PERIPH_FREQ_2
  str tmpd, [tmpa]

  /* CCR calculating */
  ldr tmpa, =I2C1_CCR
  mov tmpd, #1441 /* (1/I2C_SM_FREQ/2)/(1/APB1_FREQ) */
  @ movs tmpd, #(1/I2C_SM_FREQ/2)/(1/APB1_FREQ)
  str tmpd, [tmpa]

  /* TRISE calculating */
  ldr tmpa, =I2C1_TRISE
  movs tmpd, #23 /* ((I2C_TRISE_SM/(1/APB1_FREQ)+1) */
  @ movs tmpd, #((I2C_TRISE_SM/(1/APB1_FREQ))+1)
  str tmpd, [tmpa]

  /* Configure I2c2 */
  ldr tmpa, =I2C1_CR1
  mov tmpd, \
      I2C_CR1_PE \
    | I2C_CR1_ACK
  str tmpd, [tmpa]


  pop {tmpa, tmpd, tmp, pc}
  .size I2c1Init, .-I2c1Init




  .end
