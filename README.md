## Drone controller 

### Current setup:
  - **Board:** Arduino nano rp2040 connect
  - **Receiver:** FS-IA6B 
  - **Transmitter:** FlySky FS-I6X 
  - **ESCs:** Turnigy plush 30A
  - **Motors:** Turnigy D2830/11
  - **Battery:** HRB 3300mAh 11.1V 60C

### Connections to board:
#### Receiver:
  - **vcc:**: +5v
  - **gnd:**: gnd
  - **ch1**: GPIO15
  - **ch2**: GPIO16
  - **ch3**: GPIO17
  - **ch4**: GPIO18
#### Motors:
  - **Every ESC gnd logic**: gnd
  - **Every ESC vcc logic**: /
  - **Every ESC gnd**: Battery -
  - **Every ESC vcc**: Battery +
  - **Esc1 pwm in**: GPIO26
  - **Esc2 pwm in**: GPIO27
  - **Esc3 pwm in**: GPIO28
  - **Esc4 pwm in**: GPIO29
