#pragma once

#if defined(ARDUINO_ARCH_ESP32)
#include <esp32-hal-adc.h> // For ESP32 ADC functions
// #include <esp_adc_cal.h>
#elif defined(ARDUINO_ARCH_AVR)
#include <Arduino.h> // For Arduino functions
// #include <wiring_private.h> // for Arduino Analog functions
#endif

/**
 * @brief Wrapper class for ADC functionality.
 * @details This class provides a unified interface for reading analog values from ADC pins.
 *          It supports both ESP32 and AVR architectures.
 */
class ArchitectureADC {
public:
    ArchitectureADC() = delete;

#if defined(ARDUINO_ARCH_ESP32)
    static void setupADC() {
        // ADC_ATTEN_DB_11 is used to set the attenuation for the ADC, this allows the ADC to read voltages up to 3.1V
        analogSetAttenuation(ADC_11db);
        analogReadResolution(12); // Set ADC resolution to 12 bits (0-4095)
    }

    static void connectADC(const int pin) {
        adcAttachPin(pin); // Attach the specified pin to the ADC
    }

    /**
     * @brief Reads the analog value from the specified pin.
     * @param pin The pin number to read from.
     * @return The analog value read from the pin.
     * @retval int The analog value read from the pin (0-4095).
     */
    static int readADC(const int pin) {
        return analogRead(pin); // Read the analog value from the specified pin
    }
#endif

#if defined(ARDUINO_ARCH_AVR)
    // DEVNOTE - The Pro Micro has a different ADC as the Arduino ATMega2560, this code is not tested on the latter.
    static void setupADC() {
        // Initialize ADC for AVR
        // Set the ADC Prescaler to 16 in order to read the ADC much faster than default.
        // NOTE: Added this for the Joystick hardware as well, but untested.
        ADCSRA = (ADCSRA & B11111000) | 4; // Set prescaler to 16 for ADC

        // Set the analog reference voltage for the sensors to 2.56V on the Pro Micro.
        // See https://docs.arduino.cc/language-reference/en/functions/analog-io/analogReference/
        analogReference(INTERNAL);
    }

    static void connectADC(const int pin) {
        pinMode(pin, INPUT); // Set the specified pin as an input for ADC
    }

    /**
     * @brief Reads the analog value from the specified pin.
     * @param pin The pin number to read from.
     * @return The analog value read from the pin.
     * @retval int The analog value read from the pin (0-1023).
     */
    static int readADC(const int pin) {
        return analogRead(pin); // Read the analog value from the specified pin
    }
#endif
};