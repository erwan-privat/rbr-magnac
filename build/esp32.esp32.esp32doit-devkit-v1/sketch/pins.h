#line 1 "/home/archaoss/Arduino/rbr_magnac/pins.h"
// CC BY-NC 4.0
// eprivat 2024-08-14

#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// LEDs output //
constexpr auto led_inter  = LED_BUILTIN;
constexpr auto led_orange = 0;
constexpr auto led_rouge  = 2;
// constexpr int btn_inter  = 21;

// OLED SPI monochrome 128x64 //
// Hardware SPI pins
// constexpr auto oled_sck =  18; // HW, fixe
// constexpr auto oled_sda =  23; // HW, fixe
constexpr auto oled_res =   4; // HW, fixe
constexpr auto oled_dc  =  15; // SW, au choix
constexpr auto oled_cs  =   5; // SW, au choix

// JSY-MK-194 //
// capteur_rx doit être branché sur le TX du SJY.
// (logique : réception de l'esp32 rx reçoit la transmission
// tx du SJY.)
constexpr auto capteur_rx = 16; // bleu
constexpr auto capteur_tx = 17; // violet

constexpr auto relay_pin  = 32;

constexpr auto dimmer_com = 25;
constexpr auto dimmer_zc  = 33;
constexpr auto dimmer_ven =  0; // masse du ventilo

void initPins()
{
  pinMode(led_inter,  OUTPUT);
  pinMode(led_orange, OUTPUT);
  pinMode(led_rouge,  OUTPUT);
  // pinMode(btn_inter,  INPUT_PULLUP);
}

#endif
