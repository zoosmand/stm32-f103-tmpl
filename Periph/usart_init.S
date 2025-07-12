/**
  * System, clock, and peripheral initialization.
  *
  */
  .syntax unified
  .thumb

#include <stm32f103xb.h>
.include "stm32f1xx.inc"
.include "macroses.inc"
.include "local.inc"



.equ USART1_BAUDRATE,         115200
.equ USART1_FRACTION,         0x0001 /* Baudrate = 9600 -> Fractual = 0x0120 */
.equ USART1_BAUDRATE_CALC,    ((SystemCoreClock / (USART1_BAUDRATE * 16)) << 4) | USART1_FRACTION



/**
  * USART1 initialization.
  *
  */
  .section .text.Usart1Init
  .global Usart1Init
  .type Usart1Init, %function
Usart1Init:
  push {tmpa, tmpd, tmp, lr}
  
  /* Condigure PA9 - TX, PA10 - RX, max frequency, alternative*/
  ldr tmpa, =GPIOA_CRH
  ldr tmpd, [tmpa]
  mov tmp, ~(PIN_9_Mask | PIN_10_Mask)
  and tmpd, tmp
  mov tmp, \
      (_AF_PP | _IOS_50) << ((PIN_9_Pos - 8) * 4) \
    | _IN_FL << ((PIN_10_Pos - 8) * 4)
  orr tmpd, tmp
  str tmpd, [tmpa]

  /* Set USART1 Baudrate */
  STRW USART1_BAUDRATE_CALC, USART1_BRR

  /* Configure USART1 */
  ldr tmpa, =USART1_CR1
  mov tmpd, \
      USART_CR1_TE \
    | USART_CR1_RE \
    | USART_CR1_RXNEIE \
    | USART_CR1_UE
  str tmpd, [tmpa]

  /** Activate interrupt */
  ldr tmpa, =NVIC_ISER1
  ldr tmpd, [tmpa]
  mov tmp, (1 << (USART1_IRQn & 0x1f))
  orr tmpd, tmp
  str tmpd, [tmpa]

  /* Set NVIC Priority */
  STRW 0x0000f000, NVIC_IPR9

  pop {tmpa, tmpd, tmp, pc}
  .size Usart1Init, .-Usart1Init




  .end
