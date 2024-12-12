#define BUTTON1_PIN 16
#define BUTTON2_PIN 14

const int LED_ARRAY[] = {12, 17};
const int LED_ARRAY_LENGTH = 2;
const int button_debounce_delay = 100;

int button1_prev = 1;  // Active LOW
int button2_prev = 1;  // Active LOW

int button1_db = 0;
int button2_db = 0;
unsigned int active_led_index = 0;

void setup() {
    Serial.begin(9600);

    pinMode(BUTTON1_PIN, INPUT);
    pinMode(BUTTON2_PIN, INPUT);

    for (unsigned short i = 0; i < LED_ARRAY_LENGTH; i++) {
        pinMode(LED_ARRAY[i], OUTPUT);
        digitalWrite(LED_ARRAY[i], HIGH);  // Active LOW
    }

    button1_prev = digitalRead(BUTTON1_PIN);
    button2_prev = digitalRead(BUTTON2_PIN);
}

void loop() {
    // RESET to Default State
    int button1_curr = digitalRead(BUTTON1_PIN);
    if (button1_prev != button1_curr) {  // Falling Edge (For Active LOW)
        if (millis() - button1_db > button_debounce_delay) {
            button1_prev = button1_curr;

            if (button1_curr == LOW) {
                for (unsigned int i = 0; i < LED_ARRAY_LENGTH; i++) {
                    digitalWrite(LED_ARRAY[i], HIGH);  // Active LOW
                }
            }
        }
    } else {
        button1_db = millis();
    }

    // Switching Active LED
    int button2_curr = digitalRead(BUTTON2_PIN);
    if (button2_prev != button2_curr) {  // Falling Edge (For Active LOW)
        if (millis() - button2_db > button_debounce_delay) {
            button2_prev = button2_curr;

            if (button1_curr == LOW) {
                digitalWrite(LED_ARRAY[active_led_index], LOW); // Light the LED
                digitalWrite(LED_ARRAY[(active_led_index - 1) % LED_ARRAY_LENGTH], HIGH); // Dark the LED

                active_led_index++;
                active_led_index = active_led_index % LED_ARRAY_LENGTH;
            }
        }
    } else {
        button2_db = millis();
    }

    Serial.println((String)button1_curr + " " + (String)button2_curr);
}
