#include "ESCControl.h"
#include "PID.h"
#include "Pin.h"
#include "Receiver.h"
#include "BLEController.h"

Pin receiverCh1 = Pin(15, 3, 0);
Pin receiverCh2 = Pin(16, 4, 0);
Pin receiverCh3 = Pin(17, 5, 0);
Pin receiverCh4 = Pin(18, 6, 0);
Pin motor1 = Pin(26, 14, 0);
Pin motor2 = Pin(27, 15, 1);
Pin motor3 = Pin(28, 16, 2);
Pin motor4 = Pin(29, 17, 3);
Pin motorPins[] = {motor1, motor2, motor3, motor4};
Pin receiverPins[] = {receiverCh1, receiverCh2, receiverCh3, receiverCh4};
ESCControl escControl = ESCControl(motorPins);
Receiver receiver = Receiver(receiverPins);
PID pid = PID();
BLEController bleController = BLEController();

void setup() {
  Serial.begin(2000000);
  while (!Serial)
    ;
  Serial.println();
  Serial.println("Starting..");

  bleController.initialize();

  pid.initialize(&bleController);
  receiver.initialize();
  Serial.println("Receiver connected..");
  escControl.initialize(&receiver, &bleController);
  Serial.println("Esc control ready..");

  Serial.println("Wait for the throttle to be on the low position");
  escControl.waitForThrottleLowPosition();

  Serial.println("Ready");
}

long beforeUpdate;
void loop() {
  bleController.updateBLEState();

  pid.update(false, updateMotorsForPIDOutput);

  // escControl.updateMotorPwmByReceiverOnly(false);

}

void updateMotorsForPIDOutput(float roll, float pitch, float yaw) {
  escControl.updateMotorPwm(false, roll, pitch, yaw);
}
