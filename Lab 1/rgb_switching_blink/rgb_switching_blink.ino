// #define BLUE_LED 17
// #define RED_LED 2
// #define YELLOW_LED 15
// #define GREEN_LED 12

const int LED_ARRAY[] = {17, 2, 15, 12};
const int LED_ARRAY_LENGTH = 4;

unsigned int active_led_index = 0;

void setup() {
    for (unsigned int i = 0; i < LED_ARRAY_LENGTH; i++) {
        pinMode(LED_ARRAY[i], OUTPUT);
        digitalWrite(LED_ARRAY[i], HIGH);
    }
}

void loop() {
    unsigned int prev_active_index = (active_led_index - 1) % LED_ARRAY_LENGTH;
    digitalWrite(LED_ARRAY[prev_active_index], HIGH);
    digitalWrite(LED_ARRAY[active_led_index], LOW);

    active_led_index++;
    active_led_index = active_led_index % LED_ARRAY_LENGTH;

    delay(1000);
}
