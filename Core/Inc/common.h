/**
  ******************************************************************************
  * @file           : common.h
  * @brief          : Header for common.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Exported types ------------------------------------------------------------*/
    

/* Private typedef -----------------------------------------------------------*/



/* Private defines -----------------------------------------------------------*/
/* NVIC */
#define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 bits for pre-emption priority 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 bits for pre-emption priority 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         0x00000005U /*!< 2 bits for pre-emption priority 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 bits for pre-emption priority 1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority 0 bits for subpriority */

/* GPIO*/
#define GPIO_PIN_0                  GPIO_BSRR_BS0_Pos /*!< Select pin 0 */
#define GPIO_PIN_0_Pos              GPIO_PIN_0
#define GPIO_PIN_0_Mask             0x0000000f
#define GPIO_PIN_1                  GPIO_BSRR_BS1_Pos /*!< Select pin 1 */
#define GPIO_PIN_1_Pos              GPIO_PIN_1
#define GPIO_PIN_1_Mask             0x000000f0
#define GPIO_PIN_2                  GPIO_BSRR_BS2_Pos /*!< Select pin 2 */
#define GPIO_PIN_2_Pos              GPIO_PIN_2
#define GPIO_PIN_2_Mask             0x00000f00
#define GPIO_PIN_3                  GPIO_BSRR_BS3_Pos /*!< Select pin 3 */
#define GPIO_PIN_3_Pos              GPIO_PIN_3
#define GPIO_PIN_3_Mask             0x0000f000
#define GPIO_PIN_4                  GPIO_BSRR_BS4_Pos /*!< Select pin 4 */
#define GPIO_PIN_4_Pos              GPIO_PIN_4
#define GPIO_PIN_4_Mask             0x000f0000
#define GPIO_PIN_5                  GPIO_BSRR_BS5_Pos /*!< Select pin 5 */
#define GPIO_PIN_5_Pos              GPIO_PIN_5
#define GPIO_PIN_5_Mask             0x00f00000
#define GPIO_PIN_6                  GPIO_BSRR_BS6_Pos /*!< Select pin 6 */
#define GPIO_PIN_6_Pos              GPIO_PIN_6
#define GPIO_PIN_6_Mask             0x0f000000
#define GPIO_PIN_7                  GPIO_BSRR_BS7_Pos /*!< Select pin 7 */
#define GPIO_PIN_7_Pos              GPIO_PIN_7
#define GPIO_PIN_7_Mask             0xf0000000
#define GPIO_PIN_8                  GPIO_BSRR_BS8_Pos /*!< Select pin 8 */
#define GPIO_PIN_8_Pos              GPIO_PIN_8
#define GPIO_PIN_8_Mask             0x0000000f
#define GPIO_PIN_9                  GPIO_BSRR_BS9_Pos /*!< Select pin 9 */
#define GPIO_PIN_9_Pos              GPIO_PIN_9
#define GPIO_PIN_9_Mask             0x000000f0
#define GPIO_PIN_10                 GPIO_BSRR_BS10_Pos /*!< Select pin 10 */
#define GPIO_PIN_10_Pos             GPIO_PIN_10
#define GPIO_PIN_10_Mask            0x00000f00
#define GPIO_PIN_11                 GPIO_BSRR_BS11_Pos /*!< Select pin 11 */
#define GPIO_PIN_11_Pos             GPIO_PIN_11
#define GPIO_PIN_11_Mask            0x0000f000
#define GPIO_PIN_12                 GPIO_BSRR_BS12_Pos /*!< Select pin 12 */
#define GPIO_PIN_12_Pos             GPIO_PIN_12
#define GPIO_PIN_12_Mask            0x000f0000
#define GPIO_PIN_13                 GPIO_BSRR_BS13_Pos /*!< Select pin 13 */
#define GPIO_PIN_13_Pos             GPIO_PIN_13
#define GPIO_PIN_13_Mask            0x00f00000
#define GPIO_PIN_14                 GPIO_BSRR_BS14_Pos /*!< Select pin 14 */
#define GPIO_PIN_14_Pos             GPIO_PIN_14
#define GPIO_PIN_14_Mask            0x0f000000
#define GPIO_PIN_15                 GPIO_BSRR_BS15_Pos /*!< Select pin 15 */
#define GPIO_PIN_15_Pos             GPIO_PIN_15
#define GPIO_PIN_15_Mask            0xf0000000
#define GPIO_PIN_ALL                (uint16_t)0xffff /*!< Select all pins */

