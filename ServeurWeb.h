// eprivat 2024-09-10

#ifndef SERVEURWEB_H
#define SERVEURWEB_H

class AsyncWebServer;

namespace ServeurWeb
{
  AsyncWebServer& getServer();
  void begin();
}

#endif /* SERVEURWEB_H */
