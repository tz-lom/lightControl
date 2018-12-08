#include "settings.h"
#include <FS.h>
#include <ArduinoJson.h>
#include <algorithm>


namespace Settings {

class Config {
public:
  String wifiSSID;
  String wifiPassword;

  std::vector<powerPoint> levels;
};

Config config;

bool isWiFiAPmode()
{
  return true;
}

void load();

void setup()
{
  config.wifiSSID = "";
  config.wifiPassword = "";
  config.levels.push_back({23*60+59, 50, { 2048, 2048, 2048, 2048, 2048 }});
  load();
}

void load()
{
  File file = SPIFFS.open("/config.json", "r");
  setConfigFromJson(file.readString());
  file.close();
}

void save()
{
  File file = SPIFFS.open("/config.json", "w");
  file.print(getConfigAsJson());
  file.close();
}

void setConfigFromJson(const String& json)
{
  StaticJsonBuffer<2048> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if(!root.success()) return; // failed to parse

  config.wifiSSID = root.get<String>("ssid");
  config.wifiPassword = root.get<String>("password");

  JsonArray &levels = root["levels"];

  config.levels.clear();
  for(auto &level : levels)
  {
    powerPoint point;
    point.time = level["time"];
    point.power = level["power"];
    JsonArray& channels = level["c"];
    for(int i=0; i<std::min((size_t)channelsCount, channels.size()); ++i)
    {
      point.channels[i] = std::max(0, std::min(4095, channels[i].as<int>()));
    }
    config.levels.push_back(point);
  }
  
}

String getConfigAsJson()
{
  StaticJsonBuffer<2048> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["ssid"] = config.wifiSSID;
  root["password"] = config.wifiPassword;
  root["maxChannel"] = channelsCount;
  JsonArray& levels = root.createNestedArray("levels");
  for(auto& levelData: config.levels)
  {
    JsonObject& level = levels.createNestedObject();
    level["time"] = levelData.time;
    level["power"] = levelData.power;
    JsonArray& channels = level.createNestedArray("c");
    for(int i=0; i< channelsCount; ++i)
    {
      channels.add(levelData.channels[i]);
    }
  }
  String json;
  root.printTo(json);
  return json;
}

}

