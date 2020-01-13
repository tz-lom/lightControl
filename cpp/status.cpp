#include "status.h"
#include <vector>

namespace Status
{

std::vector<callbackType> reporters;

void registerStatusReporter(callbackType callback)
{
  reporters.push_back(callback);
}

String getStatus()
{
  StaticJsonBuffer<2048> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  for(auto &callback: reporters)
  {
    callback(root);
  }
  String json;
  root.printTo(json);
  return json;
}

}
