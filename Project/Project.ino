#include <Wire.h>
#include <SPI.h>

// I2C Protocol Pin Configuration
#define SDA_PIN 21
#define SCL_PIN 22

// RS-485(Modbus) Protocol Pin Configuration
#define IN_PIN 19
#define OUT_PIN 23
#define DIRECT_PIN 18

// SPI Protocol Pin Configuration
#define SCLK_PIN 20
#define MOSI_PIN 20
#define MISO_PIN 20
#define SS_PIN 20

// Set communication default flag
volatile bool i2c_started = false;
volatile bool spi_started = false;
volatile bool rs485_started = false;

void IRAM_ATTR i2c_interrupt() {
  // SDA goes LOW while SCL is HIGH → START condition detected
  if (digitalRead(SDA_PIN) == LOW && digitalRead(SCL_PIN) == HIGH) {
    i2c_started = true;
  }
}

void IRAM_ATTR spi_interrupt() {
  spi_started = true;  // Set flag when clock toggles
}

void IRAM_ATTR rs485_interrupt() {
  rs485_started = true;  // Set flag when data is received
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 18, 19);
  Serial2.begin(115200, SERIAL_8N1, IN_PIN, OUT_PIN);
  Wire.begin(SDA_PIN, SCL_PIN);
  // SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);

  // pinMode(SCLK_PIN, INPUT_PULLUP);

  pinMode(DIRECT_PIN, OUTPUT);
  digitalWrite(DIRECT_PIN, LOW);  // Set to receive mode

  attachInterrupt(digitalPinToInterrupt(SDA_PIN), i2c_interrupt, FALLING);   // Detect start bit on SDA and SCL
  // attachInterrupt(digitalPinToInterrupt(SCLK_PIN), spi_interrupt, RISING);   // Detect rising clock edge
  attachInterrupt(digitalPinToInterrupt(IN_PIN), rs485_interrupt, FALLING);  // Detect incoming data
  
  Serial.println("Monitoring I2C communication...");
  // Serial.println("Monitoring SPI communication...");
  Serial.println("Monitoring RS485 communication...");
}

void loop() {
  if (i2c_started) {
    Serial.println("I2C Communication Started!");
    i2c_started = false;  // Reset flag
  }

  if (spi_started) {
    Serial.println("SPI Communication Started!");
    spi_started = false;  // Reset flag
  }

  if (rs485_started) {
    Serial.println("RS485 (Modbus) Communication Started!");
    rs485_started = false;  // Reset flag
  }
}
