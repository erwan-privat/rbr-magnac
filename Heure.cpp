#include "Heure.h"
#include <NTPClient.h>
#include <Timezone.h>
#include <WiFiSerial.h>
#include <WiFiUdp.h>

namespace Heure
{
  WiFiUDP udp;
  TimeChangeRule cest {"CEST", Last, Sun, Mar, 2, 120};
  TimeChangeRule cet  {"CET" , Last, Sun, Oct, 3, 60};
  Timezone ce_tz(cest, cet);

  NTPClient time_client(udp,
    "time.google.com",
    time_offset, // décalage horaire UTC en secondes
    static_cast<unsigned long>(-1)); // pas d'update auto

  unsigned getTimeHMS()
  {
    time_t t = now();
    int hms = hour(t);
    hms *= 100;
    hms += minute(t);
    hms *= 100;
    hms += second(t);
    return hms;
  }

  bool isTimeSet()
  {
    return time_client.isTimeSet();
  }

  unsigned long getEpochTime()
  {
    return time_client.getEpochTime();
  }

  void formatTime(char tb[9])
  {
    time_t t = ce_tz.toLocal(now());
    snprintf(tb, 9, "%02d:%02d:%02d",
        hour(t), minute(t), second(t));
  }

  void taskUpdate(void*)
  {
    for (;;)
    {
      time_client.update();
      unsigned long epoch = time_client.getEpochTime();
      setTime(epoch);
      tmElements_t n;
      breakTime(ce_tz.toLocal(now()), n);
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
      delay(60'000);
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
