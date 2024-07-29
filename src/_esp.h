#ifndef _ESP32
#define _ESP32

#include <utility>
#include <Arduino.h>
#include <ps5Controller.h>

#define LJY ps5.LStickY() 
#define LJX ps5.LStickX()
#define RJY ps5.RStickY()
#define RJX ps5.RStickX() 
#define RIGHT (bool)(ps5.data.button.right)
#define LEFT (bool)(ps5.data.button.left)
#define UP (bool)(ps5.data.button.up)
#define DOWN (bool)(ps5.data.button.down)
#define L1 (bool)(ps5.data.button.l1)
#define L2 (bool)(ps5.data.button.l2)
#define R1 (bool)(ps5.data.button.r1)
#define R2 (bool)(ps5.data.button.r2)
#define L2VAL (bool)(ps5.data.analog.button.l2)
#define R2VAL (bool)(ps5.data.analog.button.r2)

extern std::pair<int, int> speed;

void calc_speed();
void read();
bool ready();
void set_speed();
void setup_pins();

#endif