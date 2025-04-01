#ifndef SPI_ADC_H
#define SPI_ADC_H

#include <Arduino.h>
#include <SPI.h>

class SPI_ADC {
  public:
    SPI_ADC(int csPin, int sckPin, int misoPin, int mosiPin);  // Constructor declaration

    void begin();  // Initialize the SPI bus with custom pins
    uint16_t read(uint8_t channel);  // Read ADC value from the specified channel

  private:
    int _csPin, _sckPin, _misoPin, _mosiPin;
    SPIClass *spi;  // Use SPIClass pointer to initialize the custom SPI bus
};

#endif
