#include "ESCControl.h"
#include "Receiver.h"
#include <WiFiNINA.h>

ESCControl::ESCControl(const Pin (&pins)[4]): motorPins(pins) {
}

void ESCControl::initialize(Receiver* receiver, BLEController* bleController) {
  this->receiver = receiver;
  this->bleController = bleController;
}

void ESCControl::waitForThrottleLowPosition() {
  // while (receiver->times[2] > 1000) {
  //   Serial.println(receiver->times[2]);
  //   digitalWrite(25, !digitalRead(25));
  //   delay(1000);
  // }
  
  Serial.print("Initialize ESCs \n");
  delay(5000);
  Serial.print("SET HIGH \n");
  analogWrite(motorPins[0].digital, 255);
  analogWrite(motorPins[1].digital, 255);
  analogWrite(motorPins[2].digital, 255);
  analogWrite(motorPins[3].digital, 255);
  delay(4000);
  Serial.print("SET LOW \n");
  analogWrite(motorPins[0].digital, 125);
  analogWrite(motorPins[1].digital, 125);
  analogWrite(motorPins[2].digital, 125);
  analogWrite(motorPins[3].digital, 125);
  Serial.print("END \n");
  delay(1000);
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

float maxPwm = 180;
float minPwm = 150;
float pwmMotor1 = minPwm;
float pwmMotor2 = minPwm;
float pwmMotor3 = minPwm;
float pwmMotor4 = minPwm;
void ESCControl::updateMotorPwm(bool showLog, float roll, float pitch, float yaw) {
  

  pwmMotor1 = pwmMotor1 + pitch;
  // pwmMotor2 = pwmMotor2 - roll + pitch;
  pwmMotor3 = pwmMotor3 - pitch;


  // pwmMotor4 = pwmMotor4 - roll - pitch;
  pwmMotor1 = constrain(pwmMotor1, minPwm, maxPwm);
  pwmMotor2 = constrain(pwmMotor2, minPwm, maxPwm);
  pwmMotor3 = constrain(pwmMotor3, minPwm, maxPwm);
  pwmMotor4 = constrain(pwmMotor4, minPwm, maxPwm);

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
