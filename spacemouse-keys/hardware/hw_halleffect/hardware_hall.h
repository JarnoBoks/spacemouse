#ifndef HARDWARE_HALL_h
#define HARDWARE_HALL_h

#include <Arduino.h>

void _calculateKinematicSensors(int *centered, int16_t *velocity);

// The following constants are here for more readable access to the arrays. You don't need to change this values!
// When using HallE sensors in centered or rawValues array
#define HES0 0
#define HES1 1
#define HES2 2
#define HES3 3
#define HES6 4
#define HES7 5
#define HES8 6
#define HES9 7

// The Hall effect sensors aren't centered arount zero, due to the nature of the hardware.
// In my version of the Spacemouse, the values vary between -425 and 285, the centerpoint is thus around -70
// The MIN and MAX warning levels have to be shifted accordingly.
#define MINMAX_MINWARNING (100 - centerPoint)
#define MINMAX_MAXWARNING (100 + centerPoint)

#endif // HARDWARE_HALL_h
