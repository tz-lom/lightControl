#pragma once

#include <Adafruit_PWMServoDriver.h>

namespace LightControl {

  extern Adafruit_PWMServoDriver pwm;

  void setup();
  void loop();

  constexpr auto schedulerTimeout = 37;
  
}
