#define BLUE_LED 17

void setup() {
  pinMode(BLUE_LED, OUTPUT);
}

void loop() {
  digitalWrite(BLUE_LED, HIGH);
  delay(500);
  digitalWrite(BLUE_LED, LOW);
  delay(500);
}
