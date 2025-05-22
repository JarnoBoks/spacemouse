#ifndef LEDRING_h
#define LEDRING_h
// This is the public header for the ledring.cpp file
// It contains all functions which can be called from the main application

#include <FastLED.h>
#include "config.h"
#include "kinematics\Kinematics.hpp"

class LedRing {
private:
    CRGB _leds[LEDRING];

    void _setLEDsOnClock(const uint16_t clock, const CRGB color);
    void _set4LEDsOnClock(const uint16_t clock, const CRGB color);
    void _setAllLEDs(const CRGB color);
    void _rotateColor(const boolean clockwise, const CRGB color);

public:
    LedRing();
    ~LedRing() {};

    void ProcessLED(boolean ledCmd);
};

#endif // LEDRING_h
