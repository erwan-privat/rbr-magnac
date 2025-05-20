// Agrège les données sur les dernières 15 min et 24 h.

#ifndef DATA_H
#define DATA_H

namespace Data
{
  inline constexpr auto ekWK_hp = 0.3041; // €/kWH
  inline constexpr auto ekWK_hc = 0.2250; // €/kWH

  extern unsigned long last_boot;

  void begin();
}

#endif /* DATA_H */
