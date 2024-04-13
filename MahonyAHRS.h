#ifndef MahonyAHRS_h
#define MahonyAHRS_h

#include "Arduino.h"
#include "Wire.h"

class MahonyAHRS {
  public:
    void begin();
    void update();
    float getRoll();
    float getPitch();
    float getYaw();
    float yaw, pitch, roll;
  private:
    int _MPU_addr;
    float _A_cal[6];
    float _G_off[3];
    float _gscale;
    float _q[4];
    float _Kp;
    float _Ki;
    float _yaw, _pitch, _roll;
    void Mahony_update(float ax, float ay, float az, float gx, float gy, float gz, float deltat);
};

#endif
