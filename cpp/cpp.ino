#include <ESP8266WiFi.h>
#include "settings.h"
#include "webserver.h"
#include "rtc.h"
#include "termometer.h"
#include "lightcontrol.h"
#include <FS.h>
#include <Wire.h>

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

void loop()
{
  WebServer::loop();
  LightControl::loop();
  Termometer::loop();
}
