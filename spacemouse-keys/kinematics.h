// This is the public header for the kinematics.cpp file
// It contains all functions which can be called from the main application

int modifierFunction(int x);

// REVIEW - Moved to ADC.h
// void readAllFromSensors(int *rawReads);

void FilterAnalogReadOuts(int *centered);

void calculateKinematic(int *centered, int16_t *velocity);

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