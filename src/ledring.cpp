// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"
#include <Arduino.h>

#ifdef LEDRING
#include "ledring.h"
#include "kinematics/kinematics.h"
#include "axis/config/AxisConfig.hpp"

LedRing::LedRing() {
    FastLED.addLeds<WS2811, LEDpin, GRB>(_leds, LEDRING);
}

#define VAL(x) Kinematics::getInstance()->getAxis(x)->getValue();
#define INV(x) Kinematics::getInstance()->getAxis(x)->getConfig()->inversion;
/**
 * @brief Processes the LED commands and updates the LED states based on axis values
 * @param ledCmd Indicates if the LED should be turned on or off, regardless of the axis values
 * @details This function updates the LED states based on the axis values and the LED command.
 */
void LedRing::ProcessLED(boolean ledCmd) {
    unsigned long now = millis();
    static unsigned long lastLEDupdate = now;
    Axis *axis; // pointer to the axis object

    if (now - lastLEDupdate >= LEDUPDATERATE_MS) {
        _setAllLEDs(CRGB::Black);
        if (ledCmd) {
            // turn all on
            _setAllLEDs(CRGB::DarkOliveGreen);
        } else {
            AxisType_t mainAxis = Kinematics::getInstance()->getMainAxis(axis);

            // USB doesn't send us commands to turn on LED
            switch (mainAxis) {
            case TRANSX:
                _setAllLEDs(CRGB::Yellow);
                // TX pos: 3 o'clock neg: 9 o'clock
                // light up the _free_ positions
                // REVIEW - check if this is correctly implemented. I'm in doubt about the inversion. I don't have a leadring so I cannot test it right now.
                if ((axis->getValue() > 0) != axis->getConfig()->inversion) {
                    _set4LEDsOnClock(9, CRGB::Red);
                } else {
                    _set4LEDsOnClock(3, CRGB::Red);
                }
                break;
            case TRANSY:
                _setAllLEDs(CRGB::Yellow);
                // TY pos: 12 o'clock neg, 6 o'clock
                if ((axis->getValue() > 0) != axis->getConfig()->inversion) {
                    _set4LEDsOnClock(6, CRGB::Red);
                } else {
                    _set4LEDsOnClock(12, CRGB::Red);
                }
                break;
            case TRANSZ:
                // TZ pos: all white, neg: all dark blue
                if ((axis->getValue() > 0) != axis->getConfig()->inversion) {
                    _setAllLEDs(CRGB::AntiqueWhite);
                    FastLED.setBrightness(50);
                } else {
                    _setAllLEDs(CRGB::DarkBlue);
                    FastLED.setBrightness(50);
                }
                break;
            case ROTX:
                _setAllLEDs(CRGB::SkyBlue);
                // RX pos: red 6 o'clock, neg red 12 o'clock
                if ((axis->getValue() > 0) != axis->getConfig()->inversion) {
                    _set4LEDsOnClock(12, CRGB::Green);
                } else {
                    _set4LEDsOnClock(6, CRGB::Green);
                }
                break;
            case ROTY:
                _setAllLEDs(CRGB::SkyBlue);
                // RY pos: red 3 o'clock, neg red 9 o'clock
                if ((axis->getValue() > 0) != axis->getConfig()->inversion) {
                    _set4LEDsOnClock(9, CRGB::Green);
                } else {
                    _set4LEDsOnClock(3, CRGB::Green);
                }
                break;
            case ROTZ:
                _setAllLEDs(CRGB::SkyBlue);
                // RZ pos: red ring wandering around counterclock wise; neg: clockwise
                if ((axis->getValue() > 0) != axis->getConfig()->inversion) {
                    _rotateColor(false, CRGB::DarkRed);
                } else {
                    _rotateColor(true, CRGB::DarkRed);
                }
                break;
            case UNINITIALIZED: /* no-break */
            default:
                // Set all LEDs to dimmed dark grey
                _setAllLEDs(CRGB::DarkGrey);
                FastLED.setBrightness(5);
                break;
            }
        }
        FastLED.show();
        lastLEDupdate += LEDUPDATERATE_MS;
    }
}

/**
 * @brief Rotates a single LED around the LED ring
 * @param clockwise Turns clockwise if true
 * @param color The CRGB color to use for the LED
 */
void LedRing::_rotateColor(const boolean clockwise, const CRGB color) {
    static int rotateLEDpos = 0;
    _leds[rotateLEDpos] = color;
    if (clockwise) {
        rotateLEDpos = (rotateLEDpos + 1) % LEDRING;
    } else {
        rotateLEDpos = (rotateLEDpos + LEDRING - 1) % LEDRING; // avoid negativ led position
    }
}

/**
 * @brief Sets all leds to given color
 * @param color the color to set all LEDs to
 * @details This function iterates through all LEDs in the LED ring and sets them to the specified color.
 */
void LedRing::_setAllLEDs(const CRGB color) {
    for (int i = 0; i < LEDRING; i++) {
        _leds[i] = color;
    }
}

/**
 * @brief Sets the LED at a specific clock position to a given color
 * @param clock Position of the LED to light up
 * @param color Color to light
 */
void LedRing::_setLEDsOnClock(const uint16_t clock, const CRGB color) {
    uint16_t pos = 0;
    pos = (clock * (LEDRING / 12)) % LEDRING;
    pos = (LEDclockOffset + pos) % LEDRING;
    _leds[pos] = color;
}

/**
 * @brief Sets 4 LEDs on the LED ring at a specific clock position to a given color
 * @details This function lights up the LED at the specified clock position and the two LEDs adjacent to it, creating a visual effect.
 * @param clock Position of the LED to light up
 * @param color Color to light
 */
void LedRing::_set4LEDsOnClock(const uint16_t clock, const CRGB color) {
    uint16_t pos = 0;
    pos = (clock * (LEDRING / 12)) % LEDRING;
    pos = (LEDclockOffset + pos) % LEDRING;

    _leds[pos] = color;
    _leds[(pos + 1) % LEDRING] = color;
    _leds[(LEDRING + pos - 1) % LEDRING] = color;
    _leds[(LEDRING + pos - 2) % LEDRING] = color;
}

#endif // #if LEDring