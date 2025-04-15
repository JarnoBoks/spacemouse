#ifndef CALIBRATION_h
#define CALIBRATION_h
// Header for calibration specific functions and variables
#include "hardware/SpaceMouseHW.h"

// REMOVE - void debugOutput1(int* rawReads, int* keyVals);
void debugOutput1(const SpaceMouseHW_ &SMHW, int *keyVals);

// REMOVE - void debugOutput2(int* centered);
void debugOutput2(SpaceMouseHW_ &SMHW);
void debugOutput4(int16_t *velocity, uint8_t *keyOut);
// REMOVE void debugOutput5(int *centered, int16_t *velocity);
void debugOutput5(SpaceMouseHW_ &SMHW, int16_t *velocity);

void printArray(int arr[], int size);
// REMOVE - void calcMinMax(int *centered);

bool isDebugOutputDue();

void updateFrequencyReport();

// REMOVE - bool busyZeroing(int *centerPoints, uint16_t numIterations, boolean debugFlag);

#endif // CALIBRATION_h