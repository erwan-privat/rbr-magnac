#include "WiFiSerial.h"

#include "ServeurWeb.h"

namespace WiFiSerial
{
  void receiveData(const byte* data, unsigned length)
  {
    String d = "";

    for (auto i = 0; i < length; ++i)
      d += char(data[i]);

    WebSerial.println(d);
  }

  void begin()
  {
    WebSerial.begin(&ServeurWeb::getServer());
    WebSerial.onMessage(receiveData);
  }
}

#define weblog(f) WebSerial.print(F(f "\n"))
#define weblogf(f, ...) WebSerial.printf(F(f), __VA_ARGS__)
