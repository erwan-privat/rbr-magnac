// eprivat 2024-09-10

#ifndef SERVEURWEB_H
#define SERVEURWEB_H

#include <ESPAsyncWebServer.h>
#include "Watts.h"
#include "html/index.h"

namespace ServeurWeb
{
  using Res = AsyncWebServerResponse;
  using Req = AsyncWebServerRequest;
  using Rst = AsyncResponseStream;

  AsyncWebServer server(80);

  // String processor(const String& var)
  // {
    // if (var == "CONSO")
    // {
      // return String(Watts::power2);
    // }
// 
    // return String();
  // }

  void process(String& tpl)
  {
    tpl.replace("$CONSO$", String(Watts::power2));
  }

  void begin()
  {
    server.on("/", HTTP_GET, [](Req* request)
    {
      String tpl(html::index);
      process(tpl);

      Rst* res = request->beginResponseStream("text/html");
      res->print(html::header);
      res->print(tpl);
      res->print(html::footer);
      request->send(res);
    });

    server.begin();
  }
}

#endif /* SERVEURWEB_H */
