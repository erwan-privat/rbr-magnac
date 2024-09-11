// eprivat 2024-09-10

#ifndef INDEX_H
#define INDEX_H

#ifdef  TEMPLATE_PLACEHOLDER
   #undef  TEMPLATE_PLACEHOLDER
#endif
#define TEMPLATE_PLACEHOLDER '$'

namespace html
{
  constexpr char* header PROGMEM = R"%(
  <!DOCTYPE html>
  <html>
    <head>
      <style>
        body {
          width: 80%;
          margin: auto;
        }

        #screen {
          width: 128px;
          height: 64px;
          /* background-color: black; */
          /* color: lightblue; */
        }
        .flex {
          display: flex;
          /* flex-direction: row | column; */
          flex-wrap: wrap;
        }
      </style>
      <script>
        "use strict";
        document.addEventListener("DOMContentLoaded", () => {
          console.log("Hello World!");
          let canvas = document.getElementById("screen");
          let ctx = canvas.getContext("2d");

          window.canvas = canvas;
          window.ctx = ctx;
        });
      </script>
      <title>RBR Magnac</title>
    </head>
    <body>
      <h1>Routeur solaire RBR Magnac</h1>
      <menu>
        <li><a href="/">Moniteur</a></li>
        <li><a href="/webserial">Web serial</a></li>
      </menu>)%";

  constexpr char* footer PROGMEM = R"%(
    </body>
  </html>)%";

  constexpr char* index PROGMEM = R"%(
      <div class="flex">
        <p>Conso : <b>$CONSO$ W</b></p>
        <canvas id="screen">Loading screen...</canvas>
      </div>
      <div>
        <img src="http://vesperaimperialis.fr/data/map/map-web.jpg"
          alt="a map of what is or what could havei been" />
      </div>)%";
}

#endif /* INDEX_H */
