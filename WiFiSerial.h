// Permet de lire le Serial à distance via une interface web
// eprivat 2024-08-21

#ifndef WIFISERIAL_H
#define WIFISERIAL_H

#include <WebSerialLite.h>
#include "ServeurWeb.h"

namespace WiFiSerial
{
  // AsyncWebServer server(80);
  
  void receiveData(uint8_t *data, size_t length)
  {
    String d = "";

    for (int i = 0; i < length; ++i)
      d += char(data[i]);

    WebSerial.println(d);
  }

  void begin()
  {
    WebSerial.begin(&ServeurWeb::getServer());
    WebSerial.onMessage(receiveData);
  }
}

#define weblog(f) WebSerial.print(F("[EP] " f "\n"))
#define weblogf(f, ...) WebSerial.printf(F("[EP] " f), __VA_ARGS__)

#endif
