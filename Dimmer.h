// eprivat 2024-08-20

#ifndef DIMMER_H
#define DIMMER_H

#include <numbers>
#include <cmath>
#include <dimmable_light.h>
#include "pins.h"
#include "Watts.h"
#include "Heure.h"
#include "WiFiSerial.h"

namespace Dimmer
{
  constexpr float seuil_chofo = 0; // W
  constexpr float max_chofo   = 2300; // W
  constexpr int   start_hc    = 20'56; // 20 h 56
  constexpr int   end_hc      =  4'56; // 04 h 56

  constexpr byte  max_value = 255;

  bool force_off = false;
  bool force_on  = false;
  bool ventil_on = false;

  DimmableLight dimmer(dimmer_com);
  byte value = 0;

//  void taskDimmerSineTest(void*)
//  {
//    constexpr float freq = 1.0 / 15.0;
//    for (;;)
//    {
//      float t = esp_timer_get_time() * 1e-6f; // sec
//      float s = 0.5 + sin(2 * PI * freq * t) * 0.5;
//      value = static_cast<byte>(round(s * max_value));
//
//      if (force_off)
//         {
//        dimmer.setBrightness(0);
//      }
//      else
//      {
//        if (value != dimmer.getBrightness())
//          dimmer.setBrightness(value);
//          
//        ventil_on = value > 128;
//      }
//
//      // digitalWrite(dimmer_ven, ventil_on ? LOW : HIGH);
//      // analogWrite(dimmer_ven, value);
//      delay(1);
//    }
//  }

  void taskDimmer(void*)
  {
    for (;;)
    {
      if (force_off)
        dimmer.setBrightness(0);
      else if (force_on)
        dimmer.setBrightness(max_value);
      else
        dimmer.setBrightness(value);

      delay(100);
    }
  }

  // amount est la valeur pour le cas linéaire que l'on
  // redresse pour compenser la non-linéarité du triac
  byte redress(float amount)
  {
    constexpr auto pi = std::numbers::pi_v<float>;

    amount = constrain(amount, 0, max_value);
    float val = std::asin(amount / max_value * 2 - 1);
    val /= pi;
    val += 0.5f;
    val *= max_value;
    return static_cast<byte>(round(val));
  }

  void taskChofo(void*)
  {
    for (;;)
    {
      float ptot = Watts::power1 - (-Watts::power2);
      float pavailable = seuil_chofo - ptot;

      float amount = pavailable * max_value / max_chofo;
      value = redress(amount);

      const auto h = Heure::getTimeHM();
      force_on = h >= start_hc || h < end_hc;

      delay(2000);

      // weblogf("tot = %f W, dispo = %f W, value = %d, "
              // "sending_th = %f, sending = %f W\n",
          // ptot, pavailable, value,
          // max_chofo * (float)value / max_value,
          // -Watts::power1);

      // weblogf("> value = %d, redressed = %d\n",
          // value, redress(amount));
      // weblogf("H = %d, force_on = %d\n", h, force_on);
      // for (int i = 0 ; i < 256 ; i += 10)
        // weblogf("%d -> %d\n", i, redress(i));
    }
  }

  void begin()
  {
    pinMode(dimmer_ven, OUTPUT);
    DimmableLight::setSyncPin(dimmer_zc);
    DimmableLight::begin();

    xTaskCreate(taskDimmer, "task dimmer", 
      3000, nullptr, 7, nullptr);

    xTaskCreate(taskChofo, "task chauffe-eau", 
      3000, nullptr, 7, nullptr);
  }
}

#endif
