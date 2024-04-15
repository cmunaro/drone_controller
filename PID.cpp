#include "PID.h"

void PID::initialize() {
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
  float pitch = calculate_pitch(Ax, Ay, Az);
  float yaw = calculate_yaw(Gx, Gy, Gz);

  Serial.print(roll);
  Serial.print("\t\t");
  Serial.print(pitch);
  Serial.print("\t\t");
  Serial.print(yaw);
  Serial.println();

  float error = 0 - pitch;

  // Proportional term
  float P = 1 * error;

  // Delta time
  float currentTime = millis();
  float dt = currentTime - lastTime;
  lastTime = currentTime;

  // Integral term
  integral += error * dt;
  float I = 0.1 * integral;
  
  // Derivative term
  float derivative = (error - prev_error) / dt;
  float D = 0.05 * derivative;
  
  // PID output
  float output = P + I + D;
  
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

float PID::calculate_pitch(float Ax, float Ay, float Az) {
  return atan2(-Ax, sqrt(Ay * Ay + Az * Az)) * 180.0 / PI;
}

// TODO: Fix constant error
float PID::calculate_yaw(float Gx, float Gy, float Gz) {
    float curr_time = millis() / 1000.0;
    float dt = curr_time - prev_time;
    yaw_angle += Gz * dt;

    prev_time = curr_time;
    return yaw_angle;
}
