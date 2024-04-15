#ifndef PID_h
#define PID_h

#include "Arduino.h"
#include <Arduino_LSM6DSOX.h>

using PIDOutputCallback = std::function<void(float, float, float)>;

class PID {
public:
  float Ax, Ay, Az;
  float Gx, Gy, Gz;
  float desired_roll = 0;
  float desired_pitch = 0;
  float desired_yaw = 0;
  float previous_error_roll = 0;
  float previous_error_pitch = 0;
  float previous_error_yaw = 0;
  float integral_roll = 0;
  float integral_pitch = 0;
  float integral_yaw = 0;
  float prev_time = 0;
  float yaw_angle = 0;
  
  void initialize();
  void update(bool showLog, PIDOutputCallback outputCallback);
private:
  float calculate_roll(float Ax, float Ay, float Az);
  float calculate_pitch(float Ax, float Ay, float Az);
  float calculate_yaw(float Gx, float Gy, float Gz);
};

#endif
