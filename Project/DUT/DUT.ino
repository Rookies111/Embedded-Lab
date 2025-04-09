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

I2C_Temp I2C(I2C_SDA, I2C_SCL);
Modbus_RTU modbus(RS485_RX, RS485_DE, RS485_TX);

void setup() {
  Serial.begin(115200);  // USB Serial Debugging

  // I2C Setup
  I2C.init();
  Serial.println("I2C Initialized");

  // RS485 Setup
  modbus.init(9600);
  Serial.println("RS485 Initialized");
}

// Function to simulate I2C communication
void i2c_test() {
  float temp = I2C.readTempb();
  Serial.println(temp);
}

// Function to simulate RS485 communication
void rs485_test() {
  double Va = (double)modbus.request(0x08, 0x0000, 0x0002);
  if (Va >= 0)  {
    Serial.printf("Voltage of A = %.2f V\n", Va/1000);
  }
}

void loop() {
  i2c_test();
  rs485_test();

  delay(2000);
}
