#include "rtc.h"
#include <Wire.h>
#include <DS3231.h>

namespace RTC
{
  DS3231 Clock;

  void setup()
  {
    Clock.setClockMode(false);
  }

  void setTime(int hour, int minute, int second)
  {
    Clock.setHour(hour);
    Clock.setMinute(minute);
    Clock.setSecond(second);
    Clock.setDate(1);
    Clock.setMonth(1);
    Clock.setYear(2019);
  }
  
  int getMinutes()
  {
    bool foo,bar;
    return Clock.getMinute() + Clock.getHour(foo,bar)*60;
  }
  
  int getSeconds()
  {
    bool foo,bar;
    return Clock.getSecond() + Clock.getMinute()*60 + Clock.getHour(foo,bar)*60*60;
  }

}
