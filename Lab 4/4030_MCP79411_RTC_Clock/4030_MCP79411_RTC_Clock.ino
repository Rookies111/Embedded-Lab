#include <Wire.h>
#include "RTC_Timer.h"

#define SDA 4
#define SCL 5
#define Address 0x6F

String strSplit(String txt, char delim);

RTC_Timer rtc(SDA, SCL, Address);
unsigned long time_now = 0;

void setup() {
  Serial.begin(115200);
  rtc.startClock();
  rtc.resetClock();
  rtc.setSecond(30);
  rtc.setMinute(59);
  rtc.setClockMode(true);
  rtc.setHour(23);
  rtc.setWeekDay("Wednesday");
  rtc.setDay(28);
  rtc.setMonth("February");
  rtc.setYear(25);
}

void loop() {
  int sec = rtc.getSecond();
  int min = rtc.getMinute();
  int hr = rtc.getHour();

  if (millis() >= time_now + 1000) {
    time_now += 1000;
    Serial.print(rtc.getDateTime() + " ");
    Serial.println(rtc.isLeapYear() ? "is leap year" : "not leap year");
  }

  if (Serial.available()) {
    String res = Serial.readStringUntil('\n');
    Serial.println(res);

    String parse[10] = strSplit(res, ' ');

    for (String word : parse) {
      if (word != "") Serial.println(word);
    }
  }
}

String[] strSplit(String txt, char delim) {
  String parse[10];
  String buffer;
  int n = 0;

  for (int i = 0; i < txt.length(); i++) {
    if (i == txt.length() - 1) {
      buffer += txt[i];
      insertAtEnd(parse, n, buffer);
    } else if (txt[i] == delim) {
      insertAtEnd(parse, n, buffer);
      buffer = "";
    } else {
      buffer += txt[i];
    }
  }

  return parse;
}

void insertAtEnd(String arr[], int &n, String value) {
  if (n < 10) arr[n] = value;
  n++;
}
