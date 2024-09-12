English below.

FRANÇAIS
=========

Vous voulez me filer un coup de pouce ? J'ai une `page Ko-Fi <https://ko-fi.com/eprivat/goal?g=0>`_. 

Lisez-moi
---------

Programme de routeur solaire pour ESP32 pour rediriger un
surplus de production vers par exemple un chauffe-eau. Cela
comprend :

* la mise à jour du programme par WiFi (over-the-air, OTA) ;
* WebSerial, un port série par web pour accéder à la
  journalisation sur http://x.x.x.x/webserial ;
* une correction du transfert de puissance suivant un profil
  arcsin pour compenser les thyristors mal calibrés ou un
  évenuel décalage de phase (typiquement : un thyristor
  ouvert à 10 % de 2400 W qui ne délivre que 30 W) ;
* gestion des heures creuses sans nécessiter de relai de
  marche forcée ;
* information sur écran avec écran de chargement (mon chat),
  l'heure, l'affichage de la consommation du foyer si le
  chauffe-eau n'est pas alimenté, ou, de la consommation
  réelle du chauffe-eau en cas de surplus, mesurée par une
  sonde JSY-MK-194 ;
* code découpé en tâches permettant une bonne séparation des
  différents modules basé sur le principe de responsabilité
  unique (*sole responsability principle*, même si j'avoue
  que ça pourrait être plus poussé) ;
* une convention de nommage étrange en franglais mais qui je
  l'éspère permet d'être comprise par les néophytes de la
  programmation.

À faire
-------

* Serveur web pour tracer les graphes de consommation etc ;
* gestion du ventilateur du thyristor (peut-être ; j'ai
  mesuré au maximum une température de 28 °C sur le gros
  radiateur équippé lorsque le chauffe-eau tirait 2400 W).

Licence
-------

Voir le fichier LICENSE. En gros, vous faites ce que vous
voulez de ce logiciel tant que vous ne vous en servez-pas
dans un cadre capitaliste.

Aide
****

Comme énoncé plus haut, si vous pouviez contribuer sur ma
`page Ko-Fi <https://ko-fi.com/eprivat/goal?g=0>`_ ça
m'aiderait énormément, si vous en avez les moyens.

ENGLISH
=======

Want to help? Please check my `Ko-Fi page
<https://ko-fi.com/eprivat/goal?g=0>`_.

Readme
------

Solar router program for ESP32 to redirect any
overproduction to say a water-heater, including:

* WiFi Over-the-air (OTA);
* WebSerial, a serial port over WiFi to access log
  information at http://x.x.x.x/webserial;
* corrected thyristor transfer function (of the form arcsin)
  to compensate badly calibrated devices or phase shifts
  between tension and current, e.g. a 10% closed thyristor
  wich would only provide 30 W instead of 240 W for a 2400 W
  device;
* off-peak hours handling (« *heures creuses* ») to override
  solar (non-)production at night or whenever without the
  need of a relay;
* screen info with loading splash screen (my cat), time,
  ip address, and displaying the actual consumption of the
  household if not powering the water-heater, or the actual
  (as in measured by a JSY-MK-194) power taken by the
  water-heater;
* task oriented code allowing good isolation of code modules
  (sole responsability principle, could be better I admit)
  and using all cores of any FreeRTOS based device;
* weird names from trying to have good coding convention
  while having understandable file and function names for
  non programmers.

Todo
----

* Web server for plotting power use and monitoring misc
  data.
* Maybe: fan control. Not needed right now, the maximum
  temperature I measured on the thyristor (equipped with a
  big radiator) is 28 °C (82 °F) while delivering 2400 W.

Can I help?
***********

As I mentionned before, please take a loof at my
`Ko-Fi page <https://ko-fi.com/eprivat/goal?g=0>`_, and
consider tipping if you can afford it, that would help
enormously.

License
-------

See LICENSE file. To recap, you do whatever you want if you
are not using it as part of a capitalist organisation.
