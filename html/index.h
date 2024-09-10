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
          border: 4px solid grey;
        }

        #screen {
          display: inline-block;
          width: 128px;
          height: 64px;
          background-color: black;
          color: lightblue;
        }
      </style>
      <script>
        "use strict";
        (function () {
          // window.alert("Herbert !");
        })();
      </script>
      <title>RBR Magnac</title>
    </head>
    <body>)%";

  constexpr char* footer PROGMEM = R"%(
    </body>
  </html>)%";

  constexpr char* index PROGMEM = R"%(
      <h1>Routeur solaire RBR Magnac</h1>
      <div>
        <p>Conso : <b>$CONSO$ W</b></p>
        <canvas id="screen">Loading screen...</canvas>
      </div>)%";
}

#endif /* INDEX_H */
