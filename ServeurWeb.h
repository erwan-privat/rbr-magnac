// eprivat 2024-09-10

#ifndef SERVEURWEB_H
#define SERVEURWEB_H

#include <ESPAsyncWebServer.h>

namespace ServeurWeb
{
  AsyncWebServer server(80);

  void begin()
  {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain",
        "RBR Magnac, serveur en ligne");
    });
    server.begin();
  }
}

#endif /* SERVEURWEB_H */
