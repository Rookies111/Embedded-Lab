#include <Wire.h>  // I2C Library
#include <SPI.h>   // SPI Library
#include "I2C_Temp.h"

// RS485 (Manual Control)
#define RS485_RX 16
#define RS485_TX 17
#define RS485_DE 21  // RS485 Direction Control

// I2C Configuration
#define I2C_SDA 4
#define I2C_SCL 5

// SPI Configuration
#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_SCK 18
#define SPI_SS 22

I2C_Temp I2C(I2C_SDA, I2C_SCL);

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
  Serial2.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);
  pinMode(RS485_DE, OUTPUT);
  digitalWrite(RS485_DE, LOW);  // Receiver mode
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
  digitalWrite(RS485_DE, HIGH);  // Enable TX
  Serial2.write("Hello RS485");
  delay(10);
  digitalWrite(RS485_DE, LOW);  // Enable RX

  delay(100);
  if (Serial2.available()) {
    String received = Serial2.readString();
    Serial.print("RS485 Received: ");
    Serial.println(received);
  }
}

void loop() {
  i2c_test();
  spi_test();
  rs485_test();

  delay(1000);
}
