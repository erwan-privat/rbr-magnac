#include <WiFi.h>
#include "WiFiMagnac.h"
#include "EpUtil.h"

namespace WiFiMagnac
{
  // Must delcare/define
  // constexpr char ssid[] = "";
  // constexpr char pass[] = "";
  // constexpr char host[] = "";
  #include "WiFiCredentials.h"

  bool connected = false;

  bool isConnected()
  {
    return connected;
  }

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
        connected = true;
        break;

      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        eplogf("Connected: %s\r\n",
            WiFi.localIP().toString().c_str());
        // Ecran::wifi_connected = true;
        break;

      case ARDUINO_EVENT_WIFI_STA_LOST_IP:
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        connected = false;
        eplog("WiFi disconnected.");
        // Ecran::wifi_connected = false;
        WiFi.reconnect();
        break;

      default:
        break;
    }
  }

  void begin()
  {
    eplog("Connecting to WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    WiFi.onEvent(onEvent);
  }
}
