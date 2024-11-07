#include "FileSystem.h"

#include "EpUtil.h"
#include "WiFiSerial.h"
// #include "FS.h"
// #include <LittleFS.h>

namespace FileSystem
{
  
  constexpr bool format_littlefs_if_failed = true;

  void writeData(int value)
  {
    // File file = SPIFFS.open(filename, FILE_WRITE);
    File file = EP_FS.open(filename, FILE_WRITE);
    if (!file)
    {
      dlog("Unable to open file test.txt for writing.");
      return;
    }

    // if (!file.printf("%d", value))
    //   dlog("Unable to write value.");
    
    if (!file.print("herbert"))
      dlog("Unable to write test string");

    file.close();
  }

  int readData()
  {
    //FIXME
    // int value = 0;
    int value = 666666;

    File file = EP_FS.open(filename, FILE_READ);
    if (!file)
    {
      dlog("Unable to open file test.txt for reading.");
      return value;
    }

    file.close();
    return value;
  }

  void begin()
  {
    if (!EP_FS.begin(format_littlefs_if_failed))
    {
      dlog("FS mount failed");
      return;
    }

    dlog("FS mounted");
  }
}
