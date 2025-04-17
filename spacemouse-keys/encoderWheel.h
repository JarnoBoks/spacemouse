#ifndef ENCODERWHEEL_H
#define ENCODERWHEEL_H
// Header file for the encoderWheel.cpp

#include <Arduino.h>
#include "config.h"
#include "kinematics.h"
#include "spaceKeys.h"

void initEncoderWheel();
void calcEncoderWheel(Kinematics &SMKIN, int debug);
void calcEncoderAsKey(SpaceKeys *SMKEYS, int debug);

#endif // ENCODERWHEEL_H