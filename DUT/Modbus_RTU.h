#ifndef Modbus_RTU_H
#define Modbus_RTU_H
#include <stdint.h>
#include <Arduino.h>

class Modbus_RTU {
  private:
    int input_pin;
    int direct_pin;
    int output_pin;
    int readData(uint8_t* data);
    uint16_t CRC(uint8_t buf[], int len);

  public:
    Modbus_RTU(int input_pin, int direct_pin, int output_pin);
    void init(int baudrate);
    int request(uint8_t address, uint16_t register_addr, uint16_t len);
};
#endif