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
        --bg:   hsl(180, 100%, 90%);
        --good: hsl(135,  90%, 30%);
        --bad:  hsl(  0,  90%, 30%);
        /* --sky: hsla(200, 100%, 14%, 50); */
        --sky: #0005;
      }

      body {
        font-family: "Inria Sans", Ubuntu, sans-serif;
        font-size: 1em;
        width: calc(100% - 60px);
        margin: auto;
        margin-bottom: 50px;
        background-color: var(--bg);
        background-image: radial-gradient(
          circle at 30px 30px,
          #fff 0%, #ff8 10%, var(--bg) 50%);
        background-attachment: fixed;
        color: #111;
      }

      menu { 
        position: absolute;
        top: 10px;
        right: 20px;
        padding: 10px;
      }

      menu li {
        list-style: none;
      }
      menu li a {
        display: inline-block;
        padding: 5px;
        text-decoration: none;
      }
      menu li a::after {
        content: "\00a0🗒️";
      }
      menu li a:hover {
        background: #0002;
      }

      h1 {
        margin-top: 20px;
        background: linear-gradient(to right,
          #f80 0%, #840 35%, #000 100%);
        background-clip: text;
        color: transparent;
      }

      .version {
        font-size: 66%;
        font-family: "Inconsolata", monospace;
        opacity: 50%;
        margin-left: 5px;
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

      #heures_hphc.hc::after {
        content: " ✓";
      }

      .eco_hp, #heures_hphc.hc {
        color: var(--good);
      }
      .eco_hc, .cumul {
        color: var(--bad);
      }

      #radiateur {
        margin-left: auto;
        margin-right: 20px;
      }

      #is_on_radi.off {
        color: var(--bad);
      }
      #is_on_radi.off::after {
        content: " ❌";
      }
      #is_on_rad.on {
        color: var(--good);
      }
      #is_on_radi.on::after {
        content: " ✓";
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
        background: var(--sky);
        border-radius: 5px 5px 0 0;
        color: #fff;
        /* text-shadow: 0 0 1px #000, 0 0 1px #000; */
        font-size: 1.3em;
      }

      .box {
        background: #fff5;
        padding: 10px;
        box-shadow: 0 0 8px #000000aa;
        margin-top: 20px;
        border-radius: 5px;
        backdrop-filter: blur(10px);
      }

      #instant {
        margin-right: 20px;
      }

      #instant label {
        width: 120px;
      }
      #instant h2::before {
        content: "⚡\00a0";
      }
      #radiateur h2::before {
        content: "🔌\00a0";
      }
      #control h2::before {
        content: "⚙️\00a0\00a0";
      }
      #donnees_conso::before {
        content: "📈\00a0";
      }
      #heures_hphc::before {
        content: "🕘\00a0";
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
    </style>
  )%";
}

#endif /* STYLE_H */
