// Permet de lire le Serial à distance via une interface web
// eprivat 2024-08-21

#ifndef WIFISERIAL_H
#define WIFISERIAL_H

#include <WebSerialLite.h>

namespace WiFiSerial
{
  AsyncWebServer server(80);
  
  void receiveData(uint8_t *data, size_t length)
  {
    String d = "";

    for (int i = 0; i < length; ++i)
      d += char(data[i]);

    WebSerial.println(d);
  }

  void begin()
  {
    WebSerial.begin(&server);
    WebSerial.onMessage(receiveData);
    server.begin();
  }
}

#define weblogf(f, ...) WebSerial.printf(F("[EP] " f), __VA_ARGS__)

#endif
