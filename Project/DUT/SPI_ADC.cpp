#include "SPI_ADC.h"
#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>

SPIClass *spi = NULL;

SPI_ADC::SPI_ADC(int csPin, int sckPin, int misoPin, int mosiPin) {
  this->_csPin = csPin;
  this->_sckPin = sckPin;
  this->_misoPin = misoPin;
  this->_mosiPin = mosiPin;
}

void SPI_ADC::begin() {
  // Set the chip select pin as an output
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);  // Deselect the chip by default

  // Create an SPI object with the custom pins (using HSPI or VSPI)
  spi = new SPIClass(VSPI);  // Use VSPI (you can change this to HSPI if you like)

  // Initialize the SPI bus with custom pins
  spi->begin(_sckPin, _misoPin, _mosiPin, _csPin);
}

uint16_t SPI_ADC::read(uint8_t channel) {
  // Check if the channel is within valid range (0-7)
  if (channel > 7) {
    Serial.println("Error: Invalid channel. Must be between 0 and 7.");
    return 0;
  }

  // Start the SPI transaction
  digitalWrite(_csPin, LOW);

  // MCP3208 SPI command for reading an ADC channel
  // MCP3208 uses a 3-byte command for each read:
  // - Start bit (1)
  // - Channel select (3 bits)
  // - Single-ended (0) or differential (1) mode (1 bit)
  // - 12-bit result (0x0 or 0x8 for a 12-bit result)
  uint8_t command1 = 0x06 | ((channel & 0x07) >> 2);  // First byte: Start bit + channel bits
  uint8_t command2 = ((channel & 0x03) << 6);         // Second byte: Remaining channel bits
  uint8_t command3 = 0x00;                            // Third byte (empty)

  // Send the command bytes
  spi->transfer(command1);
  spi->transfer(command2);
  uint8_t highByte = spi->transfer(command3);
  uint8_t lowByte = spi->transfer(command3);

  // End the SPI transaction
  digitalWrite(_csPin, HIGH);

  // Combine the high and low byte to get the result (12-bit ADC value)
  uint16_t adcValue = ((highByte & 0x0F) << 8) | lowByte;

  return adcValue;
}
