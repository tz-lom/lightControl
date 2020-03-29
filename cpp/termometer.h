#pragma once
namespace Termometer
{
  extern float temperature;
  
  void setup();
  void loop();

  constexpr auto schedulerTimeout = 1009;

}
