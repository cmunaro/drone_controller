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

void PID::update(bool showLog, PIDOutputCallback outputCallback) {
  IMU.readGyroscope(Gx, Gy, Gz);
  IMU.readAcceleration(Ax, Ay, Az);
  
  float roll = calculate_roll(Ax, Ay, Az);
  float pitch = calculate_pitch(Ax, Ay, Az);
  float yaw = calculate_yaw(Gx, Gy, Gz);

  if (showLog) {
    Serial.print(roll);
    Serial.print("\t\t");
    Serial.print(pitch);
    Serial.print("\t\t");
    Serial.print(yaw);
    Serial.println();
  }

  // Calculate error
  float error_roll = desired_roll - roll;
  float error_pitch = desired_pitch - pitch;
  float error_yaw = desired_yaw - yaw;
  
  // Compute PID output for roll
  integral_roll += error_roll;
  // float derivative_roll = error_roll - previous_error_roll;
  float pid_output_roll = Kp_roll * error_roll + Ki_roll * integral_roll; //+ Kd_roll * derivative_roll;
  
  // Compute PID output for pitch
  integral_pitch += error_pitch;
  //float derivative_pitch = error_pitch - previous_error_pitch;
  float pid_output_pitch = Kp_pitch * error_pitch + Ki_pitch * integral_pitch; //+ Kd_pitch * derivative_pitch;
  
  // Compute PID output for yaw
  integral_yaw += error_yaw;
  // float derivative_yaw = error_yaw - previous_error_yaw;
  float pid_output_yaw = Kp_yaw * error_yaw + Ki_yaw * integral_yaw; //+ Kd_yaw * derivative_yaw;

  // Update previous errors for next iteration
  previous_error_roll = error_roll;
  previous_error_pitch = error_pitch;
  previous_error_yaw = error_yaw;

  outputCallback(pid_output_roll, pid_output_pitch, pid_output_yaw);
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
