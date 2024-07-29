#ifdef _FUJI

#include "fuji.h"
#include <Arduino.h>

const int turn_speed = 100;
const int back_speed = 80;

const int DEADZONE_MIN = 120;
const int DEADZONE_MAX = 130;
// fuji board pins
const uint8_t r1_pin = 2;
const uint8_t r2_pin = 4;
const uint8_t pulsewm_r = 9;

const uint8_t l1_pin = 7;
const uint8_t l2_pin = 8;
const uint8_t pulsewm_l = 10;

uint8_t ptr_ct = 6;
bool active = false;
bool accelerate = false;
std::pair<int, int> speed, dir, angle_coeff;

void read()
{
  if (Serial.available() > 0)
  { // data available for reading
    uint8_t data = Serial.read();
    if (ptr_ct < 5)
    { // bounds check
      data_arr[ptr_ct] = data;
      ptr_ct++;
      return;
    }
    if (data == 0x0a)
    {
      ptr_ct = 0;
      active = true;
    }
  }
}

void calc_speed()
{
  // unnecessary var, remove and document
  accelerate = (bitRead(TRIGGER, 2) == 1 || bitRead(TRIGGER, 0) == 1);
  // left joystick idle
  if (LJX >= DEADZONE_MIN && LJX <= DEADZONE_MAX)
  {
    // right joystick idle
    if (RJX >= DEADZONE_MIN && RJX <= DEADZONE_MAX)
    {
      speed.first = speed.second = 0;
      dir.first = dir.second = 2;
    }
    if (RJX > DEADZONE_MAX)
    {
      speed.first = speed.second = turn_speed;
      dir.first = 1;
      dir.second = 0;
    }
    if (RJX < DEADZONE_MIN)
    {
      speed.first = speed.second = turn_speed;
      dir.first = 0;
      dir.second = 1;
    }
    return;
  }
  // non-idle
  // back
  if (LJX < DEADZONE_MIN)
  { // clean backwards
    speed.first = speed.second = back_speed;
    dir.first = dir.second = 0;
  }
  // forwards
  if (LJX > DEADZONE_MAX)
  {
    // right joystick idle
    if (RJX >= DEADZONE_MIN && RJX <= DEADZONE_MAX)
    {
      angle_coeff.first = angle_coeff.second = 0;
    }
    if (RJX < DEADZONE_MIN)
    {
      angle_coeff.second = 128 - RJX;
      angle_coeff.first = 0;
    }
    if (RJX > DEADZONE_MAX)
    {
      angle_coeff.first = 128 - RJX;
      angle_coeff.second = 0;
    }
  }

  int raw_speed = accelerate ? ((LJX - 128) << 2) : (LJX - 128);
  int r_raw_speed = raw_speed - angle_coeff.first * 5;
  int l_raw_speed = raw_speed - angle_coeff.second * 5;

  if (r_raw_speed > 255)
    r_raw_speed = 255;
  if (l_raw_speed > 255)
    l_raw_speed = 255;
  if (r_raw_speed < 0)
    r_raw_speed = 0;
  if (l_raw_speed < 0)
    l_raw_speed = 0;

  speed.first = l_raw_speed;
  speed.second = r_raw_speed;
  dir.first = 1;
  dir.second = 1;
}

void set_speed()
{
  if (dir.second == 2)
  {
    digitalWrite(r1_pin, LOW);
    digitalWrite(r2_pin, LOW);
  }
  else if (dir.second == 1)
  {
    digitalWrite(r1_pin, HIGH);
    digitalWrite(r2_pin, LOW);
  }
  else if (dir.second == 0)
  {
    digitalWrite(r1_pin, LOW);
    digitalWrite(r2_pin, HIGH);
  }

  if (dir.first == 2)
  {
    digitalWrite(l1_pin, LOW);
    digitalWrite(l2_pin, LOW);
  }
  else if (dir.first == 1)
  {
    digitalWrite(l1_pin, HIGH);
    digitalWrite(l2_pin, LOW);
  }
  else if (dir.first == 0)
  {
    digitalWrite(l1_pin, LOW);
    digitalWrite(l2_pin, HIGH);
  }

  analogWrite(pulsewm_l, speed.first);
  analogWrite(pulsewm_r, speed.second);
}

// always run
bool ready() {
    return true;
}

void setup_pins() {
  return;
}
#endif