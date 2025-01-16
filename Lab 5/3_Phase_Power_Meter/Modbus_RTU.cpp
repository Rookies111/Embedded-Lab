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

responseFrame Modbus_RTU::request(uint8_t address, uint8_t function) {
  return responseFrame(0x08, 0x03, 0x02, {0x01, 0x01}, 0x0101);
}
/*
From: http://www.ccontrolsys.com/w/How_to_Compute_the_Modbus_RTU_Message_CRC

Using the 8 character ASCII input DEADBEEF (upper case)
the checksum is 0xDD18
The code below agrees with the online calculator here:
http://www.lammertbies.nl/comm/info/crc-calculation.html

*/
uint16_t Modbus_RTU::CRC(uint16_t buf, int len) {
  uint16_t crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++) {
    int shift_amount = pos*8;
    crc ^= (buf & (0xFF << shift_amount)) >> shift_amount;  // XOR byte into least sig. byte of crc

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