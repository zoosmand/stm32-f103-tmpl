/**
  ******************************************************************************
  * @file           : utils.c
  * @brief          : Rrogram utilities
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/********************************************************************************/
/*                         printf() output supply block                         */
/********************************************************************************/
/**
  * @brief  Sends a symbol into ITM channel. It could be cought with SWO pin on an MC. 
  * @param ch: a symbol to be output
  * @param channel: number of an ITM channel
  * @retval the same symbol 
  */
 __STATIC_INLINE uint32_t ITM_SendCharChannel(uint32_t ch, uint32_t channel) {
  /* ITM enabled and ITM Port enabled */
 if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER & (1 << channel)) != 0UL)) {
   while (ITM->PORT[channel].u32 == 0UL) {
     __NOP();
   }
   ITM->PORT[channel].u8 = (uint8_t)ch;
 }
 return (ch);
}




/**
 * @brief  Sends a symbol into USART. 
 * @param device: a pointer USART_TypeDef
 * @param ch: a symbol to be output
 * @param check: a pointer to a BitBand check bit
 * @retval none: 
 */
__STATIC_INLINE void _putc(uint8_t ch) {
 if (ch == '\n') _putc('\r');

 #ifdef SWO_ITM
    ITM_SendCharChannel(ch, SWO_ITM);
 #endif

 #ifdef SWO_DSPL
    PrintCharDisplay(ch, SWO_DSPL);
 #endif

 #ifdef SWO_USART
    while (!(PREG_CHECK(SWO_USART->SR, USART_SR_TXE_Pos)));
    USART1->DR = ch;
 #endif
}



/**
 * @brief An interpretation of the __weak system _write()
 * @param file: IO file
 * @param ptr: pointer to a char(symbol) array
 * @param len: length oa the array
 * @retval length of the array 
 */
int _write(int32_t file, char *ptr, int32_t len) {
 // static uint32_t check = 0;
 for(int32_t i = 0 ; i < len ; i++) {
   _putc(*ptr++);  
 }
 return len;
}



static void dwt_init(void) {
    DWT->CTRL, 0;
    DWT->CYCCNT = 0;
    SET_BIT(DWT->CTRL, DWT_CTRL_CYCEVTENA_Msk);
    __DSB();
    __ISB();
}


void _delay_us(uint32_t us) {
  dwt_init();
  uint32_t const start = READ_REG(DWT->CYCCNT);
  uint32_t const ticks = us * (APB2_FREQ / 1000000u);
  while ((READ_REG(DWT->CYCCNT) - start) < ticks) { __asm volatile("nop"); }
  CLEAR_BIT(DWT->CTRL, DWT_CTRL_CYCEVTENA_Msk);
}






#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) { 
 printf("Wrong parameters value: file %s on line %d\n", file, line);
}
#endif /* USE_FULL_ASSERT */
