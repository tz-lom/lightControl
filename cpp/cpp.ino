#include <ESP8266WiFi.h>
#include "settings.h"
#include "webserver.h"
#include <FS.h>

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
  Settings::setup();

  if(Settings::isWiFiAPmode())
  {
    initializeAP();
  }
  else
  {
    // connect as client
    Serial.println("Connecting to WiFi...");
    //WiFi.
  }

  WebServer::setup();
}

void loop()
{
  WebServer::loop();
}
