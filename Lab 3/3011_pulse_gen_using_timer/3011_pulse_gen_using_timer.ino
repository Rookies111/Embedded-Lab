#define DIGITAL_PIN 19
#define FREQUENCY 10000000

int state = LOW;
hw_timer_t* timer;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int freq = 24600;

// Real Freq = freq/prescale/2
// Prescale = freq/real_freq/2

void IRAM_ATTR onTimer_0() {
    digitalWrite(DIGITAL_PIN, state = !state);
    // Serial.println(String("onTimer() ") + String(millis()));
}

void setup() {
    Serial.begin(115200);
    pinMode(DIGITAL_PIN, OUTPUT);
    Serial.println("Start Timer");
    timer = timerBegin(FREQUENCY);
    timerAttachInterrupt(timer, &onTimer_0);
    timerAlarm(timer, FREQUENCY/freq/2, 1, 0);
    timerStart(timer);
}

void loop() {}
