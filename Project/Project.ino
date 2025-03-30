#include <Wire.h>
#include <SPI.h>

// I2C Protocol Pin Configuration
#define I2C_SDA 21
#define I2C_SCL 22

// RS-485(Modbus) Protocol Pin Configuration
#define RS485_RX 16
#define RS485_TX 17

// SPI Protocol Pin Configuration
#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_SCK 18
#define SPI_CS 5

#define SAMPLE_COUNT 32
#define SAMPLE_DELAY 5   // Delay between samples (microseconds)

// Set communication default flag
volatile bool i2c_started = false;
volatile bool spi_started = false;
volatile bool rs485_started = false;

char sdaSeq[SAMPLE_COUNT + 1];
char sclSeq[SAMPLE_COUNT + 1];

void captureSignal(int pin, char *buffer, int length) {
    for (int i = 0; i < length; i++) {
        buffer[i] = digitalRead(pin) ? '1' : '0';
        delayMicroseconds(SAMPLE_DELAY);
    }
    buffer[length] = '\0';  // Null-terminate the string
}

void IRAM_ATTR i2c_interrupt() {
  // SDA goes LOW while SCL is HIGH → START condition detected
  if (digitalRead(I2C_SDA) == LOW && digitalRead(I2C_SCL) == HIGH) {
    i2c_started = true;
    captureSignal(I2C_SDA, sdaSeq, 16);
    captureSignal(I2C_SCL, sclSeq, 16);
    Serial.print("SDA  ="); Serial.println(sdaSeq);
    Serial.print("SCL  ="); Serial.println(sclSeq);
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
  // Serial1.begin(115200, SERIAL_8N1, 18, 19);
  // Serial2.begin(115200, SERIAL_8N1, IN_PIN, OUT_PIN);
  Wire.begin(I2C_SDA, I2C_SCL);
  // SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

  // Set all pins as INPUT
  pinMode(I2C_SDA, INPUT_PULLUP);
  pinMode(I2C_SCL, INPUT_PULLUP);
  pinMode(SPI_MISO, INPUT);
  pinMode(SPI_MOSI, INPUT);
  pinMode(SPI_SCK, INPUT);
  pinMode(SPI_CS, INPUT);
  pinMode(RS485_RX, INPUT);
  pinMode(RS485_TX, INPUT);

  // pinMode(SDA_PIN, INPUT_PULLUP);
  // pinMode(SCL_PIN, INPUT_PULLUP);

  // pinMode(SCLK_PIN, INPUT_PULLUP);

  // pinMode(DIRECT_PIN, OUTPUT);
  // digitalWrite(DIRECT_PIN, LOW);  // Set to receive mode

  attachInterrupt(digitalPinToInterrupt(I2C_SDA), i2c_interrupt, FALLING);  // Detect start bit on SDA and SCL
  // attachInterrupt(digitalPinToInterrupt(SCLK_PIN), spi_interrupt, RISING);   // Detect rising clock edge
  // attachInterrupt(digitalPinToInterrupt(IN_PIN), rs485_interrupt, FALLING);  // Detect incoming data

  Serial.println("Monitoring I2C communication...");
  // Serial.println("Monitoring SPI communication...");
  // Serial.println("Monitoring RS485 communication...");
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
  delay(1000);
}
