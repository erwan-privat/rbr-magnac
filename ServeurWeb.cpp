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
// #include <Base64.h>
#include <ESPAsyncWebServer.h>
#include <cstdarg>

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

  const char* mime_json = "application/json";
  const char* mime_html = "text/html";
  const char* mime_text = "text/plain";

  AsyncWebServer server(80);

  AsyncWebServer& getServer()
  {
    return server;
  }

  const char* btoc(bool b)
  {
    return b ? "true" : "false";
  }

  // template<typename T>
  // void printArrJson(T* stream, const std::vector<float>& arr)
  // {
  //   stream->print('[');
  //   stream->print(arr[0]);

  //   for (size_t i = 1; i < arr.size(); ++i)
  //     stream->printf(", %.0f", arr[i]);

  //   stream->print(']');
  // }
  
  int trySprintf(char* buf, size_t max_len, const char* fmt, ...)
  {
    int wrote_sz = -1;
    std::va_list al_vsn, al_vs;

    va_start(al_vsn, fmt);
    va_copy(al_vs, al_vsn);

    int s = std::vsnprintf(nullptr, 0, fmt, al_vsn);

    if (s < max_len)
      wrote_sz = std::vsprintf(buf, fmt, al_vs);
    else
      wrote_sz = -1;

    va_end(al_vsn);
    va_end(al_vs);
    return wrote_sz;
  }

  int printJsonArray(char* cbuf, size_t max_len,
      const std::vector<float>& arr)
  {
    int written = 0;
    int w;

    if (max_len < 1)
      return -1;

    cbuf[written++] = '[';

    for (auto f: arr)
    {
      w = trySprintf(cbuf + written, max_len - written,
          "%.0f,", f);
      if (w < 0) return w;
      written += w;
    }
    cbuf[written - 1] = ']'; // emplace comma, check not needed

    return written;
  }

