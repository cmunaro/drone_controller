#include "PID.h"

float pW = 0.11;
float iW = 0.005;
float dW = 0.01;
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

float pid_i_mem_pitch = 0;
float maxCorrection = 50;
float lastTime = -1;
float integral = 0;
float prev_error = 0;
void PID::update(bool showLog, PIDOutputCallback outputCallback) {
  if (lastTime = -1) {
    lastTime = millis();
  }

  IMU.readGyroscope(Gx, Gy, Gz);
  IMU.readAcceleration(Ax, Ay, Az);
  
  float roll = calculate_roll(Ax, Ay, Az);
  float pitch = calculate_pitch(Ax, Ay, Az, Gy);
  float yaw = calculate_yaw(Gx, Gy, Gz);
  
  this->bleController->publishPitchValue(pitch);
  this->bleController->publishRollValue(roll);
  this->bleController->publishYawValue(yaw);

  float pWeight, iWeight, dWeight = 0;
  if (this->bleController->pidDebugEnabled) {
    pWeight = this->bleController->pWeight;
    iWeight = this->bleController->iWeight;
    dWeight = this->bleController->dWeight;
  } else {
    pWeight = pW;
    iWeight = iW;
    dWeight = dW;
  }

  float error = (0 - pitch) / 180;

  Serial.print(pitch);
  Serial.print("\t\t");
  Serial.print(error);
  Serial.println();

  // Proportional term
  float P = pWeight * error;

  // Delta time
  float currentTime = millis();
  float dt = (currentTime - lastTime) / 1000;
  lastTime = currentTime;

  // Integral term
  integral = integral + error * dt;
  float I = iWeight * integral;
  
  // Derivative term
  // float derivative = (error - prev_error) / dt;
  // float D = 0.05 * derivative;
  
  // PID output
  float output = P + I;// + D;
  
  // Update previous error
  prev_error = error;
  
  // float pid_error_temp = 0 - roll;
  // pid_i_mem_pitch += 0.5 * pid_error_temp;
  // if (pid_i_mem_pitch > maxCorrection) pid_i_mem_pitch = maxCorrection;
  // else if (pid_i_mem_pitch < -maxCorrection) pid_i_mem_pitch = -maxCorrection;

  // float pid_output_pitch = 0.01 * pid_error_temp + pid_i_mem_pitch; //+ pid_d_gain_pitch * (pid_error_temp - pid_last_d_error);
  // if (pid_output_pitch > maxCorrection) pid_output_pitch = maxCorrection;
  // else if (pid_output_pitch < -maxCorrection) pid_output_pitch = -maxCorrection;

  //pid_last_d_error = pid_error_temp;

  outputCallback(0, output, 0);
}

float PID::calculate_roll(float Ax, float Ay, float Az) {
  return atan2(Ay, Az) * 180.0 / PI;
}

float PID::calculate_pitch(float Ax, float Ay, float Az, float Gy) {
  return atan2(Ax, sqrt(Ay * Ay + Az * Az)) * 180 / PI;
}

float PID::calculate_yaw(float Gx, float Gy, float Gz) {
    float curr_time = millis() / 1000.0;
    float dt = curr_time - prev_time;
    yaw_angle += Gz * dt;

    prev_time = curr_time;
    return yaw_angle - 90;
}
