#pragma once
#include <WString.h>
#include <vector>
#include <array>

namespace Settings 
{
  constexpr int channelsCount = 5;

  struct powerPoint {
    int time;
    int power;
    int channels[channelsCount];
  };
  
  void setup();
  
  bool isWiFiAPmode();
  String getSSID();
  String getPassword();

  void save();

  void setConfigFromJson(const String& json);
  String getConfigAsJson();

  std::vector<powerPoint> getLevels();

  bool isOverrideChannels();
  std::array<int, channelsCount> getOverrideChannels();

}
