// Configuration de la sonde JSY-MK-194T utilisant
// Modbus RTU.
// Based on git clankgeek/JSY-MK-194
// https://github.com/clankgeek/JSY-MK-194
// eprivat 2024-08-15

#ifndef WATTS_H
#define WATTS_H

#include <Arduino.h>
#include "pins.h"
#include "EpUtil.h"
#include "WiFiSerial.h"

#define SONDE_SERIAL Serial2
#define WATTS_DEBUG

namespace Watts
{
  // tore or first clamp
  float current1 = 0; // A
  float energy1 = 0; // kWh
  float energyReturned1 = 0; // kWh
  float power1 = 0; // W
  float powerFactor1 = 0;
  float voltage1 = 0; // V

  // clamp (or second clamp)
  float current2 = 0; // A
  float energy2 = 0; // kWh
  float energyReturned2 = 0; // kWh
  float power2 = 0; // W
  float powerFactor2 = 0;
  float voltage2 = 0; // V

  uint32_t frequency = 0; // Hz

  uint8_t buffer[61];

  void printValues()
  {
    eplog("----------------------------");
    eplogf("%f V, %f A, %f W\r\n", voltage1, current1, power1);
    eplogf("%f V, %f A, %f W\r\n", voltage2, current2, power2);
    // eplogf("%d Hz\r\n", frequency);

    eplog("----------------------------");
    eplogf("task watts %u from 3000\r\n",
      uxTaskGetStackHighWaterMark(nullptr));

    weblogf("%s\n", "----------------------------");
    weblogf("%f V, %f A, %f W\n", voltage1, current1, power1);
    weblogf("%f V, %f A, %f W\n", voltage2, current2, power2);
    // weblogf("%d Hz\n", frequency);

    weblogf("%s\n", "----------------------------");
    weblogf("task watts %u from 3000\n",
      uxTaskGetStackHighWaterMark(nullptr));
  }

  uint32_t readInt(int pos)
  {
    uint32_t i = 0;
    i |= buffer[pos + 0] << 3 * 8;
    i |= buffer[pos + 1] << 2 * 8;
    i |= buffer[pos + 2] << 1 * 8;
    i |= buffer[pos + 3] << 0 * 8;
    return i;
  }

  void readValues()
  {
    voltage1 = ((buffer[3] << 24) + (buffer[4] << 16) 
        + (buffer[5] << 8) + buffer[6]) * 0.0001;
    current1 = ((buffer[7] << 24) + (buffer[8] << 16) 
        + (buffer[9] << 8) + buffer[10]) * 0.0001;
    power1 = ((buffer[11] << 24) + (buffer[12] << 16) 
        + (buffer[13] << 8) + buffer[14]) 
      * (buffer[27] == 1 ? -0.0001 : 0.0001);
    energy1 = ((buffer[15] << 24) + (buffer[16] << 16) 
        + (buffer[17] << 8) + buffer[18]) * 0.0001;
    powerFactor1 = ((buffer[19] << 24) + (buffer[20] << 16) 
        + (buffer[21] << 8) + buffer[22]) * 0.001;
    energyReturned1 = ((buffer[23] << 24) 
        + (buffer[24] << 16) + (buffer[25] << 8) 
        + buffer[26]) * 0.0001;
    // buffer[27] is the sign of power1
    // buffer[28] is the sign of power2
    // buffer[29] unused
    // buffer[30] unused
    frequency = round(((buffer[31] << 24) 
          + (buffer[32] << 16) + (buffer[33] << 8) 
          + buffer[34]) * 0.01);
    voltage2 = ((buffer[35] << 24) + (buffer[36] << 16) 
        + (buffer[37] << 8) + buffer[38]) * 0.0001;
    current2 = ((buffer[39] << 24) + (buffer[40] << 16) 
        + (buffer[41] << 8) + buffer[42]) * 0.0001;
    power2 = ((buffer[43] << 24) + (buffer[44] << 16) 
        + (buffer[45] << 8) + buffer[46]) 
      * (buffer[28] == 1 ? -0.0001 : 0.0001);
    energy2 = ((buffer[47] << 24) + (buffer[48] << 16) 
        + (buffer[49] << 8) + buffer[50]) * 0.0001;
    powerFactor2 = ((buffer[51] << 24) + (buffer[52] << 16) 
        + (buffer[53] << 8) + buffer[54]) * 0.001;
    energyReturned2 = ((buffer[55] << 24) 
        + (buffer[56] << 16) + (buffer[57] << 8) 
        + buffer[58]) * 0.0001;
  }

  void taskWatts(void*)
  {
    // Message à envoyer à la sonde pour demander la valeur
    // actuellement mesurée.
    constexpr unsigned char rtu_msg_get[] = {
      0x01, 0x03, 0x00, 0x48, 0x00, 0x0E, 0x44, 0x18
    };

    for (;;)
    {
      SONDE_SERIAL.write(rtu_msg_get, 8);
      SONDE_SERIAL.flush();

      int read_bytes = 0;
      while (SONDE_SERIAL.available())
      {
        buffer[read_bytes++] = SONDE_SERIAL.read();
      }

      readValues();

      #ifdef WATTS_DEBUG
      printValues();
      #endif
      
      delay(2000);
    }
  }

  void begin()
  {
    // SERIAL_8N1 : one start bit, eight (8) data bits,
    // no (N) parity bit, and one (1) stop bit.
    SONDE_SERIAL.begin(38400, SERIAL_8N1,
      capteur_rx, capteur_tx);

    xTaskCreate(taskWatts, "task watts",
        3000, nullptr, 7, nullptr);
  }
}

#endif
