#ifndef SERVEURWEB_H
#define SERVEURWEB_H

class AsyncWebServer;

namespace ServeurWeb
{
  AsyncWebServer& getServer();
  void begin();
}

#endif /* SERVEURWEB_H */
