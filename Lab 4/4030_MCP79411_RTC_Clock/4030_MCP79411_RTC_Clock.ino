#include <Wire.h>
#include "RTC_Timer.h"

#define SDA 4
#define SCL 5
#define Address 0x6F

RTC_Timer rtc(SDA, SCL, Address);
unsigned long time_now = 0;

void setup() {
  Serial.begin(115200);
  rtc.startClock();
  rtc.resetClock();
  rtc.setSecond(6);
  rtc.setMinute(7);
  rtc.setClockMode(false);
  rtc.setHour(12);
  rtc.setWeekDay(3);
}

void loop() {
  int sec = rtc.getSecond();
  int min = rtc.getMinute();
  int hr = rtc.getHour();

  if(millis() >= time_now + 1000){
    time_now += 1000;
    Serial.print(rtc.getTime());
    Serial.println(rtc.getClockMode() ? " 12-hour" : " 24-hour");
    Serial.println(rtc.getWeekDay());
  }

  if ( Serial.available() ) {
    String res = Serial.readStringUntil(' ');
    Serial.println(res);
  }
}
