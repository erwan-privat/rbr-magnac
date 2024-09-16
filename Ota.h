#ifndef OTA_H
#define OTA_H

namespace Ota
{
  bool isUpdating();
  byte getProgress();
  void taskUpdate(void*);
  void begin();
}

#endif