/* GPIO Modes bit definitions */
#define GPIO_GPO_PP                 0b0000 // General purpose output push-pull
#define GPIO_GPO_OD                 0b0100 // General purpose output open drain
#define GPIO_IOS_2                  0b0010 // I/O speed is 2 MHz
#define GPIO_IOS_10                 0b0001 // I/O speed is 10 MHz
#define GPIO_IOS_50                 0b0011 // I/O speed is 50 MHz
#define GPIO_AF_PP                  0b1000 // Alterhative function push-pull
#define GPIO_AF_OD                  0b1100 // Alterhative function open drain
#define GPIO_IN_AN                  0b0000 // Input analog
#define GPIO_IN_FL                  0b0100 // Input floating point
#define GPIO_IN_PD                  0b1000 // correspondent ODR register pin has to be 0
#define GPIO_IN_PU                  0b1000 // correspondent ODR register pin has to be 1

/* GPIO Alternative function defines */
#define GPIO_AF_0                   0b0000 /*!< Select alternate function 0 */
#define GPIO_AF_1                   0b0001 /*!< Select alternate function 1 */
#define GPIO_AF_2                   0b0010 /*!< Select alternate function 2 */
#define GPIO_AF_3                   0b0011 /*!< Select alternate function 3 */
#define GPIO_AF_4                   0b0100 /*!< Select alternate function 4 */
#define GPIO_AF_5                   0b0101 /*!< Select alternate function 5 */
#define GPIO_AF_6                   0b0110 /*!< Select alternate function 6 */
#define GPIO_AF_7                   0b0111 /*!< Select alternate function 7 */
#define GPIO_AF_8                   0b1000 /*!< Select alternate function 8 */
#define GPIO_AF_9                   0b1001 /*!< Select alternate function 9 */
#define GPIO_AF_10                  0b1010 /*!< Select alternate function 10 */
#define GPIO_AF_11                  0b1011 /*!< Select alternate function 11 */
#define GPIO_AF_12                  0b1100 /*!< Select alternate function 12 */
#define GPIO_AF_13                  0b1101 /*!< Select alternate function 13 */
#define GPIO_AF_14                  0b1110 /*!< Select alternate function 14 */
#define GPIO_AF_15                  0b1111 /*!< Select alternate function 15 */
/* GPIO Register Offcets */
#define GPIO_IDR_Offset             (uint16_t)0x0008
#define GPIO_ODR_Offset             (uint16_t)0x000c
#define GPIO_BSRR_Offset            (uint16_t)0x0010
#define GPIO_BRR_Offset             (uint16_t)0x0014

/* EXTI */
#define EXTI_TRIGGER_NONE           0b00 /*!< No Trigger Mode */
#define EXTI_TRIGGER_RISING         0b01 /*!< Trigger Rising Mode */
#define EXTI_TRIGGER_FALLING        0b10 /*!< Trigger Falling Mode */
#define EXTI_TRIGGER_BOTH           0b11 /*!< Trigger Rising & Falling Mode */

/* IWDG */
#define IWDG_KEY_RELOAD             0x0000aaaa
#define IWDG_KEY_ENABLE             0x0000cccc
#define IWDG_KEY_ACCESS             0x00005555
#define IWDG_RELOAD_COUNTER         0x00000fff /* 4095 */



/* Exported macro ------------------------------------------------------------*/
#define GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset)      (PERIPH_BB_BASE + ((addr + periphOffset) * 32U) + (wordOffset * 4U))
#define SET_PERIPH_BB_VAL(addr, periphOffset, wordOffset, key)  (*(uint32_t*)(GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset)) = key)
#define GET_PERIPH_BB_VAL(addr, periphOffset, wordOffset)       (*(__O uint32_t*)(GET_PERIPH_BB_ADDR(addr, periphOffset, wordOffset)))

#define GET_SRAM_BB_ADDR(addr, offset)                          (SRAM_BB_BASE + (addr * 32U) + (offset * 4U))
#define SET_SRAM_BB_VAL(addr, offset, key)                      (*(uint32_t*)(GET_SRAM_BB_ADDR(addr, offset)) = key)
#define GET_SRAM_BB_VAL(addr, offset)                           (*(__O uint32_t*)(GET_SRAM_BB_ADDR(addr, offset)))

/* Private macro -------------------------------------------------------------*/
/* --- Bit Band RAM Register flag management macro --- */
#define FLAG_SET(registry, flag)                                SET_SRAM_BB_VAL((uint32_t)registry, flag, 1)
#define FLAG_CLR(registry, flag)                                SET_SRAM_BB_VAL((uint32_t)registry, flag, 0)
#define FLAG_CHECK(registry, flag)                              (GET_SRAM_BB_VAL((uint32_t)registry, flag))

