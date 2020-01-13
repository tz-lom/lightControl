#include "lightcontrol.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "settings.h"
#include "rtc.h"
#include "status.h"

namespace LightControl {

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int timer = 0;
const int timeout = 5000;

std::array<uint16_t, Settings::channelsCount> current_channels;

void setPWM(uint8_t channel, uint16_t off_time)
{
  current_channels[channel] = off_time;
  pwm.setPWM(channel, 0, off_time);
}



void reportStatus(JsonObject &root)
{
  JsonArray& channels = root.createNestedArray("channels");
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

  if(Settings::isOverrideChannels())
  {
    const auto &channels = Settings::getOverrideChannels();
    for(int i=0; i<Settings::channelsCount; ++i)
    {
       setPWM(i, channels[i]);
    }
    return;
  }
  int now = RTC::getSeconds();

  Serial.printf("time=%i\n", now);

  
  auto settings = Settings::getLevels();
  Serial.println(settings.size());
  for(auto point=settings.begin(); point!=settings.end(); ++point)
  {
    if(point->time*60 > now)
    {
      auto prev = settings.end()-1;
      if(point != settings.begin())
      {
        prev = point-1;
      }

      for(int i=0; i<Settings::channelsCount; ++i)
      {
        auto val = map(now, prev->time*60, point->time*60, prev->channels[i], point->channels[i]);
        setPWM(i, val);
        Serial.printf("set %i, %i\n", i, val);
      }
      break;
    }
  }
}
  
}
