#include "Dimmer.h"

#include "pins.h"
#include "EpUtil.h"
#include "Heure.h"
#include "Ota.h"
#include "Radiateur.h"
#include "Watts.h"
#include "WiFiSerial.h"
#include <numbers>
#include <cmath>

namespace Dimmer
{
  float seuil_chofo = 0; // W

  bool force_off = true;
  bool force_on  = false;
  bool hc_on     = true;

  DimmableLight dimmer(dimmer_com);
  byte value = 0;

  void taskDimmer(void*)
  {
    for (;;)
    {
      force_off |= Ota::updating;

      if (force_off)
        dimmer.setBrightness(0);
      else if (force_on || (hc_on && isHC()))
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

  bool isHC()
  {
    const auto h = Heure::getTimeHMS();
    return h >= start_hc || h < end_hc;
  }

  void taskChofo(void*)
  {
    constexpr size_t n_p2 = 30;
    float prev_p2[n_p2] { 0 };

    for (;;)
    {
      float p2 = -Watts::power2;
      float ptot = Watts::power1 - p2;
      float pavailable = seuil_chofo - ptot;
      float pavail_chofo = pavailable;

      if (Radiateur::is_on)
        pavail_chofo += Radiateur::max_power;

      float mean = 0;
      //TODO: set mean to 0 if p2 is zero
      for (size_t i = 0; i < n_p2; ++i)
        mean += prev_p2[i];
      mean /= n_p2;

      dlogf("M: %f -> %f / 255", mean, mean * max_value / max_chofo);

      dlogf(">>> %f + %f = ", pavail_chofo, mean);
      pavail_chofo += mean;
      dlogf("%f\n", pavail_chofo);

      float amount = pavail_chofo * max_value / max_chofo;
      value = redress(amount);

      // dlogf("%f W -> %f W\n", prev_p2, pavail_chofo);

      // dlogf("p1 = %f, p2 = %f, pa = %f, paf = %f\n",
      //     Watts::power1, p2, pavailable, pavail_chofo);

      for (size_t i = n_p2 - 1; i > 0; --i)
        prev_p2[i] = prev_p2[i - 1];

      prev_p2[0] = p2;

      delay(2000);
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

    force_off = false;
  }
}
