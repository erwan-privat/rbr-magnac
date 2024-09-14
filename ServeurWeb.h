// eprivat 2024-09-10

#ifndef SERVEURWEB_H
#define SERVEURWEB_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Base64.h>
#include "Ecran.h"
#include "Watts.h"
#include "html/index.h"
#include "html/favicon.h"

namespace ServeurWeb
{
  using std::size_t;
  using Res = AsyncWebServerResponse;
  using Req = AsyncWebServerRequest;
  using Rst = AsyncResponseStream;
  using Prm = AsyncWebParameter;

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

  // void process(String& tpl)
  // {
    // tpl.replace("$SCREEN$",
      // (const char*)Ecran::screen.getBufferPtr());
    // tpl.replace("$CONSO$", String(Watts::power2));
  // }

  void begin()
  {
    server.on("/", HTTP_GET, [](Req* req)
    {
      weblog("Requesting /");
      // String tpl(html::index);
      // process(tpl);

      Rst* res = req->beginResponseStream("text/html");
      res->print(html::header);
      // res->print(tpl);
      res->print(html::index);
      res->print(html::footer);
      req->send(res);
    });

    server.on("/favicon.ico", HTTP_GET, [](Req* req)
    {
      weblog("Requesting /favicon.ico");
      Res* response = req->beginResponse_P(200,
        "image/x-icon",
        html::favicon_ico, html::favicon_ico_len);
      req->send(response);
      // req->send_P(200, "image/x-icon", html::favicon_ico,
        // html::favicon_ico_len);
    });

    // server.on("/data", HTTP_GET, [](Req* req)
    // {
      // constexpr auto paramName = "name";
      // char buffer[32];
// 
      // if (req->hasParam(paramName))
      // {
        // Prm* parameter = req->getParam(paramName);
        // const char* p = parameter->value().c_str();
// 
        // if (!strcmp(p, "power1"))
        // {
          // std::sprintf(buffer, "%f", Watts::power1);
          // req->send(200, "text/plain", buffer);
        // }
        // else if (!strcmp(p, "power2"))
        // {
          // std::sprintf(buffer, "%f", Watts::power2);
          // req->send(200, "text/plain", buffer);
        // }
        // else if (!strcmp(p, "screen"))
        // {
          // req->send("application/octet-stream",
            // Ecran::screen_w * Ecran::screen_h / 8,
            // [](uint8_t* buf, size_t maxlen, size_t index)
              // -> size_t
            // {
              // memcpy(buf, Ecran::screen.getBufferPtr(),
                // maxlen);
              // return maxlen;
            // });
        // }
        // else if (!strcmp(p, "winie"))
        // {
          // // const byte* screen_buf = Ecran::screen.getBufferPtr();
          // // Res* response = req->beginResponse(200,
            // // "application/octet-stream",
            // // (byte*)screen_buf,
            // // Ecran::screen_w * Ecran::screen_h / 8);
          // // req->send(response);
          // req->send("application/octet-stream",
            // Ecran::screen_w * Ecran::screen_h / 8,
            // [](uint8_t* buf, size_t maxlen, size_t index)
              // -> size_t
            // {
              // memcpy(buf, bmp_winie, maxlen);
              // return maxlen;
            // });
        // }
      // }
    // });
    
    server.on("/data", HTTP_GET, [](Req* req)
    {
      weblog("Requesting /data");

      Rst* res = req->beginResponseStream("application/json");
      // constexpr auto screen_len = Ecran::screen_w * Ecran::screen_h;
      // constexpr auto screen_len64 = 1392;
      // char screen_buf64[screen_len64];
      // Base64.encode(screen_buf64,
        // (char*)Ecran::screen.getBufferPtr(), screen_len);

      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["heap"]     = ESP.getFreeHeap();
      root["ssid"]     = WiFi.SSID();
      root["power1"]   = Watts::power1;
      root["current1"] = Watts::current1;
      root["voltage1"] = Watts::voltage1;
      root["power2"]   = Watts::power2;
      root["current2"] = Watts::current2;
      root["voltage2"] = Watts::voltage2;
      root["freq"]     = Watts::frequency;
      // root["screen64"] = screen_buf64;
      root.printTo(*res);
      req->send(res);
    });

    weblog("Server begin");
    server.begin();
  }
}

#endif /* SERVEURWEB_H */
