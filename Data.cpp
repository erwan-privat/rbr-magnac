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

  void taskChart(void* p_chart)
  {
    auto& c = *reinterpret_cast<Chart*>(p_chart);

    for (;;)
    {
      if (Dimmer::isHC())
      {
        c.at(Category::P1_HP).push_back(0);
        c.at(Category::P2_HP).push_back(0);
        c.at(Category::P1_HC).push_back(Watts::power1);
        c.at(Category::P2_HC).push_back(Watts::power2);
      }
      else
      {
        c.at(Category::P1_HP).push_back(Watts::power1);
        c.at(Category::P2_HP).push_back(Watts::power2);
        c.at(Category::P1_HC).push_back(0);
        c.at(Category::P2_HC).push_back(0);
      }

      // weblogf("%s, %d, %f\n", c.id, c.res, Watts::power2);
      delay(c.res * 1000);
    }
  }

  void begin()
  {
    while (!Heure::isTimeSet())
      delay(100);

    last_boot = Heure::getEpochTime();

    for (const auto& c: charts)
    {
      xTaskCreate(taskChart, "task chart",
        3000 * 2, (void*)&c, 3, nullptr);
    }
  }
}
