#define SW2_PIN 14
#define LED_blue_PIN 17
#define LED_green_PIN 12

bool LED_blue = 0;
bool LED_green = 1;

void setup() {
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(LED_blue_PIN, OUTPUT);
  pinMode(LED_green_PIN, OUTPUT);
}

void loop() {
  if (!digitalRead(SW2_PIN)) {
    LED_blue = !LED_blue;
    LED_green = !LED_green;
    while(!digitalRead(SW2_PIN)) {};
  }

  digitalWrite(LED_blue_PIN, LED_blue);
  digitalWrite(LED_green_PIN, LED_green);
  delay(50);
}
