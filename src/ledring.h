#ifndef LEDRING_h
#define LEDRING_h
// This is the public header for the ledring.cpp file
// It contains all functions which can be called from the main application

#include <FastLED.h>
#include "config.h"
#include "kinematics/kinematics.h"

class LedRing {
public:
    LedRing();
    LedRing(Kinematics &SMKIN);
    ~LedRing();

    void ProcessLED(boolean ledCmd);

private:
    // Kinematics *_SMKIN = nullptr;

    CRGB _leds[LEDRING];

    void _setLEDsOnClock(uint16_t clock, CRGB color);
    void _set4LEDsOnClock(uint16_t clock, CRGB color);
    void _setAllLEDs(CRGB color);
    void _rotateColor(boolean clockwise, CRGB color);
};

#endif // LEDRING_h
