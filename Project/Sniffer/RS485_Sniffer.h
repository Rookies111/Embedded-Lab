#ifndef RS485_SNIFFER_H
#define RS485_SNIFFER_H

#include <Arduino.h>

#define FRAME_TIMEOUT 5  // Modbus RTU timeout in ms

class RS485_Sniffer {
public:
  RS485_Sniffer(uint8_t dirPin, uint8_t rxPin, uint8_t txPin, long baudRate = 9600);

  void begin();
  bool captureRXFrame(uint8_t *buffer, int &length);
  bool captureTXFrame(uint8_t *buffer, int &length);
  void printBinary(uint8_t *buffer, int length);  // New function

private:
  uint8_t _dirPin, _rxPin, _txPin;
  long _baudRate;

  uint8_t rxBuffer[64];
  uint8_t txBuffer[64];
  int rxIndex, txIndex;
  unsigned long lastRxReceived, lastTxReceived;
};

#endif  // RS485_SNIFFER_H
