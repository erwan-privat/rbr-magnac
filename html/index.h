#ifndef INDEX_H
#define INDEX_H

#include <Arduino.h>

// #ifdef  TEMPLATE_PLACEHOLDER
   // #undef  TEMPLATE_PLACEHOLDER
// #endif
// #define TEMPLATE_PLACEHOLDER '$'

namespace html
{
  constexpr char content[] PROGMEM = R"%(<!DOCTYPE html>
  <html lang="fr">
    <head>
      <meta charset="utf-8" />
      <style>
        body {
          font-family: sans-serif;
          width: 80%;
          margin: auto;
          background: hsl(60, 100%, 90%);
          color: #222;
        }
        label {
          display: inline-block;
          width: 235px;
        }
        label::after {
          content: "\00a0: ";
        }
        b, b.input {
          font-family: monospace;
          font-weight: bold;
          font-size: 1.2em;
        }
        #screen {
          border: 1px solid grey;
        }
        #ota.disabled {
          display: none;
        }
        #ota.enabled {
          backdrop-filter: blur(5px);
          color: white;
          display: block;
          position: fixed;
          top: 20px;
          bottom: 20px;
          left: 20px;
          right: 20px;
          z-index: 99999;
          background: #000000aa;
        }
        #ota h2 {
          padding: 20px;
        }
        #ota_text {
          display: block;
          text-align: center;
        }
        #ota_progress {
          width: calc(100% - 40px);
          margin: 20px;
          heigth: 20px;
          border: 1px solid #ededf0;
        }
      </style>
      <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/chartjs-plugin-annotation/3.0.1/chartjs-plugin-annotation.min.js" integrity="sha512-Hn1w6YiiFw6p6S2lXv6yKeqTk0PLVzeCwWY9n32beuPjQ5HLcvz5l2QsP+KilEr1ws37rCTw3bZpvfvVIeTh0Q==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
      <script>

      (function () {
        "use strict";
        window.Magnac = {};
        const byId = document.getElementById.bind(document);
        function bitswap (b) {
          b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
          b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
          b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
          return b;
        }
        
        let ota_refresh = false;

        document.addEventListener("DOMContentLoaded", () =>{ 
          const labels = ["24h", "1h", "15min"];
          Magnac.prix_hp = 2874;
          Magnac.prix_hc = 2088;
          Magnac.chtconso = {};
          Magnac.data = {};

          Chart.defaults.font.size = 14;
          labels.forEach(function (label) {
            Magnac.chtconso[label] = new Chart(
              byId("pltconso_" + label), {
                type: "line",
                data: [],
                options: {
                  scales: {
                    y: {
                      suggestedMin: 0,
                      title: {
                        display: true,
                        text: "Puissance (W)"
                      }
                    },
                  },
                },
              });
          });
          
          function rotateArray(arr, ix) {
            const left = arr.slice(0, ix);
            const right = arr.slice(ix, arr.length);
            return right.concat(left);
          }

          function hoursLabels(size, res) {
            const now = new Date;
            res *= 1000;
            return [...Array(size).keys()].map(function(i) {
              const d = new Date;
              d.setTime(now.getTime() + (i - size) * res);
              return d.toLocaleString("fr-FR")
                .split(" ")[1];
            });
          }

          function updateOta() {
            fetch("/ota").then(function (r) {
              if (!r.ok)
                throw new Error("ota HTTP " + r.status);
              return r.json();
            }).then(function (j) {
              Magnac.ota = j;
              byId("last_boot").innerHTML =
                new Date(j.last_boot * 1000)
                  .toLocaleString("fr-FR");
              byId("ota_progress").setAttribute(
                "value", j.progress);
              byId("ota_text").textContent =
                j.progress + " %";
              if (j.updating) {
                ota_refresh = true;
                byId("ota").classList
                  .toggle("disabled", false);
                byId("ota").classList
                  .toggle("enabled", true);
              }
              else if (ota_refresh)
                location.reload();
            });
          }

          function updateWatts() {
            fetch("/watts").then(function (r) {
              if (!r.ok)
                throw new Error("watts HTTP " + r.status);
              return r.json();
              }).then(function (j) {
                Magnac.watts = j;
                byId("p1").innerHTML =
                  -Math.round(j.power1) + " W";

                byId("p2").innerHTML =
                  Math.round(j.power2) + " W";

                byId("ptot").innerHTML =
                  Math.round(j.power2 - j.power1) + " W";
              });
          }

          function updateDimmer() {
            fetch("/dimmer").then(r => {
              if (!r.ok)
                throw new Error("dimmer HTTP " + r.status);
              return r.json();
            }).then(function (j) {
              Magnac.dimmer = j;
            });
          }

          // function drawScreen(bin) {
            // console.log(bin);
          // }

          // const ctx = byId("screen").getContext("2d");
          // ctx.font = "28px sans-serif";
          // ctx.fillText("RBR Magnac", 24, 36);
          // function updateScreen() {
            // fetch("/screen").then(r => {
              // if (!r.ok)
                // throw new Error("screen HTTP " + r.status);
              // return r.json();
            // }).then(function (j) {
              // Magnac.screen = j;
              // // TODO decode 64
              // console.log(j.xbmp64)
              // drawScreen(atob(j.xbmp64));
            // });
          // }

          function integrate(yy, dx) {
            return yy.reduce((a, y) => a + y * dx);
          }

          const onlyPos = arr =>
            arr.map(x => Math.max(x, 0));

          function inputChanged () {
            Magnac.prix_hp = parseInt(byId("ekWh_hp").value);
            Magnac.prix_hc = parseInt(byId("ekWh_hc").value);
            labels.forEach(updateData);
          }

          byId("ekWh_hp").addEventListener("change",
            inputChanged);
          byId("ekWh_hc").addEventListener("change",
            inputChanged);

          function closestIx(num, arr) {
            var curr = arr[0];
            var curr_ix = 0;
            var diff = Math.abs(num - curr);
            for (var ix = 1; ix < arr.length; ++ix) {
              var newdiff = Math.abs(num - arr[ix]);
              if (newdiff < diff) {
                diff = newdiff;
                curr = arr[ix];
                curr_ix = ix;
              }
            }
            return curr_ix;
          }

          // function parseHM(hm) {
            // const h = Math.floor(hm / 100);
            // const m = hm % 100;
            // return { h: h, m: m };
          // }

          // function indexOfHM(label, hm) {
            // const d = Magnac.data[label];
            // const tt = hoursLabels(d.p2.length, d.res);
            // const tt_int = tt.map(t =>
              // parseInt(t.replaceAll(":", "")));
            // const time = parseInt(hm) * 100;
            // return closestIx(tt_int, time);
          // }

          function cleanUpData(j, label) {
            j.p1_hc = j.p1_hc.map(x => -x);
            j.p1_hp = j.p1_hp.map(x => -x);
            Magnac.data[label] = j;
            const p1 = j.p1_hc.map((x, i) =>
              x + j.p1_hp[i]);
            const p2 = j.p2_hc.map((x, i) =>
              x + j.p2_hp[i]);
            Magnac.data[label].p1 = p1;
            Magnac.data[label].p2 = p2;
            return j;
          }

          function updateData(label) {
            fetch("/data_" + label).then(r => {
              if (!r.ok)
                throw new Error(`${label} HTTP${r.status}`);
              return r.json();
            }).then(function (j) {
              j = cleanUpData(j, label);
              console.log(j);

              // ["hp", "hc"].forEach(h => {
              ["hp"].forEach(h => {
                const conso = onlyPos(j["p2_" + h]);
                const cumul = integrate(conso, j.res)
                  / 3600e3;
                const prix = Magnac["prix_" + h];
                const euro = cumul * prix / 1e4;
                byId(`cumul_${h}_${label}`).innerHTML = 
                  `${cumul.toFixed(3)} kWh
                  &times; 0.${prix} €/kWh =
                  ${euro.toFixed(2)} €`;

                const eco = onlyPos(j["p1_" + h]);
                const total = integrate(eco, j.res) / 3600e3;
                const eur_eco = total * prix / 1e4;
                byId(`eco_${h}_${label}`).innerHTML =
                  `${total.toFixed(3)} kWh
                  &times; 0.${prix} €/kWh =
                  ${eur_eco.toFixed(2)} €`;
              }); // hp hc

              const t_labels = hoursLabels(j.p2.length,
                j.res);
              Magnac.chtconso[label].data = {
                labels: t_labels,
                datasets: [
                {
                  label: "Consommation/surplus (W) HP",
                  data: rotateArray(j.p2_hp, j.ix),
                  pointStyle: false,
                  fill: {
                    target: "origin",
                    above: "#ff000044",
                    below: "#00ff0044",
                  },
                },
                {
                  pointStyle: false,
                  label: "Chauffe-eau (W) HP",
                  data: rotateArray(j.p1_hp, j.ix),
                },
                {
                  label: "Consommation/surplus (W) HC",
                  data: rotateArray(j.p2_hc, j.ix),
                  pointStyle: false,
                  fill: {
                    target: "origin",
                    above: "#ff880044",
                    below: "#00ff0044",
                  },
                },
                {
                  pointStyle: false,
                  label: "Chauffe-eau (W) HC",
                  data: rotateArray(j.p1_hc, j.ix),
                },
                ],
              };

              Magnac.chtconso[label].update("none");
            });
          }

          window.setWaitingInterval = function(f, d, p) {
            f(p);
            setTimeout(function rf (p) {
              f(p);
              setTimeout(rf, d, p);
            }, d, p);
          }

          setWaitingInterval(updateOta,    1000);
          setWaitingInterval(updateWatts,  2000);
          setWaitingInterval(updateDimmer, 2000);
          setWaitingInterval(updateData, 180000, "24h");
          setWaitingInterval(updateData,   8000, "1h");
          setWaitingInterval(updateData,   2000, "15min");
          
          // updateScreen();
          // setInterval(updateScreen,     2000);
        });
      })();
      </script>
      <title>RBR Magnac</title>
    </head>
    <body>
      <h1>Routeur solaire RBR Magnac</h1>
      <menu>
        <li><a href="/webserial">Web serial</a></li>
      </menu>
      <h2>À l'instant</h2>
      <div>
        <div><label>Dernier reboot</label>
          <b id="last_boot"></b></div>
        <div><label>Consommation totale</label>
          <b id="ptot"></b></div>
        <div><label>Consommation effective</label>
          <b id="p2"></b></div>
        <div><label>Chauffe-eau</label>
          <b id="p1"></b>
        </div>
      </ul>

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
      <div><label>Cumulé HP</label>
        <b id="cumul_hp_24h"></b></div>
      <div><label>Chauffe-eau HP</label>
        <b id="eco_hp_24h"></b></div>
      <div><canvas id="pltconso_24h"></canvas></div>

      <h3>Consommation sur une heure</h3>
      <div><label>Cumulé HP</label> <b id="cumul_hp_1h"></b></div>
      <div><label>Chauffe-eau HP</label>
        <b id="eco_hp_1h"></b></p>
      <div><canvas id="pltconso_1h"></canvas></div>

      <h3>Consommation sur 15 minutes</h3>
      <div>
        <label>Cumulé HP</label> <b id="cumul_hp_15min"></b>
      </div>
      <div><label>Chauffe-eau HP</label>
        <b id="eco_hp_15min"></b></p>
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
