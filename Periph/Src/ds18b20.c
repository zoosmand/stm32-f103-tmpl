/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
OneWireBus_TypeDef* parentBus;

/* Private function prototypes -----------------------------------------------*/

__STATIC_INLINE void dS18B20_Command(OneWireDevice_t*, uint8_t);

__STATIC_INLINE void dS18B20_Write(OneWireDevice_t*, uint8_t, uint8_t*);

static int dS18B20_Read(OneWireDevice_t*, uint8_t, uint8_t*, uint8_t);

static ErrorStatus dS18B20_ReadScratchpad(OneWireDevice_t*);

static ErrorStatus DS18B20_CopyScratchpad(OneWireDevice_t*);

static void dS18B20_ErrorHandler(void);

static ErrorStatus dS18B20_ConvertTemperature(OneWireDevice_t*);

__STATIC_INLINE void dS18B20_WaitStatus(OneWireBus_TypeDef*, uint16_t);







// -------------------------------------------------------------  
__STATIC_INLINE void dS18B20_Command(OneWireDevice_t* dev, uint8_t cmd) {
  OneWire_WriteByte(parentBus, cmd);
}


// -------------------------------------------------------------  
__STATIC_INLINE void dS18B20_Write(OneWireDevice_t* dev, uint8_t len, uint8_t* buf) {
  for (int i = 0; i < len; i++){
    OneWire_WriteByte(parentBus, buf[i]);
  }
}


// -------------------------------------------------------------  
static int dS18B20_Read(OneWireDevice_t* dev, uint8_t len, uint8_t* buf, uint8_t reverse) {
  uint8_t crc = 0;

  if (reverse) {
    for (int i = (len - 1); i >= 0; i--){
      OneWire_ReadByte(parentBus, &buf[i]);
      crc = OneWire_CRC8(crc, buf[i]);
    }
  } else {
    for (int i = 0; i < len; i++){
      OneWire_ReadByte(parentBus, &buf[i]);
      crc = OneWire_CRC8(crc, buf[i]);
    }
  }

  return crc;
}




// -------------------------------------------------------------  
static ErrorStatus dS18B20_ReadScratchpad(OneWireDevice_t* dev) {

  if (OneWire_MatchROM(dev)) return (ERROR);
  dS18B20_Command(dev, ReadScratchpad);

  uint8_t crc = 0;
  for (int8_t i = 0; i < 9; i++) {
    OneWire_ReadByte(parentBus, &dev->Spad[i]);
    crc = OneWire_CRC8(crc, dev->Spad[i]);
  }
  if (crc) return (ERROR);
  
  return (SUCCESS);
}




// -------------------------------------------------------------  
static ErrorStatus dS18B20_ConvertTemperature(OneWireDevice_t* dev) {
  if (dev->Lock == DISABLE) {
    if (OneWire_MatchROM(dev)) return 1;
    uint8_t pps = OneWire_ReadPowerSupply(dev);

    if (OneWire_MatchROM(dev)) return 1;
    dS18B20_Command(dev, ConvertT);
    
    if (pps) {
      PIN_H(OneWire_PORT, OneWire_PIN);
      
      _delay_ms(750);

      PIN_L(OneWire_PORT, OneWire_PIN);
    } else {
      dS18B20_WaitStatus(parentBus, 3);
    }
  } else {
    if (OneWire_Reset(parentBus)) return (ERROR);

    dS18B20_Command(dev, SkipROM);
    dS18B20_Command(dev, ConvertT);
    dS18B20_WaitStatus(parentBus, 3);
  }
  
  return (SUCCESS);
}



static ErrorStatus DS18B20_CopyScratchpad(OneWireDevice_t* dev) {

  if (OneWire_MatchROM(dev)) return (ERROR);
  uint8_t pps = OneWire_ReadPowerSupply(dev);

  if (OneWire_MatchROM(dev)) return 1;
  OneWire_WriteByte(parentBus, CopyScratchpad);

  if (pps) {
    PIN_H(OneWire_PORT, OneWire_PIN);
    
    _delay_ms(2);

    PIN_L(OneWire_PORT, OneWire_PIN);
  } else {
    dS18B20_WaitStatus(parentBus, 3);
  }
  return (SUCCESS);
}




// -------------------------------------------------------------  
__STATIC_INLINE void dS18B20_WaitStatus(OneWireBus_TypeDef* busDev, uint16_t ms) {
  uint32_t wait_threshold = sysCnt + ms;
  while(!OneWire_ReadBit(busDev)) {
    __asm volatile("nop");
    if (sysCnt >= wait_threshold) return;
  };
}




// -------------------------------------------------------------  
static void dS18B20_ErrorHandler(void) {
  while (1) {
    /* code */
  }
}



// -------------------------------------------------------------  
ErrorStatus DS18B20_GetTemperatureMeasurment(OneWireDevice_t *dev) {

  dev->Lock = ENABLE;

  if (dS18B20_ConvertTemperature(dev)) return (ERROR);

  parentBus = (OneWireBus_TypeDef*)dev->ParentBusPtr;
  
  _delay_ms(2);
  
  if (dS18B20_ReadScratchpad(dev)) return (ERROR);
  dS18B20_WaitStatus(parentBus, 3);

  dev->Lock = DISABLE;
  return (SUCCESS);
}