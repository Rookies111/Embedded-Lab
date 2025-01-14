#include "RTC_Timer.h"
#include <stdio.h>
#include <Wire.h>

#define second_register 0x00
#define minute_register 0x01
#define hour_register 0x02
#define day_register 0x03
#define date_register 0x04
#define month_register 0x05
#define year_register 0x06

// Reference: https://ww1.microchip.com/downloads/en/DeviceDoc/22266A.pdf

RTC_Timer::RTC_Timer(int SDA, int SCL, byte address) {
  this->SDA = SDA;
  this->SCL = SCL;
  this->address = address;
  this->displayMode = true; // True for long form and false for short form.
}

// Function to start RTC clock oscillation.
void RTC_Timer::startClock() {
  Wire.begin(SDA, SCL);
  Wire.beginTransmission(address);
  Wire.write(second_register); // Go to second address of RTCC Memory register
  Wire.write(0x80);             // Start RTC clock by set bit 7 to 1 (1000 0000)
  Wire.endTransmission();
}

// Function to reset RTC Clock time and date.
void RTC_Timer::resetClock() {
  Wire.beginTransmission(address);
  Wire.write(second_register); // Go to second address of RTCC memory register
  Wire.write(0x80);             // Reset second(00h)
  Wire.write(0x00);             // Reset minute(01h)
  Wire.write(0x00);             // Reset hour(02h)
  Wire.write(0x01);             // Reset day(03h)
  Wire.write(0x01);             // Reset date(04h)
  Wire.write(0x01);             // Reset month(05h)
  Wire.write(0x00);             // Reset year(06h)
  Wire.endTransmission();
}

// Function to set display mode for RTC Clock. Only input long or short.
void RTC_Timer::setDisplayMode(String mode) {
  if (mode != "short" && mode != "long") return; // Guard condition for mode neither short or long.
  displayMode = (mode == "long");                 // Set display mode to true(long) if mode is long
}

// Function to get RTC Clock hour mode. True if the clock is in 12 hours mode, false if it's in 24 hours mode.
bool RTC_Timer::getClockMode() {
  // Go to hour address of RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(hour_register);
  Wire.endTransmission();

  // Request data from hour register at bit 6
  Wire.requestFrom(address, 1);
  return Wire.read() >> 6;
}

// Function to set RTC Clock hour mode. True for 12 hours mode and false for 24 hours mode.
void RTC_Timer::setClockMode(String mode) {
  if (mode != "12-hour" && mode != "24-hour") return; // Guard condition for mode neither 12-hour or 24-hour.

  // Calculate hour value of the current clock mode
  int hour = getHour();
  bool prevMode = getClockMode();
  if (prevMode) {
    hour = (getMeridien() == "AM") ? hour : hour + 12;
  }

  // Change clock mode
  Wire.beginTransmission(address);
  Wire.write(hour_register);                // Go to hour RTCC memory register
  if (mode == "12-hour") Wire.write(0x40); // Set bit 6 to 1 (0100 0000)
  else Wire.write(0x00);                    // Set bit 6 to 0 (0000 0000)
  Wire.endTransmission();

  setHour(hour); // Set hour value to from the calculated hour value
}

