#ifndef CALIBRATION_h
#define CALIBRATION_h
// Header for calibration specific functions and variables
#include "hardware/SpaceMouseHW.h"

// Called from main routine
void debugOutput1(SpaceMouseHW_ &SMHW, int *keyVals);
void debugOutput2(SpaceMouseHW_ &SMHW);
void debugOutput4(int16_t *velocity, uint8_t *keyOut);
void debugOutput5(SpaceMouseHW_ &SMHW, int16_t *velocity);

bool isDebugOutputDue();
void updateFrequencyReport();

#endif // CALIBRATION_h