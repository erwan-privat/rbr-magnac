#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <LittleFS.h>

#define EP_FS LittleFS

namespace FileSystem
{
  inline constexpr char filename[] = "/test.txt";

  void writeData(int);
  int readData();
  void begin();
}

#endif /* FILESYSTEM_H */
