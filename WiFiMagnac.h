// eprivat 2024-08-14

#ifndef WIFIMAGNAC_H
#define WIFIMAGNAC_H

#include <WiFi.h>
#include "EpUtil.h"

namespace WiFiMagnac
{
#include "WiFiCredentials.h"

  void onEvent(WiFiEvent_t event)
  {
    switch (event)
    {
      // case ARDUINO_EVENT_WIFI_AP_START:
      // //can set ap hostname here
      // WiFi.softAPsetHostname(AP_SSID);
      // break;
      // case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
      // Serial.print("STA IPv6: ");
      // Serial.println(WiFi.linkLocalIPv6());
      // break;
      // case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
      // Serial.print("AP IPv6: ");
      // Serial.println(WiFi.softAPlinkLocalIPv6());
      // break;

      case ARDUINO_EVENT_WIFI_STA_START:
        WiFi.setHostname(host);
        break;

      case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        break;

      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        eplogf("Connected: %s\r\n",
            WiFi.localIP().toString().c_str());
        Ecran::wifi_connected = true;
        break;

      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        eplog("WiFi disconnected.");
        Ecran::wifi_connected = false;
        WiFi.reconnect();
        break;

      default:
        break;
    }
  }

  void taskRssi(void* argv)
  {
    for (;;)
    {
      Ecran::wifi_rssi = WiFi.RSSI();
      delay(2000);
    }
  }

  void begin()
  {
    eplog("Connecting to WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    WiFi.onEvent(onEvent);

    xTaskCreate(taskRssi, "task RSSI",
      3000, nullptr, 1, nullptr);
  }

}

#endif
