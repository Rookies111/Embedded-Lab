#ifndef SPI_ADC_H
#define SPI_ADC_H
#include <Arduino.h>

class SPI_ADC {
  private:
    uint8_t cs, mosi, miso, sck;

    // SPI Bit-Banging Methods
    void spiWrite(uint8_t data);
    uint16_t spiRead();

  public:
    // Constructor
    SPI_ADC(uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck);

    // Initialize MCP3208
    void begin();

    // Read ADC value from specified channel (0-7)
    int readADC(uint8_t channel);
};

#endif
