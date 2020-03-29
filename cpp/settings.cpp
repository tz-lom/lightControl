#include "settings.h"
#include <FS.h>
#include <ArduinoJson.h>
#include <algorithm>
#include "rtc.h"

namespace Settings {

class Config {
public:
  String wifiSSID;
  String wifiPassword;

  std::vector<powerPoint> levels;

  bool isOverrideChannels;
  std::array<int, channelsCount> overrideChannels;

  PID pid;

};

Config config;

bool isWiFiAPmode()
{
  return config.wifiSSID=="";
}

String getSSID()
{
  return config.wifiSSID;
}

String getPassword()
{
  return config.wifiPassword;
}

PID getPID()
{
  return config.pid;
}

void load();

void setup()
{
  config.wifiSSID = "";
  config.wifiPassword = "";
  config.levels.push_back({23*60+59, 50, { 2048, 2048, 2048, 2048, 2048 }});
  config.isOverrideChannels = false;
  config.pid = {40, 1/40,0,0};
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

constexpr int jsonBufferSize = 1024*20;

void setConfigFromJson(const String& json)
{
  Serial.print("Set: ");
  Serial.println(json);

  
  DynamicJsonDocument jsonDocument(jsonBufferSize);
  if(deserializeJson(jsonDocument, json)) return;


  config.wifiSSID = jsonDocument["ssid"].as<String>();
  config.wifiPassword = jsonDocument["password"].as<String>();

  {
    auto pid = jsonDocument["pid"];
    config.pid.x0 = pid["x0"].as<float>();
    config.pid.p = pid["p"].as<float>();
    config.pid.i = pid["i"].as<float>();
    config.pid.d = pid["d"].as<float>();
  }

  JsonArray levels = jsonDocument["levels"];

  if(levels.size()>0)
  {
    config.levels.clear();
    for(auto level : levels)
    {
      powerPoint point;
      point.time = level["time"];
      point.power = level["power"];
      JsonArray channels = level["c"];
      for(int i=0; i<std::min((size_t)channelsCount, channels.size()); ++i)
      {
        point.channels[i] = std::max(0, std::min(4095, channels[i].as<int>()));
      }
      config.levels.push_back(point);
    }
  }

  JsonArray time = jsonDocument["time"];
  if(time.size()==3)
  {
    RTC::setTime(time[0], time[1], time[2]);
  }

  JsonObject override = jsonDocument["override"];
  JsonObject overrideChannels = override["channels"];


  config.isOverrideChannels = overrideChannels["enabled"];
  JsonArray overrideValues = overrideChannels["values"];
  for(int i=0; i<std::min(channelsCount, (int)overrideValues.size()); ++i)
  {
    config.overrideChannels[i] = overrideValues[i];
  }
  
}

String getConfigAsJson()
{
  DynamicJsonDocument root(jsonBufferSize);
  root["ssid"] = config.wifiSSID;
  root["password"] = config.wifiPassword;
  root["maxChannel"] = channelsCount;
  JsonArray levels = root.createNestedArray("levels");
  for(auto levelData: config.levels)
  {
    JsonObject level = levels.createNestedObject();
    level["time"] = levelData.time;
    level["power"] = levelData.power;
    JsonArray channels = level.createNestedArray("c");
    for(int i=0; i< channelsCount; ++i)
    {
      channels.add(levelData.channels[i]);
    }
  }

  JsonObject override_channels = root.createNestedObject("override").createNestedObject("channels");
  override_channels["enabled"] = config.isOverrideChannels;
  JsonArray overrideValues = override_channels.createNestedArray("values");
  for(auto c : config.overrideChannels)
  {
    overrideValues.add(c);
  }

  auto pid = root.createNestedObject("pid");
  pid["x0"] = config.pid.x0;
  pid["p"] = config.pid.p;
  pid["i"] = config.pid.i;
  pid["d"] = config.pid.d;
  
  String json;
  serializeJson(root, json);

  Serial.print("Get: ");
  Serial.println(json);
  
  return json;
}

std::vector<powerPoint> getLevels()
{
  return config.levels;
}

bool isOverrideChannels()
{
  return config.isOverrideChannels;
}

std::array<int, channelsCount> getOverrideChannels()
{
  return config.overrideChannels;
}


}
