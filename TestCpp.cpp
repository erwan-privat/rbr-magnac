// #include <WebSerialLite.h>
// #include "WiFiSerial.h"

#include <Arduino.h>

#include "TestCpp.h"
#include "TestCpp2.h"

namespace TestCpp
{
  void task(void*)
  {
    for (;;)
    {
      // WebSerial.printf(F("TEST: %d\n"), TestCpp2::var());
      int i = TestCpp2::var();
      delay(1000);
    }
  }

  void begin()
  {
    // WebSerial.printf(F(">>> ok: %d\n"), TestCpp2::var());
    xTaskCreate(task, "task test cpp",
      3000, nullptr, 1, nullptr);
  }
}
