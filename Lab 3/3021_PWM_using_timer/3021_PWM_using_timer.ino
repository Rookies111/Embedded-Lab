#define DIGITAL_PIN_0 18
#define DIGITAL_PIN_1 19
#define FREQUENCY 10000000

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int state_timer_0 = LOW;
hw_timer_t* timer_0;
int freq_0 = 10000;
int duty_cycle_0 = 50;

int state_timer_1 = LOW;
hw_timer_t* timer_1;
int freq_1 = 5000;
int duty_cycle_1 = 50;

String data[4];

// Real Freq = freq/prescale
// Prescale = freq/real_freq

void IRAM_ATTR onTimer_0() {
    if (state_timer_0) {
        timerAlarm(timer_0, FREQUENCY/freq_0 * duty_cycle_0/100, 1, 0);
        digitalWrite(DIGITAL_PIN_0, state_timer_0);
    } else {
        timerAlarm(timer_0, FREQUENCY/freq_0 * (100-duty_cycle_0)/100, 1, 0);
        digitalWrite(DIGITAL_PIN_0, state_timer_0);
    }
    state_timer_0 = !state_timer_0;
}

void IRAM_ATTR onTimer_1() {
    if (state_timer_1) {
        timerAlarm(timer_1, FREQUENCY/freq_1 * duty_cycle_1/100, 1, 0);
        digitalWrite(DIGITAL_PIN_1, state_timer_1);
    } else {
        timerAlarm(timer_1, FREQUENCY/freq_1 * (100-duty_cycle_1)/100, 1, 0);
        digitalWrite(DIGITAL_PIN_1, state_timer_1);
    }
    state_timer_1 = !state_timer_1;
}

void setup() {
    Serial.begin(115200);
    pinMode(DIGITAL_PIN_0, OUTPUT);
    pinMode(DIGITAL_PIN_1, OUTPUT);
    Serial.println("Start Timer");
    timer_0 = timerBegin(FREQUENCY);
    timer_1 = timerBegin(FREQUENCY);
    timerAttachInterrupt(timer_0, &onTimer_0);
    timerAttachInterrupt(timer_1, &onTimer_1);
    timerAlarm(timer_0, FREQUENCY/freq_0 * duty_cycle_0/100, 1, 0);
    timerAlarm(timer_1, FREQUENCY/freq_1 * duty_cycle_1/100, 1, 0);
    timerStart(timer_0);
    timerStart(timer_1);
}

int splitString(String input, String delimiter, String output[], int maxParts) {
  int startIndex = 0;
  int endIndex = input.indexOf(delimiter);
  int partCount = 0;

  while (endIndex != -1 && partCount < maxParts) {
    output[partCount++] = input.substring(startIndex, endIndex); // Extract part
    startIndex = endIndex + delimiter.length();
    endIndex = input.indexOf(delimiter, startIndex); // Find next delimiter
  }

  if (partCount < maxParts) {
    output[partCount++] = input.substring(startIndex); // Add the last part
  }

  return partCount; // Return the number of parts
}

void loop() {
    if (Serial.available()) {
        String incoming_data = Serial.readString();
        Serial.println("Received: " + incoming_data);
        int count = splitString(incoming_data, " ", data, 4);
        if (data[0].equals("Duty")) {
            int number = data[1].toInt() - 1;
            int new_duty_cycle = data[3].toInt();
            if (number == 0) {
                if (new_duty_cycle <= 0 || new_duty_cycle >= 100) {
                    timerStop(timer_0);
                    digitalWrite(DIGITAL_PIN_0, new_duty_cycle >= 100);
                } else if (duty_cycle_0 <= 0 || duty_cycle_0 >= 100) {
                    timerStart(timer_0);
                    timerAlarm(timer_0, FREQUENCY/freq_0 * new_duty_cycle/100, 1, 0);
                }
                duty_cycle_0 = new_duty_cycle;
            } else if (number == 1) {
                if (new_duty_cycle <= 0 || new_duty_cycle >= 100) {
                    timerStop(timer_1);
                    digitalWrite(DIGITAL_PIN_1, new_duty_cycle >= 100);
                } else if (duty_cycle_1 <= 0 || duty_cycle_1 >= 100) {
                    timerStart(timer_1);
                    timerAlarm(timer_1, FREQUENCY/freq_1 * new_duty_cycle/100, 1, 0);
                }
                duty_cycle_1 = new_duty_cycle;
            }
        }
    }
}
