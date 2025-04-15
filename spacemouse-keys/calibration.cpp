// File for calibration specific functions

#include <Arduino.h>
#include "calibration.h"
#include "kinematics.h"
#include "config.h"
#ifdef EEPROM_CALIBRATION
#include "sensitivity.h"
#include "EEPROM.h"
#include "eepromStorage.h"
#endif #include "hardware/SpaceMouseHW.h"

/// @brief Hold characters to plot them
char debugOutputBuffer[20];

/**
 * @brief Prints the current status of the key
 */
void _printKey(int i, int keyval) {
    Serial.print("K");
    Serial.print(i);
    Serial.print(":");
    Serial.print(keyval);
}

/**
 * @brief Prints the current status of the keys
 * @param keyVals pointer to the int array containing the key values
 */
void printKeys(int *keyVals) {
    for (int i = 0; i < NUMKEYS; i++) {
        if (i != 0) {
            Serial.print(", ");
        }
        _printKey(i, keyVals[i]);
    }
}
/**
 * @brief Prints the current status of the keys
 * @param keyVals pointer to the uint8_t array containing the key values
 */
void printKeys(uint8_t *keyVals) {
    for (int i = 0; i < NUMKEYS; i++) {
        if (i != 0) {
            Serial.print(", ");
        }
        _printKey(i, (int)keyVals[i]);
    }
}

/// Array for the velocity names.
char const *velNames[] = {"TX:", "TY:", "TZ:", "RX:", "RY:", "RZ:"}; // 6

/**
 * @brief Prints the raw ADC 10-bit values, if the output is due (every x miliseconds)
 * @param rawReads  pointer to the int array where the raw ADC values are stored
 * @param keyVals   pointer to the int array where the raw key readings are stored (no debouncing)
 */
