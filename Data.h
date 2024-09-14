// eprivat 2024-09-14

#ifndef DATA_H
#define DATA_H

#include "Watts.h"
#include "WiFiSerial.h"

namespace Data
{
  using std::size_t;

  constexpr auto resolution = 10; // sec
  constexpr auto max_data = 3600 * 24 / resolution; // bytes

  float p1_buf[max_data];
  float p2_buf[max_data];

  size_t p1_ix = 0;
  size_t p2_ix = 0;

  void taskData(void*)
  {
    for (;;)
    {
      p1_buf[p1_ix] = Watts::power1;
      p1_buf[p2_ix] = Watts::power2;
      p1_ix = (p1_ix + 1) % max_data;
      p2_ix = (p2_ix + 1) % max_data;

      // weblogf("task data %u unused from 3000\r\n",
        // uxTaskGetStackHighWaterMark(nullptr));
      delay(resolution * 1000);
    }
  }

  void begin()
  {
    xTaskCreate(taskData, "task data",
      3000, nullptr, 3, nullptr);
  }
}

#endif /* DATA_H */
