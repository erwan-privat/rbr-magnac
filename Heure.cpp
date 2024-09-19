#include "Heure.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

namespace Heure
{
  WiFiUDP udp;

  extern NTPClient time_client(udp,
    "time.google.com",
    time_offset, // décalage horaire UTC en secondes
    static_cast<unsigned long>(-1)); // pas d'update auto

  int getTimeHMS()
  {
    int hms = time_client.getHours();
    hms *= 100;
    hms += time_client.getMinutes();
    hms *= 100;
    hms += time_client.getSeconds();
    return hms;
  }

  void taskUpdate(void*)
  {
    for (;;)
    {
      time_client.update();
      // eplogf("task NTP update %u unused from 3000\r\n",
          // uxTaskGetStackHighWaterMark(nullptr));
      delay(1000);
    }
  }

  void taskForceUpdate(void*)
  {
    for (;;)
    {
      time_client.forceUpdate();
      // eplogf("task NTP force update %u from 3000\r\n",
          // uxTaskGetStackHighWaterMark(nullptr));
      // vTaskDelay(update_interval / portTICK_PERIOD_MS);
      delay(10'000);
    }
  }

  void begin()
  {
    time_client.begin();

    xTaskCreate(taskUpdate, "NTP update",
      3000, nullptr, 3, nullptr);
    xTaskCreate(taskForceUpdate, "NTP force update",
      3000, nullptr, 2, nullptr);
  }
}
