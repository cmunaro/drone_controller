#ifndef Pin_h
#define Pin_h

#include <stdint.h>

class Pin {
public:
  Pin(uint8_t gpio, uint8_t digital, uint8_t analog);
  uint8_t gpio, digital, analog;
};

#endif
