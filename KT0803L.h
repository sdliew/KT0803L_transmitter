/**
 * Library implementation for KT0803L chip
 */

#ifndef _KT0803L   // Prevent this file from being compiled more than once
#define _KT0803L

#include <Arduino.h>
#include <Wire.h>


const uint8_t KT0803_ADDRESS = 0x3E;

const uint8_t KT0803_REGA_00 = 0x00;
const uint8_t KT0803_REGA_01 = 0x01;
const uint8_t KT0803_REGA_02 = 0x02;
const uint8_t KT0803_REGA_04 = 0x04;
const uint8_t KT0803_REGA_0B = 0x0B;
const uint8_t KT0803_REGA_0E = 0x0E;
const uint8_t KT0803_REGA_0F = 0x0F;
const uint8_t KT0803_REGA_10 = 0x10;
const uint8_t KT0803_REGA_12 = 0x12;
const uint8_t KT0803_REGA_13 = 0x13;
const uint8_t KT0803_REGA_14 = 0x14;
const uint8_t KT0803_REGA_16 = 0x16;
const uint8_t KT0803_REGA_1E = 0x1E;

/*
uint8_t KT0803L_REGISTER[18] = {
    0x00, 0x01, 0x02, 0x04, 0x0B,
    0x0C, 0x0E, 0x0F, 0x10, 0x12,
    0x13, 0x14, 0x15, 0x16, 0x17,
    0x1E, 0x26, 0x27,
};
*/
/**
 * KT0803L declares 18 registers
 */
//const uint8_t KT0803L_REG_COUNT = 18;

/**
 * KT0803L new registers
 */
const uint8_t KT0803L_REGA_0C = 0x0C;
const uint8_t KT0803L_REGA_15 = 0x15;
const uint8_t KT0803L_REGA_17 = 0x17;
const uint8_t KT0803L_REGA_1E = 0x1E;
const uint8_t KT0803L_REGA_26 = 0x26;
const uint8_t KT0803L_REGA_27 = 0x27;
/*
const uint8_t KT0803L_REGS[KT0803L_REG_COUNT] = {
  KT0803_REGA_00,
  KT0803_REGA_01,
  KT0803_REGA_02,
  KT0803_REGA_04,
  KT0803_REGA_0B,
  KT0803_REGA_0E,
  KT0803_REGA_0F,
  KT0803_REGA_10,
  KT0803_REGA_12,
  KT0803_REGA_12,
  KT0803_REGA_14,
  KT0803_REGA_16,
  KT0803L_REGA_15,
  KT0803L_REGA_0C,
  KT0803L_REGA_17,
  KT0803L_REGA_1E,
  KT0803L_REGA_26,
  KT0803L_REGA_27
};

*/
typedef enum {
    PGA_12DB = 0x07,
    PGA_8DB  = 0x06,
    PGA_4DB  = 0x05,
    PGA_0DB  = 0x04,
    PGA_N0DB = 0x00,
    PGA_N4DB = 0x01,
    PGA_N8DB = 0x02,
    PGA_N12DB = 0x03,
} PGA;

class KT0803L 
{
  protected:
    uint16_t currentFrequency;
    char strFrequency[8];     // For storing formated frequency
    
  public:
    KT0803L(uint8_t sda, uint8_t sci, uint16_t freq);
    ~KT0803L();
    void mute(uint8_t enable);
    void setPGA(PGA pga);
    void setRFgain(uint8_t rfgain);
    void setXtal(void);
    void setFrequency(uint16_t frequency);
    void setRegister(uint8_t reg, uint8_t value);
    uint8_t getRegister(const uint8_t reg);
    
    char *convertToChar(uint16_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator, bool remove_leading_zeros);

    inline uint16_t getFrequency(void) { return currentFrequency; };
    
    inline char *formatFrequency(uint16_t value, char *strValue, char decimalSeparator)
    {
        convertToChar(value, strValue, 4, 3, decimalSeparator, true);
        return strValue;
    };
    
    inline char *formatCurrentFrequency(char decimalSeparator = '.')
    {
      convertToChar(currentFrequency, strFrequency, 4, 3, decimalSeparator, true);
      return strFrequency;
    };
};

#endif KT0803L
