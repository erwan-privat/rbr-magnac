#include "Data.h"
#include "Heure.h"
#include "Watts.h"
// #include "WiFiSerial.h"

#include <Arduino.h>
#include <NTPClient.h>

namespace Data
{
  extern unsigned long last_boot = 0;

  float buf_p1_2[size2];
  float buf_p2_2[size2];
  float buf_p1_180[size180];
  float buf_p2_180[size180];

  extern unsigned ix2   = 0;
  extern unsigned ix180 = 0;

  void taskData2(void*)
  {
    for (;;)
    {
      buf_p1_2[ix2] = Watts::power1;
      buf_p2_2[ix2] = Watts::power2;
      ix2 = (ix2 + 1) % size2;

      // weblogf("task data2 %u unused from 3000\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(res2 * 1000);
    }
  }

  // FIXME moyenne des points par 15 min ?
  void taskData180(void*)
  {
    for (;;)
    {
      buf_p1_180[ix180] = Watts::power1;
      buf_p2_180[ix180] = Watts::power2;
      ix180 = (ix180 + 1) % size180;

      // weblogf("task data180 %u unused from 3000\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(res180 * 1000);
    }
  }

  void begin()
  {
    while (!Heure::time_client.isTimeSet())
      delay(100);
    
    last_boot = Heure::time_client.getEpochTime()
      - Heure::time_offset;
    xTaskCreate(taskData2, "task data2",
      3000, nullptr, 3, nullptr);
    xTaskCreate(taskData180, "task data180",
      3000, nullptr, 3, nullptr);
  }
}
