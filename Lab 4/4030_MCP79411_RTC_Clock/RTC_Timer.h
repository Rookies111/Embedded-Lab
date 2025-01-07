#ifndef RTC_Timer_H 
#define RTC_Timer_H
#include <Arduino.h>

class RTC_Timer { 
private: 
  int SDA;
  int SCL;
  byte address;
  
public:
  RTC_Timer(int SDA, int SCL, byte Address);

  // Start RTC Clock oscillation.
  void startClock();

  // Reset RTC Clock time and date.
  void resetClock();

  // Get RTC Clock hour mode. True if the clock is in 12 hours mode, false if it's in 24 hours mode.
  bool getClockMode();

  // Set RTC Clock hour mode. True for 12 hours mode and false for 24 hours mode.
  void setClockMode(bool isTwelveHour);

  // Get RTC Clock second.
  int getSecond();

  // Set RTC Clock second.
  void setSecond(int second);

  // Get RTC Clock minute.
  int getMinute();

  // Set RTC Clock minute.
  void setMinute(int minute);

  // Get RTC Clock hour. 
  int getHour();

  // Set RTC Clock hour.
  void setHour(int hour);

  // Get RTC Clock meridien(AM/PM) with 0 as AM and 1 as PM.
  String getMeridien();

  // Get RTC Clock time in hh:mm:ss format.
  String getTime();

  // Get RTC Clock week day.
  String getWeekDay();

  // Set RTC Clock week day. Starting with 1 as Sunday and 7 as Saturday.
  void setWeekDay(String weekDay);

  // Get RTC Clock day.
  int getDay();

  // Set RTC Clock day.
  void setDay(int day);

  // Get RTC Clock month.
  String getMonth();

  // Set RTC Clock month.
  void setMonth(String month);

  // Get RTC Clock year.
  int getYear();

  // Set RTC Clock year.
  void setYear(int year);

  // Get RTC Clock leap year value.
  bool isLeapYear();

  // Set RTC Clock leap year value.
  void setLeapYear(bool isLeapYear);

  // Get RTC Clock Date (week day day month and year).
  String getDate();

  // Get RTC Clock date and time.
  String getDateTime();
}; 
  
#endif