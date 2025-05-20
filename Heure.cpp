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
    // problème avec les serveurs NTP libres, google for now
    "time.google.com",
    0, // utilisant Timezone lib, pas besoin
    static_cast<unsigned long>(-1)); // pas d'update auto

  std::time_t getLocalNow()
  {
    return ce_tz.toLocal(now());
  }

  unsigned getTimeHMS()
  {
    std::time_t t = getLocalNow();
    std::size_t hms = hour(t);
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

  std::time_t getEpochTime()
  {
    return time_client.getEpochTime();
  }

  void formatTime(char tb[9])
  {
    std::time_t t = getLocalNow();
    snprintf(tb, 9, "%02d:%02d:%02d",
        hour(t), minute(t), second(t));
  }

  void taskUpdate(void*)
  {
    for (;;)
    {
      time_client.update();
      std::time_t epoch = time_client.getEpochTime();
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
      3000 * 2, nullptr, 3, nullptr);
    xTaskCreate(taskForceUpdate, "NTP force update",
      3000 * 2, nullptr, 2, nullptr);
  }
}
