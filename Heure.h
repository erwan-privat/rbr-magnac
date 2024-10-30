// eprivat 2024-08-14

#ifndef HEURE_H
#define HEURE_H

class NTPClient;

namespace Heure
{
  inline constexpr long time_offset = 0;
  // extern NTPClient time_client;
  
  unsigned getTimeHMS();
  bool isTimeSet();
  unsigned long getEpochTime();
  void formatTime(char time_buffer[9]); // hh:mm:ss\0
  void taskUpdate(void*);
  void taskForceUpdate(void*);
  void begin();
}

#endif
