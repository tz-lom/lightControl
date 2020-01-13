#pragma once
#include <ArduinoJson.h>
#include <functional>

namespace Status
{

using callbackType = std::function<void(JsonObject &root)>;

void registerStatusReporter(callbackType callback);

String getStatus();


}
