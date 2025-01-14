#include <Wire.h>
#include "RTC_Timer.h"

#define SDA 4
#define SCL 5
#define Address 0x6F

// Reference: https://kidbright.info/files/Sch_KidBright32%20V1.3.pdf

void strSplit();

RTC_Timer rtc(SDA, SCL, Address);
unsigned long time_now = 0;

void setup() {
  Serial.begin(115200);
  rtc.startClock();
}

void loop() {
  if (millis() >= time_now + 1000) {
    time_now += 1000;
    Serial.println(rtc.getDateTime());
  }

  if (Serial.available()) {
    int length = 11;
    String parse[length];

    String res = Serial.readStringUntil('\n');
    // Serial.println(res);

    if (res[0] == '/') {
      strSplit(res, ' ', parse);
      parser(parse, length);
    }
  }
}

// Function to split a string based on a delimiter
void strSplit(String txt, char delim, String output[]) {
  int n = 0;  // Counter for parts
  String buffer = "";

  for (int i = 0; i < txt.length(); i++) {
    char c = txt.charAt(i);

    if (c == delim) {
      output[n] = buffer;  // Store the part in the output array
      n++;
      buffer = "";  // Reset buffer
    } else {
      buffer += c;  // Add character to buffer
    }
  }

  // Add the last part if there's anything left in the buffer
  if (buffer.length() > 0) {
    output[n] = buffer;
    n++;
  }
}

void parser(String parse[], int length) {
  int i = 1;
  // Serial.println(parse[0]);

  if (parse[0] == "/reset") {
    rtc.resetClock();
  }

  for (int i = 1; i < length; i += 2) {
    if (parse[i] == "") break;
    // Serial.println(parse[i] +" "+ parse[i+1]);

    parse[i].toLowerCase();
    if (parse[0] == "/setTime") {
      if (parse[i] == "second") rtc.setSecond(parse[i + 1].toInt());
      if (parse[i] == "minute") rtc.setMinute(parse[i + 1].toInt());
      if (parse[i] == "hour") rtc.setHour(parse[i + 1].toInt());
    } else if (parse[0] == "/setDate") {
      if (parse[i] == "day") rtc.setDay(parse[i + 1].toInt());
      if (parse[i] == "month") rtc.setMonth(parse[i + 1]);
      if (parse[i] == "year") rtc.setYear(parse[i + 1].toInt() - 2000);
      if (parse[i] == "weekday") rtc.setWeekDay(parse[i+1]);
    } else if (parse[0] == "/setClockMode") {
      rtc.setClockMode(parse[i]);
    } else if (parse[0] == "/setDisplayMode") {
      rtc.setDisplayMode(parse[i]);
    } else {
      return;
    }
  }
  Serial.println("RTC Clock has been updated!");
}
