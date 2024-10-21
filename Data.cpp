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

  void taskChart(void* p_key)
  {
    Key k = *reinterpret_cast<Key*>(p_key);
    Chart& c = charts.at(k);

    for (;;)
    {
      if (Dimmer::isHC())
      {
        c[Category::P1_HP].push_back(0);
        c[Category::P2_HP].push_back(0);
        c[Category::P1_HC].push_back(Watts::power1);
        c[Category::P2_HC].push_back(Watts::power2);
      }
      else
      {
        c[Category::P1_HP].push_back(Watts::power1);
        c[Category::P2_HP].push_back(Watts::power2);
        c[Category::P1_HC].push_back(0);
        c[Category::P2_HC].push_back(0);
      }

      // weblogf("%s, %d, %f\n", c.id, c.res, Watts::power2);
      delay(c.res * 1000);
    }   
  }

  void begin()
  {
    while (!Heure::time_client.isTimeSet())
      delay(100);
    
    last_boot = Heure::time_client.getEpochTime()
      - Heure::time_offset;

    for (const auto& k: charts)
    {
      xTaskCreate(taskChart, "task chart",
        3000, (void*)&k, 3, nullptr);
    }
  }
}
