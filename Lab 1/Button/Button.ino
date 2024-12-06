#define SW1_PIN 16
#define SW2_PIN 14

bool state = 1;

void setup() {
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() {
  // state = !state;
  Serial.println(digitalRead(SW2_PIN));
  Serial.println(digitalRead(SW1_PIN));
  digitalWrite(2, state);
  delay(100);
}