// Function to get RTC Clock second value.
int RTC_Timer::getSecond() {
  // Go to second address of RTCC Memory register
  Wire.beginTransmission(address);
  Wire.write(second_register);
  Wire.endTransmission();

  // Request data from second register, and calculate the second value
  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = (res & 0x70) >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

// Function to set RTC Clock second value.
void RTC_Timer::setSecond(int second) {
  if (second < 0 || second > 59) return; // Guard condition for input out of range

  // Calculate 10 second value and 1 second value
  byte ten = byte(second / 10) << 4;
  byte one = byte(second % 10);

  // Change second value in RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(second_register);
  Wire.write(ten | one | 0x80);
  Wire.endTransmission();
}

// Function to get RTC Clock minute value.
int RTC_Timer::getMinute() {
  // Go to minute address of RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(minute_register);
  Wire.endTransmission();

  // Request data from minute register, and calculate the minute value
  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = res >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

// Function to set RTC Clock minute value.
void RTC_Timer::setMinute(int minute) {
  if (minute < 0 || minute > 59) return; // Guard condition for input out of range

  // Calculate 10 minute value and 1 minute value
  byte ten = byte(minute / 10) << 4;
  byte one = byte(minute % 10);

  // Change minute value in RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(minute_register);
  Wire.write(ten | one);
  Wire.endTransmission();
}

// Function to get RTC Clock hour value.
int RTC_Timer::getHour() {
  const bool clockMode = getClockMode(); // Get clock mode for mask selection

  // Go to hour address of RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(hour_register);
  Wire.endTransmission();

  // Request data from hour register, and calculate the hour value
  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = (res & (clockMode ? 0x10 : 0x30)) >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

// Function to set RTC Clock hour value.
void RTC_Timer::setHour(int hour) {
  if (hour < 0 || hour > 23) return; // Guard condition for input out of range

  // Calculate for hour adjustment in different clock mode
  byte meridien;
  bool clockMode = RTC_Timer::getClockMode();
  if (clockMode) {
    meridien = byte(hour >= 12) << 5;
    hour = (hour % 12 == 0) ? 12 : hour % 12;
  }

  // Calculate 10 hour value and 1 hour value
  byte ten = (byte(hour / 10) << 4) | meridien | (clockMode << 6);
  byte one = byte(hour % 10);

  // Change hour value in RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(hour_register);
  Wire.write(ten | one);
  Wire.endTransmission();
}

// Function to get meridien value from RTC Clock.
String RTC_Timer::getMeridien() {
  // Go to hour address of RTCC Memory register
  Wire.beginTransmission(address);
  Wire.write(hour_register);
  Wire.endTransmission();

  // Request data from hour register on bit 5 and format output
  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  return bool((res & 0x20) >> 5) ? "PM" : "AM";
}

// Function to get time from RTC Clock.
String RTC_Timer::getTime() {
  // Get clock mode, second, minute, hour, and meridien value from RTC Clock
  bool clockMode = getClockMode();
  int sec = getSecond();
  int min = getMinute();
  int hr = getHour();
  String meridien = getMeridien();

  // Format output string and return
  char txt[11] = "";
  if (clockMode) {
    sprintf(txt, "%02d:%02d:%02d %s", hr, min, sec, meridien);
  } else {
    sprintf(txt, "%02d:%02d:%02d", hr, min, sec);
  }
  return String(txt);
}

// Function to get RTC Clock day value.
String RTC_Timer::getWeekDay() {
  const String weekDayList[7] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
  };

  // Go to day address of RTCC Memory register
  Wire.beginTransmission(address);
  Wire.write(day_register);
  Wire.endTransmission();

  // Request data from day register on first 3 bit and map it on weekDayList
  Wire.requestFrom(address, 1);
  byte res = Wire.read() & 0x07;
  return weekDayList[res - 1];
}

// Function to set RTC Clock day value.
void RTC_Timer::setWeekDay(String weekDay) {
  const String weekDayList[7] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
  };

  // Get the index of weekDay in weekDayList array
  int n = sizeof(weekDayList) / sizeof(weekDayList[0]);
  auto ptr = std::find(weekDayList, weekDayList + n, weekDay);
  int dayNum = ptr - weekDayList + 1;

  if (dayNum < 1 || dayNum > 7) return; // Guard condition for input out of range

  // Change day value in RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(day_register);
  Wire.write(dayNum);
  Wire.endTransmission();
}

// Function to get RTC Clock date value.
int RTC_Timer::getDay() {
  // Go to date address of RTCC Memory register
  Wire.beginTransmission(address);
  Wire.write(date_register);
  Wire.endTransmission();

  // Request data from date register, and calculate the date value
  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = res >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

// Function to set RTC Clock date value.
void RTC_Timer::setDay(int day) {
  if (day < 1 || day > 31) return; // Guard condition for input out of range

  // Calculate 10 date value and 1 date value
  byte ten = byte(day / 10) << 4;
  byte one = byte(day % 10);

  // Change date value in RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(date_register);
  Wire.write(ten | one);
  Wire.endTransmission();
}

// Function to get RTC Clock month value.
String RTC_Timer::getMonth() {
  const String monthList[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
  };

  // Go to month address of RTCC Memory register
  Wire.beginTransmission(address);
  Wire.write(month_register);
  Wire.endTransmission();

  // Request data from month register and calculate for month number
  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = (res & 0x10) >> 4;
  byte one = res & 0x0F;
  int monthNum = 10 * ten + one;

  return monthList[monthNum - 1]; // Map monthNum on monthList
}

// Function to set RTC Clock month value.
void RTC_Timer::setMonth(String month) {
  const String monthList[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
  };

  // Get the index of month in monthList array
  int n = sizeof(monthList) / sizeof(monthList[0]);
  auto ptr = std::find(monthList, monthList + n, month);
  int monthNum = ptr - monthList + 1;

  if (monthNum < 1 || monthNum > 12) return; // Guard condition for input out of range

  // Calculate 10 month value and 1 month value
  byte ten = byte(monthNum / 10) << 4;
  byte one = byte(monthNum % 10);

  // Change month value in RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(month_register);
  Wire.write(ten | one);
  Wire.endTransmission();
}

// Function to get RTC Clock year value.
int RTC_Timer::getYear() {
  // Go to year address of RTCC Memory register
  Wire.beginTransmission(address);
  Wire.write(year_register);
  Wire.endTransmission();

  // Request data from year register and calculate for year value
  Wire.requestFrom(address, 1);
  byte res = Wire.read(); // Get response from RTCC memory
  byte ten = res >> 4;
  byte one = res & 0x0F;
  return 10 * ten + one;
}

// Function to set RTC Clock year value.
void RTC_Timer::setYear(int year) {
  if (year < 1 || year > 99) return; // Guard condition for input out of range

  // Calculate 10 year value and 1 year value
  byte ten = byte(year / 10) << 4;
  byte one = byte(year % 10);

  // Change year value in RTCC memory register
  Wire.beginTransmission(address);
  Wire.write(year_register);
  Wire.write(ten | one);
  Wire.endTransmission();
}

// Function to get date from RTC Clock.
String RTC_Timer::getDate() {
  // Get day, date, month, and year value from RTC Clock
  String weekDay = getWeekDay();
  int day = getDay();
  String month = getMonth();
  int year = getYear();

  // Format output string and return
  char txt[30] = "";
  if (displayMode) {
    sprintf(txt, "%s, %s %d, 20%02d", weekDay, month, day, year);
  } else {
    sprintf(txt, "%02d/%s/%02d", day, month.substring(0, 3), year);
  }
  return String(txt);
}

// Function to get both date and time from RTC Clock.
String RTC_Timer::getDateTime() {
  String date = getDate();
  String time = getTime();

  return time + ", " + date;
}
