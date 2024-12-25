#include <Wire.h>

#define I2C_SDA 4
#define I2C_SCL 5
#define I2C_Address 0x6F

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(115200);
  Wire.beginTransmission(I2C_Address);
  Wire.write(0x00); // 0b1101 1110
  Wire.write(1 << 7); // 0b1101 1110
  Wire.endTransmission();
}

void loop() {
  Wire.beginTransmission(I2C_Address);
  Wire.write(0x00); // 0b1101 1110
  int res = Wire.endTransmission();
  // Serial.println(res);

  uint8_t count = Wire.requestFrom(I2C_Address, 1);
  while (Wire.available()) {
    byte buff = Wire.read();
    // buff[1] = Wire.read();
    Serial.println(buff & (128-1), BIN);
    // Serial.println(buff[1], BIN);
  }

}

void getTime() {

}
