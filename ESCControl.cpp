#include "ESCControl.h"
#include "Receiver.h"
#include <WiFiNINA.h>

ESCControl::ESCControl(const Pin (&pins)[4]): motorPins(pins) {
}

void ESCControl::initialize(Receiver* receiver) {
  this->receiver = receiver;
}

void ESCControl::waitForThrottleLowPosition() {
  while (receiver->times[2] > 1000) {
    Serial.println(receiver->times[2]);
    digitalWrite(25, !digitalRead(25));
    delay(1000);
  }
}

void ESCControl::updateMotorPwmByReceiverOnly(bool showLog) {
  int pwmMotor1 = map(receiver->times[0], 1000, 2000, 127, 255);
  int pwmMotor2 = map(receiver->times[1], 1000, 2000, 127, 255);
  int pwmMotor3 = map(receiver->times[2], 1000, 2000, 127, 255);
  int pwmMotor4 = map(receiver->times[3], 1000, 2000, 127, 255);
  
  analogWrite(motorPins[0].digital, pwmMotor1);
  analogWrite(motorPins[1].digital, pwmMotor2);
  analogWrite(motorPins[2].digital, pwmMotor3);
  analogWrite(motorPins[3].digital, pwmMotor4);

  if (showLog) {
    Serial.print(pwmMotor1);
    Serial.print("\t\t");
    Serial.print(pwmMotor2);
    Serial.print("\t\t");
    Serial.print(pwmMotor3);
    Serial.print("\t\t");
    Serial.println(pwmMotor4);
  }
}

float pwmMotor1 = 127;
float pwmMotor2 = 127;
float pwmMotor3 = 127;
float pwmMotor4 = 127;
void ESCControl::updateMotorPwm(bool showLog, float roll, float pitch, float yaw) {
  
  pwmMotor1 = pwmMotor1 + roll + pitch;
  pwmMotor2 = pwmMotor2 - roll + pitch;
  pwmMotor3 = pwmMotor3 + roll - pitch;
  pwmMotor4 = pwmMotor4 - roll - pitch;
  pwmMotor1 = constrain(pwmMotor1, 127, 255);
  pwmMotor2 = constrain(pwmMotor2, 127, 255);
  pwmMotor3 = constrain(pwmMotor3, 127, 255);
  pwmMotor4 = constrain(pwmMotor4, 127, 255);

  analogWrite(motorPins[0].digital, pwmMotor1);
  analogWrite(motorPins[1].digital, pwmMotor2);
  analogWrite(motorPins[2].digital, pwmMotor3);
  analogWrite(motorPins[3].digital, pwmMotor4);

  if (showLog) {
    Serial.print(roll);
    Serial.print("\t\t");
    Serial.print(pitch);
    Serial.print("\t\t");
    Serial.print(yaw);
    Serial.print("\t\t");
    Serial.print(pwmMotor1);
    Serial.print("\t\t");
    Serial.print(pwmMotor2);
    Serial.print("\t\t");
    Serial.print(pwmMotor3);
    Serial.print("\t\t");
    Serial.println(pwmMotor4);
  }
}

int ESCControl::coerceIn(int value, int minValue, int maxValue) {
  return (value < minValue) ? minValue : ((value > maxValue) ? maxValue : value);
}
