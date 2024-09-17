// Agrège les données sur les dernières 15 min et 24 h.

#ifndef DATA_H
#define DATA_H

namespace Data
{
  inline constexpr auto ekWK_hc  = 0.2088; // €/kWH
  inline constexpr auto ekWK_hp  = 0.2874; // €/kWH

  inline constexpr auto res2    = 2; // sec
  inline constexpr auto res180  = 180; // sec
  inline constexpr auto size2   = 512;
  inline constexpr auto size180 = 512;

  extern unsigned long last_boot;

  extern float buf_p1_2[size2];
  extern float buf_p2_2[size2];
  extern float buf_p1_180[size180];
  extern float buf_p2_180[size180];

  extern unsigned ix2;
  extern unsigned ix180;

  void begin();
}

#endif /* DATA_H */
