#include <Wire.h>  // I2C Library
#include <SPI.h>   // SPI Library
#include "I2C_Temp.h"
#include "Modbus_RTU.h"

// RS485 (Manual Control)
#define RS485_RX 19
#define RS485_TX 23
#define RS485_DE 18  // RS485 Direction Control

// I2C Configuration
#define I2C_SDA 4
#define I2C_SCL 5

// SPI Configuration
#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_SCK 18
#define SPI_SS 22

I2C_Temp I2C(I2C_SDA, I2C_SCL);
Modbus_RTU modbus(RS485_RX, RS485_DE, RS485_TX);

// Test buffer for SPI
byte spi_test_data = 0x55;

void setup() {
  Serial.begin(115200);  // USB Serial Debugging

  // I2C Setup
  I2C.init();
  Serial.println("I2C Initialized");

  // SPI Setup
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_SS);
  pinMode(SPI_SS, OUTPUT);
  digitalWrite(SPI_SS, HIGH);
  Serial.println("SPI Initialized");

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
  digitalWrite(SPI_SS, LOW);
  byte response = SPI.transfer(spi_test_data);
  digitalWrite(SPI_SS, HIGH);
  Serial.print("SPI Test Response: 0x");
  Serial.println(response, HEX);
}

// Function to simulate RS485 communication
void rs485_test() {
  // Request Voltage of A
  double Va = (double)modbus.request(0x08, 0x0000, 0x0002);
  if (Va >= 0)  {
    Serial.print("Voltage of A = ");
    Serial.print(Va/1000);
    Serial.println(" V");
  }
}

void loop() {
  i2c_test();
  spi_test();
  rs485_test();

  delay(1000);
}
