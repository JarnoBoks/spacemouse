#ifndef HARDWARE_JOYSTICK_h
#define HARDWARE_JOYSTICK_h

#include <Arduino.h>

void _calculateKinematicSensors(int *centered, int16_t *velocity);

// The following constants are here for more readable access to the arrays. You don't need to change this values!
// Axes in centered or rawValues array
#define AX 0
#define AY 1
#define BX 2
#define BY 3
#define CX 4
#define CY 5
#define DX 6
#define DY 7

#endif
