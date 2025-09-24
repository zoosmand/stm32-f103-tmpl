/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

__STATIC_INLINE void dS18B20_Command(uint8_t);

__STATIC_INLINE void dS18B20_Write(uint8_t, uint8_t*);

static int dS18B20_Read(uint8_t, uint8_t*, uint8_t);

static int dS18B20_ReadScratchpad(uint8_t*, uint8_t*);

static void dS18B20_ErrorHandler(void);

static int dS18B20_ConvertTemperature(uint8_t*);

__STATIC_INLINE void dS18B20_WaitStatus(uint16_t);







// -------------------------------------------------------------  
__STATIC_INLINE void dS18B20_Command(uint8_t cmd) {
  OneWire_WriteByte(cmd);
}


// -------------------------------------------------------------  
__STATIC_INLINE void dS18B20_Write(uint8_t len, uint8_t* buf) {
  for (int i = 0; i < len; i++){
    OneWire_WriteByte(buf[i]);
  }
}


// -------------------------------------------------------------  
static int dS18B20_Read(uint8_t len, uint8_t* buf, uint8_t reverse) {
  uint8_t crc = 0;

  if (reverse) {
    for (int i = (len - 1); i >= 0; i--){
      OneWire_ReadByte(&buf[i]);
      crc = OneWire_CRC8(crc, buf[i]);
    }
  } else {
    for (int i = 0; i < len; i++){
      OneWire_ReadByte(&buf[i]);
      crc = OneWire_CRC8(crc, buf[i]);
    }
  }

  return crc;
}




// -------------------------------------------------------------  
static int dS18B20_ReadScratchpad(uint8_t* buf, uint8_t* addr) {

  if (OneWire_MatchROM(addr)) return (1);
  dS18B20_Command(ReadScratchpad);

  uint8_t crc = 0;
  for (int8_t i = 0; i < 9; i++) {
    OneWire_ReadByte(&buf[i]);
    crc = OneWire_CRC8(crc, buf[i]);
  }
  if (crc) return 1;
  
  return (0);
}




// -------------------------------------------------------------  
static int dS18B20_ConvertTemperature(uint8_t* addr) {

  if (*addr) {

    if (OneWire_MatchROM(addr)) return 1;
    uint8_t pps = OneWire_ReadPowerSupply(addr);

    if (OneWire_MatchROM(addr)) return 1;
    dS18B20_Command(ConvertT);
    
    if (pps) {
      PIN_H(OneWire_PORT, OneWire_PIN);
      
      uint32_t delay_threshold = sysCnt + 750;
      while (delay_threshold >= sysCnt) {__asm volatile("nop");};

      PIN_L(OneWire_PORT, OneWire_PIN);
    } else {
      dS18B20_WaitStatus(3);
    }

  } else {
    if (OneWire_Reset()) return (1);

    dS18B20_Command(SkipROM);
    dS18B20_Command(ConvertT);
    dS18B20_WaitStatus(3);
  }
  
  return 0;

}




// -------------------------------------------------------------  
__STATIC_INLINE void dS18B20_WaitStatus(uint16_t ms) {
  uint32_t wait_threshold = sysCnt + ms;
  while(!OneWire_ReadBit()) {
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
int DS18B20_GetTemperatureMeasurment(OneWireDevice_t *dev) {

  // OneWireDevice_t* devs = Get_OwDevices();

  dS18B20_ConvertTemperature(dev->addr);
  
  uint32_t delay_threshold = sysCnt + 3;
  while (delay_threshold >= sysCnt) {__asm volatile("nop");};
  
  dS18B20_ReadScratchpad(dev->spad, dev->addr);
  dS18B20_WaitStatus(3);

  return 0;
}