#include "Modbus_RTU.h"

class arrReturn{
  uint8_t arr[20];
  int len;
};

Modbus_RTU modbus(19, 18, 23);

void setup() {
  Serial.begin(115200);
  Serial.print("CRC: ");
  Serial.println(modbus.CRC(0x0F0F, 2), HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
