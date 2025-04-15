#ifndef KINEMATICS_h
#define KINEMATICS_h
// This is the public header for the kinematics.cpp file
// It contains all functions which can be called from the main application

#include "hardware/SpaceMouseHW.h"

int modifierFunction(int x);

void calculateKinematic(SpaceMouseHW_ &SMHW, int16_t *velocity);

void switchXY(int16_t *velocity);
void switchYZ(int16_t *velocity);

void exclusiveMode(int16_t *velocity);

// Define position in velocity array.
#define TRANSX 0
#define TRANSY 1
#define TRANSZ 2
#define ROTX 3
#define ROTY 4
#define ROTZ 5
#endif // KINEMATICS_h