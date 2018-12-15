#include "lightcontrol.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "settings.h"
#include "rtc.h"

namespace LightControl {

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int timer = 0;
const int timeout = 5000; 

void setup()
{
  pwm.begin();
  pwm.setPWMFreq(1000);
}


int map(int now, int tBeg, int tEnd, int from, int to)
{
  if(tBeg==tEnd)
  {
    return to;
  }
  else
  {
    return from + (to-from)*(now-tBeg)/(tEnd-tBeg);
  }
}

void loop()
{
  if(timer-- > 0) return;
  timer = timeout;
  
  int now = RTC::getMinutes();

  Serial.printf("time=%i\n", now);

  
  auto settings = Settings::getLevels();
  for(auto point=settings.begin(); point!=settings.end(); ++point)
  {
    if(point->time>now)
    {
      auto prev = settings.end()-1;
      if(point != settings.begin())
      {
        prev = point-1;
      }

      for(int i=0; i<Settings::channelsCount; ++i)
      {
        auto val = map(now, prev->time, point->time, prev->channels[i], point->channels[i]);
        pwm.setPWM(i, 0, val);
        Serial.printf("set %i, %i\n", i, val);
      }
      break;
    }
  }
}
  
}
