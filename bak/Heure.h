// eprivat 2024-08-14

#ifndef HEURE_H
#define HEURE_H

class NTPClient;

namespace Heure
{
  constexpr long getTimeOffset();
  NTPClient& getTimeClient();
  int getTimeHM();
  void taskUpdate(void*);
  void taskForceUpdate(void*);
  void begin();
}

#endif
