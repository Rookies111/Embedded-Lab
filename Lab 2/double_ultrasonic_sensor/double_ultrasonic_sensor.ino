const int SDA1 = 4; // Port SDA1 Pin IO4
const int SCL1 = 5; // Port SCL1 Pin IO5
const int IO18 = 18; // Pin IO18

const double air_velocity = 346;

// For The First One

int counter_start1 = 0;
int counter_stop1 = 0;
int time_capture1 = 0;
double distance1 = 0;
int state_counter1 = 0;

void start_counter1() {
    counter_start1 = micros();
    state_counter1 = 1;
}

void end_counter1() {
    counter_stop1 = micros();
    time_capture1 = counter_stop1 - counter_start1;
    distance1 = ((double)time_capture1)/20000 * air_velocity;
    state_counter1 = 0;
}

void IRAM_ATTR do_counter1() {
    if (state_counter1 == 0) {
        start_counter1();
    } else if (state_counter1 == 1) {
        end_counter1();
    }
}

// For The Second One

int counter_start2 = 0;
int counter_stop2 = 0;
int time_capture2 = 0;
double distance2 = 0;
int state_counter2 = 0;

void start_counter2() {
    counter_start2 = micros();
    state_counter2 = 1;
}

void end_counter2() {
    counter_stop2 = micros();
    time_capture2 = counter_stop2 - counter_start2;
    distance2 = ((double)time_capture2)/20000 * air_velocity;
    state_counter2 = 0;
}

void IRAM_ATTR do_counter2() {
    if (state_counter2 == 0) {
        start_counter2();
    } else if (state_counter2 == 1) {
        end_counter2();
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(SDA1, OUTPUT);
  pinMode(SCL1, INPUT);
  pinMode(IO18, INPUT);
  attachInterrupt(digitalPinToInterrupt(SCL1), do_counter1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IO18), do_counter2, CHANGE);
}

void loop() {
    delay(1500);
    digitalWrite(SDA1, HIGH);
    delay(10);
    digitalWrite(SDA1, LOW);

    delay(1500);

    Serial.print("Low level Time Capture of SCL1 = ");
    Serial.print(time_capture1);
    Serial.print("\tDistance from SCL1 Calc. = ");
    Serial.println(distance1);

    Serial.print("Low level Time Capture of IO18 = ");
    Serial.print(time_capture2);
    Serial.print("\tDistance from IO18 Calc. = ");
    Serial.println(distance2);
}
