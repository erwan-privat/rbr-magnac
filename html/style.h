#ifndef STYLE_H
#define STYLE_H

namespace html
{
  constexpr char style[] PROGMEM = R"%(
    <style>
      body {
        font-family: Ubuntu, "Segoe UI", sans-serif;
        font-size: 1em;
        width: 80%;
        margin: auto;
        background: hsl(50, 100%, 90%);
        color: #222;
      }

      label {
        display: inline-block;
        width: 170px;
      }

      label::after {
        content: "\00a0: ";
      }

      b, b.input {
        font-family: "Courier New", monospace;
        font-weight: bold;
        /* font-size: 1.1em; */
      }

      .eco_hp {
        color: hsl(135, 90%, 30%);
      }
      .eco_hc, .cumul {
        color: hsl(0, 90%, 30%);
      }

      canvas {
        margin-top: 30px;
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

      .flex-container {
        display: flex;
        flex-wrap: wrap;
        justify-content: space-between;
      }
    </style>
  )%";
}

#endif /* STYLE_H */
