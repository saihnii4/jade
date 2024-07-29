#include <utility>
#include <ps5Controller.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_err.h"

// #ifdef ps5
// #define ps5 ps5
// #else
// #define ps5 PS4
// #endif

// #define MAC "88:03:4C:7A:61:2C"
// #define MAC "96:80:5e:b7:30:de"
// #define MAC "40:99:22:90:fc:08"
// #define MAC "e0:5a:1b:74:05:7c"
#define MAC "e8:47:3a:a4:fd:4e"

void remove_pair()
{
  uint8_t pairedDeviceBtAddr[10][6];

  btStart();
  esp_bluedroid_init();
  esp_bluedroid_enable();
  int count = esp_bt_gap_get_bond_device_num();
  Serial.print(" bonded device found:");
  Serial.println(count);
  if (count > 0)
  {
    esp_err_t tError = esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
    for (int i = 0; i < count; i++)
      esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
  }
}

// #define _FUJI

#ifndef _FUJI
#include <src/_esp.h>
#else
#include <src/fuji.h>
#endif

void loop()
{
  if (!ready())
    return;
  read();
  calc_speed();
  set_speed();
  // Serial.println("testiGkung");

  // Serial.printf("%d %d/luu\n", speed.first, speed.second);
}

void setup()
{
  Serial.begin(19200);
  // remove_pair();
  ps5.begin(MAC);
  setup_pins();
  Serial.println("ON!");
}
