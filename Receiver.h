#ifndef Receiver_h
#define Receiver_h

#include "Arduino.h"
#include "Pin.h"

class Receiver {
public:
  Receiver(const Pin (&pins)[4]);
  void initialize();
  volatile unsigned long times[4] = {0, 0, 0, 0};
  
private:
  static void isr0();
  static void isr1();
  static void isr2();
  static void isr3();

  volatile static Receiver* instance;
  const Pin (&inputPins)[4];
  volatile unsigned long risingTimes[4] = {0, 0, 0, 0};
};

#endif
