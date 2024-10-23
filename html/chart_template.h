#ifndef CHART_TEMPLATE_H
#define CHART_TEMPLATE_H

namespace html
{
  constexpr char chart_template[] PROGMEM = R"%(
      <div class="box">
      <h3>%TITLE%</h3>
      <div class="flex-container">
        <div>
          <div><label>Cumulé jour</label>
            <b id="cumul_hp_%LABEL%">x.xxx kWh
                × 0.2874 €/kWh =
                <span class="cumul">
                  0.00 €
                </span></b></div>
          <div><label>Cumulé nuit</label>
            <b id="cumul_hc_%LABEL%">x.xxx kWh
                × 0.2088 €/kWh =
                <span class="cumul">
                  0.00 €
                </span></b></div>
          <div><label>Cumulé total</label>
            <b id="cumul_tot_%LABEL%">x.xxx kWh,
              <span class="cumul">
                0.00 €
              </span></b></div>
        </div>
        <div>
          <div><label>Chauffe-eau surplus</label>
            <b id="eco_hp_%LABEL%">x.xxx kWh
                × 0.2874 €/kWh =
                <span class="eco_hp">
                  0.00 €
                </span></b></div>
          <div><label>Chauffe-eau nuit</label>
            <b id="eco_hc_%LABEL%">x.xxx kWh
                × 0.2088 €/kWh =
                <span class="eco_hc">
                  0.00 €
                </span></b></div>
          <div><label>Auto-consommation</label>
            <b id="autoconso_%LABEL%">(x.xx + x.xx) / x.xx =
                <span class="eco_hc">
                  x %
                </span></b>
          </div>
        </div>
      </div>

      <div><canvas id="pltconso_%LABEL%"></canvas></div>
      </div>
)%";


#endif /* CHART_TEMPLATE_H */
