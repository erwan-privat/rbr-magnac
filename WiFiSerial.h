// Interface web pour la journalisation.

#ifndef WIFISERIAL_H
#define WIFISERIAL_H

#include "AsyncWebServerConfig.h"

#include <Arduino.h>
#include <WebSerialLite.h>

namespace WiFiSerial
{
  void receiveData(const byte* data, unsigned length);
  void begin();
}

#define weblog(f) WebSerial.print(F(f "\n"))
#define weblogf(f, ...) WebSerial.printf(F(f), __VA_ARGS__)

#endif
