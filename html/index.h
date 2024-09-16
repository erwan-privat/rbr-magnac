// eprivat 2024-09-10

#ifndef INDEX_H
#define INDEX_H

#ifdef  TEMPLATE_PLACEHOLDER
   #undef  TEMPLATE_PLACEHOLDER
#endif
#define TEMPLATE_PLACEHOLDER '$'

namespace html
{
  constexpr char header[] PROGMEM = R"%(<!DOCTYPE html>
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
          color: white;
          display: block;
          position: fixed;
          top: 20px;
          bottom: 20px;
          left: 20px;
          right: 20px;
          z-index: 99999;
          background: #000000dd;
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
        const scale = 2;
        const byId = document.getElementById.bind(document);
        
        let ota_refresh = false;

        document.addEventListener("DOMContentLoaded", () => {
          
          const chtconso180 = new Chart(byId("pltconso180"), {
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
          
          const chtconso2 = new Chart(byId("pltconso2"), {
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

          // const canvas = byId("screen");
          // const ctx = canvas.getContext("2d");
          // canvas.width = 128 * scale;
          // canvas.height = 64 * scale;
          // let screen = [];

          // function drawByte(x, y, n) {
            // for (var i = 0; i < 8; ++i) {
              // ctx.fillStyle = (n & (1 << i)) > 0
                // ? "#00a5ff" : "black";
              // ctx.fillRect((x + i) * scale, y * scale,
                // scale, scale);
            // }
          // }

          // function drawScreen() {
            // for (let i = 0 ; i < screen.length; ++i) {
              // drawByte(
                // i * 8 % 128,
                // Math.floor(i * 8 / 128),
                // (screen[i]));
            // }
          // }

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
              return d.toLocaleString("fr-FR").split(" ")[1];
            });
          }

          function updateData() {
            fetch("/data").then(r => {
              if (!r.ok)
                throw new Error("HTTP error " + r.status);
              return r.json();
            }).then(j => {
              window.Magnac = j;
              byId("last_boot").innerHTML =
                new Date(j.last_boot * 1000)
                  .toLocaleString("fr-FR");

              byId("ota_progress").setAttribute("value",
                j.ota.progress);
              byId("ota_text").textContent =
                j.ota.progress + " %";
              if (j.ota.updating) {
                ota_refresh = true;
                byId("ota").classList
                  .toggle("disabled", false);
                byId("ota").classList
                  .toggle("enabled", true);
              }
              else if (ota_refresh)
                window.location.reload();

              byId("p1").innerHTML =
                -Math.round(j.watts.power1) + " W";

              byId("p2").innerHTML =
                Math.round(j.watts.power2) + " W";

              byId("ptot").innerHTML =
                Math.round(j.watts.power2
                  - j.watts.power1) + " W";

              chtconso2.data = {
                labels: hoursLabels(j.data.p1_2.length,
                  j.data.res2),
                datasets: [
                {
                  label: "Consommation (W)",
                  data: rotateArray(j.data.p2_2, j.data.ix2),
                },
                {
                  label: "Chauffe-eau (W)",
                  data: rotateArray(j.data.p1_2,
                    j.data.ix2).map(x => -x),
                }]
              };
              chtconso2.update("none");

              chtconso180.data = {
                labels: hoursLabels(j.data.p1_180.length,
                  j.data.res180).map(t => t.slice(0, 5)),
                datasets: [
                {
                  label: "Consommation (W)",
                  data: rotateArray(j.data.p2_180,
                    j.data.ix180),
                },
                {
                  label: "Chauffe-eau (W)",
                  data: rotateArray(j.data.p1_180,
                    j.data.ix180).map(x => -x),
                }]
              };
              chtconso180.update("none");
            });
          }

          setInterval(updateData, 2000);
        });
      })();
      </script>
      <title>RBR Magnac</title>
    </head>
    <body>
      <h1>Routeur solaire RBR Magnac</h1>
      <menu>
        <li><a href="/webserial">Web serial</a></li>
      </menu>)%";

  constexpr char footer[] PROGMEM = R"%(
    </body>
  </html>)%";

  constexpr char index[] PROGMEM = R"%(
      <h2>À l'instant</h2>
      <ul class="data">
        <li>Dernier reboot : <b id="last_boot"></b></li>
        <li>Consommation totale : <b id="ptot"></b></li>
        <li>Consommation effective : <b id="p2"></b></li>
        <li>Consommation du chauffe-eau :
          <b id="p1"></b>
        </li>
      </ul>
      <h2>Consommation</h2>
      <!-- <div><canvas id="screen">Loading screen...
      </canvas></div> -->
      <h3>Conso sur 24 heures</h3>
      <div><canvas id="pltconso180"></canvas></div>
      <h3>Conso sur 15 minutes</h3>
      <div><canvas id="pltconso2"></canvas></div>

      <div id="ota" class="disabled">
      <h2>Mise à jour OTA</h2>
        <b id="ota_text">Pas de mise à jour en cours.</b>
        <progress id="ota_progress" max="100">
          Pas de mise à jour en cours.
        </progress>
      </div>
      )%";
}

#endif /* INDEX_H */
