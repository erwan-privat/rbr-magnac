#include "ServeurWeb.h"

#include "html/index.h"
#include "html/style.h"
#include "html/script.h"
#include "html/favicon.h"
#include "Data.h"
#include "Dimmer.h"
#include "ChartData.h"
#include "Ecran.h"
#include "Heure.h"
#include "Radiateur.h"
#include "Ota.h"
#include "Watts.h"
#include "WiFiSerial.h"
#include <ESPAsyncWebServer.h>
// #include <Base64.h>

// #define INCLUDE_TEST_JSON
#ifdef INCLUDE_TEST_JSON
#include <ArduinoJson.h>
#endif

// Stringify for JSON
#define S(cc) "\"" cc "\""

namespace ServeurWeb
{
  using std::size_t;
  using Res = AsyncWebServerResponse;
  using Req = AsyncWebServerRequest;
  using Rst = AsyncResponseStream;
  using Prm = AsyncWebParameter;
  using Data::Category;


  AsyncWebServer server(80);

  AsyncWebServer& getServer()
  {
    return server;
  }

  const char* btoc(bool b)
  {
    return b ? "true" : "false";
  }

  // void printArrJson(Rst* stream, const float* arr, size_t size)
  // {
  //   stream->print('[');
  //   stream->print(arr[0]);

  //   for (size_t i = 1; i < size; ++i)
  //     stream->printf(", %.0f", arr[i]);

  //   stream->print(']');
  // }

  void printArrJson(Rst* stream, const std::vector<float>& arr)
  {
    stream->print('[');
    stream->print(arr[0]);

    for (size_t i = 1; i < arr.size(); ++i)
      stream->printf(", %.0f", arr[i]);

    stream->print(']');
  }

  void serveChart(Req* req, const Data::Chart& chart)
  {
    Rst* res = req->beginResponseStream("application/json");
    res->print('{');
    res->printf(S("id") ":\"%s\",", chart.id);
    res->printf(S("res") ":%d,", chart.res); 
    res->printf(S("ix") ":%d,", chart[Category::P1_HP].index); 
    res->print(S("p1_hp") ":");
    printArrJson(res, chart[Category::P1_HP].buffer);
    res->print(',');
    res->print(S("p2_hp") ":");
    printArrJson(res, chart[Category::P2_HP].buffer);
    res->print(',');
    res->print(S("p1_hc") ":");
    printArrJson(res, chart[Category::P1_HC].buffer);
    res->print(',');
    res->print(S("p2_hc") ":");
    printArrJson(res, chart[Category::P2_HC].buffer);
    res->print('}');
    req->send(res);
  }

  String processor(const String& var)
  {
    // TODO
    return var;
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
      weblog("GET /robots.txt");
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

      res->printf("{"
          "\"force_on\": %s,"
          "\"force_off\": %s,"
          "\"hc_on\": %s,"
          "\"start_hc\": %d,"
          "\"end_hc\": %d,"
          "\"time\": %d,"
          "\"force_on_radi\": %s,"
          "\"force_off_radi\": %s,"
          "\"is_on_radi\": %s"
          "}",
        btoc(Dimmer::force_on),
        btoc(Dimmer::force_off),
        btoc(Dimmer::hc_on),
        Dimmer::start_hc,
        Dimmer::end_hc,
        Heure::getTimeHMS(),
        btoc(Radiateur::force_on),
        btoc(Radiateur::force_off),
        btoc(Radiateur::is_on)
      );

      req->send(res);
    });

    // server.on("/screen", HTTP_GET, [](Req* req)
    // {
    //   weblog("GET /screen");
    //   constexpr auto screen64_size = 1368;
    //   char screen64[screen64_size + 1];

    //   auto* screen = (char*)Ecran::getScreen()
    //     .getBufferPtr();

    //   Base64.encode(screen64, screen, screen64_size); 

    //   Rst* res = req->beginResponseStream("text/plain");
    //   res->print(screen64);
    //   req->send(res);
    // });
    
    server.on("/chart", HTTP_GET, [](Req* req)
    {
      weblog("GET /chart");
      
      if (Ota::updating)
        return;

      for (const auto& [k, c]: Data::charts)
      {
        if (req->hasParam(c.id))
          serveChart(req, c);
      }
    });


    server.on("/control", HTTP_GET, [](Req* req)
    {
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

    server.on("/test_chunked", HTTP_GET, [](Req* req)
    {
      static int counter = 0;
      Res* res = req->beginChunkedResponse("text/plain",
          [](uint8_t* buffer, size_t max_len, size_t index)
          -> size_t
        {
          size_t amount = 0;

          if (counter == 1000)
          {
            counter = 0;
            return amount;
          }

          if (counter % 3 == 0)
          {
            for (; amount < 26; ++amount)
              buffer[amount] = 'A' + amount;
          }
          else if (counter % 3 == 1)
          {
            for (; amount < 26; ++amount)
              buffer[amount] = 'a' + amount;
          }
          else if (counter % 3 == 2)
          {
            for (; amount < 10; ++amount)
              buffer[amount] = '0' + amount;
          }

          ++counter;

          if (counter % 100 == 0)
            weblogf("max_len = %d, index = %d, counter =%d\n",
              max_len, index, counter);

          return amount;
        });
      req->send(res);
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
