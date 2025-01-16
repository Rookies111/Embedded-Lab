#ifndef Modbus_RTU_H
#define Modbus_RTU_H
#include <stdint.h>
#include <Arduino.h>

class responseFrame {
  uint8_t address;
  uint8_t funcCode;
  uint8_t count;
  uint8_t data[50];
  uint16_t crc;
};

class Modbus_RTU {
private:
  int input_pin;
  int direct_pin;
  int output_pin;

public:
  Modbus_RTU(int input_pin, int direct_pin, int output_pin);
  void init(int baudrate);
  responseFrame request(uint8_t address, uint8_t function);
  // void send(uint8_t address, uint8_t function);
  uint16_t CRC(uint16_t buf, int len);
};
#endif