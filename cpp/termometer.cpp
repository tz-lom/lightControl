#include "termometer.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "status.h"


namespace Termometer
{

  OneWire oneWire(D3);
  DallasTemperature termosensor(&oneWire);

  float temperature = 0.0;


  void reportStatus(JsonObject root)
  {
    root["temperature"] = temperature;
  }
  
  void setup()
  {
      termosensor.begin();
      termosensor.setResolution(12);
      termosensor.setWaitForConversion(true);
      Status::registerStatusReporter(&reportStatus);
  }

  void loop()
  {
    temperature = termosensor.getTempCByIndex(0);
    termosensor.requestTemperatures();
  }
}
