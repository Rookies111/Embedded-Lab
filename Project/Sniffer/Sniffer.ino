#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include "I2CSniffer.h"
#include "RS485_Sniffer.h"

// I2C pin configuration
#define I2C_SDA 16
#define I2C_SCL 17

// RS485(Modbus) pin configuration
#define RS485_RX 14
#define RS485_TX 13
#define RS485_DE 12
#define BAUDRATE 9600

I2CSniffer I2C_sniffer(I2C_SDA, I2C_SCL);
RS485_Sniffer RS485_sniffer(RS485_DE, RS485_RX, RS485_TX, BAUDRATE);

bool isCaptureI2C = true;
bool isCaptureRS485 = false;

// Function to convert bytes to binary string
String bytesToBinaryString(uint8_t* buffer, int length) {
  String binaryString = "";
  for (int i = 0; i < length; i++) {
    for (int j = 7; j >= 0; j--) {
      binaryString += ((buffer[i] >> j) & 1) ? '1' : '0';
    }
  }
  return binaryString;
}

void setup() {
  Serial.begin(115200);
  I2C_sniffer.begin();
  RS485_sniffer.begin();
  Serial.println("Start sniffing...");
}

void loop() {
  uint32_t dataBuffer;

  uint8_t rxData[64], txData[64];
  int rxLen = 0, txLen = 0;

  JsonDocument msg;

  if (Serial.available()) {
    DeserializationError error = deserializeJson(msg, Serial.readString());

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    const char* msgType = msg["type"];
    const char* msgData = msg["data"];

    if (msgType == "I2C" && msgData == "Enable") {
      isCaptureI2C = true;
      isCaptureRS485 = false;
    } else if (msgType == "Modbus" && msgData == "Enable") {
      isCaptureI2C = false;
      isCaptureRS485 = true;
    } else if (msgType == "Keep Alive") {
      Serial.write("pong");
    }
  }

  if (isCaptureRS485) {
    String txBinary = "", rxBinary = "";
    bool txCaptured = false, rxCaptured = false;
    unsigned long startTime = millis();

    // Wait up to 50ms to capture both TX and RX
    while (millis() - startTime < 50) {
      if (!txCaptured && RS485_sniffer.captureTXFrame(txData, txLen)) {
        txBinary = bytesToBinaryString(txData, txLen);
        txCaptured = true;
      }
      if (!rxCaptured && RS485_sniffer.captureRXFrame(rxData, rxLen)) {
        rxBinary = bytesToBinaryString(rxData, rxLen);
        rxCaptured = true;
      }

      if (txCaptured && rxCaptured) break;  // Exit loop early if both are received
    }

    // Ensure at least one frame is captured before sending
    if (txCaptured && rxCaptured) {
      // Construct full JSON message before sending
      String jsonString = "{\"type\":\"Modbus\", \"data\":\"" + txBinary + ";" + rxBinary + "\"}\n";

      // Send as a single write operation to prevent splitting
      Serial.print(jsonString);
      Serial.flush();  // Ensure data is sent before the next loop iteration
    }

    delay(1);
  }

  if (isCaptureI2C) {
    if (I2C_sniffer.capture(dataBuffer)) {
      Serial.print("{\"type\": \"I2C\", \"data\": \"");

      // Convert captured data to binary string
      for (int i = 24; i >= 0; i--) {
        Serial.print((dataBuffer >> i) & 1);
      }

      Serial.println("\"}");
    }

    delay(300);
  }
}
