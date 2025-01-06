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

  // Get RTC Clock second value.
  int getSecond();

  // Set RTC Clock second value.
  void setSecond(int second);

  // Get RTC Clock minute value.
  int getMinute();

  // Set RTC Clock minute value.
  void setMinute(int minute);

  // Get RTC Clock hour value. 
  int getHour();

  // Get RTC Clock meridien(AM/PM) value with 0 as AM and 1 as PM.
  String getMeridien();

  // Set RTC Clock hour value.
  void setHour(int hour);

  // Get RTC Clock time in hh:mm:ss format.
  String getTime();

  // get RTC Clock week day.
  String getWeekDay();

  // Set RTC Clock week day. Starting with 1 as Sunday and 7 as Saturday.
  void setWeekDay(int dayNum);

  // Get RTC Clock hour mode. True if the clock is in 12 hours mode, false if it's in 24 hours mode.
  bool getClockMode();

  // Set RTC Clock hour mode. True for 12 hours mode and false for 24 hours mode.
  void setClockMode(bool isTwelveHour);
}; 
  
#endif