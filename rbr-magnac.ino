// RBR — Routeur solaire pour Magnac.
// https://github.com/Archaoss/rbr-magnac/

#include "EpUtil.h"
#include "Ecran.h"
#include "WiFiMagnac.h"
#include "WiFiSerial.h"
#include "Ota.h"
#include "Heure.h"
#include "Watts.h"
#include "Dimmer.h"
#include "ServeurWeb.h"
#include "Data.h"

void setup()
{
  EP_SERIAL.begin(115200); // bauds
  while (!EP_SERIAL)
    yield();
  eplog("Serial ok");
  
  Ecran::begin();
  // WiFi doit être lancé avant tout ce qui en a besoin,
  // à savoir Heure (NTP), mise à jour OTA, WiFiSerial.
  // Sinon le Winie clignotte et ça fout les miquettes.
  WiFiMagnac::begin();
  Ota::begin();
  Heure::begin();
  // Accès au Serial sur http://192.168.x.x/webserial
  // x.x affiché sur l'écran
  WiFiSerial::begin();
  ServeurWeb::begin();
  Data::begin();
  Watts::begin();
  Dimmer::begin();

  eplog("Herbert !");
  weblogf("%s\n", "Herbert !");
}

void loop()
{
}

