#ifndef I2C_SNIFFER_H
#define I2C_SNIFFER_H

#include <Arduino.h>

class I2CSniffer {
public:
    I2CSniffer(int sdaPin, int sclPin);  // Constructor
    void begin();  // Initialize pins
    bool capture(uint32_t &capturedData);  // Capture 25-bit I2C data
    void printBinary(uint32_t num, uint8_t length);  // Print binary format

private:
    int _sda, _scl;
    bool waitForStartCondition();  // Detect I2C start condition
};

#endif
