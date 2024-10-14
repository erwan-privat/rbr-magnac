#include "ServeurWeb.h"

#include "html/index.h"
#include "html/style.h"
#include "html/script.h"
#include "html/favicon.h"
#include "Data.h"
#include "Dimmer.h"
#include "Radiateur.h"
#include "Ecran.h"
#include "Heure.h"
#include "Ota.h"
#include "Watts.h"
#include "WiFiSerial.h"
#include <ESPAsyncWebServer.h>
#include <Base64.h>

// #define INCLUDE_TEST_JSON
#ifdef INCLUDE_TEST_JSON
#include <ArduinoJson.h>
#endif

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

  const char* btoc(bool b)
  {
    return b ? "true" : "false";
  }

  void printArrJson(Rst* stream,
    const float* arr, size_t size)
  {
    stream->print('[');
    stream->print(arr[0]);

    for (size_t i = 1; i < size; ++i) {
      if (i == size / 2)
        yield();

      stream->printf(", %.0f", arr[i]);
    }

    stream->print(']');
  }

  void serveData(Req* req, size_t size, int r, unsigned ix,
    const float* p1_hp, const float* p2_hp,
    const float* p1_hc, const float* p2_hc)
  {
      Rst* res = req->beginResponseStream("application/json");

      res->printf("{\"res\": %d, \"ix\": %d, ", r, ix);

      res->print("\"p1_hp\": ");
      printArrJson(res, p1_hp, size);
      yield();
      res->print(',');
      res->print("\"p1_hc\": ");
      printArrJson(res, p1_hc, size);
      yield();
      res->print(',');
      res->print("\"p2_hp\": ");
      printArrJson(res, p2_hp, size);
      yield();
      res->print(',');
      res->print("\"p2_hc\": ");
      printArrJson(res, p2_hc, size);
      res->print('}');

      req->send(res);
  }


  void begin()
  {
    server.on("/", HTTP_GET, [](Req* req)
    {
      weblog("GET /");
      Rst* res = req->beginResponseStream("text/html");
      res->print(html::index_start);
      res->print(html::style);
      res->print(html::script);
      res->print(html::index_end);
      req->send(res);
    });

    server.on("/favicon.ico", HTTP_GET, [](Req* req)
    {
      weblog("GET /favicon.ico");
      Res* response = req->beginResponse(200,
        "image/x-icon",
        html::favicon_ico, html::favicon_ico_len);
      req->send(response);
    });

    server.on("/robots.txt", HTTP_GET, [](Req* req)
    {
      weblog("Request /robots.txt");
      req->send(200, "text/plain",
          "User-agent: *\n"
          "Disallow: /\n");
    });
    
    server.on("/ota", HTTP_GET, [](Req* req)
    {
      weblog("GET /ota");

      Rst* res = req->beginResponseStream(
        "application/json");
      res->printf("{\"last_boot\": %lu, \"updating\": %s,"
          "\"progress\": %u}",
        Data::last_boot, btoc(Ota::updating),
        Ota::progress);

      req->send(res);
    });
    
    server.on("/watts", HTTP_GET, [](Req* req)
    {
      weblog("GET /watts");

      Rst* res = req->beginResponseStream(
        "application/json");
      res->printf("{\"power1\": %f, \"power2\": %f}",
          Watts::power1, Watts::power2);
      req->send(res);
    });

    server.on("/dimmer", HTTP_GET, [](Req* req)
    {
      weblog("GET /dimmer");

      Rst* res = req->beginResponseStream(
        "application/json");

      res->printf("{\"force_on\": %s, \"force_off\": %s,"
          "\"hc_on\": %s, \"start_hc\": %d, \"end_hc\": %d,"
          "\"time\": %d}",
        btoc(Dimmer::force_on), btoc(Dimmer::force_off),
        btoc(Dimmer::hc_on), Dimmer::start_hc,
        Dimmer::end_hc, Heure::getTimeHMS());

      req->send(res);
    });

    server.on("/screen", HTTP_GET, [](Req* req)
    {
      weblog("GET /screen");
      constexpr auto screen64_size = 1368;
      char screen64[screen64_size + 1];

      auto* screen = (char*)Ecran::getScreen()
        .getBufferPtr();

      Base64.encode(screen64, screen, screen64_size); 

      Rst* res = req->beginResponseStream("text/plain");
      res->print(screen64);
      req->send(res);
    });

    server.on("/data_15min", HTTP_GET, [](Req* req)
    {
      weblog("GET /data_15min");

      if (Ota::updating)
        return;

      int m = esp_timer_get_time();
      int prev_m = m;

      serveData(req,
        Data::size_15min,
        Data::res_15min,
        Data::ix_15min,
        Data::buf_p1_hp_15min,
        Data::buf_p2_hp_15min,
        Data::buf_p1_hc_15min,
        Data::buf_p2_hc_15min);   

      m = esp_timer_get_time();
      weblogf("data_15min %d µs\n", m - prev_m);
    });

    server.on("/data_1h", HTTP_GET, [](Req* req)
    {
      weblog("GET /data_1h");

      if (Ota::updating)
        return;

      int m = esp_timer_get_time();
      int prev_m = m;

      serveData(req,
        Data::size_1h,
        Data::res_1h,
        Data::ix_1h,
        Data::buf_p1_hp_1h,
        Data::buf_p2_hp_1h,
        Data::buf_p1_hc_1h,
        Data::buf_p2_hc_1h);   

      m = esp_timer_get_time();
      weblogf("data_1h %d µs\n", m - prev_m);
    });

    server.on("/data_24h", HTTP_GET, [](Req* req)
    {
      weblog("GET /data_24h");

      if (Ota::updating)
        return;

      int m = esp_timer_get_time();
      int prev_m = m;

      serveData(req,
        Data::size_24h,
        Data::res_24h,
        Data::ix_24h,
        Data::buf_p1_hp_24h,
        Data::buf_p2_hp_24h,
        Data::buf_p1_hc_24h,
        Data::buf_p2_hc_24h);   

      m = esp_timer_get_time();
      weblogf("data_24h %d µs\n", m - prev_m);
    });

    // server.on("/control", HTTP_POST, [](Req* req)
    server.on("/control", HTTP_GET, [](Req* req)
    {
      // weblog("POST /control");
      weblog("GET /control");

      if (req->hasParam("force_off"))
      {
        const Prm* p_off = req->getParam("force_off");
        Dimmer::force_off = p_off->value() == "true";
        weblogf("force_off: %s\n", p_off->value());
      }

      if (req->hasParam("force_on"))
      {
        const Prm* p_on = req->getParam("force_on");
        Dimmer::force_on = p_on->value() == "true";
        weblogf("force_on: %s\n", p_on->value());
      }

      if (req->hasParam("hc_on"))
      {
        const Prm* p = req->getParam("hc_on");
        Dimmer::hc_on = p->value() == "true";
        weblogf("hc_on: %s\n", p->value());
      }

      if (req->hasParam("force_off_radi"))
      {
        const Prm* p_off = req->getParam("force_off_radi");
        Radiateur::force_off = p_off->value() == "true";
        weblogf("force_off_radi: %s\n", p_off->value());
      }

      if (req->hasParam("force_on_radi"))
      {
        const Prm* p_on = req->getParam("force_on_radi");
        Radiateur::force_on = p_on->value() == "true";
        weblogf("force_on_radi: %s\n", p_on->value());
      }

      req->send(200);
    });

#ifdef INCLUDE_TEST_JSON
    server.on("/test-lib", HTTP_GET, [](Req* req)
    {
      weblog("GET /test-lib");

      unsigned long tot = 0;
      unsigned long m;
      unsigned long prev_m = esp_timer_get_time();
      auto chrono = [&](const char* tag)
      {
        m = esp_timer_get_time();
        tot += m - prev_m;
        weblogf("test-lib: %d µs (%s)\n", m - prev_m, tag);
        prev_m = m;
      };

      chrono("start");
      DynamicJsonBuffer jsonBuffer;
      chrono("jsonBuffer");
      JsonObject& root = jsonBuffer.createObject();
      root["power1"] = Watts::power1;
      root["power2"] = Watts::power2;
      root["current1"] = Watts::current1;
      root["voltage1"] = Watts::voltage1;
      root["current2"] = Watts::current2;
      root["voltage2"] = Watts::voltage2;
      root["freq"]     = Watts::frequency;
      chrono("createObject");

      auto& arr1 = root.createNestedArray("arr1");
      for (int i = 0; i < 1000; ++i)
        arr1.add(i);
      chrono("createArray1");
      auto& arr2 = root.createNestedArray("arr2");
      for (int i = 0; i < 1000; ++i)
        arr2.add(i + 1000);
      chrono("createArray2");

      Rst* res = req->beginResponseStream(
        "application/json");
      chrono("beginResponseStream");
      root.printTo(*res);
      chrono("printTo");
      req->send(res);
      chrono("send");
      weblogf("test-lib tot: %d µs\n", tot);
      // jsonBuffer.clear();
    });

    server.on("/test-nolib", HTTP_GET, [](Req* req)
    {
      weblog("GET /test-nolib");

      unsigned long tot = 0;
      unsigned long m;
      unsigned long prev_m = esp_timer_get_time();
      auto chrono = [&](const char* tag)
      {
        m = esp_timer_get_time();
        tot += m - prev_m;
        weblogf("test-nolib: %d µs (%s)\n", m - prev_m, tag);
        prev_m = m;
      };

      chrono("start");
      Rst* res = req->beginResponseStream(
        "application/json");
      chrono("beginResponseStream");

      res->print('{');

      res->printf("\"power1\": %f,",   Watts::power1);
      res->printf("\"power2\": %f,",   Watts::power2);
      res->printf("\"current1\": %f,", Watts::current1);
      res->printf("\"voltage1\": %f,", Watts::voltage1);
      res->printf("\"current2\": %f,", Watts::current2);
      res->printf("\"voltage2\": %f,", Watts::voltage2);
      res->printf("\"freq\": %lu,",    Watts::frequency);
      chrono("printed object");

      res->print("\"arr1\": [0");
      for (int i = 1; i < 1000; ++i)
        res->printf(", %d", i);
      res->print("], \"arr2\": [0");
      for (int i = 1; i < 1000; ++i)
        res->printf(", %d", i + 1000);
      res->print("]}");

      chrono("printed array");

      req->send(res);
      chrono("send");
      weblogf("test-nolib tot: %d µs\n", tot);
    });
#endif

    server.begin();
  }
}
