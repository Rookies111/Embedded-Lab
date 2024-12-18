#define DIGITAL_PIN 19
#define FREQUENCY 20000

int state_timer_0 = LOW;
hw_timer_t* timer;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer_0() {
    if (!state_timer_0) {
        timerAlarm(timer, 20, 1, 0);
        digitalWrite(DIGITAL_PIN, state_timer_0);
    } else {
        timerAlarm(timer, 80, 1, 0);
        digitalWrite(DIGITAL_PIN, state_timer_0);
    }
    state_timer_0 = !state_timer_0;
}

void setup() {
    Serial.begin(115200);
    pinMode(DIGITAL_PIN, OUTPUT);
    Serial.println("Start Timer");
    timer = timerBegin(FREQUENCY);
    timerAttachInterrupt(timer, &onTimer_0);
    timerAlarm(timer, 20, 1, 0);
    timerStart(timer);
}

void loop() {}
