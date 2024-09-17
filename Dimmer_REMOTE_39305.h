#ifndef DIMMER_H
#define DIMMER_H

#include <Arduino.h>
#include <dimmable_light.h>

namespace Dimmer
{
  inline constexpr float seuil_chofo =  -50; // W
  inline constexpr float max_chofo   = 2300; // W
  inline constexpr int   start_hc    = 20'56; // 20 h 56
  inline constexpr int   end_hc      =  4'56; // 04 h 56
  inline constexpr byte  max_value   = 255;

  extern bool force_off;
  extern bool force_on;
  extern DimmableLight dimmer;

  void taskDimmer(void*);
  // amount est la valeur pour le cas linéaire que l'on
  // redresse pour compenser la non-linéarité du triac
  unsigned char redress(float amount);
  void taskChofo(void*);
  void begin();
}

#endif
