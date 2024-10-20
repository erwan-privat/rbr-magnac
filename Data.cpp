#include "Data.h"

#include "ChartData.h"
#include "Dimmer.h"
#include "Heure.h"
#include "Watts.h"
#include "WiFiSerial.h"

#include <Arduino.h>
#include <NTPClient.h>

namespace Data
{
  unsigned long last_boot = 0;

  void taskData_15min(void*)
  {
    for (;;)
    {
      if (Dimmer::isHC())
      {
        charts.at(Key::D_15MIN)[Category::P1_HP]
          .push_back(0);
        charts.at(Key::D_15MIN)[Category::P2_HP]
          .push_back(0);
        charts.at(Key::D_15MIN)[Category::P1_HC]
          .push_back(Watts::power1);
        charts.at(Key::D_15MIN)[Category::P2_HC]
          .push_back(Watts::power2);
      }
      else
      {
        charts.at(Key::D_15MIN)[Category::P1_HP]
          .push_back(Watts::power1);
        charts.at(Key::D_15MIN)[Category::P2_HP]
          .push_back(Watts::power2);
        charts.at(Key::D_15MIN)[Category::P1_HC]
          .push_back(0);
        charts.at(Key::D_15MIN)[Category::P2_HC]
          .push_back(0);
      }

      // weblogf("task data2 %u unused from 3000\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(charts.at(Key::D_15MIN).res * 1000);
    }
  }

  void taskData_1h(void*)
  {
    for (;;)
    {
      if (Dimmer::isHC())
      {
        charts.at(Key::D_1H)[Category::P1_HP]
          .push_back(0);
        charts.at(Key::D_1H)[Category::P2_HP]
          .push_back(0);
        charts.at(Key::D_1H)[Category::P1_HC]
          .push_back(Watts::power1);
        charts.at(Key::D_1H)[Category::P2_HC]
          .push_back(Watts::power2);
      }
      else
      {
        charts.at(Key::D_1H)[Category::P1_HP]
          .push_back(Watts::power1);
        charts.at(Key::D_1H)[Category::P2_HP]
          .push_back(Watts::power2);
        charts.at(Key::D_1H)[Category::P1_HC]
          .push_back(0);
        charts.at(Key::D_1H)[Category::P2_HC]
          .push_back(0);
      }

      // weblogf("task data2 %u unused from 3000\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(charts.at(Key::D_1H).res * 1000);
    }
  }

  // TODO moyenne des points par 15 min ?
  void taskData_24h(void*)
  {
    for (;;)
    {
      if (Dimmer::isHC())
      {
        charts.at(Key::D_24H)[Category::P1_HP]
          .push_back(0);
        charts.at(Key::D_24H)[Category::P2_HP]
          .push_back(0);
        charts.at(Key::D_24H)[Category::P1_HC]
          .push_back(Watts::power1);
        charts.at(Key::D_24H)[Category::P2_HC]
          .push_back(Watts::power2);
      }
      else
      {
        charts.at(Key::D_24H)[Category::P1_HP]
          .push_back(Watts::power1);
        charts.at(Key::D_24H)[Category::P2_HP]
          .push_back(Watts::power2);
        charts.at(Key::D_24H)[Category::P1_HC]
          .push_back(0);
        charts.at(Key::D_24H)[Category::P2_HC]
          .push_back(0);
      }

      // weblogf("task data180 %u unused from 3000\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(charts.at(Key::D_24H).res * 1000);
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
