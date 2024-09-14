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
        (function () {
          "use strict";
          const scale = 2;
          document.addEventListener("DOMContentLoaded",
          function () {
            let canvas = document.getElementById("screen");
            canvas.width = 128 * scale;
            canvas.height = 64 * scale;
            let ctx = canvas.getContext("2d");

            function bitswap(b) {
              b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
              b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
              b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
              return b;
            }

            function drawByte(x, y, n) {
              for (var i = 0; i < 8; ++i) {
                ctx.fillStyle = (n & (1 << i)) > 0
                  ? "#00a5ff" : "black";
                ctx.fillRect((x + i) * scale, y * scale,
                  scale, scale);
              }
            }

            // let screen = [];
            let screen = atob("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAAAAAAAAAAAAABwAAAAAgAAAAAAAAAAAAAA4AAAAADACAAAAAAAAAAAAuAAAAABAAgAAAAAAAAAAANgAAAAAzAAAAAAAAAAAAABYAQAACDIAAAAAAAAAAAAAcAEAAICAAQAAAAAAAAAAAPAFAACEDQAAAAAAAAAAAADYBgAAYXQCAAAAAAAAAAAA4AoAACCSAQAAAAAAAAAAALADAACSCgAAAAAAAAAAAADgFACACGsAAAAAAAAAAAAAYBEAMECCAQAAAAAAAAAAAGGjDAKEHgAAAAAAAAAAABBgvkuMEWAAAAAAAAAAAAAAJm0bEUSGBAAAAAAAAAAAAJDth0CQGAQAAAAAAAAAAACx/zYiIWAQAAAAAAAAAAAA5P+NBISEAgIAAAAAAAAAANj/x1CQgIgEAQAAAAAAAADY/46BAUIgBQAAAAAAAAAA9v8vJQQAQgAAAAAAAAAAAPb/Z0QAAAADAAAAAAAAAAD8v/8YAgAQAAAAAAAAAAAA+/+fixAAAAAAAAAAAAAAAPrffyMEAYAAAAAAAAAAAEDy/zsUIAAAAAAAAAAAAABAz+/iRwIIAAEAAAAAAAAACL2/fRCAAAACAAAAAAAAALL/3/2TJAQACgAAAAAAAADE/n1/iwQBAAgAAAAAAABAyf/n/2YIAAA0AAAAAAAAAJl/v/s2IwAApAAAAAAAABDy/v3+zYQAAEgAAAAAAADA5urzfyMZAABYAAAAAAAADJm7z/+7SUEAkgEAAAAAACD5dPy/TyYJACQBAAAAAABhZkWx+3SZDCBkAgAAAAAAlNxJ4N/XJZKEmAYAAAAAwJC9u0h22c2QELIEAQAAAABqdgqcdm1fJ0mmGRAAAAAgCd9dtrCxM2lmbEAAAAAAILV9G0vr7v7GNFkDAAAAAEhmdjsbTWrLls1TTAIAAADIyN5+93TTvTlbtgAAAAAANpv9cszWXLfrtiwCAAAAABBpt585mWTb5rZtCgAAAADKTP65ba+9bd3blgAAAAAAapNtY+Mzs217b5skAAAAAJwU/+4OzHb37m1rAAAAAACRXNrWtmTOnb+3TQIAAAAAZlv/1bubuf39vg0AAAAAAEy6bZdrsu927+szBAAAAACZ7P6a3iT7/7+/lgAAAAAAJjXbZhTB3L339pdIAAAAADL1fRdBiHb/398tEQIAAAAJTfdZCiL67317a6ZMAAAAJnnbZiJgyv//70+iEQAAAJK2fpvEhPT93v62PTcAAAAZtu2ZFCS1v//7n+1MAgAATcy9JxFB4f63323bkgAAAA==");

            window.screen = screen;

            function drawScreen() {
              for (let i = 0 ; i < screen.length; ++i) {
                drawByte(
                  i * 8 % 128,
                  Math.floor(i * 8 / 128),
                  (screen[i]));
              }
            }

            // window.winie = winie;
            window.drawScreen = drawScreen;
            window.drawByte = drawByte;

            drawScreen();

            function updateData() {
              fetch("/data?name=power2").then(function (r) {
                r.text().then(function (t) {
                  document.getElementById("p2").innerHTML
                    = Math.round(t) + " W";
                });
              });
              fetch("/data?name=power1").then(function (r) {
                r.text().then(function (t) {
                  document.getElementById("p1").innerHTML =
                    -Math.round(t) + " W";
                });
              });

              window.loadBinaryResource = function (url) {
                const req = new XMLHttpRequest();
                req.open("GET", url, false);
                req.overrideMimeType("text/plain; "
                  + "charset=x-user-defined");
                req.send(null);
                return req.status === 200
                  ? req.responseText : "";
              }

              screen = loadBinaryResource("/data?name=screen");
              // drawScreen();    
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
        <li><a href="/">Moniteur</a></li>
        <li><a href="/webserial">Web serial</a></li>
      </menu>)%";

  constexpr char footer[] PROGMEM = R"%(
    </body>
  </html>)%";

  constexpr char index[] PROGMEM = R"%(
      <ul class="data">
        <li>Consommation totale : <b id="p2"></b></li>
        <li>Consommation du chauffe-eau : <b id="p1"></b></li>
      </ul>
      <canvas id="screen">Loading screen...</canvas>
      <canvas id="pltconso"></canvas>
      <canvas id="pltchofo"></canvas>
      )%";
}

#endif /* INDEX_H */
