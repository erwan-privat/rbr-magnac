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
  <html>
    <head>
      <meta charset="utf-8" />
      <style>
        body {
          width: 80%;
          margin: auto;
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
      <script src="https://cdn.jsdelivr.net/npm/chart.js">
      </script>
      <script>
      // window.Magnac = {};
      // window.Magnac.bitswap = function (b) {
        // b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
        // b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
        // b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
        // return b;
      // }

      (function () {
        "use strict";
        window.Magnac = {};
        const byId = document.getElementById.bind(document);
        
        let ota_refresh = false;

        document.addEventListener("DOMContentLoaded",
          function() {
            const chtconso_15min = new Chart(
              byId("pltconso_15min"), {
                type: "line",
                data: [],
                options: {
                  scales: {
                    y: {
                      title: {
                        display: true,
                        text: 'Puissance (W)'
                      }
                    },
                  },
                },
            });

            const chtconso_1h = new Chart(
              byId("pltconso_1h"), {
                type: "line",
                data: [],
                options: {
                  scales: {
                    y: {
                      title: {
                        display: true,
                        text: 'Puissance (W)'
                      }
                    },
                  },
                },
            });

            const chtconso_24h = new Chart(
              byId("pltconso_24h"), {
                type: "line",
                data: [],
                options: {
                  respsonsive: true,
                  scales: {
                    y: {
                      title: {
                        display: true,
                        text: 'Puissance (W)'
                      }
                    },
                  },
                },
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
              window.Magnac.ota = j;
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
                window.location.reload();
            });
          }

          function updateWatts() {
            fetch("/watts").then(function (r) {
              if (!r.ok)
                throw new Error("watts HTTP " + r.status);
              return r.json();
              }).then(function (j) {
                window.Magnac.watts = j;
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
              window.Magnac.dimmer = j;
            });
          }

          function updateData_15min() {
            fetch("/data_15min").then(r => {
              if (!r.ok)
                throw new Error("data_15min HTTP " + r.status);
              return r.json();
            }).then(function (j) {
              window.Magnac.data_15min = j;
              chtconso_15min.data = {
                labels: hoursLabels(j.p2.length, j.res),
                datasets: [
                {
                  label: "Consommation/surplus (W)",
                  data: rotateArray(j.p2, j.ix),
                  pointStyle: false,
                  fill: {
                    target: 'origin',
                    above: "#ff000044",
                    below: "#00ff0044",
                  },
                },
                {
                  pointStyle: false,
                  label: "Chauffe-eau (W)",
                  data: rotateArray(j.p1,
                    j.ix).map(x => -x),
                }]
              };
              chtconso_15min.update("none");
            });
          }

          function updateData_1h() {
            fetch("/data_1h").then(r => {
              if (!r.ok)
                throw new Error("data_1h HTTP " + r.status);
              return r.json();
            }).then(function (j) {
              window.Magnac.data_1h = j;
              chtconso_1h.data = {
                labels: hoursLabels(j.p2.length, j.res),
                datasets: [
                {
                  label: "Consommation/surplus (W)",
                  data: rotateArray(j.p2, j.ix),
                  pointStyle: false,
                  fill: {
                    target: 'origin',
                    above: "#ff000044",
                    below: "#00ff0044",
                  },
                },
                {
                  pointStyle: false,
                  label: "Chauffe-eau (W)",
                  data: rotateArray(j.p1,
                    j.ix).map(x => -x),
                }]
              };
              chtconso_1h.update("none");
            });
          }

          function updateData_24h() {
            fetch("/data_24h").then(r => {
              if (!r.ok)
                throw new Error("data_24h HTTP " + r.status);
              return r.json();
            }).then(function (j) {
              window.Magnac.data_24h = j;
              chtconso_24h.data = {
                labels: hoursLabels(j.p2.length, j.res),
                datasets: [
                {
                  label: "Consommation/surplus (W)",
                  data: rotateArray(j.p2, j.ix),
                  pointStyle: false,
                  fill: {
                    target: 'origin',
                    above: "#ff000044",
                    below: "#00ff0044",
                  },
                },
                {
                  pointStyle: false,
                  label: "Chauffe-eau (W)",
                  data: rotateArray(j.p1,
                    j.ix).map(x => -x),
                }]
              };
              chtconso_24h.update("none");
            });
          }

          updateOta();
          updateWatts();
          updateDimmer();
          updateData_24h();
          updateData_1h();
          updateData_15min();

          setInterval(updateOta,        1000);
          setInterval(updateWatts,      2000);
          setInterval(updateDimmer,     2000);
          setInterval(updateData_24h, 180000);
          setInterval(updateData_1h,    8000);
          setInterval(updateData_15min, 2000);
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
      <ul class="data">
        <li>Dernier reboot : <b id="last_boot"></b></li>
        <li>Consommation totale : <b id="ptot"></b></li>
        <li>Consommation effective : <b id="p2"></b></li>
        <li>Consommation du chauffe-eau :
          <b id="p1"></b>
        </li>
      </ul>
      <h2>Consommation/surplus</h2>
      <!-- <div><canvas id="screen">Loading screen...
      </canvas></div> -->
      <h3>Consommation sur 24 heures</h3>
      <div><canvas id="pltconso_24h"></canvas></div>
      <h3>Consommation sur une heure</h3>
      <div><canvas id="pltconso_1h"></canvas></div>
      <h3>Consommation sur 15 minutes</h3>
      <div><canvas id="pltconso_15min"></canvas></div>

      <div id="ota" class="disabled">
      <h2>Mise à jour OTA</h2>
        <b id="ota_text">Pas de mise à jour en cours.</b>
        <progress id="ota_progress" max="100">
          Pas de mise à jour en cours.
        </progress>
      </div>
    </body>
  </html>)%";
}

#endif /* INDEX_H */
