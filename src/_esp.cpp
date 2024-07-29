#include "_esp.h"
#include <ps5Controller.h>
#include <Arduino.h>
bool active = true;
bool accelerate = false;
std::pair<int, int> speed, dir, angle_coeff;

const int turn_speed = 200;
const int back_speed = 160;

const int DEADZONE_MIN = -10;
const int DEADZONE_MAX = 10;
const int DEADZONE = 0;

#define MAX 800
#define MIN -800
const uint8_t pulsewm_r = 16;
const uint8_t rdrive = 4;
const uint8_t rsleep = 17;
const uint8_t pulsewm_l = 26;
const uint8_t ldrive = 25;
const uint8_t lsleep = 27;

const uint8_t LED = 2;

// read not really necessary as esp32 can receive values on fly
void read()
{
  return;
}

void calc_speed()
{
  // unnecessary var, remove and document
  accelerate = L1 || L2 || UP;
  // left joystick idle
  if (LJY >= DEADZONE_MIN && LJY <= DEADZONE_MAX)
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
  if (LJY < DEADZONE_MIN)
  { // clean backwards
    speed.first = speed.second = back_speed;
    dir.first = dir.second = 0;
    return;
  }
  // forwards
  if (LJY > DEADZONE_MAX)
  {
    // right joystick idle
    if (RJX >= DEADZONE_MIN && RJX <= DEADZONE_MAX)
    {
      angle_coeff.first = angle_coeff.second = 0;
    }
    if (RJX < DEADZONE_MIN)
    {
      angle_coeff.second = abs(DEADZONE - RJX);
      angle_coeff.first = 0;
    }
    if (RJX > DEADZONE_MAX)
    {
      angle_coeff.first = abs(DEADZONE - RJX);
      angle_coeff.second = 0;
    }
  }

  int raw_speed = accelerate ? ((LJY - DEADZONE) << 1) : (LJY - DEADZONE);
  // Serial.printf("%d %d %d\n", LJY, angle_coeff.first, angle_coeff.second);
  int r_raw_speed = raw_speed * 3 - angle_coeff.second * 2;
  int l_raw_speed = raw_speed * 3 - angle_coeff.first * 2;

  if (r_raw_speed > MAX)
    r_raw_speed = MAX;
  if (l_raw_speed > MAX)
    l_raw_speed = MAX;
  if (r_raw_speed < MIN)
    r_raw_speed = MIN;
  if (l_raw_speed < MIN)
    l_raw_speed = MIN;

  speed.first = l_raw_speed;
  speed.second = r_raw_speed;
  dir.first = 1;
  dir.second = 1;
}

// speed processing
void set_speed()
{
  ledcWrite(pulsewm_r, speed.second);
  if (speed.second != 0)
    digitalWrite(rdrive, dir.second ? LOW : HIGH);
  ledcWrite(pulsewm_l, speed.first);
  if (speed.first != 0)
    digitalWrite(ldrive, dir.first ? LOW : HIGH);
};

void setup_pins()
{
  pinMode(LED, OUTPUT);
  pinMode(rdrive, OUTPUT);
  pinMode(rsleep, OUTPUT);

  pinMode(ldrive, OUTPUT);
  pinMode(lsleep, OUTPUT);

  ledcAttach(pulsewm_r, 12000, 10);
  ledcAttach(pulsewm_l, 12000, 10);

  digitalWrite(rsleep, HIGH);
  digitalWrite(lsleep, HIGH);
}

bool ready()
{
  return ps5.isConnected();
}