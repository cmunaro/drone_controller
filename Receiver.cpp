#include "Receiver.h"

volatile Receiver* Receiver::instance = nullptr;

Receiver::Receiver(const Pin (&pins)[4]) : inputPins(pins) {
  instance = this;
}

void Receiver::initialize() {
  attachInterrupt(digitalPinToInterrupt(inputPins[0].digital), isr0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(inputPins[1].digital), isr1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(inputPins[2].digital), isr2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(inputPins[3].digital), isr3, CHANGE);
}

void Receiver::isr0() {  
  if (gpio_get(instance->inputPins[0].gpio)) {
    instance->risingTimes[0] = micros();
  } else {
    instance->times[0] = micros() - instance->risingTimes[0];
  }
}

void Receiver::isr1() {
  if (gpio_get(instance->inputPins[1].gpio)) {
    instance->risingTimes[1] = micros();
  } else {
    instance->times[1] = micros() - instance->risingTimes[1];
  }
}

void Receiver::isr2() {
  if (gpio_get(instance->inputPins[2].gpio)) {
    instance->risingTimes[2] = micros();
  } else {
    instance->times[2] = micros() - instance->risingTimes[2];
  }
}

void Receiver::isr3() {
  if (gpio_get(instance->inputPins[3].gpio)) {
    instance->risingTimes[3] = micros();
  } else {
    instance->times[3] = micros() - instance->risingTimes[3];
  }
}