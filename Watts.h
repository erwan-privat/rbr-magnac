// Configuration de la sonde JSY-MK-194T utilisant
// Modbus RTU.
// Based on git clankgeek/JSY-MK-194
// https://github.com/clankgeek/JSY-MK-194

#ifndef WATTS_H
#define WATTS_H

#include <cstdint>

namespace Watts
{
  // tore or first clamp
  extern float current1; // A
  extern float energy1; // kWh
  extern float energyReturned1; // kWh
  extern float power1; // W
  extern float powerFactor1;
  extern float voltage1; // V

  // clamp (or second clamp)
  extern float current2; // A
  extern float energy2; // kWh
  extern float energyReturned2; // kWh
  extern float power2; // W
  extern float powerFactor2;
  extern float voltage2; // V

  extern uint32_t frequency; // Hz

  void printValues();
  uint32_t readInt(int pos);
  void readValues();
  void taskWatts(void*);
  void begin();
}

#endif
