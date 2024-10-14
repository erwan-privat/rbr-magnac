#ifndef RADIATEUR_H
#define RADIATEUR_H

namespace Radiateur
{
  inline constexpr int max_power = 750; // W

  extern bool force_off;
  extern bool force_on;
  extern bool is_on;
  void begin();
}

#endif /* RADIATEUR_H */
