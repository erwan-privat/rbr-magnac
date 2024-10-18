// EpSerial.h — Raccourci des Serial.print*

#ifndef EPUTIL_H
#define EPUTIL_H

#include <Arduino.h>

#ifndef EP_SERIAL
#define EP_SERIAL Serial
#endif

template <typename T>
void print(T t)
{
  EP_SERIAL.print(t);
}

template <typename T>
void println(T t)
{
  EP_SERIAL.print(t);
  EP_SERIAL.println();
}

#define eplog(s) println(F(s))
#define eplogf(f, ...) EP_SERIAL.printf(F(f), __VA_ARGS__)
  
#endif