/* --- Bit Band GPIO pin management macro --- */
#define PIN_H(port, pinSource)                                  SET_PERIPH_BB_VAL((uint32_t)port, GPIO_BSRR_Offset, pinSource, 1)
#define PIN_L(port, pinSource)                                  SET_PERIPH_BB_VAL((uint32_t)port, GPIO_BSRR_Offset, (pinSource + 16U), 1)
#define PIN_LEVEL(port, pinSource)                              (GET_PERIPH_BB_VAL((uint32_t)port, GPIO_IDR_Offset, pinSource))

/* --- Bit Band Peripheral Registrie key management macro --- */
#define PREG_SET(peripheral, key)                               SET_PERIPH_BB_VAL((uint32_t)&peripheral, 0, key, 1)
#define PREG_CLR(peripheral, key)                               SET_PERIPH_BB_VAL((uint32_t)&peripheral, 0, key, 0)
#define PREG_CHECK(peripheral, key)                             (GET_PERIPH_BB_VAL((uint32_t)&peripheral, 0, key))

/* --- Task control --- */
#define CRON_SYSTICK_EVENT                                      FLAG_CHECK(&_GEREG_, _SYSTICKF_)
#define CRON_SEC_EVENT                                          FLAG_CHECK(&_GEREG_, _SYSSECF_)
// #define TASK_CTRL(task)                                         if (FLAG_CHECK(task.Scheduler->counterReg, task.Scheduler->entranceFlag)) task.Callback((uint32_t*)&task);



// ----------------------------------------------------------------------------

/**
 * @brief Data transmit diretion mode struct
 * 
 * - TX - transmission from an MCU bus to a peripheral device
 * 
 * - RX - transmission from a peripheral device to an MCU bus
 * 
 * - NONE - no transmission (dummy mode)
 * 
 * ---
 * 
 */
typedef enum {
  TX    = 0,
  RX    = 1,
  NOTR  = (!TX & !RX)
} DataTransmitDirection_TypeDef;



typedef struct {
  uint32_t*             counter;
  uint32_t*             counterSrc;
  uint32_t              period;
  uint32_t*             counterReg;
  uint32_t              entranceFlag;
} task_scheduler_t;


// ----------------------------------------------------------------------------

typedef struct {
  // task_scheduler_t*     Scheduler;
  GPIO_TypeDef*         Port;
  uint16_t              Pin;
  FunctionalState       Lock;
  uint8_t               BlinkCount;
  uint32_t              BlinkDuration;
  uint32_t              BlinkPause;
  uint32_t*             BlinkThreshold;
  uint32_t*             CounterSrc;
  ErrorStatus           (*Callback)(uint32_t*);
} HearbeatDevice_TypeDev;


// ----------------------------------------------------------------------------

typedef struct {
  uint32_t pressure;
  uint32_t temperature;
  uint16_t humidity;
  uint16_t gas_resistance;
  uint8_t gas_range;
  bool gas_valid;
  bool heater_stable;
} BMxX80_results_t;

typedef struct {
  int16_t par_t1;
  int16_t par_t2;
  int16_t par_t3;
  int16_t par_p1;
  int16_t par_p2;
  int16_t par_p3;
  int16_t par_p4;
  int16_t par_p5;
  int16_t par_p6;
  int16_t par_p7;
  int16_t par_p8;
  int16_t par_p9;
  int16_t par_p10;
  int16_t par_h1;
  int16_t par_h2;
  int16_t par_h3;
  int16_t par_h4;
  int16_t par_h5;
  int16_t par_h6;
  int16_t par_h7;
  int16_t par_g1;
  int16_t par_g2;
  int16_t par_g3;
} BMx680_calib_t;

typedef struct {
  int16_t dig_t1;
  int16_t dig_t2;
  int16_t dig_t3;
  int16_t dig_p1;
  int16_t dig_p2;
  int16_t dig_p3;
  int16_t dig_p4;
  int16_t dig_p5;
  int16_t dig_p6;
  int16_t dig_p7;
  int16_t dig_p8;
  int16_t dig_p9;
  int16_t dig_h1;
  int16_t dig_h2;
  int16_t dig_h3;
  int16_t dig_h4;
  int16_t dig_h5;
  int16_t dig_h6;
} BMx280_calib_t;

/**
 * @brief   Bosch BMx280 device type definition struct.
 */
