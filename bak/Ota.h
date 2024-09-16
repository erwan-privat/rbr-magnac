// eprivat 2024-08-14

#ifndef OTA_H
#define OTA_H

#include <ArduinoOTA.h>
#include "Ecran.h"
#include "Dimmer.h"

namespace Ota
{
  void taskUpdate(void* argv)
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
        Ecran::ota_progress = 0;
        Ecran::ota_updating = true;
        Dimmer::force_off = true;
      })
      .onProgress([](unsigned progress,
            unsigned total)
      {
        int percentage = progress / (total / 100);
        eplogf("OTA progress: %u %%\r", percentage);

        Ecran::ota_progress = percentage;
      })
      .onEnd([]()
      {
        eplog("Mise à jour terminée.");
        // Ecran::ota_updating = false;
        // Dimmer::force_off = false;
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
        Ecran::ota_error = error;
        Ecran::ota_updating = false;
        Dimmer::force_off = false;
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

#endif
