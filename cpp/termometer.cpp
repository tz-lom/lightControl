#include "termometer.h"
#include <OneWire.h>
#include <DallasTemperature.h>


namespace Termometer
{

  OneWire oneWire(D3);
  DallasTemperature termosensor(&oneWire);

  
  void setup()
  {
      termosensor.begin();

  }

  void loop()
  {
    termosensor.requestTemperatures();
    termosensor.getTempCByIndex(0);
  }
}

