#ifndef ESCControl_h
#define ESCControl_h

#include "Arduino.h"
#include "Receiver.h"
#include "Pin.h"
#include "BLEController.h"

class ESCControl {
public:
  ESCControl(const Pin (&pins)[4]);
  void initialize(Receiver* receiver, BLEController* bleController);
  void updateMotorPwmByReceiverOnly(bool showLog);
  void updateMotorPwm(bool showLog, float roll, float pitch, float yaw);
  void waitForThrottleLowPosition();
  
private:
  int coerceIn(int value, int minValue, int maxValue);

  Pin motorPins[4];
  int motorPwmFrequency;
  Receiver* receiver;
  BLEController* bleController;
  long pwm1, pwm2, pwm3, pwm4;
};

#endif
