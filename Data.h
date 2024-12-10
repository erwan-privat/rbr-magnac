// Agrège les données sur les dernières 15 min et 24 h.

#ifndef DATA_H
#define DATA_H

#include <unordered_map>

namespace Data
{
  inline constexpr auto ekWK_hc = 0.2098; // €/kWH
  inline constexpr auto ekWK_hp = 0.2888; // €/kWH

  extern unsigned long last_boot;

  void begin();
}

#endif /* DATA_H */
