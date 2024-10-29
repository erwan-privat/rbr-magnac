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
  using Data::Chart;

  AsyncWebServer server(80);

  AsyncWebServer& getServer()
  {
    return server;
  }

  const char* btoc(bool b)
  {
    return b ? "true" : "false";
  }

  void printArrJson(Rst* stream, const float* arr, size_t size)
  {
    stream->print('[');
    stream->print(arr[0]);

    for (size_t i = 1; i < size; ++i)
      stream->printf(", %.0f", arr[i]);

    stream->print(']');
  }

  void printArrJson(Rst* stream, const std::vector<float>& arr)
  {
    stream->print('[');
    stream->print(arr[0]);

    for (size_t i = 1; i < arr.size(); ++i)
      stream->printf(", %.0f", arr[i]);

    stream->print(']');
  }

  void serveChart(Req* req, const Chart& chart)
  {
    Rst* res = req->beginResponseStream("application/json");
    res->print('{');
    res->printf(S("id") ":\"%s\",", chart.id);
    res->printf(S("res") ":%d,", chart.res); 
    res->printf(S("ix") ":%d,", chart.at(Category::P1_HP).index); 
    res->print(S("p1_hp") ":");
    printArrJson(res, chart.at(Category::P1_HP).buffer);
    res->print(',');
    res->print(S("p2_hp") ":");
    printArrJson(res, chart.at(Category::P2_HP).buffer);
    res->print(',');
    res->print(S("p1_hc") ":");
    printArrJson(res, chart.at(Category::P1_HC).buffer);
    res->print(',');
    res->print(S("p2_hc") ":");
    printArrJson(res, chart.at(Category::P2_HC).buffer);
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

    server.on("/chart", HTTP_GET, [](Req* req)
    {
      weblog("GET /chart");
      
      if (Ota::updating)
        return;

      for (auto& c : Data::charts)
      {
        if(req->hasParam(c.id))
          serveChart(req, c);
      }   
    });

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

    server.begin();
  }
}
