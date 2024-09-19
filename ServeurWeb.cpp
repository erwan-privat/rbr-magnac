#include "ServeurWeb.h"

#include "html/index.h"
#include "html/favicon.h"
#include "Data.h"
#include "Dimmer.h"
#include "Ecran.h"
#include "Heure.h"
#include "Ota.h"
#include "Watts.h"
#include "WiFiSerial.h"

#include <ArduinoJson.h>

#define CONFIG_ASYNC_TCP_USE_WDT 1
#include <ESPAsyncWebServer.h>
#include <Base64.h>

namespace ServeurWeb
{
  using std::size_t;
  using Res = AsyncWebServerResponse;
  using Req = AsyncWebServerRequest;
  using Rst = AsyncResponseStream;
  // using Prm = AsyncWebParameter;

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
      res->print(html::content);
      req->send(res);
    });

    server.on("/favicon.ico", HTTP_GET, [](Req* req)
    {
      Res* response = req->beginResponse_P(200,
        "image/x-icon",
        html::favicon_ico, html::favicon_ico_len);
      req->send(response);
    });
    
    server.on("/ota", HTTP_GET, [](Req* req)
    {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["last_boot"] = Data::last_boot;
      root["updating"]  = Ota::updating;
      root["progress"]  = Ota::progress;

      Rst* res = req->beginResponseStream(
        "application/json");
      root.printTo(*res);
      req->send(res);
    });
    
    server.on("/watts", HTTP_GET, [](Req* req)
    {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["power1"] = Watts::power1;
      root["power2"] = Watts::power2;
      // root["current1"] = Watts::current1;
      // root["voltage1"] = Watts::voltage1;
      // root["current2"] = Watts::current2;
      // root["voltage2"] = Watts::voltage2;
      // root["freq"]     = Watts::frequency;

      Rst* res = req->beginResponseStream(
        "application/json");
      root.printTo(*res);
      req->send(res);
    });

    server.on("/dimmer", HTTP_GET, [](Req* req)
    {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["force_off"] = Dimmer::force_off;
      root["force_on"]  = Dimmer::force_on;
      root["start_hp"]  = Dimmer::start_hc;
      root["end_hp"]    = Dimmer::end_hc;
      root["time"]      = Heure::getTimeHMS();

      Rst* res = req->beginResponseStream(
        "application/json");
      root.printTo(*res);
      req->send(res);
    });

    server.on("/screen", HTTP_GET, [](Req* req)
    {
      constexpr auto screen64_size = 1368;
      char screen64[screen64_size + 1];

      auto* screen = (char*)Ecran::getScreen()
        .getBufferPtr();

      Base64.encode(screen64, screen, screen64_size); 

      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["length"] = screen64_size;
      auto& arr_screen = root.createNestedArray("xbmp64");
      for (auto pix : screen64)
        arr_screen.add(pix);

      Rst* res = req->beginResponseStream(
        "application/json");
      root.printTo(*res);
      req->send(res);
    });

    server.on("/data_15min", HTTP_GET, [](Req* req)
    {
      if (Ota::updating)
        return;

      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["res"] = Data::res_15min;
      root["ix"] = Data::ix_15min;
      auto& arr_p1_hp = root.createNestedArray("p1_hp");
      for (auto f : Data::buf_p1_hp_15min)
        arr_p1_hp.add(f);
      auto& arr_p2_hp = root.createNestedArray("p2_hp");
      for (auto f : Data::buf_p2_hp_15min)
        arr_p2_hp.add(f);
      auto& arr_p1_hc = root.createNestedArray("p1_hc");
      for (auto f : Data::buf_p1_hc_15min)
        arr_p1_hc.add(f);
      auto& arr_p2_hc = root.createNestedArray("p2_hc");
      for (auto f : Data::buf_p2_hc_15min)
        arr_p2_hc.add(f);

      Rst* res = req->beginResponseStream(
        "application/json");
      root.printTo(*res);
      req->send(res);
    });

    server.on("/data_1h", HTTP_GET, [](Req* req)
    {
      if (Ota::updating)
        return;

      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["res"] = Data::res_1h;
      root["ix"] = Data::ix_1h;
      auto& arr_p1_hp = root.createNestedArray("p1_hp");
      for (auto f : Data::buf_p1_hp_1h)
        arr_p1_hp.add(f);
      auto& arr_p2_hp = root.createNestedArray("p2_hp");
      for (auto f : Data::buf_p2_hp_1h)
        arr_p2_hp.add(f);
      auto& arr_p1_hc = root.createNestedArray("p1_hc");
      for (auto f : Data::buf_p1_hc_1h)
        arr_p1_hc.add(f);
      auto& arr_p2_hc = root.createNestedArray("p2_hc");
      for (auto f : Data::buf_p2_hc_1h)
        arr_p2_hc.add(f);

      Rst* res = req->beginResponseStream(
        "application/json");
      root.printTo(*res);
      req->send(res);
    });

    server.on("/data_24h", HTTP_GET, [](Req* req)
    {
      if (Ota::updating)
        return;

      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["res"] = Data::res_24h;
      root["ix"] = Data::ix_24h;
      auto& arr_p1_hp = root.createNestedArray("p1_hp");
      for (auto f : Data::buf_p1_hp_24h)
        arr_p1_hp.add(f);
      auto& arr_p2_hp = root.createNestedArray("p2_hp");
      for (auto f : Data::buf_p2_hp_24h)
        arr_p2_hp.add(f);
      auto& arr_p1_hc = root.createNestedArray("p1_hc");
      for (auto f : Data::buf_p1_hc_24h)
        arr_p1_hc.add(f);
      auto& arr_p2_hc = root.createNestedArray("p2_hc");
      for (auto f : Data::buf_p2_hc_24h)
        arr_p2_hc.add(f);

      Rst* res = req->beginResponseStream(
        "application/json");
      root.printTo(*res);
      req->send(res);
    });

    server.begin();
  }
}
