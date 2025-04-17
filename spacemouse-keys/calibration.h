#ifndef CALIBRATION_h
#define CALIBRATION_h

// Header file for calibration specific functions and variables
#include "hardware/SpaceMouseHW.h"
#include "kinematics.h"

// Called from main routine
// SECTION EEPROM_CALIBRATION
// Definition of the calibration options that a user can enter in the Serial Monitor.
#define CALIBRATION_OPTIONS {1, 11, 20, 2, 3, 4, 5, 6, 61, 7, 8, 9, 10, 0, -1}
#define NUM_CALIBRATION_OPTIONS 15
bool validCalibrationOption(int value);
// !SECTION EEPROM_CALIBRATION

void debugOutput1(SpaceMouseHW_ &SMHW, int *keyVals);
void debugOutput2(SpaceMouseHW_ &SMHW);
void debugOutput4(Kinematics &SMKIN, uint8_t *keyOut);
void debugOutput5(SpaceMouseHW_ &SMHW, Kinematics &SMKIN);

bool isDebugOutputDue();
void updateFrequencyReport();

void debugInput(int &dbg, Kinematics &SMKIN, SpaceMouseHW_ &SMHW);

bool checkForAxis(const char *word, velocityAxis_t &axis, bool &isGT, bool &isMF);
void checkForPositiveNegative(const char *word, int8_t &value);
void handleTwoWordsAndFloat(const char *word1, const char *word2, float value, Kinematics &SMKIN);
void handleOneWordAndFloat(const char *word1, float value);

#endif // CALIBRATION_h