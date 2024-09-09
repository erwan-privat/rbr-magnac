#line 1 "/home/archaoss/Arduino/rbr_magnac/EpUtil.h"
// EpSerial.h — Raccourci des Serial.print*
// CC BY-NC 4.0
// eprivat 2024-07-29

#ifndef EPUTIL_H
#define EPUTIL_H

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

#define eplog(s) println(F("[EP] " s))
#define eplogf(f, ...) EP_SERIAL.printf(F("[EP] " f), __VA_ARGS__)
  
#endif
