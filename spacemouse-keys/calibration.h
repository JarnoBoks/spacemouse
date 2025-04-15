#ifndef CALIBRATION_h
#define CALIBRATION_h
// Header for calibration specific functions and variables
#include "hardware/SpaceMouseHW.h"

// Called from main routine
// SECTION EEPROM_CALIBRATION
#include "sensitivity.h"

// Definition of the calibration options that a user can enter in the Serial Monitor.
#define CALIBRATION_OPTIONS {1, 11, 20, 2, 3, 4, 5, 6, 61, 7, 8, 9, 10, 0, -1}
#define NUM_CALIBRATION_OPTIONS 15
bool validCalibrationOption(int value);

// !SECTION EEPROM_CALIBRATION

void debugOutput1(SpaceMouseHW_ &SMHW, int *keyVals);
void debugOutput2(SpaceMouseHW_ &SMHW);

#ifdef EEPROM_CALIBRATION
void debugOutput4(int16_t *velocity, uint8_t *keyOut, sensitivities_t *sensitivities);
#else
void debugOutput4(int16_t *velocity, uint8_t *keyOut);
#endif

void debugOutput5(SpaceMouseHW_ &SMHW, int16_t *velocity);

bool isDebugOutputDue();
void updateFrequencyReport();

bool busyZeroing(int *centerPoints, uint16_t numIterations, boolean debugFlag);

// SECTION EEPROM_CALIBRATION
uint8_t updateModfunc(long input);
uint8_t readModfunc();
void printModfunc(uint8_t modfunc, bool introtext);

// Positions of inversions in the byte
#define AX_INVX 0
#define AX_INVY 1
#define AX_INVZ 2
#define AX_INVRX 3
#define AX_INVRY 4
#define AX_INVRZ 5

// Get inversion is called with the inversions byte and the axis
#define GET_INVERSION(x, y) ((x >> y) & 1)
#define DEFAULT_INVERSION (DEF_INVRZ << AX_INVRZ) + (DEF_INVRY << AX_INVRY) + (DEF_INVRX << AX_INVRX) + (DEF_INVZ << AX_INVZ) + (DEF_INVY << AX_INVY) + (DEF_INVX << AX_INVX)

uint8_t updateInversions(long input);
uint8_t readInversions();
void printInversions(uint8_t inversions, bool introtext);

// !SECTION EEPROM_CALIBRATION

#endif // CALIBRATION_h