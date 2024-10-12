#include<LiquidCrystal_I2C.h>

#include "KT0803L.h"

#define IO_PIN        33
#define RST_PIN       4
#define SDA           21
#define SCL           22

uint16_t frequency = 1020;       // 102.0 Mhz
uint16_t sensorValue = 4095;     // variable for storing value from sensor


LiquidCrystal_I2C lcd(0x27, 16, 2);


KT0803L fmtx(SDA, SCL, 400000);

void IRAM_ATTR isr()
{
  
  sensorValue = analogRead(IO_PIN);
  Serial.println("isr triggered");
 
}

void setup(void)
{
  char float_buff[16];

  pinMode(RST_PIN,OUTPUT);
  pinMode(IO_PIN, INPUT);
  
  /* Reset transmitter */
  digitalWrite(RST_PIN,LOW);
  delay(100);
  digitalWrite(RST_PIN,HIGH);
  
  Serial.begin(115200);

//  Wire.begin();

//  attachInterrupt(IO_PIN, isr, RISING);

  // Start LCD display device
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  
  printRegisters();

  /* Set default frequency */
  fmtx.setFrequency(frequency);

  Serial.println("Mute disable");
  fmtx.mute(0);

  Serial.println("RF gain PGA_12DB (12dB Max)");
  fmtx.setRFgain(PGA_12DB);
  
  Serial.println("Set PGA");
  fmtx.setPGA(PGA_0DB);

  fmtx.setXtal();

  showDisplay();  
}


void printRegisters(void)
{
  uint8_t data;
    
  data = fmtx.getRegister(KT0803_REGA_00);
  Serial.print("Register ");
  Serial.print(KT0803_REGA_00,HEX);
  Serial.print(" : ");
  Serial.println(data,HEX);

  data = fmtx.getRegister(KT0803_REGA_01);
  Serial.print("Register ");
  Serial.print(KT0803_REGA_01,HEX);
  Serial.print(" : ");
  Serial.println(data,HEX);

  data = fmtx.getRegister(KT0803_REGA_02);
  Serial.print("Register ");
  Serial.print(KT0803_REGA_02,HEX);
  Serial.print(" : ");
  Serial.println(data,HEX);

  data = fmtx.getRegister(KT0803_REGA_04);
  Serial.print("Register ");
  Serial.print(KT0803_REGA_04,HEX);
  Serial.print(" : ");
  Serial.println(data,HEX);

  data = fmtx.getRegister(KT0803_REGA_0B);
  Serial.print("Register ");
  Serial.print(KT0803_REGA_0B,HEX);
  Serial.print(" : ");
  Serial.println(data,HEX);
}

void showDisplay(void) 
{  
  lcd.setCursor(1, 0);
  lcd.print("FM Transmitter");
  lcd.setCursor(4,1);
  lcd.print(fmtx.formatCurrentFrequency('.'));
//  lcd.print(fmtx.convertToChar(fmtx.getFreq(), strValue, 5, 4, '.', true));
  lcd.print("Mhz");
}

void loop(void)
{  
  sensorValue = analogRead(IO_PIN);
  
  if (sensorValue < 300) {
      fmtx.setFrequency(fmtx.getFrequency() + 1);
      showDisplay();
  } else if (sensorValue < 1000) {
      fmtx.setFrequency(fmtx.getFrequency() - 1);
      showDisplay();
  }
  
  delay(100);
}
