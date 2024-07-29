#ifdef _FUJI

#include <utility>
#include <Arduino.h>

// data_arr[0] - left joystick Y axis
// data_arr[1] - left joystick X axis
// data_arr[2] - right joystick Y axis
// data_arr[3] - right joystick X axis
// data_arr[4] - triggers
uint8_t data_arr[6];

#define TRIGGER data_arr[4]
#define LJY data_arr[0]
#define LJX data_arr[1]
#define RJY data_arr[2]
#define RJX data_arr[3]

extern std::pair<int, int> speed;

void set_speed();
void calc_speed();
void read();
bool ready();
void setup_pins();
#endif