// RBR — Routeur solaire pour Magnac.
// eprivat 2024-08-12

#include <Arduino.h>
#include "pins.h"
#include "EpUtil.h"
#include "Ecran.h"
#include "WiFiMagnac.h"
#include "WiFiSerial.h"
#include "Ota.h"
#include "Heure.h"
#include "Watts.h"
#include "Dimmer.h"
  
void setup()
{
  EP_SERIAL.begin(115200); // bauds
  while (!EP_SERIAL)
    yield();
  eplog("Serial ok");
  
  // initPins();
  
  Ecran::begin();
  // WiFi doit être lancé avant tout ce qui en a besoin,
  // à savoir Heure (NTP), mise à jour OTA, WiFiSerial.
  // Sinon ça fout les miquettes et le Winie clignotte.
  WiFiMagnac::begin();
  Ota::begin();
  // Accès au Serial sur http://192.168.x.x/webserial
  // x.x affiché sur l'écran
  WiFiSerial::begin();
  Heure::begin();
  Watts::begin();
  Dimmer::begin();

  eplog("Herbert !");
  weblogf("%s\n", "Herbert !");
}

void loop()
{
}

