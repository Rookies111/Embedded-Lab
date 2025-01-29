const int slave_addr = 1;
const int dirpin = 18;
uint8_t buf[128];
int8_t indexMax = 0;
int8_t m_error = 0;
unsigned long prev_t;

uint8_t packet1[8] = { 0x08, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x92 };
uint8_t packet2[8] = { 0x08, 0x03, 0x00, 0x32, 0x00, 0x02, 0x65, 0x5D };

void setup() {
  Serial.begin(115200);
  pinMode(dirpin, OUTPUT);

  Serial1.begin(9600, SERIAL_8N1, 23, 19);
  Serial.println("BEGIN");
  Serial.println();
}

void loop() {
  digitalWrite(dirpin, HIGH);
  delay(10);
  Serial1.write(packet1, 8);
  delay(9);
  digitalWrite(dirpin, LOW);
  data_recv(slave_addr);
  if (m_error == 0) {
    Serial.print("Response packet1: ");
    for (int i = 0; i < indexMax; i++) {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.print("Error (TIMEOUT = 1): ");
    Serial.println(m_error);
  }
  delay(15000);
}

void data_recv(uint8_t ss) {
  unsigned long prev_t = millis();
  while (Serial1.available() == 0) {
    if (millis() - prev_t > 20000) {
      m_error = 1;
      return;
    }
  }
  indexMax = 0;
  m_error = 0;
  prev_t = millis();
  while (Serial1.available() > 0) {
    buf[indexMax++] = Serial1.read();
    prev_t = millis();
    while (Serial1.available() == 0) {
      if (millis() - prev_t > 500) {
        break;
      }
    }
  }
}