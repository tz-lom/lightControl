#include "lightcontrol.h"

#include <Wire.h>
#include "settings.h"
#include "rtc.h"
#include "status.h"

namespace LightControl {

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

std::array<uint16_t, Settings::channelsCount> current_channels;

void setPWM(uint8_t channel, uint16_t off_time)
{
  current_channels[channel] = off_time;
  pwm.setPWM(channel, 0, off_time);
}



void reportStatus(JsonObject root)
{
  JsonArray channels = root.createNestedArray("channels");
  for(auto ch: current_channels)
  {
    channels.add(ch);
  }
}

void setup()
{
  pwm.begin();
  pwm.setPWMFreq(200);
  Status::registerStatusReporter(reportStatus);
}


int map(int now, int tBeg, int tEnd, int from, int to)
{
  //Serial.printf("map %i %i %i %i %i\n", now, tBeg, tEnd, from, to);
  
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

  if(Settings::isOverrideChannels())
  {
    const auto &channels = Settings::getOverrideChannels();
    for(int i=0; i<Settings::channelsCount; ++i)
    {
       setPWM(i, channels[i]);
    }
    return;
  }
  int now = RTC::getMilliseconds();

  //Serial.printf("time=%i\n", now);

  
  auto settings = Settings::getLevels();
 // Serial.println(settings.size());
  for(auto point=settings.begin(); point!=settings.end(); ++point)
  {
    if(point->time*60*1000 > now)
    {
      auto prev = settings.end()-1;
      if(point != settings.begin())
      {
        prev = point-1;
      }

      for(int i=0; i<Settings::channelsCount; ++i)
      {
        auto val = map(now, (prev==(settings.end()-1))?0:prev->time*60*1000, point->time*60*1000, prev->channels[i], point->channels[i]);
        setPWM(i, val);
        //Serial.printf("set %i, %i\n", i, val);
      }
      break;
    }
  }
}
  
}
