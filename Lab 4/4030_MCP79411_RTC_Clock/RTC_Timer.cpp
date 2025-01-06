#include "RTC_Timer.h"
#include <stdio.h>
#include <Wire.h>

// Reference: https://ww1.microchip.com/downloads/en/DeviceDoc/22266A.pdf

RTC_Timer::RTC_Timer(int SDA, int SCL, byte address) {
  this -> SDA = SDA;
  this -> SCL = SCL;
  this -> address = address;
}

void RTC_Timer::startClock() {
  Wire.begin(SDA, SCL);
  Wire.beginTransmission(address);
  Wire.write(0x00); // Go to address 00h of RTCC Memory (second)
  Wire.write(0x80); // Start RTC clock by set bit 7 to 1 (1000 0000)
  Wire.endTransmission();
}

void RTC_Timer::resetClock() {
  Wire.beginTransmission(address);
  Wire.write(0x00); // Start on 00h RTCC memory register
  Wire.write(0x80); // Reset second(00h) on RTCC memory register
  Wire.write(0x00); // Reset minute(01h) on RTCC memory register
  Wire.write(0x00); // Reset hour(02h) on RTCC memory register
  Wire.write(0x01); // Reset day(03h) on RTCC memory register
  Wire.write(0x01); // Reset date(04h) on RTCC memory register
  Wire.write(0x01); // Reset month(05h) on RTCC memory register
  Wire.write(0x01); // Reset year(06h) RTCC memory register
  Wire.endTransmission();
}

int RTC_Timer::getSecond() {
  Wire.beginTransmission(address);
  Wire.write(0x00); // Go to address 00h of RTCC Memory (second)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = (res & 0x70) >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

void RTC_Timer::setSecond(int second) {
  if (second < 0 || second > 59) return; // Guard condition for input out of range

  byte ten = byte(second / 10) << 4;
  byte one = byte(second % 10);

  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.write( ten | one | 0x80 );
  Wire.endTransmission();
}

int RTC_Timer::getMinute() {
  Wire.beginTransmission(address);
  Wire.write(0x01); // Go to address 01h of RTCC Memory (minute)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = (res & 0x70) >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

void RTC_Timer::setMinute(int minute) {
  if (minute < 0 || minute > 59) return; // Guard condition for input out of range

  byte ten = byte(minute / 10) << 4;
  byte one = byte(minute % 10);

  Wire.beginTransmission(address);
  Wire.write(0x01);
  Wire.write(ten | one);
  Wire.endTransmission();
}

int RTC_Timer::getHour() {
  Wire.beginTransmission(address);
  Wire.write(0x02); // Go to address 02h of RTCC Memory (hour)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = (res & 0x10) >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

String RTC_Timer::getMeridien() {
  Wire.beginTransmission(address);
  Wire.write(0x02); // Go to address 02h of RTCC Memory (hour)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  return bool((res & 0x20) >> 5) ? "PM" : "AM";
}

void RTC_Timer::setHour(int hour) {
  byte meridien = 0;

  if (hour < 0 || hour > 23) return; // Guard condition for input out of range

  bool clockMode = RTC_Timer::getClockMode();
  if (clockMode) {
    meridien = byte(hour >= 12) << 5;
    hour = (hour % 12 == 0) ? 12 : hour % 12;
  }

  byte ten = (byte(hour / 10) << 4) | meridien | (clockMode << 6);
  byte one = byte(hour % 10);
  
  Wire.beginTransmission(address);
  Wire.write(0x02);
  Wire.write(ten | one);
  Wire.endTransmission();
}

String RTC_Timer::getTime() {
  bool clockMode = RTC_Timer::getClockMode();
  int sec = RTC_Timer::getSecond();
  int min = RTC_Timer::getMinute();
  int hr = RTC_Timer::getHour();
  String meridien = RTC_Timer::getMeridien();

  char txt[11] = "";
  if (clockMode) {
    sprintf(txt, "%02d:%02d:%02d %s", hr, min, sec, meridien);
  } else {
    sprintf(txt, "%02d:%02d:%02d", hr, min, sec);
  }
  return String(txt);
}

String RTC_Timer::getWeekDay() {
  Wire.beginTransmission(address);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read() & 7;
  switch (res) {
    case 1: return "Sunday";
    case 2: return "Monday";
    case 3: return "Tuesday";
    case 4: return "Wednesday";
    case 5: return "Thursday";
    case 6: return "Friday";
    case 7: return "Saturday";
  }
}

// Test
void RTC_Timer::setWeekDay(int dayNum) {
  Wire.beginTransmission(address);
  Wire.write(0x03);
  Wire.write(dayNum);
  Wire.endTransmission();
}

/**
 * isTwelveHour - Clock mode. True if the clock is in 12 hours mode, otherwise false.
 */
void RTC_Timer::setClockMode(bool isTwelveHour) {
  Wire.beginTransmission(address);
  Wire.write(0x02);
  if (isTwelveHour) Wire.write(0x40); // 0100 0000
  else Wire.write(0x00);
  Wire.endTransmission();
}

bool RTC_Timer::getClockMode() {
  Wire.beginTransmission(address);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);
  return Wire.read() >> 6;
}
