#include "ServeurWeb.h"
#include "html/index.h"
#include "html/favicon.h"
#include "Data.h"
#include "Dimmer.h"
#include "Heure.h"
#include "Ota.h"
#include "Watts.h"
#include "WiFiSerial.h"
#include <ArduinoJson.h>
// #include <Base64.h>

#include "ServeurWeb.h"

namespace ServeurWeb
{
  using std::size_t;
  using Res = AsyncWebServerResponse;
  using Req = AsyncWebServerRequest;
  using Rst = AsyncResponseStream;
  using Prm = AsyncWebParameter;

  AsyncWebServer server(80);

  AsyncWebServer& getServer()
  {
    return server;
  }

  void begin()
  {
    server.on("/", HTTP_GET, [](Req* req)
    {
      Rst* res = req->beginResponseStream("text/html");
      res->print(html::header);
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
      // FIXME separate differents data calls to reduce req
      // time and please the watchdog.
      unsigned start_json = esp_timer_get_time();

      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["last_boot"] = Data::last_boot;
      root["heap"] = ESP.getFreeHeap();
      root["server_heap"] =
        uxTaskGetStackHighWaterMark(nullptr);

      auto& ota = root.createNestedObject("ota");
      // FIXME Do not use Ecran.h, does not make sense
      ota["updating"] = Ota::updating;
      ota["progress"] = Ota::progress;

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
      data["res2"] = Data::res2;
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

      unsigned end_json = esp_timer_get_time();
      weblogf("JSON time %u\n", (end_json - start_json) / 1000);

      yield();

      unsigned start_req = esp_timer_get_time();

      Rst* res = req->beginResponseStream("application/json");
      root.printTo(*res);
      req->send(res);

      unsigned end_req = esp_timer_get_time();
      weblogf("Req time %u\n", (end_req - start_req) / 1000);
    });

    server.begin();
  }
}
