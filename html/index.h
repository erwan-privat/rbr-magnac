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
      <style>
        body {
          width: 80%;
          margin: auto;
        }
        #screen {
          border: 1px solid grey;
        }
      </style>
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
          const byId = document.getElementById
            .bind(document);

          document.addEventListener("DOMContentLoaded",
          () => {
            let canvas = byId("screen");
            canvas.width = 128 * scale;
            canvas.height = 64 * scale;
            let ctx = canvas.getContext("2d");

            function drawByte(x, y, n) {
              for (var i = 0; i < 8; ++i) {
                ctx.fillStyle = (n & (1 << i)) > 0
                  ? "#00a5ff" : "black";
                ctx.fillRect((x + i) * scale, y * scale,
                  scale, scale);
              }
            }

            let screen = [];

            function drawScreen() {
              for (let i = 0 ; i < screen.length; ++i) {
                drawByte(
                  i * 8 % 128,
                  Math.floor(i * 8 / 128),
                  (screen[i]));
              }
            }

            function updateData() {
              fetch("/data").then(r => {
                if (!r.ok)
                  throw new Error("HTTP error " + r.status);
                return r.json();
              }).then(j => {
                byId("p1").innerHTML = -Math.round(j.power1)
                  + " W";
                byId("p2").innerHTML = Math.round(j.power2)
                  + " W";
                byId("ptot").innerHTML = Math.round(j.power2
                  - j.power1) + " W";
              });
            }

            setInterval(updateData, 1000);
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
      <ul class="data">
        <li>Consommation totale : <b id="ptot"></b></li>
        <li>Consommation effective : <b id="p2"></b></li>
        <li>Consommation du chauffe-eau : <b id="p1"></b></li>
      </ul>
      <canvas id="screen">Loading screen...</canvas>
      <canvas id="pltconso"></canvas>
      <canvas id="pltchofo"></canvas>
      )%";
}

#endif /* INDEX_H */
