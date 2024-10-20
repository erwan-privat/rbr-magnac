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
