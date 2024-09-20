#ifndef INDEX_H
#define INDEX_H

#include <Arduino.h>

// #ifdef  TEMPLATE_PLACEHOLDER
   // #undef  TEMPLATE_PLACEHOLDER
// #endif
// #define TEMPLATE_PLACEHOLDER '$'

namespace html
{
  constexpr char index_start[] PROGMEM = R"%(<!DOCTYPE html>
  <html lang="fr">
    <head>
      <meta charset="utf-8" />)%";

  constexpr char index_end[] PROGMEM = R"%(
      <title>RBR Magnac</title>
    </head>
    <body>
      <h1>Routeur solaire RBR Magnac</h1>
      <menu>
        <li><a href="/webserial">Web serial</a></li>
      </menu>
      <div class="flex-container">
        <div>
          <h2>À l'instant</h2>
          <div><label>Dernier reboot</label>
            <b id="last_boot"></b></div>
          <div><label>Conso totale</label>
            <b id="ptot"></b></div>
          <div><label>Conso effective</label>
            <b id="p2"></b></div>
          <div><label>Chauffe-eau</label>
            <b id="p1"></b></div>
        </div>
        <div>
          <h2>Contrôle</h2>
          <div>
            <label for="force_off">
              Arrêt forcé
            </label>
            <input type="checkbox" id="force_off"
              name="force_off" />
          </div>
          <div>
            <label for="force_on">
              Marche forcée
            </label>
            <input type="checkbox" id="force_on"
              name="force_on"/>
          </div>
          <div>
            <label for="hc_on">
              Marche heure creuse
            </label>
            <input type="checkbox" id="hc_on"
              disabled="disabled" />
          </div>
          <div>
            <b id="heures_hphc"></b>
          </div>
        </div>
      </div>

      <h2>Consommation/surplus</h2>
      <div>
        <label for="ekWh_hp">Heure pleine</label>
        <b>0.<input type="number"
               id="ekWh_hp" value="2874" />
        €/kWh</b>
      </div>
      <div>
        <label for="ekWh_hc">Heure creuse</label>
        <b>0.<input type="number"
               id="ekWh_hc" value="2088" />
        €/kWh</b>
      </div>

      <h3>Consommation sur 24 heures</h3>
      <div class="flex-container">
        <div>
          <div><label>Cumulé jour</label>
            <b id="cumul_hp_24h"></b></div>
          <div><label>Cumulé nuit</label>
            <b id="cumul_hc_24h"></b></div>
          <div><label>Cumulé total</label>
            <b id="cumul_tot_24h"></b></div>
        </div>
        <div>
          <div><label>Chauffe-eau surplus</label>
            <b id="eco_hp_24h"></b></div>
          <div><label>Chauffe-eau nuit</label>
            <b id="eco_hc_24h"></b></div>
        </div>
      </div>

      <div><canvas id="pltconso_24h"></canvas></div>

      <h3>Consommation sur une heure</h3>
      <div class="flex-container">
        <div>
          <div><label>Cumulé jour</label>
            <b id="cumul_hp_1h"></b></div>
          <div><label>Cumulé nuit</label>
            <b id="cumul_hc_1h"></b></div>
          <div><label>Cumulé total</label>
            <b id="cumul_tot_1h"></b></div>
        </div>
        <div>
          <div><label>Chauffe-eau surplus</label>
            <b id="eco_hp_1h"></b></div>
          <div><label>Chauffe-eau nuit</label>
            <b id="eco_hc_1h"></b></div>
        </div>
      </div>
      
      <div><canvas id="pltconso_1h"></canvas></div>

      <h3>Consommation sur 15 minutes</h3>
      <div class="flex-container">
        <div>
          <div><label>Cumulé jour</label>
            <b id="cumul_hp_15min"></b></div>
          <div><label>Cumulé nuit</label>
            <b id="cumul_hc_15min"></b></div>
          <div><label>Cumulé total</label>
            <b id="cumul_tot_15min"></b></div>
        </div>
        <div>
          <div><label>Chauffe-eau surplus</label>
            <b id="eco_hp_15min"></b></div>
          <div><label>Chauffe-eau nuit</label>
            <b id="eco_hc_15min"></b></div>
        </div>
      </div>
      
      <div><canvas id="pltconso_15min"></canvas></div>

      <div id="ota" class="disabled">
        <h2>Mise à jour OTA</h2>
        <b id="ota_text">Pas de mise à jour en cours.</b>
        <progress id="ota_progress" max="100">
          Pas de mise à jour en cours.
        </progress>
      </div>

      <!-- <div><canvas id="screen">Écran</canvas></div> -->
    </body>
  </html>)%";
}

#endif /* INDEX_H */
