#include "RTC_Timer.h"
#include <stdio.h>
#include <Wire.h>

// Reference: https://ww1.microchip.com/downloads/en/DeviceDoc/22266A.pdf

RTC_Timer::RTC_Timer(int SDA, int SCL, byte address) {
  this->SDA = SDA;
  this->SCL = SCL;
  this->address = address;
}

void RTC_Timer::startClock() {
  Wire.begin(SDA, SCL);
  Wire.beginTransmission(address);
  Wire.write(0x00);  // Go to address 00h of RTCC Memory (second)
  Wire.write(0x80);  // Start RTC clock by set bit 7 to 1 (1000 0000)
  Wire.endTransmission();
}

void RTC_Timer::resetClock() {
  Wire.beginTransmission(address);
  Wire.write(0x00);  // Start on 00h RTCC memory register
  Wire.write(0x80);  // Reset second(00h)
  Wire.write(0x00);  // Reset minute(01h)
  Wire.write(0x00);  // Reset hour(02h)
  Wire.write(0x01);  // Reset day(03h)
  Wire.write(0x01);  // Reset date(04h)
  Wire.write(0x01);  // Reset month(05h)
  Wire.write(0x00);  // Reset year(06h)
  Wire.endTransmission();
}

/**
 * isTwelveHour - Clock mode. True if the clock is in 12 hours mode, otherwise false.
 */
void RTC_Timer::setClockMode(bool isTwelveHour) {
  Wire.beginTransmission(address);
  Wire.write(0x02);
  if (isTwelveHour) Wire.write(0x40);  // 0100 0000
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

int RTC_Timer::getSecond() {
  Wire.beginTransmission(address);
  Wire.write(0x00);  // Go to address 00h of RTCC Memory (second)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read();  // Get response from RTCC memory
  byte ten = (res & 0x70) >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

void RTC_Timer::setSecond(int second) {
  if (second < 0 || second > 59) return;  // Guard condition for input out of range

  byte ten = byte(second / 10) << 4;
  byte one = byte(second % 10);

  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.write(ten | one | 0x80);
  Wire.endTransmission();
}

int RTC_Timer::getMinute() {
  Wire.beginTransmission(address);
  Wire.write(0x01);  // Go to address 01h of RTCC Memory (minute)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read();  // Get response from RTCC memory
  byte ten = res >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

void RTC_Timer::setMinute(int minute) {
  if (minute < 0 || minute > 59) return;  // Guard condition for input out of range

  byte ten = byte(minute / 10) << 4;
  byte one = byte(minute % 10);

  Wire.beginTransmission(address);
  Wire.write(0x01);
  Wire.write(ten | one);
  Wire.endTransmission();
}

int RTC_Timer::getHour() {
  Wire.beginTransmission(address);
  Wire.write(0x02);  // Go to address 02h of RTCC Memory (hour)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read();  // Get response from RTCC memory
  byte ten = (res & 0x10) >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

void RTC_Timer::setHour(int hour) {
  byte meridien = 0;

  if (hour < 0 || hour > 23) return;  // Guard condition for input out of range

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

String RTC_Timer::getMeridien() {
  Wire.beginTransmission(address);
  Wire.write(0x02);  // Go to address 02h of RTCC Memory (hour)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read();  // Get response from RTCC memory
  return bool((res & 0x20) >> 5) ? "PM" : "AM";
}

String RTC_Timer::getTime() {
  bool clockMode = getClockMode();
  int sec = getSecond();
  int min = getMinute();
  int hr = getHour();
  String meridien = getMeridien();

  char txt[11] = "";
  if (clockMode) {
    sprintf(txt, "%02d:%02d:%02d %s", hr, min, sec, meridien);
  } else {
    sprintf(txt, "%02d:%02d:%02d", hr, min, sec);
  }
  return String(txt);
}

String RTC_Timer::getWeekDay() {
  const String weekDayList[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
  };

  Wire.beginTransmission(address);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read() & 0x07;
  return weekDayList[res - 1];
}

void RTC_Timer::setWeekDay(String weekDay) {
  String weekDayList[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
  };
  
  int n = sizeof(weekDayList) / sizeof(weekDayList[0]);
  auto ptr = std::find(weekDayList, weekDayList + n, weekDay);
  int dayNum = ptr - weekDayList + 1;

  if (dayNum < 1 || dayNum > 7) return;  // Guard condition for input out of range

  Wire.beginTransmission(address);
  Wire.write(0x03);
  Wire.write(dayNum);
  Wire.endTransmission();
}

int RTC_Timer::getDay() {
  Wire.beginTransmission(address);
  Wire.write(0x04);  // Go to address 04h of RTCC Memory (day)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read();  // Get response from RTCC memory
  byte ten = res >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

void RTC_Timer::setDay(int day) {
  if (day < 1 || day > 31) return;  // Guard condition for input out of range

  byte ten = byte(day / 10) << 4;
  byte one = byte(day % 10);

  Wire.beginTransmission(address);
  Wire.write(0x04);
  Wire.write(ten | one);
  Wire.endTransmission();
}

String RTC_Timer::getMonth() {
  const String monthList[12] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
  };

  Wire.beginTransmission(address);
  Wire.write(0x05);  // Go to address 05h of RTCC Memory (month)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read();  // Get response from RTCC memory
  byte ten = (res & 0x10) >> 4;
  byte one = res & 0x0F;
  int monthNum = 10 * ten + one;

  return monthList[monthNum - 1];
}

void RTC_Timer::setMonth(String month) {
  const String monthList[12] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
  };

  int n = sizeof(monthList) / sizeof(monthList[0]);
  auto ptr = std::find(monthList, monthList + n, month);
  int monthNum = ptr - monthList + 1;

  if (monthNum < 1 || monthNum > 7) return;  // Guard condition for input out of range

  Wire.beginTransmission(address);
  Wire.write(0x05);
  Wire.write(monthNum);
  Wire.endTransmission();
}

int RTC_Timer::getYear() {
  Wire.beginTransmission(address);
  Wire.write(0x06);  // Go to address 06h of RTCC Memory (year)
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte res = Wire.read();  // Get response from RTCC memory
  byte ten = res >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

void RTC_Timer::setYear(int year) {
  if (year < 0 || year > 99) return;  // Guard condition for input out of range

  byte ten = byte(year / 10) << 4;
  byte one = byte(year % 10);

  if (year % 4 == 0) setLeapYear(true);
  else setLeapYear(false);

  Wire.beginTransmission(address);
  Wire.write(0x06);
  Wire.write(ten | one);
  Wire.endTransmission();
}

bool RTC_Timer::isLeapYear() {
  Wire.beginTransmission(address);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);
  return Wire.read() >> 5;
}

void RTC_Timer::setLeapYear(bool isLeapYear) {
  Wire.beginTransmission(address);
  Wire.write(0x05);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  byte month = Wire.read() & 0x1F;

  Wire.beginTransmission(address);
  Wire.write(0x05);
  if (isLeapYear) Wire.write(0x20 | month);  // 0010 0000
  else Wire.write(0x00 | month);
  Wire.endTransmission();
}

String RTC_Timer::getDate() {
  String weekDay = getWeekDay();
  int day = getDay();
  String month = getMonth();
  int year = getYear();

  char txt[27] = "";
  sprintf(txt, "%s, %s %d, %02d", weekDay, month, day, year);
  return String(txt);
}

String RTC_Timer::getDateTime() {
  String date = getDate();
  String time = getTime();

  return time + ", " + date;
}
