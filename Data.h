// Agrège les données sur les dernières 15 min et 24 h.

#ifndef DATA_H
#define DATA_H

#include <unordered_map>

namespace Data
{
  inline constexpr auto ekWK_hc = 0.2088; // €/kWH
  inline constexpr auto ekWK_hp = 0.2874; // €/kWH

  inline constexpr auto res_15min  =   2; // sec
  inline constexpr auto size_15min = 450;
  inline constexpr auto res_1h     =   8; // sec
  inline constexpr auto size_1h    = 450;
  inline constexpr auto res_24h    = 180; // sec
  inline constexpr auto size_24h   = 480;

  extern unsigned long last_boot;

  extern float buf_p1_hp_15min[size_15min];
  extern float buf_p2_hp_15min[size_15min];
  extern float buf_p1_hc_15min[size_15min];
  extern float buf_p2_hc_15min[size_15min];
  
  extern float buf_p1_hp_1h[size_1h];
  extern float buf_p2_hp_1h[size_1h];
  extern float buf_p1_hc_1h[size_1h];
  extern float buf_p2_hc_1h[size_1h];

  extern float buf_p1_hp_24h[size_24h];
  extern float buf_p2_hp_24h[size_24h];
  extern float buf_p1_hc_24h[size_24h];
  extern float buf_p2_hc_24h[size_24h];

  extern unsigned ix_15min;
  extern unsigned ix_1h;
  extern unsigned ix_24h;

  void begin();
}

#endif /* DATA_H */
