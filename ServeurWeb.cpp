#include "ServeurWeb.h"

// #include "html/index.h"
// #include "html/style.h"
// #include "html/script.h"
#include "html/index.min.h"
#include "html/favicon.h"
#include "ChartData.h"
#include "Data.h"
#include "Dimmer.h"
#include "Ecran.h"
#include "EpUtil.h"
#include "Heure.h"
#include "Radiateur.h"
#include "Ota.h"
#include "Watts.h"
#include "WiFiSerial.h"
#include <ESPAsyncWebServer.h>

#include "FileSystem.h"

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

  constexpr char mime_html[] = "text/html";
  constexpr char mime_text[] = "text/plain";
  constexpr char mime_json[] = "application/json";
  constexpr char mime_icon[] = "image/x-icon";

  struct ControlQuery
  {
    const char* name;
    bool& value;
  };

  AsyncWebServer server(80);

  std::vector<ControlQuery> queries {
    { "force_off",      Dimmer::force_off    },
    { "force_on",       Dimmer::force_on     },
    { "hc_on",          Dimmer::hc_on        },
    { "force_off_radi", Radiateur::force_off },
    { "force_on_radi",  Radiateur::force_on  },
  };

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
      stream->printf(",%.0f", arr[i]);

    stream->print(']');
  }

  void printArrJson(Rst* stream, const std::vector<float>& arr)
  {
    stream->print('[');
    stream->print(arr[0]);

    for (size_t i = 1; i < arr.size(); ++i)
      stream->printf(",%.0f", arr[i]);

    stream->print(']');
  }

  void serveChart(Req* req, const Chart& chart)
  {
    Rst* res = req->beginResponseStream(mime_json);
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

  template<typename... L> 
  inline constexpr size_t sizeof_several(const L&... list)
  {
    // Minus one for the null terminating char and to avoid
    // the use for strlen.
    return (0 + ... + (sizeof list - 1));
  }

  void begin()
  {
    server.on("/", HTTP_GET, [](Req* req)
    {
      dlog("GET /");
      // TODO Chunked response
      Rst* res = req->beginResponseStream(mime_html);
      // res->print(html::index_start);
      // res->print(html::style);
      // res->print(html::script);
      // res->print(html::index_end);
      res->print(html::index_min);

      // constexpr size_t tot_size = sizeof_several(
      //   html::index_start,
      //   html::style,
      //   html::script,
      //   html::index_end,
      // );
      
      // Res* res = req->beginChunkedResponse(mime_html,
      // [](uint8_t* buffer, size_t max_len, size_t already_sent)
      //     -> size_t
      // {
      //   size_t left = imageSize - alreadySent;

      //   if (left >= max_len)
      //   {
      //     memcpy(buffer, tot_buffer + already_sent, max_len);
      //     return maxLen;
      //   }

      //   memcpy(buffer, jpg + already_sent, left);
      //   return left;
      // });

      req->send(res);
    });

    server.on("/favicon.ico", HTTP_GET, [](Req* req)
    {
      dlog("GET /favicon.ico");
      Res* response = req->beginResponse(200, mime_icon,
        html::favicon_ico, html::favicon_ico_len);
      req->send(response);
    });

    server.on("/robots.txt", HTTP_GET, [](Req* req)
    {
      dlog("Request /robots.txt");
      req->send(200, mime_text,
          "User-agent: *\n"
          "Disallow: /\n");
    });
    
    server.on("/ota", HTTP_GET, [](Req* req)
    {
      dlog("GET /ota");

      Rst* res = req->beginResponseStream(mime_json);
      res->printf("{\"last_boot\": %lu, \"updating\": %s,"
          "\"progress\": %u}",
        Data::last_boot, btoc(Ota::updating),
        Ota::progress);

      req->send(res);
    });
    
    server.on("/watts", HTTP_GET, [](Req* req)
    {
      dlog("GET /watts");

      Rst* res = req->beginResponseStream(mime_json);
      res->printf("{\"power1\": %f, \"power2\": %f}",
          Watts::power1, Watts::power2);
      req->send(res);
    });

    server.on("/dimmer", HTTP_GET, [](Req* req)
    {
      dlog("GET /dimmer");

      Rst* res = req->beginResponseStream(mime_json);

      res->printf("{"
          "\"force_on\": %s,"
          "\"force_off\": %s,"
          "\"hc_on\": %s,"
          "\"start_hc\": %d,"
          "\"end_hc\": %d,"
          "\"time\": %d,"
          "\"force_on_radi\": %s,"
          "\"force_off_radi\": %s,"
          "\"is_on_radi\": %s,"
          "\"seuil\": %0f"
          "}",
        btoc(Dimmer::force_on),
        btoc(Dimmer::force_off),
        btoc(Dimmer::hc_on),
        Dimmer::start_hc,
        Dimmer::end_hc,
        Heure::getTimeHMS(),
        btoc(Radiateur::force_on),
        btoc(Radiateur::force_off),
        btoc(Radiateur::is_on),
        Dimmer::seuil_chofo
      );

      req->send(res);
    });

    server.on("/chart", HTTP_GET, [](Req* req)
    {
      dlog("GET /chart");
      // TODO Chunked response
      
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
      dlog("GET /control");

      for (auto& cq : queries)
      {
        if (req->hasParam(cq.name))
        {
          const Prm* param = req->getParam(cq.name);
          cq.value = param->value() == "true";
          dlogf("%s: %s\n", cq.name, param->value());
        }
      }

      constexpr char seuil_prm[] = "seuil";
      constexpr int min_seuil = -500;
      constexpr int max_seuil =  500;
      if (req->hasParam(seuil_prm))
      {
        const Prm* param = req->getParam(seuil_prm);
        float seuil = constrain(param->value().toFloat(),
            min_seuil, max_seuil);
        dlogf("Seuil : %f\n", seuil);
        Dimmer::seuil_chofo = seuil;
      }

      req->send(200);
    });

    server.on("/test_spiffs", HTTP_GET, [](Req* req)
    {
      dlog("GET /test_spiffs");

      int value = 333;

      if (req->hasParam("format"))
      {
        if (!EP_FS.format())
          dlog("Unable to format");

        dlog("FS formatted");
      }

      if (req->hasParam("set"))
      {
        const Prm* param = req->getParam("set");
        value = param->value().toInt();
        dlogf("writing value: %d\n", value);
        FileSystem::writeData(value);
        req->send(200, mime_text, "write ok");
      }
      else if (req->hasParam("get"))
      {
        req->send(EP_FS, FileSystem::filename, mime_text);
        dlog("Sent txt");
        return;
      }
      else if (req->hasParam("info"))
      {
        dlogf("LittleFS: %lu of %lu bytes used.\n",
           EP_FS.usedBytes(), EP_FS.totalBytes());
        req->send(200);
      }

      req->send(500);
    });

    server.onNotFound([](Req* req)
    {
      req->send(404, mime_text, "Adresse introuvable.");
    });

    server.begin();
  }
}
