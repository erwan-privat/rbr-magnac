// RBR — Routeur solaire pour Magnac.
// https://github.com/erwan-privat/rbr-magnac/

#include "Ecran.h"
#include "EpUtil.h"
#include "FileSystem.h"
#include "Data.h"
#include "Dimmer.h"
#include "Heure.h"
#include "Ota.h"
#include "Radiateur.h"
#include "ServeurWeb.h"
#include "Watts.h"
#include "WiFiMagnac.h"
#include "WiFiSerial.h"

void setup()
{
  EP_SERIAL.begin(115200); // bauds
  while (!EP_SERIAL)
    yield();
  eplog("Serial ok");
  
  FileSystem::begin();
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
  Radiateur::begin();

  eplog("Herbert !");
  weblog("Herbert !");
}

void loop()
{
}

