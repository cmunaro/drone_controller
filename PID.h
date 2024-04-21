#ifndef PID_h
#define PID_h

#include "Arduino.h"
#include <Arduino_LSM6DSOX.h>
#include <functional>
#include "BLEController.h"

using PIDOutputCallback = std::function<void(double, double, double)>;

class PID {
public:
  float Ax, Ay, Az;
  float Gx, Gy, Gz;
  double desired_roll = 0;
  double desired_pitch = 0;
  double desired_yaw = 0;
  double previous_error_roll = 0;
  double previous_error_pitch = 0;
  double previous_error_yaw = 0;
  double integral_roll = 0;
  double integral_pitch = 0;
  double integral_yaw = 0;
  double prev_time = 0;
  double yaw_angle = 0;
  
  void initialize(BLEController* bleController);
  void update(bool showLog, PIDOutputCallback outputCallback);
private:
  BLEController* bleController;
  double calculate_roll(double Ax, double Ay, double Az);
  double calculate_pitch(double Ax, double Ay, double Az, double Gy);
  double calculate_yaw(double Gx, double Gy, double Gz);
};

#endif
