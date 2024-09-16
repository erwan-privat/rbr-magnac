// eprivat 2024-09-10

#ifndef SERVEURWEB_H
#define SERVEURWEB_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Base64.h>
#include "Watts.h"
#include "Data.h"
#include "Ecran.h"
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
      // weblog("Requesting /");
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
      Res* response = req->beginResponse_P(200,
        "image/x-icon",
        html::favicon_ico, html::favicon_ico_len);
      req->send(response);
    });

    server.on("/data", HTTP_GET, [](Req* req)
    {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["last_boot"] = Data::last_boot;
      root["heap"] = ESP.getFreeHeap();
      root["server_heap"] =
        uxTaskGetStackHighWaterMark(nullptr);

      auto& ota = root.createNestedObject("ota");
      // FIXME Do not use Ecran.h, does not make sense
      ota["updating"] = Ecran::ota_updating;
      ota["progress"] = Ecran::ota_progress;

      auto& watts = root.createNestedObject("watts");
      watts["power1"] = Watts::power1;
      watts["power2"] = Watts::power2;
      // watts["current1"] = Watts::current1;
      // watts["voltage1"] = Watts::voltage1;
      // watts["current2"] = Watts::current2;
      // watts["voltage2"] = Watts::voltage2;
      // watts["freq"]     = Watts::frequency;

      auto& dimmer = root.createNestedObject("dimmer");
      dimmer["force_off"] = Dimmer::force_off;
      dimmer["force_on"]  = Dimmer::force_on;
      dimmer["start_hc"]  = Dimmer::start_hc;
      dimmer["end_hc"]    = Dimmer::end_hc;
      dimmer["time"]      = Heure::getTimeHM();

      auto& data = root.createNestedObject("data");
      data["res2"]   = Data::res2;
      data["ix2"] = Data::ix2;
      auto& arr_p1_2 = data.createNestedArray("p1_2");
      for (auto f : Data::buf_p1_2)
        arr_p1_2.add(f);
      auto& arr_p2_2 = data.createNestedArray("p2_2");
      for (auto f : Data::buf_p2_2)
        arr_p2_2.add(f);

      data["res180"] = Data::res180;
      data["ix180"] = Data::ix180;
      auto& arr_p1_180 = data.createNestedArray("p1_180");
      for (auto f : Data::buf_p1_180)
        arr_p1_180.add(f);
      auto& arr_p2_180 = data.createNestedArray("p2_180");
      for (auto f : Data::buf_p2_180)
        arr_p2_180.add(f);

      Rst* res = req->beginResponseStream("application/json");
      root.printTo(*res);
      req->send(res);
    });

    // weblog("Server begin");
    server.begin();
  }
}

#endif /* SERVEURWEB_H */
