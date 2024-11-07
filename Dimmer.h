#ifndef DIMMER_H
#define DIMMER_H

#include <Arduino.h>
#include <dimmable_light.h>

namespace Dimmer
{
  // On garde une petite marge pour compenser la courbe de
  // réponse du triac.
  // inline constexpr float seuil_chofo =  -150; // W
  inline constexpr float max_chofo   = 2300; // W
  inline constexpr int   start_hc    = 20'56'00; // 20 h 56
  inline constexpr int   end_hc      =  4'56'00; // 04 h 56

  inline constexpr byte  max_value   = 255;

  extern float seuil_chofo;
  extern bool force_off;
  extern bool force_on;
  extern bool hc_on;

  extern DimmableLight dimmer;

  bool isHC();
  void begin();
}

#endif
