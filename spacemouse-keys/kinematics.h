#ifndef KINEMATICS_h
#define KINEMATICS_h
// This is the public header for the kinematics.cpp file
// It contains all functions which can be called from the main application

#include "hardware/SpaceMouseHW.h"

#include "sensitivity.h"

int modifierFunction(int x);

void readAllFromJoystick(int *rawReads);

void FilterAnalogReadOuts(int *centered);

#ifdef EEPROM_CALIBRATION
void calculateKinematic(int *centered, int16_t *velocity, sensitivities_t *sensitivities, uint8_t modFunc, uint8_t inversions);
#else
void calculateKinematic(SpaceMouseHW_ &SMHW, int16_t *velocity);
#endif

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