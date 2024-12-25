#include <Wire.h> 

#define LM73_ADDR 0x4D
#define SEND_DELAY 1000

String response;
float data;

float readTemperature();

void setup() {
  Serial.begin(9600);  
  Wire.begin(4, 5);
  setResolution(0b11);
}

void loop() {
  if (Serial.available()) {
    manageCommand(Serial.readString());
  }

  response = "";
  data = readTemperature();
  response = String(data);
  Serial.print("Current Temperature: ");
  Serial.print(response);
  Serial.println(" C");
  
  delay(SEND_DELAY);
}

int splitString(String input, String delimiter, String output[], int maxParts) {
  int startIndex = 0;
  int endIndex = input.indexOf(delimiter);
  int partCount = 0;

  while (endIndex != -1 && partCount < maxParts) {
    output[partCount++] = input.substring(startIndex, endIndex); // Extract part
    startIndex = endIndex + delimiter.length();
    endIndex = input.indexOf(delimiter, startIndex); // Find next delimiter
  }

  if (partCount < maxParts) {
    output[partCount++] = input.substring(startIndex); // Add the last part
  }

  return partCount; // Return the number of parts
}

void manageCommand(String input) {
  String output[3];
  int count = splitString(input, " ", output, 3);
  if (output[0].equals("resolution_number")) {
    if (output[1].equals("=")) {
      int value = output[2].toInt();
      if (value < 0 || value > 3) {
        Serial.print("Invailid Resolution Number: ");
        Serial.println(value, int);
        return;
      }

      setResolution(value);
    }
  }
}

void setResolution(uint8_t resolution) {
  Serial.print("Received: ");
  Serial.println(resolution, BIN);

  Wire.beginTransmission(LM73_ADDR);
  Wire.write(0x04);
  int res = Wire.endTransmission();
  if (res != 0) {
    switch (res) {
      case 1:
        Serial.println("data too long to fit in transmit buffer.");
        break;
      case 2:
        Serial.println("received NACK on transmit of address.");
        break;
      case 3:
        Serial.println("received NACK on transmit of data.");
        break;
      case 4:
        Serial.println("other error. What have you done!");
        break;
      case 5:
        Serial.println("timeout");
        break;
    }
  }

  uint8_t count = Wire.requestFrom(LM73_ADDR, 1);

  if (count != 1) return;

  byte buff[1];
  buff[0] = Wire.read();

  uint8_t extractedResolution = resolution & 0b11;
  extractedResolution <<= 5;
  buff[0] &= ~(0b11 << 5);
  buff[0] |= extractedResolution;

  Serial.print("Control/Status Byte: ");
  Serial.println(buff[0], BIN);

  Wire.beginTransmission(LM73_ADDR);
  Wire.write(0x04);
  Wire.write(buff[0]);
  res = Wire.endTransmission();
  if (res != 0) {
    switch (res) {
      case 1:
        Serial.println("data too long to fit in transmit buffer.");
        break;
      case 2:
        Serial.println("received NACK on transmit of address.");
        break;
      case 3:
        Serial.println("received NACK on transmit of data.");
        break;
      case 4:
        Serial.println("other error. What have you done!");
        break;
      case 5:
        Serial.println("timeout");
        break;
    }
  }
}

//โค้ดมาจาก http://quantum.engr.tu.ac.th/smf/index.php?topic=94.0
float readTemperature() {
  Wire.beginTransmission(LM73_ADDR);
  Wire.write(0x00);
  int res = Wire.endTransmission();
  if (res != 0) {
    switch (res) {
      case 1:
        Serial.println("data too long to fit in transmit buffer.");
        return 0;
      case 2:
        Serial.println("received NACK on transmit of address.");
        return 0;
      case 3:
        Serial.println("received NACK on transmit of data.");
        return 0;
      case 4:
        Serial.println("other error. What have you done!");
        return 0;
      case 5:
        Serial.println("timeout");
        return 0;
    }
  }
 
  uint8_t count = Wire.requestFrom(LM73_ADDR, 2);
  float temp = 0.0;
  if (count == 2) {
    byte buff[2];
    buff[0] = Wire.read();
    buff[1] = Wire.read();
    Serial.println(buff[0], BIN);
    Serial.println(buff[1], BIN);
    temp += (int)(buff[0] << 1);
    if (buff[1]&0b10000000) temp += 1.0;
    if (buff[1]&0b01000000) temp += 0.5;
    if (buff[1]&0b00100000) temp += 0.25;
    if (buff[1]&0b00010000) temp += 0.125;

    // Negative if first bit of buff[0] is '1'
    if (buff[0]&0b10000000) temp *= -1.0; 
  }
  return temp;
}