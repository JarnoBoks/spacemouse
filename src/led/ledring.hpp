#pragma once

#include <observers/IObserver.hpp>
#include "config.h" // For NUM_LED_LEDRING

#include <FastLED.h> // Include FastLED library for LED control

// Only include the LedRing class if NUM_LED_LEDRING and LEDPIN are defined
#if defined(NUM_LED_LEDRING) && defined(LEDPIN)
 * @brief Class representing an LED ring.
 * @details This class implements the IObserver interface and is responsible for managing the LED rings state, including the number of LEDs and their brightness.
 *          It can be used to update the LED ring based on changes in the observed object.
 */
class LedRing : public IObserver {
 private:
     CRGB m_Leds[NUM_LED_LEDRING];

     unsigned long m_lastOutput = 0;
     const bool isLedOutputDue();

     void setAllLEDs(const CRGB color);
     void set4LEDsOnClock(const uint16_t clock, const CRGB color);

     uint8_t m_lastRotatedLed = 0;
     void rotateColor(const bool clockwise, const CRGB color);

 public:
     LedRing();

     void update(IObservable *knobAxisCollection) override;

     void TurnOn();
     void TurnOff();
 };
#endif // defined(NUM_LED_LEDRING)