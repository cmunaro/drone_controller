#include "Pin.h"

Pin::Pin(uint8_t gpio, uint8_t digital, uint8_t analog)
    : gpio(gpio), digital(digital), analog(analog) {
}