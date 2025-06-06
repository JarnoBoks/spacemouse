#include "LedRing.hpp"

#include <FastLED.h> // Include FastLED library for LED control
#include "config.h"  // Default configuration for the LED ring (LEDUPDATERATE_MS, LEDPIN, NUM_LED_LEDRING)
#include <visitors/KnobAxisCollectionGetMainAixs.hpp>

#ifndef LEDUPDATERATE_MS
#define LEDUPDATERATE_MS 150
#endif

LedRing::LedRing() {
    FastLED.addLeds<WS2811, LEDPIN, GRB>(m_Leds, NUM_LED_LEDRING);
}

const bool LedRing::isLedOutputDue() {
    unsigned long now = millis();
    if (now - m_lastOutput >= LEDUPDATERATE_MS) {
        m_lastOutput = now;
        return true;
    }
    return false;
}

/**
 * @brief Turn on the LED ring by setting all LEDs to dark olive green.
 */
void LedRing::TurnOn() {
    setAllLEDs(CRGB::DarkOliveGreen);
    FastLED.show();
}

/**
 * @brief Turn off the LED ring by setting all LEDs to black.
 */
void LedRing::TurnOff() {
    setAllLEDs(CRGB::Black);
    FastLED.show();
}

void LedRing::update(IObservable *knobaxiscollection) {
    if (!isLedOutputDue()) {
        return;
    }

    KnobAxisCollection *knobAxisCollection = static_cast<KnobAxisCollection *>(knobaxiscollection);
    KnobAxisCollectionGetMainAxis visitor(VELOCITYDEADZONEFORLED); // Create a visitor to get the main axis
    knobAxisCollection->accept(visitor);                           // Accept the visitor to find the main axis

    KnobAxis *mainAxis = visitor.getMainAxis(); // Get the main axis from the visitor
    if (!mainAxis) {
        ESP_INFO("No main axis, skipping LED update.");
        return; // If no main axis is found, exit the function
    }

    // setAllLEDs(CRGB::Black); // Clear all LEDs before updating // REVIEW - Is this needed?

    bool posMove = (mainAxis->getGateValue() > 0);
    switch (mainAxis->getType()) {
    case TRANSX:
        setAllLEDs(CRGB::Yellow);
        // TX pos: 3 o'clock neg: 9 o'clock
        // light up the _free_ positions
        if (posMove) {
            set4LEDsOnClock(9, CRGB::Red);
        } else {
            set4LEDsOnClock(3, CRGB::Red);
        }
        break;
    case TRANSY:
        setAllLEDs(CRGB::Yellow);
        // TY pos: 12 o'clock neg, 6 o'clock
        if (posMove) {
            set4LEDsOnClock(6, CRGB::Red);
        } else {
            set4LEDsOnClock(12, CRGB::Red);
        }
        break;
    case TRANSZ:
        // TZ pos: all white, neg: all dark blue
        if (posMove) {
            setAllLEDs(CRGB::AntiqueWhite);
            FastLED.setBrightness(50);
        } else {
            setAllLEDs(CRGB::DarkBlue);
            FastLED.setBrightness(50);
        }
        break;
    case ROTX:
        setAllLEDs(CRGB::SkyBlue);
        // RX pos: red 6 o'clock, neg red 12 o'clock
        if (posMove) {
            set4LEDsOnClock(12, CRGB::Green);
        } else {
            set4LEDsOnClock(6, CRGB::Green);
        }
        break;
    case ROTY:
        setAllLEDs(CRGB::SkyBlue);
        // RY pos: red 3 o'clock, neg red 9 o'clock
        if (posMove) {
            set4LEDsOnClock(9, CRGB::Green);
        } else {
            set4LEDsOnClock(3, CRGB::Green);
        }
        break;
    case ROTZ:
        setAllLEDs(CRGB::SkyBlue);
        // RZ pos: red ring wandering around counterclock wise; neg: clockwise
        if (posMove) {
            rotateColor(false, CRGB::DarkRed);
        } else {
            rotateColor(true, CRGB::DarkRed);
        }
        break;
    default:
        // Set all LEDs to dimmed dark grey
        setAllLEDs(CRGB::DarkGrey);
        FastLED.setBrightness(5);
        break;
    }
    FastLED.show();
}

/**
 * @brief Rotates a single LED around the LED ring
 * @param clockwise Turns clockwise if true
 * @param color The CRGB color to use for the LED
 */
void LedRing::rotateColor(const bool clockwise, const CRGB color) {
    static int rotateLEDpos = 0;
    m_Leds[m_lastRotatedLed] = color;
    if (clockwise) {
        m_lastRotatedLed = (++m_lastRotatedLed) % NUM_LED_LEDRING;
    } else {
        m_lastRotatedLed = (m_lastRotatedLed + NUM_LED_LEDRING - 1) % NUM_LED_LEDRING; // avoid negativ led position
    }
}

/**
 * @brief Sets all leds to given color
 * @param color the color to set all LEDs to
 * @details This function iterates through all LEDs in the LED ring and sets them to the specified color.
 */
void LedRing::setAllLEDs(const CRGB color) {
    for (int i = 0; i < NUM_LED_LEDRING; i++) {
        m_Leds[i] = color;
    }
}

#if 0 // NOT USED
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
#endif

/**
 * @brief Sets 4 LEDs on the LED ring at a specific clock position to a given color
 * @details This function lights up the LED at the specified clock position and the two LEDs adjacent to it, creating a visual effect.
 * @param clock Position of the LED to light up
 * @param color Color to light
 */
void LedRing::set4LEDsOnClock(const uint16_t clock, const CRGB color) {
    uint16_t pos = 0;
    pos = (clock * (NUM_LED_LEDRING / 12)) % NUM_LED_LEDRING;
    pos = (LEDclockOffset + pos) % NUM_LED_LEDRING;

    m_Leds[pos] = color;
    m_Leds[(pos + 1) % NUM_LED_LEDRING] = color;
    m_Leds[(NUM_LED_LEDRING + pos - 1) % NUM_LED_LEDRING] = color;
    m_Leds[(NUM_LED_LEDRING + pos - 2) % NUM_LED_LEDRING] = color;
}
