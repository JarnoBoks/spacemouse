// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"
#include <Arduino.h>
#ifdef LEDRING
#include "ledring.h"
#include "kinematics.h"

LedRing::LedRing(Kinematics &SMKIN) : _SMKIN(&SMKIN) {
    FastLED.addLeds<WS2811, LEDpin, GRB>(_leds, LEDRING);
} // Constructor with parameters

LedRing::~LedRing() {
    // Destructor
}

/// @brief process the LEDs connected via FastLED. Call this in loop()
/// @param velocity array with velocity informations
/// @param ledCmd transmit if the LED shall be on (as it may be demanded over USB)
void LedRing::ProcessLED(boolean ledCmd) {
    unsigned long now = millis();
    static unsigned long lastLEDupdate = now;

    if (now - lastLEDupdate >= LEDUPDATERATE_MS) {
        _setAllLEDs(CRGB::Black);
        if (ledCmd) {
            // turn all on
            _setAllLEDs(CRGB::DarkOliveGreen);
        } else {
            // USB doesn't send us commands to turn on LED
            switch (_SMKIN->GetMainVelocity()) {
            case transX:
                _setAllLEDs(CRGB::Yellow);
                // TX pos: 3 o'clock neg: 9 o'clock
                // light up the _free_ positions
                // REVIEW - check if this is correctly implemented. I'm in doubt about the inversion. I don't have a leadring so I cannot test it right now.
                if ((_SMKIN->GetVelocity(transX) > 0) != _SMKIN->GetAxisInvert(transX)) {
                    _set4LEDsOnClock(9, CRGB::Red);
                } else {
                    _set4LEDsOnClock(3, CRGB::Red);
                }
                break;
            case transY:
                _setAllLEDs(CRGB::Yellow);
                // TY pos: 12 o'clock neg, 6 o'clock
                if ((_SMKIN->GetVelocity(transY) > 0) != _SMKIN->GetAxisInvert(transY)) {
                    _set4LEDsOnClock(6, CRGB::Red);
                } else {
                    _set4LEDsOnClock(12, CRGB::Red);
                }
                break;
            case transZ:
                // TZ pos: all white, neg: all dark blue
                if ((_SMKIN->GetVelocity(transZ) > 0) != _SMKIN->GetAxisInvert(transZ)) {
                    _setAllLEDs(CRGB::AntiqueWhite);
                    FastLED.setBrightness(50);
                } else {
                    _setAllLEDs(CRGB::DarkBlue);
                    FastLED.setBrightness(50);
                }
                break;
            case rotX:
                _setAllLEDs(CRGB::SkyBlue);
                // RX pos: red 6 o'clock, neg red 12 o'clock
                if ((_SMKIN->GetVelocity(rotX) > 0) != _SMKIN->GetAxisInvert(rotX)) {
                    _set4LEDsOnClock(12, CRGB::Green);
                } else {
                    _set4LEDsOnClock(6, CRGB::Green);
                }
                break;
            case rotY:
                _setAllLEDs(CRGB::SkyBlue);
                // RY pos: red 3 o'clock, neg red 9 o'clock
                if ((_SMKIN->GetVelocity(rotY) > 0) != _SMKIN->GetAxisInvert(rotY)) {
                    _set4LEDsOnClock(9, CRGB::Green);
                } else {
                    _set4LEDsOnClock(3, CRGB::Green);
                }
                break;
            case rotZ:
                _setAllLEDs(CRGB::SkyBlue);
                // RZ pos: red ring wandering around counterclock wise; neg: clockwise
                if ((_SMKIN->GetVelocity(rotZ) > 0) != _SMKIN->GetAxisInvert(rotZ)) {
                    _rotateColor(false, CRGB::DarkRed);
                } else {
                    _rotateColor(true, CRGB::DarkRed);
                }
                break;
            default: // all very dimm
                _setAllLEDs(CRGB::DarkGrey);
                FastLED.setBrightness(5);
                break;
            }
        }
        FastLED.show();
        lastLEDupdate += LEDUPDATERATE_MS;
    }
}

/// @brief rotate a single around the LED ring
/// @param clockwise turns clockwise if true
/// @param color which CRGB color
void LedRing::_rotateColor(boolean clockwise, CRGB color) {
    static int rotateLEDpos = 0;
    _leds[rotateLEDpos] = color;
    if (clockwise) {
        rotateLEDpos = (rotateLEDpos + 1) % LEDRING;
    } else {
        rotateLEDpos = (rotateLEDpos + LEDRING - 1) % LEDRING; // avoid negativ led position
    }
}

/// @brief set all leds to given color
/// @param color
void LedRing::_setAllLEDs(CRGB color) {
    for (int i = 0; i < LEDRING; i++) {
        _leds[i] = color;
    }
}

/// @brief set LED on LED ring regarding the ring as a clock
/// @param clock position of the LED to light up
/// @param color color to light
void LedRing::_setLEDsOnClock(uint16_t clock, CRGB color) {
    uint16_t pos = 0;
    pos = (clock * (LEDRING / 12)) % LEDRING;
    pos = (LEDclockOffset + pos) % LEDRING;
    _leds[pos] = color;
}

/// @brief set 4 LEDs on LED ring regarding the ring as a clock
/// @param clock position of the LED to light up
/// @param color color to light
void LedRing::_set4LEDsOnClock(uint16_t clock, CRGB color) {
    uint16_t pos = 0;
    pos = (clock * (LEDRING / 12)) % LEDRING;
    pos = (LEDclockOffset + pos) % LEDRING;

    _leds[pos] = color;
    _leds[(pos + 1) % LEDRING] = color;
    _leds[(LEDRING + pos - 1) % LEDRING] = color;
    _leds[(LEDRING + pos - 2) % LEDRING] = color;
}

#endif // #if LEDring
       //