void debugOutput1(int *rawReads, int *keyVals) {
    void debugOutput1(SpaceMouseHW_ & SMHW, int *keyVals) {
        if (isDebugOutputDue()) {
            SMHW.PrintRawReads();
            // Report back 0-1023 raw ADC 10-bit values if enabled
            printKeys(keyVals);
            Serial.print(DEBUG_LINE_END);
        }
        char const *velNames[] = {"TX:", "TY:", "TZ:", "RX:", "RY:", "RZ:"}; // 6

        void debugOutput1(SpaceMouseHW_ & SMHW, int *keyVals) {
            if (isDebugOutputDue()) {
                SMHW.PrintRawReads();
                // Report back 0-1023 raw ADC 10-bit values if enabled
                for (int i = 0; i < NUMKEYS; i++) {
                    Serial.print("K");
                    Serial.print(i);
                    Serial.print(":");
                    Serial.print(keyVals[i]);
                    Serial.print(", ");
                }
                Serial.print(DEBUG_LINE_END);
            }
        }

        void debugOutput2(SpaceMouseHW_ & SMHW) {
            if (isDebugOutputDue()) {
                SMHW.PrintCentered();
                Serial.print(DEBUG_LINE_END);
            }
        }

        /// @brief Report translation and rotation values if enabled.
        /// @param velocity pointer to velocity array
        /// @param keyOut pointer to keyOut array
        void debugOutput4(int16_t *velocity, uint8_t *keyOut) {
            //
            if (isDebugOutputDue()) {
                for (int i = 0; i < 6; i++) {
                    sprintf(debugOutputBuffer, "%2.2s: %4d ", velNames[i], velocity[i]);
                    Serial.print(debugOutputBuffer);
                }

                printKeys(keyOut);
                Serial.print(DEBUG_LINE_END);
            }
        }

#ifdef EEPROM_CALIBRATION

        /**
         *
         * @brief Report translation & rotation values, and configured sensititivity parameters.
         * @param velocity pointer to velocity array
         * @param keyOut pointer to keyOut array
         * @param sensitivities pointer to the sensitivities array
         */
        void debugOutput4(int16_t *velocity, uint8_t *keyOut, sensitivities_t *sensitivities) {
            //
            if (isDebugOutputDue()) {
                for (int i = 0; i < 6; i++) {
                    sprintf(debugOutputBuffer, "%2.2s: %4d ", velNames[i], velocity[i]);
                    Serial.print(debugOutputBuffer);
                }
                printKeys(keyOut);
                Serial.print(F(" || "));
                printSensitivity(sensitivities, false);
                Serial.print(DEBUG_LINE_END);
            }
        }
#endif

        /// @brief Report single axis and resulting velocities info side by side for direct reference. Very useful if you need to alter which inputs are used in the arithmetic above.
        /// @param centered pointer to arrays of 8 axis
        /// @param velocity pointer to array of 6 velocities

        void debugOutput5(SpaceMouseHW_ & SMHW, int16_t *velocity) {
            if (isDebugOutputDue()) {
                SMHW.PrintCentered();
                Serial.print(" || ");
                for (int i = 0; i < 6; i++) {
                    sprintf(debugOutputBuffer, "%2.2s: %4d ", velNames[i], velocity[i]);
                    Serial.print(debugOutputBuffer);
                }
                Serial.print(DEBUG_LINE_END);
            }
        }

        // Variables and function to get the min and maximum value of the centered values
        int minMaxCalcState = 0; // little state machine -> setup in 0 -> measure in 1 -> output in 2 ->  end in 3
        int minValue[8];         // Array to store the minimum values
        int maxValue[8];         // Array to store the maximum values
        unsigned long startTime; // Start time for the measurement

        /// @brief This function records the minimum and maximum movement of the joysticks: After initialization, move the mouse for 15s and see the printed output. Replug/reset the mouse, to enable the semi-automatic calibration for a second time.
        /// @param centered pointer to the array with the centered joystick values
        void calcMinMax(int *centered) {
            if (minMaxCalcState == 0) {
                delay(2000);
                // Initialize the arrays
                for (int i = 0; i < 8; i++) {
                    minValue[i] = 1023; // Set the min value to the maximum possible value
                    maxValue[i] = 0;    // Set the max value to the minimum possible value
                }
                startTime = millis(); // Record the current time
                minMaxCalcState = 1;  // next State: measure!
                Serial.println(F("Please start moving the spacemouse around for 15 sec!"));
            } else if (minMaxCalcState == 1) {
                if (millis() - startTime < 15000) {
                    for (int i = 0; i < 8; i++) {
                        // Update the minimum and maximum values
                        if (centered[i] < minValue[i]) {
                            minValue[i] = centered[i];
                        }
                        if (centered[i] > maxValue[i]) {
                            maxValue[i] = centered[i];
                        }
                    }
                } else {
                    // 15s are over. go to next state and report via console
                    Serial.println(F("\n\nStop moving the spacemouse. These are the result. Copy them in config.h"));
                    minMaxCalcState = 2;
                }
            } else if (minMaxCalcState == 2) {
                Serial.print(F("#define MINVALS "));
                printArray(minValue, 8);
                Serial.print(F("#define MAXVALS "));
                printArray(maxValue, 8);
                for (int i = 0; i < 8; i++) {
                    if (abs(minValue[i]) < 250) {
                        Serial.print(F("Warning: minValue["));
                        Serial.print(i);
                        Serial.print("] ");
                        Serial.print(axisNames[i]);
                        Serial.print(F(" is small: "));
                        Serial.println(minValue[i]);
                    }
                    if (abs(maxValue[i]) < 250) {
                        Serial.print(F("Warning: maxValue["));
                        Serial.print(i);
                        Serial.print("] ");
                        Serial.print(axisNames[i]);
                        Serial.print(F(" is small: "));
                        Serial.println(maxValue[i]);
                    }
                }
                minMaxCalcState = 3; // no further reporting
            }
        }

        /// @brief Check, if a new debug output shall be generated. This is used in order to generate a debug line only every DEBUGDELAY ms, see config.h
        /// @return true, if debug message is due
        bool isDebugOutputDue() {
            static unsigned long lastDebugOutput = 0; // time from millis(), when the last debug output was given

            if (millis() - lastDebugOutput > DEBUGDELAY) {
                lastDebugOutput = millis();
                return true;
            } else {
                return false;
            }
        }

        uint16_t iterationsPerSecond = 0;      // count the iterations within one second
        unsigned long lastFrequencyUpdate = 0; // time from millis(), when the last frequency was calculated

        /// @brief update and report the function to learn at what frequency the loop is running
        void updateFrequencyReport() {
            // increase iterations counter
            iterationsPerSecond++;
            if (millis() - lastFrequencyUpdate > 1000) { // if one second has past: report frequency
                Serial.print("Frequency: ");
                Serial.print(iterationsPerSecond);
                Serial.println(" Hz");
                lastFrequencyUpdate = millis(); // reset timer
                iterationsPerSecond = 0;        // reset iteration counter
            }
        }

#ifdef EEPROM_CALIBRATION
        /**
         * @brief   Checks if the value is a valid calibration option.
         */
        bool validCalibrationOption(int value) {
            int calibrationOptions[NUM_CALIBRATION_OPTIONS] = CALIBRATION_OPTIONS;
            for (uint8_t i = 0; i < NUM_CALIBRATION_OPTIONS; i++) {
                if (value == calibrationOptions[i]) {
                    return true;
                }
            }
            return false;
        }

        uint8_t _readByte(int address, int defValue) {
            uint8_t value;
            // Read value from the EEPROM, the values are stored as float.
            EEPROM.get(address, value);

            // We assume that if one of the values isn't set, the entire struct isn't set.
            if (value == 0xFF || FORCE_EEPROM_INIT) {
                EEPROM.put(address, defValue);
                value = defValue;
            }

            return value;
        }

        uint8_t readModfunc() {
            return _readByte(EEPROM_ADDRESS_MODFUNC, MODFUNC);
        }

        /**
         * Modfunc commands send through the Serial Monitor should be in the format
         *
         *   3 00000 x              where x is the new modfunc value
         *
         */

#define MODFUNC_CMD_OFFSET 3000000L
        uint8_t updateModfunc(long input) {
            uint8_t value = (uint8_t)(input - MODFUNC_CMD_OFFSET);
            if (value < 0 || value > 4) {
                // We have developed four modifier functions
                Serial.println(F("Invalid modfunc"));
                value = readModfunc();
            } else {

                // Write the value to the Serial Monitor for user feedback.
                Serial.print(F("Updated modifier function as: "));
                Serial.println(value);

                // Store the sensitivity in the EEPROM. EEPROM.put() uses EEPROM.update and thus only writes data if the data has changed.
                EEPROM.put(EEPROM_ADDRESS_MODFUNC, value);
            }

            return value;
        }

        void printModfunc(uint8_t modfunc, bool introtext) {
            if (introtext) {
                Serial.println(F("Current modfunc: "));
            }

            Serial.print(modfunc);

            if (introtext) {
                Serial.println();
            }
        }

#define INVERSION_CMD_OFFSET 10000L // Value consists of 4 digits
        uint8_t updateInversions(long input) {
            uint8_t ret = readInversions();

            // Get the command, divide by ITEMPOS removes the value from the input.
            // (fe. input 2010150, cmd = (input/1000) = 201)
            uint16_t cmd = (input / INVERSION_CMD_OFFSET);

            // Get interger value (last digit) and check if this is 0 or 1
            long value = input - (cmd * INVERSION_CMD_OFFSET);

            // Check if the value and the command are within our boundaries.
            if (value < 0 || value > 1 || cmd < 400 || cmd > 405) {
                // We have developed four modifier functions
                Serial.println(F("Invalid inversion"));
            } else {

                Serial.print(F("Updating inversion to "));
                Serial.println(value);

                if (value) {
                    // Set the commanded bit
                    ret |= (1u << (cmd - 400));
                } else {
                    // Unset the commamded bit
                    ret &= ~(1u << (cmd - 400));
                }

                // Store the value in the EEPROM. EEPROM.put() uses EEPROM.update and thus only writes data if the data has changed.
                EEPROM.put(EEPROM_ADDRESS_INVERSIONS, ret);
                printInversions(ret, true);
            }

            return ret;
        }

        uint8_t readInversions() {

            uint8_t defValue = DEFAULT_INVERSION;

            // Read values from the EEPROM
            return _readByte(EEPROM_ADDRESS_INVERSIONS, defValue);
        }

#define PRINT_INVERSION(y) Serial.print(GET_INVERSION(inversions, y))
        void printInversions(uint8_t inversions, bool introtext) {
            if (introtext) {
                Serial.println(F("Current inversions: "));
            }

            Serial.print(F("{ INVX: "));
            PRINT_INVERSION(AX_INVX);
            Serial.print(F(", INVY: "));
            PRINT_INVERSION(AX_INVY);
            Serial.print(F(", INVZ: "));
            PRINT_INVERSION(AX_INVZ);
            Serial.print(F(", INVRX: "));
            PRINT_INVERSION(AX_INVRX);
            Serial.print(F(", INVRY: "));
            PRINT_INVERSION(AX_INVRY);
            Serial.print(F(", INVRZ: "));
            PRINT_INVERSION(AX_INVRZ);
            Serial.print(F(" }"));

            if (introtext) {
                Serial.println();
            }
        }

#endif