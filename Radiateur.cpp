#include "Radiateur.h"

#include "pins.h"
#include "Ota.h"
#include "Watts.h"
#include "WiFiSerial.h"

namespace Radiateur
{
  extern bool force_off = true;
  extern bool force_on = false;
  extern bool is_on = false;

  void taskRadiateur(void*)
  {
    for (;;)
    {
      force_off |= Ota::updating;

      if (force_off)
        digitalWrite(relay_pin, LOW);
      else if (force_on)
        digitalWrite(relay_pin, HIGH);
      else 
        digitalWrite(relay_pin, is_on ? HIGH : LOW);

      delay(1000);
    }
  }

  void taskTrigger(void*)
  {
    for (;;)
    {
      float p2 = -Watts::power2;
      is_on = p2 > Radiateur::max_power;

      // à adapter en fonction du temps de démarrage du
      // radiateur
      delay(10000);
    }
  }

  void begin()
  {
    pinMode(relay_pin, OUTPUT);

    xTaskCreate(taskRadiateur, "task radiateur", 
      3000, nullptr, 7, nullptr);
    xTaskCreate(taskTrigger, "task trigger", 
      3000, nullptr, 6, nullptr);

    force_off = false;
  }
}
