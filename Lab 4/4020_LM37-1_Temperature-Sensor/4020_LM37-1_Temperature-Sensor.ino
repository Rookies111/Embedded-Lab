#include <Wire.h> 

#define LM73_ADDR 0x4D
#define SEND_DELAY 100

String response;
float data;

float readTemperature();

void setup() {
  Serial.begin(115200);  
  Wire.begin(4, 5);
}

void loop() {
  response = "";
  data = readTemperature();

  response = String(data);
  Serial.print("Current Temperature: ");
  Serial.print(response);
  Serial.println(" C");

  delay(SEND_DELAY);
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