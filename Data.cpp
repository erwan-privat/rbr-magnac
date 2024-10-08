#include "Data.h"

#include "Heure.h"
#include "Dimmer.h"
#include "Watts.h"

#include <Arduino.h>
#include <NTPClient.h>

namespace Data
{
  extern unsigned long last_boot = 0;

  float buf_p1_hp_15min[size_15min];
  float buf_p2_hp_15min[size_15min];
  float buf_p1_hc_15min[size_15min];
  float buf_p2_hc_15min[size_15min];
  
  float buf_p1_hp_1h[size_1h];
  float buf_p2_hp_1h[size_1h];
  float buf_p1_hc_1h[size_1h];
  float buf_p2_hc_1h[size_1h];

  float buf_p1_hp_24h[size_24h];
  float buf_p2_hp_24h[size_24h];
  float buf_p1_hc_24h[size_24h];
  float buf_p2_hc_24h[size_24h];

  extern unsigned ix_15min = 0;
  extern unsigned ix_1h    = 0;
  extern unsigned ix_24h   = 0;

  void taskData_15min(void*)
  {
    for (;;)
    {
      if (Dimmer::isHC())
      {
        buf_p1_hc_15min[ix_15min] = Watts::power1;
        buf_p2_hc_15min[ix_15min] = Watts::power2;
        buf_p1_hp_15min[ix_15min] = 0;
        buf_p2_hp_15min[ix_15min] = 0;
      }
      else
      {

        buf_p1_hc_15min[ix_15min] = 0;
        buf_p2_hc_15min[ix_15min] = 0;
        buf_p1_hp_15min[ix_15min] = Watts::power1;
        buf_p2_hp_15min[ix_15min] = Watts::power2;
      }

      ix_15min = (ix_15min + 1) % size_15min;

      // weblogf("task data2 %u unused from 3000\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(res_15min * 1000);
    }
  }

  void taskData_1h(void*)
  {
    for (;;)
    {
      if (Dimmer::isHC())
      {
        buf_p1_hc_1h[ix_1h] = Watts::power1;
        buf_p2_hc_1h[ix_1h] = Watts::power2;
        buf_p1_hp_1h[ix_1h] = 0;
        buf_p2_hp_1h[ix_1h] = 0;
      }
      else
      {

        buf_p1_hc_1h[ix_1h] = 0;
        buf_p2_hc_1h[ix_1h] = 0;
        buf_p1_hp_1h[ix_1h] = Watts::power1;
        buf_p2_hp_1h[ix_1h] = Watts::power2;
      }

      ix_1h = (ix_1h + 1) % size_1h;

      // weblogf("task data2 %u unused from 3000\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(res_1h * 1000);
    }
  }

  // TODO moyenne des points par 15 min ?
  void taskData_24h(void*)
  {
    for (;;)
    {
      if (Dimmer::isHC())
      {
        buf_p1_hc_24h[ix_24h] = Watts::power1;
        buf_p2_hc_24h[ix_24h] = Watts::power2;
        buf_p1_hp_24h[ix_24h] = 0;
        buf_p2_hp_24h[ix_24h] = 0;
      }
      else
      {

        buf_p1_hc_24h[ix_24h] = 0;
        buf_p2_hc_24h[ix_24h] = 0;
        buf_p1_hp_24h[ix_24h] = Watts::power1;
        buf_p2_hp_24h[ix_24h] = Watts::power2;
      }

      ix_24h = (ix_24h + 1) % size_24h;

      // weblogf("task data180 %u unused from 3000\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(res_24h * 1000);
    }
  }

  void begin()
  {
    while (!Heure::time_client.isTimeSet())
      delay(100);
    
    last_boot = Heure::time_client.getEpochTime()
      - Heure::time_offset;
    xTaskCreate(taskData_15min, "task data_15min",
      3000, nullptr, 3, nullptr);
    xTaskCreate(taskData_1h, "task data_1h",
      3000, nullptr, 3, nullptr);
    xTaskCreate(taskData_24h, "task data_24h",
      3000, nullptr, 3, nullptr);
  }
}
