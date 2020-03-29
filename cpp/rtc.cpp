#include "rtc.h"
#include <Wire.h>
#include <DS3231.h>

namespace RTC
{
  constexpr bool useInternalTimer = false;
  
  DS3231 Clock;

  unsigned long millisecondsOffset = 0;
  constexpr auto millisecondsPerDay = 24*60*60*1000;

  void setup()
  {
    if(!useInternalTimer)
    {
      Clock.setClockMode(false);
    }
  }

  void loop()
  {
    const auto seconds = getSeconds();
    const auto milliseconds = millis() % millisecondsPerDay;

    millisecondsOffset += (seconds - div(milliseconds, 1000).quot)*1000;
  }

  void setTime(int hour, int minute, int second)
  {
    if(!useInternalTimer)
    {
      Clock.setHour(hour);
      Clock.setMinute(minute);
      Clock.setSecond(second);
      Clock.setDate(1);
      Clock.setMonth(1);
      Clock.setYear(2020);
    }
    loop();
  }
  
  int getSeconds()
  {
    if(!useInternalTimer)
    {
      bool foo,bar;
      return Clock.getSecond() + Clock.getMinute()*60 + Clock.getHour(foo,bar)*60*60;
    }
    else
    {
      return (millis()/1000) % (60*60*24);
    }
  }

  int getMilliseconds()
  {
    const auto now = millis();
    return now%millisecondsPerDay + millisecondsOffset;
  }

}
