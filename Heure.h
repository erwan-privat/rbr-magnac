// eprivat 2024-08-14

#ifndef HEURE_H
#define HEURE_H

#include <ctime>

class NTPClient;

namespace Heure
{
  unsigned getTimeHMS();
  bool isTimeSet();
  std::time_t getEpochTime();
  std::time_t getLocalNow();
  void formatTime(char time_buffer[9]); // hh:mm:ss\0
  void taskUpdate(void*);
  void taskForceUpdate(void*);
  void begin();
}

#endif
