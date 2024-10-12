#include "KT0803L.h"

KT0803L::KT0803L(uint8_t sda=21, uint8_t sck=22, uint16_t freq = 400000)
{
    Wire.begin(sda, sck, freq);//KT0803_ADDRESS);
}

KT0803L::~KT0803L()
{
//    Wire.end();
}

uint8_t KT0803L::getRegister(const uint8_t reg) 
{
  uint8_t data;
  
  Wire.beginTransmission(KT0803_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(KT0803_ADDRESS, (uint8_t) 1);
  data = Wire.read();
  Wire.endTransmission();

  return data;
}


void KT0803L::setRegister(uint8_t reg, uint8_t value)
{

    Wire.beginTransmission(KT0803_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();

    delayMicroseconds(3000); // A little dalay
}

void KT0803L::setRFgain(uint8_t rfgain)
{
    uint8_t reg0_1, reg2, reg3;

    reg0_1 = getRegister(KT0803_REGA_01);
    reg2 = getRegister(KT0803_REGA_13);
    reg3 = getRegister(KT0803_REGA_02);

    rfgain &= 0x0F;
    reg0_1 = (reg0_1 & 0x3F) | (rfgain << 6);
    
    if (rfgain & 0x04) {
        reg2 |= 0x80;
    } else {
        reg2 &= ~0x80;
    }

    if (rfgain & 0x08) {
        reg3 |= 0x40;
    } else {
        reg3 &= ~0x40;
    }

    setRegister(KT0803_REGA_01, reg0_1);
    setRegister(KT0803_REGA_13, reg2);
    setRegister(KT0803_REGA_02, reg3);
}

void KT0803L::setPGA(PGA pga)
{
    uint8_t reg;
    uint8_t pga_val;
    Serial.print("pga:");
    Serial.println(pga,HEX);
    reg = getRegister(KT0803_REGA_01);
    
    pga_val = (uint8_t) pga;
    pga_val &= ~0xC7;
    Serial.print("pga_val:");
    Serial.println(pga_val,HEX);
    reg = (reg & 0xC7) | pga_val;
    Serial.print("reg:");
    Serial.println(reg,HEX);
    
    setRegister(KT0803_REGA_01, reg);
}

void KT0803L::setXtal(void)
{
    uint8_t reg;
    
    reg = getRegister(KT0803_REGA_1E);
    
    Serial.print("Xtal bit:");
    Serial.println(reg,HEX);
    
    setRegister(KT0803_REGA_1E, reg | 0x40);

    Serial.print("Xtal bit:");
    Serial.println(reg | 0x40,HEX);
}

void KT0803L::mute(uint8_t enable) 
{
  uint8_t r02 = getRegister(KT0803_REGA_02);

  bitWrite(r02, 3, enable == 1);

  setRegister(KT0803_REGA_02, r02); 
}

/**
 * See KT0803L datasheet, section 4.1 "Register 0x00"
 */
void  KT0803L::setFrequency(uint16_t frequency) 
{  
  currentFrequency = frequency;
  
  frequency *= 2;
  
  uint8_t r00 = getRegister(KT0803_REGA_00);
  uint8_t r01 = getRegister(KT0803_REGA_01);
  uint8_t r02 = getRegister(KT0803_REGA_02);

  bitWrite(r02, 7, bitRead(frequency, 0));

  bitWrite(r00, 0, bitRead(frequency, 1));
  bitWrite(r00, 1, bitRead(frequency, 2));
  bitWrite(r00, 2, bitRead(frequency, 3));
  bitWrite(r00, 3, bitRead(frequency, 4));
  bitWrite(r00, 4, bitRead(frequency, 5));
  bitWrite(r00, 5, bitRead(frequency, 6));
  bitWrite(r00, 6, bitRead(frequency, 7));
  bitWrite(r00, 7, bitRead(frequency, 8));

  bitWrite(r01, 0, bitRead(frequency, 9));
  bitWrite(r01, 1, bitRead(frequency, 10));
  bitWrite(r01, 2, bitRead(frequency, 11));

  setRegister(KT0803_REGA_00, r00);
  setRegister(KT0803_REGA_01, r01);
  setRegister(KT0803_REGA_02, r02);
}

char *KT0803L::convertToChar(uint16_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator, bool remove_leading_zeros)
{
    char d;
    for (int i = (len - 1); i >= 0; i--)
    {
        d = value % 10;
        value = value / 10;
        strValue[i] = d + 48;
    }
    
    strValue[len] = '\0';
    
    if (dot > 0)
    {
        for (int i = len; i >= dot; i--)
        {
            strValue[i + 1] = strValue[i];
        }
        strValue[dot] = separator;
    }

    if (remove_leading_zeros)
    {
        if (strValue[0] == '0')
        {
            strValue[0] = ' ';
            if (strValue[1] == '0')
                strValue[1] = ' ';
        }
    }

    return strValue;
}
