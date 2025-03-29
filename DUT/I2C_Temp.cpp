#include <Wire.h>
#include "I2C_Temp.h"

#define LM73_ADDR 0x4D

I2C_Temp::I2C_Temp(int SDA, int SCL) {
  this->SDA = SDA;
  this->SCL = SCL;
}

void I2C_Temp::init() {
  Wire.begin(SDA, SCL);
}

//โค้ดมาจาก http://quantum.engr.tu.ac.th/smf/index.php?topic=94.0
float I2C_Temp::readTemp() {
  Wire.beginTransmission(LM73_ADDR);
  Wire.write(0x00);
  int res = Wire.endTransmission();
  if (res == 0) { return 0; }
  if (res == 1) {
    Serial.println("data too long to fit in transmit buffer.");
  } else if (res == 2) {
    Serial.println("received NACK on transmit of address.");
  } else if (res == 3) {
    Serial.println("received NACK on transmit of data.");
  } else if (res == 4) {
    Serial.println("other error. What have you done!");
  } else if (res == 5) {
    Serial.println("timeout");
  }

  uint8_t count = Wire.requestFrom(LM73_ADDR, 2);
  float temp = 0.0;
  if (count == 2) {
    byte buff[2];
    buff[0] = Wire.read();
    buff[1] = Wire.read();
    temp += (int)(buff[0] << 1);
    if (buff[1] & 0b10000000) temp += 1.0;
    if (buff[1] & 0b01000000) temp += 0.5;
    if (buff[1] & 0b00100000) temp += 0.25;
    if (buff[1] & 0b00010000) temp += 0.125;

    // Negative if first bit of buff[0] is '1'
    if (buff[0] & 0b10000000) temp *= -1.0;
  }
  return temp;
}