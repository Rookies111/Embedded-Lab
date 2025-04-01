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

int Modbus_RTU::request(uint8_t address, uint16_t register_addr, uint16_t size) {
  uint8_t packet[8] = {address, 0x03, register_addr >> 8, register_addr & 0xff, size >> 8, size & 0xff};
  uint16_t crc = CRC(packet, 6);
  packet[6] = crc & 0xff;
  packet[7] = crc >> 8;

  digitalWrite(direct_pin, HIGH);
  delayMicroseconds(500);  // Adjusted for proper timing
  Serial1.write(packet, 8);
  Serial1.flush();  // Ensure the transmission is complete
  delayMicroseconds(500);
  digitalWrite(direct_pin, LOW);

  uint8_t buff[128];
  int length = readData(buff);

  if (length <= 0) {
    Serial.println("Error -1: Request Timeout");
    return -1;
  }

  uint16_t receivedCRC = CRC(buff, length - 2);
  if ((uint8_t(receivedCRC & 0xff) != buff[length - 2]) || (uint8_t(receivedCRC >> 8) != buff[length - 1])) {
    Serial.println("Error -2: CRC Invalid.");
    return -2;
  }

  int value = 0;
  for (int i = 3; i < length - 2; i++) {
    value = (value << 8) | buff[i];
  }

  return value;
}

int Modbus_RTU::readData(uint8_t* data) {
  unsigned long start_time = millis();
  int index = 0;

  while (millis() - start_time < 1000) {  // 1-second timeout
    while (Serial1.available()) {
      data[index++] = Serial1.read();
      start_time = millis();  // Reset timeout on data reception
      if (index >= 128) break;  // Prevent buffer overflow
    }
  }

  return (index > 0) ? index : -1;
}

uint16_t Modbus_RTU::CRC(uint8_t packet[], int len) {
  uint16_t crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++) {
    crc ^= packet[pos];
    for (int i = 8; i != 0; i--) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}
