#ifndef OTA_H
#define OTA_H

#include <Arduino.h>

namespace Ota
{
  extern bool updating;
  extern byte progress;
  extern int  error;

  void taskUpdate(void*);
  void begin();
}

#endif
