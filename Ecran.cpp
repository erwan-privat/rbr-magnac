#include "Ecran.h"
#include "pins.h"
#include "images.h"
#include "Dimmer.h"
#include "Heure.h"
#include "Ota.h"
#include "Watts.h"
#include "WiFiMagnac.h"
#include <NTPClient.h>
#include <dimmable_light.h>
#include <WiFi.h>

namespace Ecran
{
  constexpr auto screen_w    = 128; // px
  constexpr auto screen_h    =  64; // px
  constexpr auto normal_font = u8g2_font_gb24st_t_3;
  constexpr auto small_font  = u8g2_font_t0_11b_tf;
  constexpr auto icon_font   = u8g2_font_open_iconic_all_1x_t;
  constexpr auto ico_wifi    = 0x00f7;
  constexpr auto ico_nope    = 0x0057;
  constexpr auto ico_warn    = 0x0118;
  constexpr auto max_switch  = 7;

  constexpr auto ota_warn_delay = 500'000u; // µs

  // U8G2_R0 pour sans rotation (paysage)
  OledScreen screen(U8G2_R0, oled_cs, oled_dc, oled_res);

  char heure[] = "hh:mm:ss";
  int switch_watts = 0;
  unsigned last_bang_micros = esp_timer_get_time();
  bool warn_toggle = false;
  
  unsigned getLength()
  {
    return screen_w * screen_h / 8;
  }

  OledScreen& getScreen()
  {
    return screen;
  }

  void progressbar(int x, int y, int w, int h, float value)
  {
    screen.drawFrame(x, y, w,  h);
    screen.drawBox(x + 2, y + 2, (w - 4) * value,  h - 4);
  }

  void drawOtaProgress()
  {
    constexpr int margin = 20;
    constexpr int height = 20;

    screen.setDrawColor(2); // XOR
    screen.setCursor(screen_w / 2 - 16, 40);
    screen.setFont(Ecran::normal_font);

    if (Ota::updating)
    {
      screen.print(Ota::progress);
      screen.print(F(" %"));
      progressbar(
        margin, screen_h - margin - height,
        screen_w - 2 * margin, height,
        Ota::progress / 100.0f);
    }
    else
    {
      screen.setDrawColor(1);
      screen.printf(F("OTA error %d"), Ota::error);
    }
    
    screen.setDrawColor(1);
  }

  void drawWiFiStatus()
  {
    screen.setFont(icon_font);
    
    auto micros = esp_timer_get_time();

    if(Ota::error != 666
        && micros - ota_warn_delay > last_bang_micros)
    {
      warn_toggle = !warn_toggle;
      last_bang_micros = micros;
    }

    // eplogf("%c\r", warn_toggle ? '!' : '-');
    if (warn_toggle)
      screen.drawGlyph(58, 12, ico_warn);

    if (WiFiMagnac::isConnected())
    {
      screen.drawGlyph(0, screen_h, ico_wifi);
      screen.setFont(small_font);
      screen.setCursor(16, screen_h);
      IPAddress ip = WiFi.localIP();
      screen.print(ip[2]);
      screen.print('.');
      screen.print(ip[3]);
      // screen.print(wifi_rssi);
      // screen.print(F(" dBm"));
    }
    else
    {
      screen.drawGlyph(0, screen_h, ico_nope);
      screen.setFont(small_font);
      screen.setCursor(16, screen_h);
      screen.print(F("WiFi"));
    }
  }

  void drawTime()
  {
    strcpy(heure, Heure::time_client.getFormattedTime()
      .c_str());
    screen.setFont(small_font);
    u8g2_uint_t w = screen.getUTF8Width(heure);
    screen.setCursor(screen_w - w, screen_h);
    screen.print(heure);
  }

  void drawWatts()
  {
    constexpr int pos_y = 36;
    int p1 = round(-Watts::power1);
    int p2 = round(Watts::power2);

    screen.setFont(normal_font);
    u8g2_uint_t w = screen.getUTF8Width("-9999 W");
    w += 46;
    screen.setCursor((screen_w - w) / 2, pos_y);

    switch (switch_watts)
    {
      default:
      case 0:
        screen.printf(F("%4d W"), p1 > 0 ? p1 : p2);
        break;
    }

    auto img = p1 > 0 ? bmp_chauffe_eau
      : bmp_chauffe_eau_nope;

    screen.drawXBMP((screen_w + w) / 2 - 40, 8, 44, 40, img);
  }
 
  void drawDimmer()
  {
    screen.setFont(small_font);
    screen.setCursor(0, 12);
    byte value = Dimmer::dimmer.getBrightness();
    // screen.printf(F(" > %d | %d %%"), value,
        // (int)round(value * 100 / 255.0));
    screen.printf(F("%d %%"), (int)round(value * 100 / 255.f));
  }

  void taskScreen(void* argv)
  {
    while (!Heure::time_client.isTimeSet())
    {
      screen.clearBuffer();
      screen.setFont(normal_font);
      screen.drawXBMP(0, 0, screen_w, screen_h, bmp_winie);
      screen.setCursor(screen_w - 20, 20);
      screen.print('R');
      screen.setCursor(screen_w - 20, 40);
      screen.print('B');
      screen.setCursor(screen_w - 20, 60);
      screen.print('R');
      screen.sendBuffer();
      delay(1000);
    }

    for (;;)
    {
      // on enlève le splash screen quand l'heure est à jour

      // eplogf("task screen %u unused from 3000\r\n",
      //   uxTaskGetStackHighWaterMark(nullptr));
      screen.clearBuffer();

      if (Ota::updating)
      {
        drawOtaProgress();
      }
      else
      {
        drawWiFiStatus();
        drawTime();
        drawWatts();
        drawDimmer();
      }
      
      screen.sendBuffer();
      delay(10);
    }
  }

  void taskTimerSwitch(void*)
  {
    for (;;) {
      switch_watts = (switch_watts + 1) % max_switch;
      delay(2000);
    }
  }

  void begin()
  {
    screen.begin();
    screen.setDrawColor(1);

    xTaskCreate(taskScreen, "task screen", 
      3000, nullptr, 3, nullptr);

    xTaskCreate(taskTimerSwitch, "task screen timer",
        3000, nullptr, 2, nullptr);
    
    screen.clearBuffer();
    screen.drawXBMP(0, 0, screen_w, screen_h, bmp_winie);
    screen.sendBuffer();
  }
}
