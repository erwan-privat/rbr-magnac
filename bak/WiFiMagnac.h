#ifndef WIFIMAGNAC_H
#define WIFIMAGNAC_H

class WiFiEvent_t;

namespace WiFiMagnac
{
  void onEvent(WiFiEvent_t);
  void taskRssi(void*);
  void begin();
}

#endif
