// Agrrège les données sur les dernières 15 min et 24 h.
// eprivat 2024-09-14

#ifndef DATA_H
#define DATA_H

#include "Watts.h"
#include "WiFiSerial.h"

namespace Data
{
  // constexpr auto resolution = 60; // sec
  // constexpr auto max_data = 3600 * 24 / resolution; // bytes
  constexpr auto res2    = 2; // sec
  constexpr auto res180  = 180; // sec
  constexpr auto size2   = 512;
  constexpr auto size180 = 512;

  float buf_p1_2[size2];
  float buf_p2_2[size2];
  float buf_p1_180[size180];
  float buf_p2_180[size180];

  std::size_t ix2   = 0;
  std::size_t ix180 = 0;

  void taskData2(void*)
  {
    for (;;)
    {
      buf_p1_2[ix2]    = Watts::power1;
      buf_p2_2[ix2] = Watts::power2;
      ix2 = (ix2 + 1) % size2;

      weblogf("task data2 %u unused from 3000\n",
        uxTaskGetStackHighWaterMark(nullptr));
      delay(res2 * 1000);
    }
  }

  void taskData180(void*)
  {
    for (;;)
    {
      buf_p1_180[ix180] = Watts::power1;
      buf_p2_180[ix180] = Watts::power2;
      ix180 = (ix180 + 1) % size180;

      weblogf("task data180 %u unused from 3000\n",
        uxTaskGetStackHighWaterMark(nullptr));
      delay(res180 * 1000);
    }
  }

  void begin()
  {
    xTaskCreate(taskData2, "task data2",
      3000, nullptr, 3, nullptr);
    xTaskCreate(taskData180, "task data180",
      3000, nullptr, 3, nullptr);
  }
}

#endif /* DATA_H */
