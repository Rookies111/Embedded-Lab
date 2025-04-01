#include "RS485_Sniffer.h"

RS485_Sniffer::RS485_Sniffer(uint8_t dirPin, uint8_t rxPin, uint8_t txPin, long baudRate)
  : _dirPin(dirPin), _rxPin(rxPin), _txPin(txPin), _baudRate(baudRate),
    rxIndex(0), txIndex(0), lastRxReceived(0), lastTxReceived(0) {}

void RS485_Sniffer::begin() {
  Serial2.begin(_baudRate, SERIAL_8N1, _rxPin, _txPin);  // RX UART
  Serial1.begin(_baudRate, SERIAL_8N1, _txPin, -1);      // TX UART

  pinMode(_dirPin, OUTPUT);
  digitalWrite(_dirPin, LOW);  // Always in receive mode
}

bool RS485_Sniffer::captureRXFrame(uint8_t *buffer, int &length) {
  while (Serial2.available()) {
    rxBuffer[rxIndex++] = Serial2.read();
    lastRxReceived = millis();

    if (rxIndex >= sizeof(rxBuffer)) {
      rxIndex = 0;
    }

    delayMicroseconds(100);
    yield();
  }

  if (rxIndex > 0 && (millis() - lastRxReceived) > FRAME_TIMEOUT) {
    memcpy(buffer, rxBuffer, rxIndex);
    length = rxIndex;
    rxIndex = 0;
    return true;
  }

  return false;
}

bool RS485_Sniffer::captureTXFrame(uint8_t *buffer, int &length) {
  while (Serial1.available()) {
    txBuffer[txIndex++] = Serial1.read();
    lastTxReceived = millis();

    if (txIndex >= sizeof(txBuffer)) {
      txIndex = 0;
    }

    delayMicroseconds(100);
    yield();
  }

  if (txIndex > 0 && (millis() - lastTxReceived) > FRAME_TIMEOUT) {
    memcpy(buffer, txBuffer, txIndex);
    length = txIndex;
    txIndex = 0;
    return true;
  }

  return false;
}

void RS485_Sniffer::printBinary(uint8_t *buffer, int length) {
  for (int i = 0; i < length; i++) {
    for (int j = 7; j >= 0; j--) {
      Serial.print((buffer[i] >> j) & 1 ? '1' : '0');
    }
    Serial.print(" ");  // Space between bytes
  }
  Serial.println();
}
