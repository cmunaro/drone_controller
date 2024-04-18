#ifndef BLEController_h
#define BLEController_h

#include "Arduino.h"
#include <ArduinoBLE.h>

class BLEController {
public:
  void initialize();
  void publishPitchValue(float value);
  void publishRollValue(float value);
  void publishYawValue(float value);
  void updateBLEState();
  float pWeight = 0.09;
  float iWeight = 0;
  float dWeight = 0;
  bool pidDebugEnabled = false;
private:
  BLEDevice central;
};

#endif