#define CHECKED_INC(w, written, cbuf)             \
  do                                              \
  {                                               \
    if (w > 0)                                    \
    {                                             \
      written += w;                               \
    }                                             \
    else                                          \
    {                                             \
      weblog("*** Full buffer, trying next one"); \
      cbuf[written++] = ' ';                      \
      return written;                             \
    }                                             \
  } while(false)

  void serveChartTest(Req* req, const Data::Chart& chart)
  {
    int chunk_number = 0;
    Res* res = req->beginChunkedResponse(mime_json,
      [=](uint8_t* buffer, size_t max_len, size_t index)
        mutable -> size_t
      {
        weblogf("CHUNK %d, max_len = %d, index = %d\n",
            chunk_number, max_len, index);

        auto cbuf = reinterpret_cast<char*>(buffer);
        size_t written = 0;
        int w;

        switch (chunk_number)
        {
          case 0:
          cbuf[written++] = '{';
          w = trySprintf(cbuf + written, max_len - written,
            "\"id\":\"%s\","
            "\"res\": %d,"
            "\"ix\": %d",
            chart.id,
            chart.res,
            chart[Category::P1_HP].index);

          CHECKED_INC(w, written, cbuf);
          break;
          case 1:
          w = trySprintf(cbuf + written, max_len - written,
            ",\"p1_hp\":");
          CHECKED_INC(w, written, cbuf);
          w = printJsonArray(cbuf + written, max_len - written,
              chart[Category::P1_HP].buffer);
          CHECKED_INC(w, written, cbuf);
          break;
          case 2:
          w = trySprintf(cbuf + written, max_len - written,
            ",\"p2_hp\":");
          CHECKED_INC(w, written, cbuf);
          w = printJsonArray(cbuf + written, max_len - written,
              chart[Category::P2_HP].buffer);
          CHECKED_INC(w, written, cbuf);
          break;
          case 3:
          w = trySprintf(cbuf + written, max_len - written,
            ",\"p1_hc\":");
          CHECKED_INC(w, written, cbuf);
          w = printJsonArray(cbuf + written, max_len - written,
              chart[Category::P1_HC].buffer);
          CHECKED_INC(w, written, cbuf);
          break;
          case 4:
          w = trySprintf(cbuf + written, max_len - written,
            ",\"p2_hc\":");
          CHECKED_INC(w, written, cbuf);
          w = printJsonArray(cbuf + written, max_len - written,
              chart[Category::P2_HC].buffer);
          CHECKED_INC(w, written, cbuf);

          cbuf[written++] = '}';
          break;
          default:
          chunk_number = 0;
          return 0;
        }

        weblogf("written = %d / %d in chunk %d\n", written, max_len, chunk_number);

        ++chunk_number;
        return written;
      });
    req->send(res);
  }

  // void serveChart(Req* req, const Data::Chart& chart)
  // {
  //   Rst* res = req->beginResponseStream(mime_json);
  //   res->print('{');
  //   res->printf(S("id") ":\"%s\",", chart.id);
  //   res->printf(S("res") ":%d,", chart.res); 
  //   res->printf(S("ix") ":%d,", chart[Category::P1_HP].index); 
  //   res->print(S("p1_hp") ":");
  //   printArrJson(res, chart[Category::P1_HP].buffer);
  //   res->print(',');
  //   res->print(S("p2_hp") ":");
  //   printArrJson(res, chart[Category::P2_HP].buffer);
  //   res->print(',');
  //   res->print(S("p1_hc") ":");
  //   printArrJson(res, chart[Category::P1_HC].buffer);
  //   res->print(',');
  //   res->print(S("p2_hc") ":");
  //   printArrJson(res, chart[Category::P2_HC].buffer);
  //   res->print('}');
  //   req->send(res);
  // }

  // String processor(const String& var)
  // {
  //   if (var == "VAR")
  //     return "   >>> processed <<<   ";

  //   return var;
  // }

  void begin()
  {
    server.on("/", HTTP_GET, [](Req* req)
    {
      weblog("GET /");
      // Rst* res = req->beginResponseStream(mime_html);
      // res->print(html::index_start);
      // res->print(html::style);
      // res->print(html::script);
      // res->print(html::index_end);
      // req->send(res);
      int chunk_number = 0;
      Res* res = req->beginChunkedResponse(mime_html,
        [=](uint8_t* buffer, size_t max_len, size_t index)
          mutable -> size_t
        {
          weblogf("CHUNK %d, max_len = %d, index = %d\n",
              chunk_number, max_len, index);

          const char* const chunks[] = {
            html::index_start,
            html::style,
            html::script1,
            html::script2,
            html::script3,
            html::index_top,
            html::index_conso,
          };

          auto cbuf = reinterpret_cast<char*>(buffer);
          size_t written = 0;

          if (chunk_number < sizeof chunks / sizeof (char*))
          {
            int w = trySprintf(cbuf + written, max_len - written,
                "%s", chunks[chunk_number]);
            CHECKED_INC(w, written, cbuf);
            ++chunk_number;
            return written;
          }

          chunk_number = 0;
          return 0;
        });
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

      Rst* res = req->beginResponseStream(mime_json);
      res->printf("{\"last_boot\": %lu, \"updating\": %s,"
          "\"progress\": %u}",
        Data::last_boot, btoc(Ota::updating),
        Ota::progress);

      req->send(res);
    });
    
    server.on("/watts", HTTP_GET, [](Req* req)
    {
      weblog("GET /watts");

      Rst* res = req->beginResponseStream(mime_json);
      res->printf("{\"power1\": %f, \"power2\": %f}",
          Watts::power1, Watts::power2);
      req->send(res);
    });

    server.on("/dimmer", HTTP_GET, [](Req* req)
    {
      weblog("GET /dimmer");

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

      // for (const auto& [k, c]: Data::charts)
      for (const auto& c: Data::charts)
      {
        if (req->hasParam(c.id))
          serveChartTest(req, c);
          // serveChart(req, c);
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
      Res* res = req->beginChunkedResponse(mime_text,
          [](uint8_t* buffer, size_t max_len, size_t index)
          -> size_t
        {
          size_t amount = 0;

          if (counter == 100)
          {
            weblogf("counter reached 100 with amount %d\n", amount);
            counter = 0;
            return amount;
          }

          if (counter % 4 == 0)
          {
            for (; amount < 26; ++amount)
              buffer[amount] = 'A' + amount;
          }
          else if (counter % 4 == 1)
          {
            for (; amount < 26; ++amount)
              buffer[amount] = 'a' + amount;
          }
          else if (counter % 4 == 2)
          {
            for (; amount < 10; ++amount)
              buffer[amount] = '0' + amount;
          }
          else if (counter % 4 == 3)
          {
            buffer[amount++] = '%';
            buffer[amount++] = 'V';
            buffer[amount++] = 'A';
            buffer[amount++] = 'R';
            buffer[amount++] = '%';
          }

          ++counter;

          if (counter % 100 == 0)
          {
            weblogf("max_len = %d, index = %d, counter = %d\n",
              max_len, index, counter);
          }

          return amount;
        });
        // }, processor);
      req->send(res);
    });

    server.begin();
  }
}
