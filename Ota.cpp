#include <ArduinoOTA.h>
#include "Ota.h"
#include "EpUtil.h"

namespace Ota
{
  bool updating = false;
  byte progress = 0;

  bool isUpdating()
  {
    return updating;
  }

  byte getProgress()
  {
    return progress;
  }

  void taskUpdate(void*)
  {
    for (;;)
    {
      ArduinoOTA.handle();
      delay(2000);
      // eplogf("task OTA %u unused from 3000\r\n",
        // uxTaskGetStackHighWaterMark(nullptr));
    }
  }

  void begin()
  {
    ArduinoOTA.setHostname("esp32-magnac");
    ArduinoOTA.begin();
    ArduinoOTA
      .onStart([]()
      {
        eplog("Mise à jour en cours...");
        progress = 0;
        updating = true;
      })
      .onProgress([](unsigned p,unsigned total)
      {
        progress = p / (total / 100);
        eplogf("OTA progress: %u %%\r", progress);
      })
      .onEnd([]()
      {
        eplog("Mise à jour terminée.");
        updating = false;
      })
      .onError([](ota_error_t error)
      {
        eplogf("OTA error[%u]: \r\n", error);
        if (error == OTA_AUTH_ERROR)
        {
          eplog("Auth Failed");
        }
        else if (error == OTA_BEGIN_ERROR) 
        {
          eplog("Begin Failed");
        }
        else if (error == OTA_CONNECT_ERROR) 
        {
          eplog("Connect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR) 
        {
          eplog("Receive Failed");
        }
        else if (error == OTA_END_ERROR) 
        {
          eplog("End Failed");
        }

        updating = false;
      });


    // FIXME
    // On attend une seconde au cas où il y ait une update
    // avant un éventuel bug.
    delay(1000);
    ArduinoOTA.handle();

    xTaskCreate(taskUpdate, "task OTA",
      3000, nullptr, 4, nullptr);
  }
}
