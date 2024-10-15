#ifndef SCRIPT_H
#define SCRIPT_H

namespace html
{
  constexpr char script[] PROGMEM = R"%(
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script>
    (function () {
      "use strict";
      window.Magnac = {};
      window.byId = document.getElementById.bind(document);
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
          var ongoing = false;
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

              ongoing = true;
            }
            else if (ota_refresh)
            {
              ongoing = false;
              setTimeout(() => window.location.reload(),
                3000);
              return;
            }

          }).catch(function (e) {
            console.error(e);
          }).finally(function (j) {
            setTimeout(updateOta, ongoing ? 500 : 3000);
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

              var ptot = j.power2 - j.power1;
              if (Magnac.dimmer?.hc_on)
                ptot = j.power2;
              byId("ptot").innerHTML =
                Math.round(ptot) + " W";

            }).catch(function (e) {
              console.error(e);
            }).finally(function (j) {
              setTimeout(updateWatts, 2000);
            });
        }

        function decodeHMS(hms) {
          var s = hms % 100;
          hms = Math.floor(hms / 100);
          var m = hms % 100;
          hms = Math.floor(hms / 100);
          var h = hms;

          const toS = n => String(n).padStart(2, "0");
          return `${toS(h)} h ${toS(m)}`;
        }

        function updateDimmer() {
          fetch("/dimmer").then(r => {
            if (!r.ok)
              throw new Error("dimmer HTTP " + r.status);
            return r.json();
          }).then(function (j) {
            Magnac.dimmer = j;
            byId("force_off"     ).checked = j.force_off;
            byId("force_on"      ).checked = j.force_on;
            byId("hc_on"         ).checked = j.hc_on;
            byId("force_off_radi").checked = j.force_off_radi;
            byId("force_on_radi" ).checked = j.force_on_radi;

            var is_on = byId("is_on_radi");
            is_on.innerHTML = "normalement "
              + (j.is_on_radi ? "allumé" : "éteint");
            is_on.classList.toggle("on", j.is_on_radi);
            is_on.classList.toggle("off", !j.is_on_radi);

            const hphc = byId("heures_hphc");
            hphc.innerHTML =
              `${decodeHMS(j.start_hc)} –­
               ${decodeHMS(j.end_hc)}`;
            const in_hc = j.time >= j.start_hc
              && j.time < j.end_hc;
            // const in_hc = j.hc_on
            //   && j.time >= j.start_hc && j.time < j.end_hc;
            hphc.classList.toggle("hc", in_hc);
          }).catch(function (e) {
            console.error(e);
          }).finally(function (j) {
            setTimeout(updateDimmer, 1000);

            byId("force_off"     ).disabled = false;
            byId("force_on"      ).disabled = false;
            byId("hc_on"         ).disabled = false;
            byId("force_off_radi").disabled = false;
            byId("force_on_radi" ).disabled = false;
          });
        }

        function updateControl(event) {
          const checked = event.currentTarget.checked;
          const name    = event.currentTarget.name;
          const param = `${name}=${checked}`;
          byId("force_off"     ).disabled = true;
          byId("force_on"      ).disabled = true;
          byId("hc_on"         ).disabled = true;
          byId("force_off_radi").disabled = true;
          byId("force_on_radi" ).disabled = true;
          console.log(param)
          fetch("/control?" + param).then(r => {
            if (!r.ok)
              throw new Error(`control HTTP${r.status}`);
          });
        }

        byId("force_on"      ).addEventListener("change",
          updateControl);
        byId("force_off"     ).addEventListener("change",
          updateControl);
        byId("hc_on"         ).addEventListener("change",
          updateControl);
        byId("force_on_radi" ).addEventListener("change",
          updateControl);
        byId("force_off_radi").addEventListener("change",
          updateControl);

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

        const time_updateData = {};
        time_updateData["24h"]   = 180000;
        time_updateData["1h"]    =   8000;
        time_updateData["15min"] =   2000;

        function updateData(label) {
          fetch("/data_" + label).then(r => {
            if (!r.ok)
              throw new Error(`${label} HTTP${r.status}`);
            return r.json();
          }).then(function (j) {
            j = cleanUpData(j, label);

            const hphc = [];
            ["hp", "hc"].forEach(h => {
              const conso = onlyPos(j["p2_" + h]);
              const cumul = integrate(conso, j.res)
                / 3600e3;
              const prix = Magnac["prix_" + h];
              const euro = cumul * prix / 1e4;
              byId(`cumul_${h}_${label}`).innerHTML =
                `${cumul.toFixed(3)} kWh
                &times; 0.${prix} €/kWh =
                <span class="cumul">
                  ${euro.toFixed(2)} €
                </span>`;


              const eco = onlyPos(j["p1_" + h]);
              const total = integrate(eco, j.res) / 3600e3;
              const eur_eco = total * prix / 1e4;

              hphc.push({ cumul, euro, total });

              byId(`eco_${h}_${label}`).innerHTML =
                `${total.toFixed(3)} kWh
                &times; 0.${prix} €/kWh =
                <span class="eco_${h}">
                  ${eur_eco.toFixed(2)} €
                </span>`;
            }); // hp hc

            const cumtot = hphc[0].cumul + hphc[1].cumul;
            const eurtot = hphc[0].euro  + hphc[1].euro;
            byId(`cumul_tot_${label}`).innerHTML =
              `${cumtot.toFixed(3)} kWh,
              <span class="cumul">
                ${eurtot.toFixed(2)} €
              </span>`;

            const consotot = cumtot + hphc[0].total;
            var ratio = 1 - cumtot / consotot;
            ratio = ratio || 0;
            byId(`autoconso_${label}`).innerHTML =
              `1 - ${cumtot.toFixed(3)}
                / ${consotot.toFixed(3)}
                = <span class="eco_hp">
                ${(ratio * 100).toFixed(2)} %
                </span>`;

            const t_labels = hoursLabels(j.p2.length,
              j.res);
            Magnac.chtconso[label].data = {
              labels: t_labels,
              datasets: [
              {
                label: "Consommation/surplus HP (W)",
                data: rotateArray(j.p2_hp, j.ix),
                pointStyle: false,
                borderColor: "#36a2eb",
                fill: {
                  target: "origin",
                  above: "#ff000044",
                  below: "#00ff0044",
                },
              },
              {
                pointStyle: false,
                borderColor: "#cf0911",
                label: "Chauffe-eau HP (W)",
                data: rotateArray(j.p1_hp, j.ix),
              },
              {
                label: "Consommation HC (W)",
                data: rotateArray(j.p2_hc, j.ix),
                pointStyle: false,
                borderColor: "#7a49a5",
                fill: {
                  target: "origin",
                  above: "#7a49a544",
                  below: "#00ff0044",
                },
              },
              {
                pointStyle: false,
                borderColor: "#ff8800",
                label: "Chauffe-eau HC (W)",
                data: rotateArray(j.p1_hc, j.ix),
              },
              ],
            };

            Magnac.chtconso[label].update("none");
          }).catch(function (e) {
            console.error(e);
          }).finally(function (j) {
            setTimeout(updateData, time_updateData[label],
              label);
          });
        }

        updateOta();
        updateWatts();
        updateDimmer();

        updateData("24h");
        updateData("1h");
        updateData("15min");
      });
    })();
    </script>
  )%";
}

#endif /* SCRIPT_H */
