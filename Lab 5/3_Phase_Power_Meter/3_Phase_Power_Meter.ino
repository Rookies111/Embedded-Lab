#include "Modbus_RTU.h"

Modbus_RTU modbus(19, 18, 23);

void setup() {
  Serial.begin(115200);
  Serial.print("CRC: ");
  modbus.init(9600);
}

void loop() {
  Serial.println("---------------------------------------------------");
  // Request Voltage of A
  double Va = (double)modbus.request(0x08, 0x0000, 0x0002);
  if (Va >= 0)  {
    Serial.print("Voltage of A = ");
    Serial.print(Va/1000);
    Serial.println(" V");
  }

  // Request Current of A
  double Ia = (double)modbus.request(0x08, 0x000C, 0x0002);
  if (Ia >= 0)  {
    Serial.print("Current of A = ");
    Serial.print(Ia/1000);
    Serial.println(" A");
  }

  // Request Power of A
  double Pa = (double)modbus.request(0x08, 0x0012, 0x0002);
  if (Pa >= 0) {
    Serial.print("Power of A = ");
    Serial.print(Pa);
    Serial.println(" W");
  }

  // Request Power Factor of A
  double PFa = (double)modbus.request(0x08, 0x0022, 0x0002);
  if (PFa >= 0) {
    Serial.print("Power Factor of A = ");
    Serial.println(PFa/1000);
  }

  // Request Frequency
  double freq = (double)modbus.request(0x08, 0x0032, 0x0002);
  if (freq >= 0) {
    Serial.print("Frequency = ");
    Serial.print(freq/1000);
    Serial.println(" Hz");
  }
}
