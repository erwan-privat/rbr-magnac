#include "Dimmer.h"
#include "pins.h"
#include "Watts.h"
#include "Heure.h"
// #include "WiFiSerial.h"
#include "Ota.h"
#include <numbers>
#include <cmath>

namespace Dimmer
{
  extern bool force_off = true;
  extern bool force_on  = false;

  extern DimmableLight dimmer(dimmer_com);
  byte value = 0;

  void taskDimmer(void*)
  {
    for (;;)
    {
      force_off |= Ota::updating;

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

  bool isHC()
  {
    const auto h = Heure::getTimeHMS();
    return h >= start_hc || h < end_hc;
  }

  void taskChofo(void*)
  {
    for (;;)
    {
      float ptot = Watts::power1 - (-Watts::power2);
      float pavailable = seuil_chofo - ptot;

      float amount = pavailable * max_value / max_chofo;
      value = redress(amount);

      // FIXME removed hc for tests
      // force_on = isHC();

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

    force_off = false;
  }
}
