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
  StaticJsonDocument<512> jsonDocument;
  JsonObject root = jsonDocument.to<JsonObject>();

  for(auto &callback: reporters)
  {
    callback(root);
  }
  String json;
  serializeJson(jsonDocument, json);
  return json;
}

}
