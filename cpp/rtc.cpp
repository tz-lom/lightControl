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
  }
  
  int getMinutes()
  {
    bool foo,bar;
    return Clock.getMinute() + Clock.getHour(foo,bar)*60;
  }

}

