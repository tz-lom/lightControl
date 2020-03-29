#include <ESP8266WiFi.h>
#include "settings.h"
#include "webserver.h"
#include "rtc.h"
#include "termometer.h"
#include "lightcontrol.h"
#include "cooler.hpp"
#include <FS.h>
#include <Wire.h>
#include <array>

void initializeAP()
{
  Serial.println("Setting soft-AP ... ");
  
  IPAddress local_ip(192,168,100,1);
  IPAddress gateway(192,168,100, 1);
  IPAddress subnet(255,255,255,0);

  Serial.print("Setting IP config ....... ");
  Serial.println(WiFi.softAPConfig(local_ip, gateway, subnet) ? "OK" : "FAIL");

  Serial.print("Starting Access Point ... ");
  Serial.println(WiFi.softAP("Light controller configuration") ? "OK" : "FAIL");
}

void setup()
{
  SPIFFS.begin();
  Serial.begin(115200);
  Serial.println();
  //set I2C
  Wire.begin(D1,D2);
  Wire.setClock(100000);
  
  Settings::setup();
  RTC::setup();
  Termometer::setup();
  WebServer::setup();
  LightControl::setup();
  Cooler::setup();



  if(Settings::isWiFiAPmode())
  {
    initializeAP();
  }
  else
  {
    // connect as client
    Serial.print("Connecting to WiFi ");
    Serial.print(Settings::getSSID().c_str());
    Serial.print(" : ");
    Serial.println(Settings::getPassword().c_str());
    WiFi.mode(WIFI_STA);
    Serial.println(WiFi.begin(Settings::getSSID().c_str(), Settings::getPassword().c_str()) ? "OK" : "FAIL");

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");

      loop();
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    WiFi.hostname("lightController");
  }
}

struct TaskDefinition
{
  const std::function<void()> callback;
  const unsigned long timeout;
  unsigned long passed;

  TaskDefinition(std::function<void()> callback, unsigned long timeout):
    callback(callback), timeout(timeout), passed(0) {}

  void loop(unsigned long _passed)
  {
    passed += _passed;
    if(timeout < passed)
    {
      passed = 0;
      callback();
    }
  }
};

std::array<TaskDefinition, 4> tasks ={
  TaskDefinition{&WebServer::loop, WebServer::schedulerTimeout}, 
  TaskDefinition{&LightControl::loop, LightControl::schedulerTimeout}, 
  TaskDefinition{[](){Termometer::loop(); Cooler::loop(); }, Termometer::schedulerTimeout},
  TaskDefinition{&RTC::loop, RTC::schedulerTimeout}
  };

void loop()
{
  const auto now = millis();
  static auto prev = now;

  const auto passed = now>=prev?now-prev:now;
  prev = now;
  
//  Serial.print(prev);
//  Serial.print(' ');
//  Serial.print(now);
//  Serial.print(' ');
//  Serial.println(passed);

  for(auto &task: tasks)
  {
    task.loop(passed);
  }
}
