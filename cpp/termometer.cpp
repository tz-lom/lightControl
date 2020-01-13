#include "termometer.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "status.h"


namespace Termometer
{

  OneWire oneWire(D3);
  DallasTemperature termosensor(&oneWire);

  int timer = 0;
  const int timeout = 50000;

  float temperature = 0.0;


  void reportStatus(JsonObject &root)
  {
    root["temperature"] = temperature;
  }
  
  void setup()
  {
      termosensor.begin();
      Status::registerStatusReporter(&reportStatus);
  }

  void loop()
  {
    if(timer-- > 0) return;
    timer = timeout;
  
    termosensor.requestTemperatures();
    temperature = termosensor.getTempCByIndex(0);
  }
}

