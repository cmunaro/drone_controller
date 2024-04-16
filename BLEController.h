#ifndef BLEController_h
#define BLEController_h

#include "Arduino.h"
#include <ArduinoBLE.h>

class BLEController {
public:
  void initialize();
  void publishPitchValue(float value);
  void updateBLEState();
  
  float pWeight, iWeight, dWeight = 0;

private:
  BLEDevice central;
};

#endif
