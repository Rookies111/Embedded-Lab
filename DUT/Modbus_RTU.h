#ifndef Modbus_RTU_H
#define Modbus_RTU_H
#include <stdint.h>
#include <Arduino.h>

class Modbus_RTU {
  private:
    int RX_PIN, DE_PIN, TX_PIN;
    int readData(uint8_t* data);
    uint16_t CRC(uint8_t buf[], int len);

  public:
    Modbus_RTU(int RX_PIN, int DE_pin, int TX_pin);
    void init(int baudrate);
    int request(uint8_t address, uint16_t register_addr, uint16_t len);
};
#endif