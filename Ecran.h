#ifndef ECRAN_H
#define ECRAN_H

#include <U8g2lib.h>

namespace Ecran
{
  // U8G2    // nom de la bibliothèque
  // SH1106  // contrôleur
  // 128X64  // taille en px²
  // NONAME  // nom de l'écran
  // F       // Full buffer, garde en mémoire tout l'écran
  // 4W      // 4-Wire mode (clock, data, cs, dc)
  // HW      // Hardware, doit être branché au bon endroit
  // SPI     // protocole SPI (et non I2C)
  using OledScreen = U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI;
  OledScreen& getScreen();
  unsigned getLength();
  void begin();
}

#endif
