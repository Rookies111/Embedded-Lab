/* Millisec function */
const int SW1 = 4; // Port SDA1 Pin IO4
/*Reference KidBright Board :
https://kidbright.info/files/Sch_KidBright32%20V1.3.pdf */
bool button_status_1 = false;
int counter_start = 0;
int counter_stop = 0;
int time_capture = 0;
int state_counter = 0;

void start_counter() {
    counter_start = millis();
    state_counter = 1;
}

void end_counter() {
    counter_stop = millis();
    time_capture = counter_stop - counter_start;
    Serial.print("Low level Time capture = ");
    Serial.println(time_capture);
    state_counter = 0;
}

void IRAM_ATTR do_counter() {
    if (state_counter == 0) {
        start_counter();
    } else if (state_counter == 1) {
        end_counter();
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(SW1, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SW1), do_counter, CHANGE);
}

void loop() {}