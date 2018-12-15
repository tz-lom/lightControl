#include "termometer.h"
#include <OneWire.h>
#include <DallasTemperature.h>


namespace Termometer
{

  OneWire oneWire(D3);
  DallasTemperature termosensor(&oneWire);

  int timer = 0;
  const int timeout = 500; 

  
  void setup()
  {
      termosensor.begin();

  }

  void loop()
  {
    if(timer-- > 0) return;
    timer = timeout;
  
    termosensor.requestTemperatures();
    termosensor.getTempCByIndex(0);
  }
}