typedef struct {  
  uint8_t               DevID;
  uint8_t*              RawBufPtr;
  BMxX80_results_t      Results;
  int16_t*              CalibBufPtr;
  FunctionalState       Lock;
  I2C_TypeDef*          I2Cx;
  uint8_t               I2C_Address;
  SPI_TypeDef*          SPIx;
  GPIO_TypeDef*         SPINssPort;
  uint16_t              SPINssPin;
  DMA_TypeDef*          DMAx;
  DMA_Channel_TypeDef*  DMAxTx;
  DMA_Channel_TypeDef*  DMAxRx;
  ErrorStatus           (*Callback)(uint32_t*);
} BMxX80_TypeDef;


// ----------------------------------------------------------------------------

/**
 * @brief   EEPROM W25Qxx device type definition struct.
 */
typedef struct {
  uint8_t               ID;
  uint8_t               ManID;
  uint8_t               Type;
  uint64_t              UniqID;
  uint16_t              BlockCount;
  uint32_t              Capacity;
  FunctionalState       Lock;
  SPI_TypeDef*          SPIx;
  DMA_TypeDef*          DMAx;
  DMA_Channel_TypeDef*  DMAxTx;
  DMA_Channel_TypeDef*  DMAxRx;
  ErrorStatus           (*Callback)(uint32_t*);
} W25qxx_TypeDef;


// ----------------------------------------------------------------------------

/**
 * @brief   MAX72xx device type definition struct.
 */
typedef struct {
  uint8_t               SegCnt;
  uint8_t               MaxSegCnt;
  uint16_t*             BufPtr;
  FunctionalState       Lock;
  SPI_TypeDef*          SPIx;
  GPIO_TypeDef*         SPINssPort;
  uint16_t              SPINssPin;
  DMA_TypeDef*          DMAx;
  DMA_Channel_TypeDef*  DMAxTx;
  ErrorStatus           (*Callback)(uint32_t*);
} Max72xx_TypeDef;


// ----------------------------------------------------------------------------

/**
 * @brief   TM163x device type definition struct.
 */
typedef struct {
  FunctionalState       Lock;
  GPIO_TypeDef*         PortSck;
  GPIO_TypeDef*         PortDio;
  uint16_t              PinSck;
  uint16_t              PinDio;
  uint8_t               Dig0;
  uint8_t               Dig1;
  uint8_t               Dig2;
  uint8_t               Dig3;
  uint8_t               Dig4;
  uint8_t               Dig5;
  uint8_t               Dig6;
  uint8_t               Dig7;
  ErrorStatus           (*Callback)(uint32_t*);
} TM163x_TypeDef;


// ----------------------------------------------------------------------------

/**
 * @brief   WHxxxx device type definition struct.
 */
typedef struct {
  FunctionalState       Lock;
  I2C_TypeDef*          I2Cx;
  uint8_t               I2C_Address;
  DMA_TypeDef*          DMAx;
  DMA_Channel_TypeDef*  DMAxTx;
  ErrorStatus           (*Callback)(uint32_t*);
} WHxxxx_TypeDef;


// ----------------------------------------------------------------------------

/**
 * @brief   SSD13xx device type definition struct.
 */
typedef struct {
  FunctionalState       Lock;
  I2C_TypeDef*          I2Cx;
  uint8_t               I2C_Address;
  uint8_t*              BufPtr;
  uint16_t              BufSize;
  DMA_TypeDef*          DMAx;
  DMA_Channel_TypeDef*  DMAxTx;
  ErrorStatus           (*Callback)(uint32_t*);
} SSD13xx_TypeDef;


// ----------------------------------------------------------------------------

/**
 * @brief   OnwWire device type definition struct.
 */
typedef struct {
  FunctionalState       Lock;
  uint8_t               Family;
  uint8_t               Type;
  uint8_t               Addr[8];
  uint8_t               Spad[9];
  uint32_t*             ParentBusPtr;
  ErrorStatus           (*Callback)(uint32_t*);
} OneWireDevice_t;

/**
 * @brief   OnwWire bus type definition struct.
 */
typedef struct {
  FunctionalState       Lock;
  GPIO_TypeDef*         Port;
  uint16_t              Pin;
  OneWireDevice_t*      Devs;
  uint8_t               Count;
  ErrorStatus           (*Callback)(uint32_t*);
} OneWireBus_TypeDef;



// ----------------------------------------------------------------------------

/* Linked List prototype structures  */
// typedef struct {
//   uint32_t  ItemPtr;
//   uint32_t  NextItemPtr;
//   uint32_t  PrevItemPtr;
// } list_item_t;


// typedef struct {
//   uint32_t      LastUsedItemPtr;
//   uint32_t      Capacity;
//   list_item_t*  ItemListPtr;
// } linked_list_t;



/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);



#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H */
