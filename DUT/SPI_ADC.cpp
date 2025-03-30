#include "SPI_ADC.h"

// Constructor
SPI_ADC::SPI_ADC(uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck) {
  this->cs = cs;
  this->mosi = mosi;
  this->miso = miso;
  this->sck = sck;
}

// Initialize SPI Pins
void SPI_ADC::begin() {
  pinMode(cs, OUTPUT);
  pinMode(mosi, OUTPUT);
  pinMode(miso, INPUT);
  pinMode(sck, OUTPUT);

  digitalWrite(cs, HIGH);  // Deselect SPI_ADC
}

// SPI Write Function (Bit-Banging)
void SPI_ADC::spiWrite(uint8_t data) {
  for (int i = 7; i >= 0; i--) {
    digitalWrite(mosi, (data >> i) & 0x01);
    digitalWrite(sck, HIGH);
    delayMicroseconds(1);
    digitalWrite(sck, LOW);
  }
}

// SPI Read Function (Bit-Banging)
uint16_t SPI_ADC::spiRead() {
  uint16_t value = 0;
  for (int i = 11; i >= 0; i--) {
    digitalWrite(sck, HIGH);
    delayMicroseconds(1);
    if (digitalRead(miso)) {
      value |= (1 << i);
    }
    digitalWrite(sck, LOW);
  }
  return value;
}

// Read ADC Value
int SPI_ADC::readADC(uint8_t channel) {
  if (channel > 7) return -1;  // Invalid channel

  digitalWrite(cs, LOW);  // Select SPI_ADC

  // Start bit (1), Single-Ended (1), and Channel Bits
  uint8_t command = (0x18 | channel) << 3;  // 0b11000 (Start + SGL)
  spiWrite(command);

  // Read 12-bit ADC Data
  uint16_t adc_value = spiRead();

  digitalWrite(cs, HIGH);  // Deselect SPI_ADC
  return adc_value;
}
