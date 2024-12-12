const int SDA1 = 4; // Port SDA1 Pin IO4
const int SCL1 = 5; // Port SCL1 Pin IO5

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
    Serial.print("Low level Time Capture of SCL1 = ");
    Serial.println(time_capture1);
    Serial.print("Distance from SCL1 Calc. = ");
    Serial.println(distance1);
    state_counter1 = 0;
}

void IRAM_ATTR do_counter1() {
    if (state_counter1 == 0) {
        start_counter1();
    } else if (state_counter1 == 1) {
        end_counter1();
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(SDA1, OUTPUT);
  pinMode(SCL1, INPUT);
  attachInterrupt(digitalPinToInterrupt(SCL1), do_counter1, CHANGE);
}

void loop() {
    delay(3000);
    digitalWrite(SDA1, HIGH);
    delay(10);
    digitalWrite(SDA1, LOW);

}
