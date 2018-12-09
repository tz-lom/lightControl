#include "webserver.h"
#include <ESP8266WebServer.h>
#include <FS.h>
#include <functional>
#include "settings.h"

namespace WebServer {
ESP8266WebServer server(80);

std::function<void()> serveFile(const char* fname, const char* contentType)
{
  return [fname, contentType](){
    File file = SPIFFS.open(fname, "r");
    if(!file)
    {
      server.send(404, "text/html", "Not found");
      return;
    }

    server.setContentLength(file.size());
    server.send_P(200, contentType, "");
  
    uint8_t buffer[1200];
    while(auto size = file.read(buffer, sizeof(buffer)))
    {
        server.sendContent_P((const char*)buffer, size);
    }
    file.close();
  };
}

void setup()
{
  server.on("/", serveFile("/index.htm", "text/html"));
  server.on("/d3.v4.min.js", serveFile("/d3.v4.min.js", "text/javascript"));
  server.on("/settings", HTTP_GET, [](){
    server.send(200, "application/json", Settings::getConfigAsJson());
  });

  server.on("/settings/save", HTTP_POST, [](){
    Settings::setConfigFromJson(server.arg("plain"));
    Settings::save();
    server.send(200, "text/html", "OK");
  });

  server.begin();

}

void loop()
{
  server.handleClient();
}

}
