#include "lightcontrol.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

namespace LightControl {

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup()
{
  //set I2C
  Wire.begin(D1,D2);
  Wire.setClock(100000);

  pwm.begin();
  pwm.setPWMFreq(1000);
}

void loop()
{
  
}
  
}

