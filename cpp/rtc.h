#pragma once

namespace RTC
{
  void setup();
  
  int getMinutes();
  int getSeconds();
  void setTime(int hour, int minute, int second);
}

