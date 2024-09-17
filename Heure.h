// eprivat 2024-08-14

#ifndef HEURE_H
#define HEURE_H

class NTPClient;

namespace Heure
{
  inline constexpr long time_offset = 7200;
  inline constexpr unsigned long update_interval = 3600'000l;
  extern NTPClient time_client;
  
  int getTimeHM();
  void taskUpdate(void*);
  void taskForceUpdate(void*);
  void begin();
}

#endif
