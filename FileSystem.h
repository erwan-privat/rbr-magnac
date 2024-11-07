#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <SPIFFS.h>

// #define EP_FS LittleFS
#define EP_FS SPIFFS

namespace FileSystem
{
  inline constexpr char filename[] = "/test.txt";

  void writeData(int);
  int readData();
  void begin();
}

#endif /* FILESYSTEM_H */
