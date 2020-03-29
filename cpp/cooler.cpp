#include "cooler.hpp"

#include "lightcontrol.h"
#include "termometer.h"
#include "settings.h"
#include "status.h"

namespace Cooler {

constexpr auto coolerChannel=5;

float u = 0;
float e = 0;
float e1 = 0;
float e2 = 0;

void reportStatus(JsonObject root)
{
  root["pid_u"] = u;
}

void setup()
{
  Status::registerStatusReporter(&reportStatus);
}

void loop()
{
  const auto k = Settings::getPID();

  e2 = e1;
  e1 = e;
  e = k.x0 - Termometer::temperature;

  u += k.p*(e-e1) + k.i*e + k.d*(e-2*e1+e2);

  const uint16_t off_time = (u<0?0:(u>1?1:u))*4095;

  LightControl::pwm.setPWM(coolerChannel, 0, off_time);
}

}
