#ifndef ECRAN_H
#define ECRAN_H

namespace Ecran
{
  void progressbar(int x, int y, int w, int h, float value);
  void drawOtaProgress();
  void drawWiFiStatus();
  void drawTime();
  void drawWatts();
  void drawDebug();
  void taskScreen(void*);
  void taskTimerSwitch(void*);
  void begin();
}

#endif
