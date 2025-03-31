#include <SPI.h>  // SPI Library
#include "I2C_Temp.h"
#include "Modbus_RTU.h"
#include "SPI_ADC.h"

// Kidbright Schematic: https://kidbright.info/files/Sch_KidBright32%20V1.3.pdf
// RS485 (Manual Control)
#define RS485_RX 19
#define RS485_TX 23
#define RS485_DE 18  // RS485 Direction Control

// I2C Configuration
#define I2C_SDA 4
#define I2C_SCL 5

// SPI Configuration
#define SPI_MISO 34
#define SPI_MOSI 35
#define SPI_SCK 32
#define SPI_SS 33

I2C_Temp I2C(I2C_SDA, I2C_SCL);
Modbus_RTU modbus(RS485_RX, RS485_DE, RS485_TX);
// SPI_ADC adc(SPI_SS, SPI_SCK, SPI_MISO, SPI_MOSI);

void setup() {
  Serial.begin(115200);  // USB Serial Debugging

  // I2C Setup
  I2C.init();
  Serial.println("I2C Initialized");

  // SPI Setup
  // adc.begin();
  // Serial.println("SPI Initialized");

  // RS485 Setup
  modbus.init(9600);
  Serial.println("RS485 Initialized");
}

// Function to simulate I2C communication
void i2c_test() {
  float temp = I2C.readTemp();
  Serial.println(temp);
}

// Function to simulate SPI communication
void spi_test() {
  // uint16_t value = adc.read(1);  // Read channel 0
  Serial.print("ADC Value: ");
  // Serial.println(value);
}

// Function to simulate RS485 communication
void rs485_test() {
  double Va = (double)modbus.request(0x08, 0x0000, 0x0002);
  if (Va >= 0)  {
    Serial.print("Voltage of A = ");
    Serial.print(Va/1000);
    Serial.println(" V");
  }
}

void loop() {
  i2c_test();
  // spi_test();
  rs485_test();

  delay(1000);
}
