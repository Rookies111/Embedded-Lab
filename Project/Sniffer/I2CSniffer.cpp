#include "I2CSniffer.h"

I2CSniffer::I2CSniffer(int sdaPin, int sclPin) {
  _sda = sdaPin;
  _scl = sclPin;
}

void I2CSniffer::begin() {
  pinMode(_sda, INPUT_PULLUP);
  pinMode(_scl, INPUT_PULLUP);
  // Serial.println("I2C Sniffer Initialized.");
}

bool I2CSniffer::waitForStartCondition() {
  while (true) {
    if (digitalRead(_sda) == 0 && digitalRead(_scl) == 1) {
      delayMicroseconds(5);
      if (digitalRead(_sda) == 0 && digitalRead(_scl) == 1) {
        return true;
      }
    }
  }
}

bool I2CSniffer::capture(uint32_t &capturedData) {
  uint8_t bitCount = 1;
  capturedData = 0;

  // Wait for I2C Start Condition
  if (!waitForStartCondition()) return false;

  // Serial.println("I2C Start Condition Detected!");

  // Store the Start Bit (SDA=0)
  capturedData = 0;

  // Wait for the first clock pulse
  while (digitalRead(_scl) == 1)
    ;

  // Capture 24 bits (Start Bit + Address + Data)
  while (bitCount < 25) {
    while (digitalRead(_scl) == 0)
      ;
    delayMicroseconds(2);

    int sdaValue = digitalRead(_sda);
    capturedData = (capturedData << 1) | (sdaValue & 1);
    bitCount++;

    while (digitalRead(_scl) == 1)
      ;
  }

  return true;
}

void I2CSniffer::printBinary(uint32_t num, uint8_t length) {
  for (int i = length - 1; i >= 0; i--) {
    Serial.print((num >> i) & 1);
  }
}
