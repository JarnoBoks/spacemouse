#include <Arduino.h>
#include "config.h"

// Setup arrays for reading the analog pins of the Arduino, see config.h

/// @brief  Array containing ADC pin configuration for the sensors.
int pinList[8] = PINLIST;

/// @brief  Array containing inversion parameters for the raw sensor readings.
int invertList[8] = INVERTLIST;

/**
 *  @brief Function to read and store analogue voltages for each sensor/joystick axis.
 *  The function reads the pins that are setup in config.h and inverts the reading if necessary.
 *
 *  @param rawReads pointer to the array that will contain the read analog values
 */
void readAllFromSensors(int *rawReads) {
    for (int i = 0; i < 8; i++) {
        if (invertList[i] == 1) {
            // invert the reading
            rawReads[i] = 1023 - analogRead(pinList[i]);
        } else {
            rawReads[i] = analogRead(pinList[i]);
        }
    }
}