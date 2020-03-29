#pragma once

namespace RTC
{
  void setup();
  void loop();

  constexpr auto schedulerTimeout = 5623;

  int getMilliseconds();  
  int getSeconds();
  void setTime(int hour, int minute, int second);

  
}
