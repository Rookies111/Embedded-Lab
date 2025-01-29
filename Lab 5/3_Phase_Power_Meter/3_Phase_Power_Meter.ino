#include "Modbus_RTU.h"

class arrReturn{
  uint8_t arr[20];
  int len;
};

Modbus_RTU modbus(19, 18, 23);

void setup() {
  Serial.begin(115200);
  Serial.print("CRC: ");
  modbus.init(9600);
  modbus.request(0x08, 0x0000, 0x0002);
  // Serial.println(modbus.CRC(0x0F), HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
