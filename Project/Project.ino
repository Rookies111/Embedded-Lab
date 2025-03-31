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

// Set communication default flag
volatile bool i2c_started = false;
volatile bool spi_started = false;
volatile bool rs485_started = false;

char sdaSeq[SAMPLE_COUNT + 1];
char sclSeq[SAMPLE_COUNT + 1];

// Variables to store last states of pins (for edge detection)
static int lastSCL = LOW;
static int lastSDA = LOW;
static int lastSPI_SCK = LOW;
static int lastRS485_RX = LOW;

// Debounce delay
#define DEBOUNCE_DELAY 50  // 50 milliseconds debounce time

// Function to capture I2C data
void captureI2C(int sdaPin, int sclPin, char *buffer, int maxBits) {
  int lastSCL = digitalRead(sclPin);
  int index = 0;

  while (index < maxBits) {
    int currentSCL = digitalRead(sclPin);

    // Detect rising edge of SCL
    if (lastSCL == LOW && currentSCL == HIGH) {
      buffer[index++] = digitalRead(sdaPin) ? '1' : '0';
    }

    lastSCL = currentSCL;  // Update previous clock state
  }

  buffer[maxBits] = '\0';  // Null-terminate string
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  // Set all pins as INPUT
  pinMode(I2C_SDA, INPUT_PULLUP);
  pinMode(I2C_SCL, INPUT_PULLUP);
  pinMode(SPI_MISO, INPUT);
  pinMode(SPI_MOSI, INPUT);
  pinMode(SPI_SCK, INPUT);
  pinMode(SPI_CS, INPUT);
  pinMode(RS485_RX, INPUT);
  pinMode(RS485_TX, INPUT);

  // Start polling I2C, SPI, and RS485 communication
  Serial.println("Monitoring I2C, SPI, and RS485 communication...");
}

void loop() {
  // Poll I2C (checking for start condition)
  int currentSCL = digitalRead(I2C_SCL);
  int currentSDA = digitalRead(I2C_SDA);

  // Check for I2C start condition (SDA falling while SCL is high)
  if (lastSCL == HIGH && lastSDA == HIGH && currentSDA == LOW) {
    i2c_started = true;
    captureI2C(I2C_SDA, I2C_SCL, sdaSeq, SAMPLE_COUNT);
    captureI2C(I2C_SCL, I2C_SCL, sclSeq, SAMPLE_COUNT);
    Serial.print("SDA = ");
    Serial.println(sdaSeq);
    Serial.print("SCL = ");
    Serial.println(sclSeq);
    delay(DEBOUNCE_DELAY);  // Debounce the event
  }

  // Poll SPI (checking the clock state)
  int currentSPI_SCK = digitalRead(SPI_SCK);

  // Check for rising edge on SPI clock (SCK)
  if (lastSPI_SCK == LOW && currentSPI_SCK == HIGH) {
    spi_started = true;
    Serial.println("SPI Communication Detected!");
    delay(DEBOUNCE_DELAY);  // Debounce the event
  }

  // Poll RS485 (check for data on RX pin)
  int currentRS485_RX = digitalRead(RS485_RX);

  // Check for rising edge on RS485 RX pin
  if (lastRS485_RX == LOW && currentRS485_RX == HIGH) {
    rs485_started = true;
    Serial.println("RS485 (Modbus) Communication Detected!");
    delay(DEBOUNCE_DELAY);  // Debounce the event
  }

  // Print status messages for communication flags
  if (i2c_started) {
    Serial.println("I2C Communication Started!");
    i2c_started = false;  // Reset flag after processing
  }

  if (spi_started) {
    Serial.println("SPI Communication Started!");
    spi_started = false;  // Reset flag after processing
  }

  if (rs485_started) {
    Serial.println("RS485 (Modbus) Communication Started!");
    rs485_started = false;  // Reset flag after processing
  }

  // Update the last states for the next loop iteration
  lastSCL = currentSCL;
  lastSDA = currentSDA;
  lastSPI_SCK = currentSPI_SCK;
  lastRS485_RX = currentRS485_RX;

  delayMicroseconds(5);  // Add a small delay before polling again
}
