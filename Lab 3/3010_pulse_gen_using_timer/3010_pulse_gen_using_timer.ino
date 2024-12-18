#define DIGITAL_PIN 19
#define FREQUENCY 1000000

int state = LOW;
hw_timer_t* timer;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer_0() {
    digitalWrite(DIGITAL_PIN, state = !state);
    Serial.println(String("onTimer() ") + String(millis()));
}

void setup() {
    Serial.begin(115200);
    pinMode(DIGITAL_PIN, OUTPUT);
    Serial.println("Start Timer");
    timer = timerBegin(FREQUENCY);
    timerAttachInterrupt(timer, &onTimer_0);
    timerAlarm(timer, 20000, 1, 0);
    timerStart(timer);
}

void loop() {}
