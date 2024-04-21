#include "PID.h"

double pW = 0.17;
double iW = 0.003;
double dW = 0.002;
void PID::initialize(BLEController* bleController) {
  this->bleController = bleController;
  this->bleController->setPIDValues(pW, iW, dW);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  Serial.println();

  Serial.print("Gyroscope sample rate = ");  
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println("Hz");
  Serial.println();
}

double pid_i_mem_pitch = 0;
double maxCorrection = 50;
double lastTime = -1;
double integral = 0;
double prev_error = 0;
void PID::update(bool showLog, PIDOutputCallback outputCallback) {
  if (lastTime = -1) {
    lastTime = millis();
  }

  IMU.readGyroscope(Gx, Gy, Gz);
  IMU.readAcceleration(Ax, Ay, Az);
  
  double roll = calculate_roll(Ax, Ay, Az);
  double pitch = calculate_pitch(Ax, Ay, Az, Gy);
  double yaw = calculate_yaw(Gx, Gy, Gz);
  
  this->bleController->publishPitchValue(pitch);
  this->bleController->publishRollValue(roll);
  this->bleController->publishYawValue(yaw);

  double pWeight, iWeight, dWeight = 0;
  if (this->bleController->pidDebugEnabled) {
    pWeight = this->bleController->pWeight;
    iWeight = this->bleController->iWeight;
    dWeight = this->bleController->dWeight;
  } else {
    pWeight = pW;
    iWeight = iW;
    dWeight = dW;
  }

  double error = (0 - pitch) / 180;

  // Proportional term
  double P = pWeight * error;

  // Delta time
  double currentTime = millis();
  double dt = (currentTime - lastTime) / 1000;
  lastTime = currentTime;

  // Integral term
  integral = integral + error * dt;
  double I = iWeight * integral;
  
  // Derivative term
  double derivative = (error - prev_error) / dt;
  double D = dWeight * derivative;
  
  // PID output
  double output = P + I + D;


  Serial.print(pitch);
  Serial.print("\t\t");
  Serial.print(error);
  Serial.print("\t\t");
  Serial.print(derivative);
  Serial.print("\t\t");
  Serial.print(output);
  Serial.println();
  
  // Update previous error
  prev_error = error;

  outputCallback(0, output, 0);
}

double PID::calculate_roll(double Ax, double Ay, double Az) {
  return atan2(Ay, Az) * 180.0 / PI;
}

double PID::calculate_pitch(double Ax, double Ay, double Az, double Gy) {
  return atan2(Ax, sqrt(Ay * Ay + Az * Az)) * 180 / PI;
}

double PID::calculate_yaw(double Gx, double Gy, double Gz) {
    double curr_time = millis() / 1000.0;
    double dt = curr_time - prev_time;
    yaw_angle += Gz * dt;

    prev_time = curr_time;
    return yaw_angle - 90;
}
