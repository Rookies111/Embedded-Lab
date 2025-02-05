#include "esp32-hal-gpio.h"
#include "HardwareSerial.h"
#include "Modbus_RTU.h"
#include <stdint.h>

Modbus_RTU::Modbus_RTU(int input_pin, int direct_pin, int output_pin) {
  this->input_pin = input_pin;
  this->direct_pin = direct_pin;
  this->output_pin = output_pin;
}

void Modbus_RTU::init(int baudrate) {
  pinMode(direct_pin, OUTPUT);
  Serial1.begin(baudrate, SERIAL_8N1, output_pin, input_pin);
}

int Modbus_RTU::readData(uint8_t* data) {
  unsigned long prev_time = millis();
  while (Serial1.available() == 0) {
    if (millis() - prev_time > 20000) {
      return -1;
    }
  }
  int indexMax = 0;
  prev_time = millis();
  while (Serial1.available() > 0) {
    data[indexMax++] = Serial1.read();
    prev_time = millis();
    while (Serial1.available() == 0) {
      if (millis() - prev_time > 1000) {
        break;
      }
    }
  }

  return indexMax;
}

int Modbus_RTU::request(uint8_t address, uint16_t register_addr, uint16_t size) {
  uint8_t packet[8] = {address, 0x03, register_addr >> 8, register_addr & 0xff, size >> 8, size & 0xff};
  uint16_t crc = CRC(packet, 6);
  packet[6] = crc & 0xff;
  packet[7] = crc >> 8;

  // Serial.print("Request: [");
  // for (int i = 0; i < 8; i++) {
  //   Serial.print(" ");
  //   Serial.print(packet[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println("]");

  digitalWrite(direct_pin, HIGH);
  delay(10);
  Serial1.write(packet, 8);
  delay(9);
  digitalWrite(direct_pin, LOW);

  uint8_t buff[128];
  int length = readData(buff);

  // Serial.print("Response: [");
  // for (int i = 0; i < length; i++) {
  //   Serial.print(" ");
  //   Serial.print(buff[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println("]");

  if (length <= 0) {
    Serial.println("Error -1: Request Timeout");
    return -1;
  }

  uint16_t receivedCRC = CRC(buff, length-2);

  if ((uint8_t(receivedCRC & 0xff) != buff[length-2]) || (uint8_t(receivedCRC >> 8) != buff[length-1])) {
    Serial.println("Error -2: CRC Invalid.");
    Serial.println("Calculated vs Received");
    Serial.print(receivedCRC & 0xff, HEX);
    Serial.print(" != ");
    Serial.print(buff[length-2], HEX);
    Serial.print(" or ");
    Serial.print(receivedCRC >> 8, HEX);
    Serial.print(" != ");
    Serial.println(buff[length-1], HEX);
    return -2;
  }

  int value = 0;
  int pos = 0;
  int temp = 0;
  for (int i = length-3; i > 2; i--) {
    int shiftAmount = (pos++)*8;
    temp |= (buff[i] << shiftAmount);
  }

  value = (int)temp;
  return value;
}
/*
From: http://www.ccontrolsys.com/w/How_to_Compute_the_Modbus_RTU_Message_CRC

Using the 8 character ASCII input DEADBEEF (upper case)
the checksum is 0xDD18
The code below agrees with the online calculator here:
http://www.lammertbies.nl/comm/info/crc-calculation.html

*/
uint16_t Modbus_RTU::CRC(uint8_t packet[], int len) {
  uint16_t crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++) {
    crc ^= packet[pos];  // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) {  // Loop over each bit
      if ((crc & 0x0001) != 0) {    // If the LSB is set
        crc >>= 1;                  // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else        // Else LSB is not set
        crc >>= 1;  // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;
}