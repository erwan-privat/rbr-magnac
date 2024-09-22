#ifndef STYLE_H
#define STYLE_H

namespace html
{
  constexpr char style[] PROGMEM = R"%(
  <link rel="preconnect"
        href="https://fonts.googleapis.com">
  <link rel="preconnect"
        href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Inconsolata:wght@200..900&family=Inria+Sans:ital,wght@0,300;0,400;0,700;1,300;1,400;1,700&family=Ubuntu:ital,wght@0,300;0,400;0,500;0,700;1,300;1,400;1,500;1,700&display=swap" rel="stylesheet">
    <style>
      :root {
        /* --bg:   hsl(50, 100%, 90%); */
        --bg:   hsl(180, 100%, 90%);
        --good: hsl(135, 90%, 30%);
        --bad:  hsl(0,   90%, 30%);
        --box-bg: hsla(50, 100%, 90%, 40%);
        /* --box-bg: #fefefe; */
      }

      body {
        font-family: "Inria Sans", Ubuntu, sans-serif;
        font-size: 1em;
        width: calc(100% - 60px);
        margin: auto;
        background-color: var(--bg);
        background-image: radial-gradient(
          circle at 30px 30px,
          #fff 0%, #ff8 10%, var(--bg) 50%);
        background-attachment: fixed;
        color: #222;
      }

      h1 {
        margin-top: 20px;
        background: linear-gradient(to right,
          #f80 0%, #840 35%, #000 100%);
        background-clip: text;
        color: transparent;
      }

      label {
        display: inline-block;
        width: 170px;
      }

      label::after {
        content: "\00a0: ";
      }

      b {
        font-family: "Inconsolata", monospace;
        font-weight: bold;
      }
      b input {
        font: inherit;
        background: #fff8;
        border: 0;
        border-bottom: 1px solid #0008;
      }

      #heures_hphc::before {
        content: "🕘 \00a0";
      }

      #heures_hphc.hc::after {
        content: " ✓";
      }

      .eco_hp, #heures_hphc.hc {
        color: var(--good);
      }
      .eco_hc, .cumul {
        color: var(--bad);
      }

      canvas {
        margin-top: 30px;
      }

      .flex-container {
        display: flex;
        flex-wrap: wrap;
        justify-content: space-between;
      }

      .box h3, .box h2 {
        margin: -10px;
        margin-bottom: 10px;
        padding: 10px;
        background: #0005;
        border-radius: 5px 5px 0 0;
        color: #fff;
        text-shadow: 0 0 1px #000, 0 0 1px #000;
      }

      .box {
        background: #fff5;
        padding: 10px;
        box-shadow: 0 0 8px #000000aa;
        margin-top: 20px;
        border-radius: 5px;
        backdrop-filter: blur(10px);
      }

      #instant label {
        width: 120px;
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
        left: 20px;
        right: 20px;
        z-index: 1000;
        background: #000000aa;
      }

      #ota h2 {
        padding: 20px;
        text-align: center;
      }

      #ota_text {
        display: block;
        text-align: center;
       }

      #ota_progress {
        width: calc(100% - 40px);
        margin: 20px;
        height: 20px;
        border: 1px solid #ededf0;
      }

      /* dev test only */
      .chart img {
        width: 100%;
        margin-top: 30px;
      }
    </style>
  )%";
}

#endif /* STYLE_H */
