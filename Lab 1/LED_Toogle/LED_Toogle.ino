#define SW2_PIN 14
#define LED_blue_PIN 

bool LED_blue = 0;
bool LED_green = 1;

void setup() {
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode()

  SW2_curr = digitalRead(SW2_PIN);
}

void loop() {
  if (SW2_PIN != digitalRead(SW2_PIN)) {
    SW2_curr = digitalRead(SW2_PIN);
    LED_blue = !LED_blue;
    LED_green = !LED_green;
  }
}
