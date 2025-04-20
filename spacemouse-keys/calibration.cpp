// File for calibration specific functions

#include <Arduino.h>
#include "text.h"
#include "calibration.h"
#include "kinematics.h"
#include "config.h"
#include "hardware/SpaceMouseHW.h"

/**
 * @brief Prints the raw (optionally inverted) ADC 10-bit values and the status of the raw key readings (without debouncing), if the output is due (every x miliseconds)
 * @param keyVals   pointer to the int array where the raw key readings are stored (no debouncing)
 */
void Calibration::DebugOutputRawInverted() {
    if (_debug == 1 && _isDebugOutputDue()) {
        _SMHW->PrintRawReads();
        // Report back 0-1023 raw ADC 10-bit values if enabled
        _SMKEYS->PrintKeyVals();
        Serial.print(DEBUG_LINE_END);
    }
}

/**
 * @brief Prints the centered values of the sensors and the status of the keys, if the output is due (every x miliseconds)
 * @details Centered values are the difference between the raw values and the centerPoint values.
 */
void Calibration::DebugOutputCentered() {
    if (_debug == 2 && _isDebugOutputDue()) {
        _SMHW->PrintCentered();
        Serial.print(DEBUG_LINE_END);
    }
}

void Calibration::DebugOutputDeadzonedMapped() {
    if (_debug == 3 && _isDebugOutputDue()) {
        _SMHW->PrintCentered();
        Serial.print(DEBUG_LINE_END);
    }
}

/**
 * @brief Report translation & rotation values, configured sensititivity parameters and status of the mouse buttons.
 * @param SMKIN Pointer to the Kinematics object containing the current translation & rotation values aswell as the sensitivity configuration.
 * @param keyOut Pointer to the array containing the status of the mousebuttons
 */
void Calibration::DebugOutput4() {
    if (_debug == 4)
        _debugOutput_VelocitiesKeys();
}

/**
 * @brief Report single axis and  translation & rotation values side by side for direct reference. Very useful if you need to alter which inputs are used in the arithmetic above.
 *
 */
void Calibration::DebugOutput5() {
    if (_debug == 5 && _isDebugOutputDue()) {
        _SMHW->PrintCentered();
        Serial.print(" || ");
        _SMKIN->PrintVelocities();
        Serial.print(DEBUG_LINE_END);
    }
}

void Calibration::DebugOutput6() {
    if (_debug == 6)
        _debugOutput_VelocitiesKeys();
}

void Calibration::DebugOutput61() {
    if (_debug == 61)
        _debugOutput_VelocitiesKeys();
}

void Calibration::_debugOutput_VelocitiesKeys() {
    if (_isDebugOutputDue()) {
        _SMKIN->PrintVelocities();
        _SMKEYS->PrintKeyState();
        Serial.print(DEBUG_LINE_END);
    }
}

/**
 * @brief Check, if a new debug output shall be generated. This is used in order to generate a debug line only every DEBUGDELAY ms, see config.h
 * @return true, if debug output is due
 */
bool Calibration::_isDebugOutputDue() {
    static unsigned long lastDebugOutput = 0; // time from millis(), when the last debug output was given

    if (millis() - lastDebugOutput > DEBUGDELAY) {
        lastDebugOutput = millis();
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Update the frequency report. This function is called every second to report the frequency of the loop.
 * @details The frequency is calculated by counting the number of iterations in one second and printing it to the serial monitor.
 */
void Calibration::UpdateFrequencyReport() {
    // increase iterations counter
    _iterationsPerSecond++;
    if (millis() - _lastFrequencyUpdate > 1000) { // if one second has past: report frequency
        Serial.print("Freq: ");
        Serial.print(_iterationsPerSecond);
        Serial.println(" Hz");
        _lastFrequencyUpdate = millis(); // reset timer
        _iterationsPerSecond = 0;        // reset iteration counter
    }
}

/**
 * @brief  Handle the input from the serial monitor. This function reads the input from the serial monitor and processes it.
 * @details The function reads the input until a newline character or the buffer limit is reached. It then tokenizes the input and handles the command based on the number of received words.
 */
void Calibration::DebugInput() {
    // NOTE - The input buffer size is limited to 64 bytes. If increased above 127 bytes, check typecast in the while loop!!
    char inputBuffer[64]; // Buffer to store the input command
    uint8_t bytesRead = 0;

    // Read the input into the buffer until a newline character or buffer limit
    while (Serial.available() > 0 && bytesRead < sizeof(inputBuffer) - 1) {
        char c = Serial.read();
        if (c == '\n') {
            break; // Stop reading at newline
        }
        if (c == '\r') {
            continue; // Ignore carriage return
        }
        inputBuffer[bytesRead++] = toupper(c);
    }
#if ARDUINO_ARCH_ESP32
    if (bytesRead == sizeof(inputBuffer) - 1) {
        Serial.println(F("Reduce command!"));
        return; // Buffer is full, exit the function
    }
#endif

    inputBuffer[bytesRead] = '\0'; // Null-terminate the string

#if ARDUINO_ARCH_ESP32
    Serial.print(F("---- Received: ")); // Print the input string
    Serial.println(inputBuffer);        // Print the input string
#endif

    if (bytesRead > 0) {
        char *wordPtr = NULL; // Pointer to the current word buffer
        char *words[3] = {};  // Array of word pointers
        byte index = 0;       // Index for the words array

        wordPtr = strtok(inputBuffer, " ");                                                // Tokenize the input string by spaces
        while (wordPtr != NULL && wordPtr - inputBuffer < (char)sizeof(inputBuffer) - 1) { // cast to char to prevent signed/unsigned comparison. Will fit easily due to the buffer size of 64.
            words[index] = wordPtr;                                                        // Store the token in the words array
            index++;                                                                       // Increment the index for the next word
            if (index > 3) {
                break; // Stop if we have three words
            }
            wordPtr = strtok(NULL, " "); // Get the next token
        }

        switch (index) {
        case 1:
            // One word command received, handle it
            _handleOneWord(words); // Handle the command
            break;
        case 2:
            // Two words command received, handle it
            _handleTwoWords(words); // Handle the command
            break;
        case 3:
            // Three words command received, handle it.
            _handleThreeWords(words); // Handle the command
            break;
        case 0: /* no-break */
        default:
            Serial.print(CF(Error_CommandUnkown));
            Serial.println(inputBuffer);
            return; // Not enough or too many words, exit the function
        }
    }
}

/**
 * @brief Handle one word commands.
 * @details The first word is the command and there are no parameters.
 * @param words[] Array of words received from the serial monitor.
 * @return
 */
int8_t Calibration::_handleOneWord(char *words[]) {
    int8_t ret = 0;

    // --------------- SHOW ----------------------------------------------------
    if (strcmp_P(words[0], CMD_SHOW) == 0) {
        // Show all stored calibration values
        _SMKIN->PrintAxisConfigurations();
#if 0
        _SMKIN->PrintTransRotInversions(true);
        _SMKIN->PrintModulationFunction(true);
#endif
        Serial.println(F("\nHardware configuration"));

        _SMHW->PrintDeadzone();
        _SMHW->PrintMinMax();

        // --------------- IDLE ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_IDLE) == 0) {
        // Calibrate Idle position
        Serial.println(F("Calibrating Idle"));
        _SMHW->BusyZeroing(2000, true); // Call the function to calibrate the idle position

        // --------------- MINMAX ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_MINMAX) == 0) {
        // Calibrate Idle position
        _SMHW->CalibrateMinMax(); // Call the function to start the min/max calibration without storing the values in EEPROM

        // --------------- SENS ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_SENS) == 0) {
        _SMKIN->PrintAxisConfigurations(); // Print the current sensitivities

        // --------------- DZ ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_DEADZONE) == 0) {
        _SMHW->PrintDeadzone();
        // --------------- MODFUNC ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_MODFUNC) == 0) {
#if 0
        // Sets all axis/modfuncs to the same value.
        Serial.println(F("Setting all axis/modfuncs to the same value."));
        _SMKIN->PrintModulationFunction(true);
#endif
    } else {
        Serial.println(CF(Error_CommandUnkown));
        ret = -1;
    }
    return ret;
}

/**
 * @brief Handle two words commands.
 * @details The first word is the command and the second word is the parameter.
 * @param words[] Array of words received from the serial monitor.
 * @return
 */
int8_t Calibration::_handleTwoWords(char *words[]) {
    // int8_t Calibration::_handleTwoWords(const char *word1, const char *word2) {
    int8_t ret = 0;

    // When we have two words, the first word is the command and the second word is the parameter.
    // We have to check if both words are supplied and if the second word is a number or a string.
    if (isWordEmpty(words[0], Error_EmptyCommand)) {
        return -1; // No command given
    }

    if (isWordEmpty(words[1], Error_EmptyParameter)) {
        return -2; // No parameter given
    }

    long value = 0; // Default value for the second word
    if (!convertWordNumber(words[1], &value, Error_ParameterNoNumber)) {
        return -3; // Second parameter is not a number
    }

    // --------------- DEBUG x ----------------------------------------------------
    if (strcmp_P(words[0], CMD_DEBUG) == 0) {
        Serial.print(F("Debug level -> "));
        Serial.println(value);
        _debug = (DebugLevel_t)value; // Set the debug level

        // Inform the hardware of the debuglevel update.
        _SMHW->SetAnalogReferenceVoltage(_debug);

        // --------------- MODFUNC x ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_MODFUNC) == 0) {
#if 0
        Serial.print(F("Modifier -> "));
        Serial.println(value);
        _SMKIN->SetModulationFunction(value); // Call the function to set the modulation function
#endif

        // --------------- DEADZONE x ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_DEADZONE) == 0) {
        Serial.print(F("Deadzone -> "));
        Serial.println(value);
        _SMHW->SetDeadzone(value); // Call the function to set the deadzone
        // --------------- DEADZONE x ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_MINMAX) == 0) {
        // Call the function to start the min/max calibration and store the values in EEPROM if value is 1, otherwise do not store the values in EEPROM
        _SMHW->CalibrateMinMax((value == 1));
    } else {
        Serial.println(CF(Error_CommandUnkown));
        return -1; // Unknown command
    }

    return ret;
}

/**
 * @brief Handle three words commands.
 * @details The first word is the command, the second word is the axis+sensitivity, and the third word is the value.
 * @param words[] Array of words received from the serial monitor.
 * @return
 */
int8_t Calibration::_handleThreeWords(char *words[]) {
    // Handle three words command. The first word is the command, the second word is the axis, and the third word is the value.
    // We have to check if all three words are supplied and if the third word is a float.
    if (isWordEmpty(words[0], Error_EmptyCommand)) {
        return -1; // No command given
    }
    if (isWordEmpty(words[1], Error_EmptyParameter)) {
        return -2; // No parameter given
    }
    if (isWordEmpty(words[2], Error_EmptyValue)) {
        return -3; // No value given
    }

    float value; // Default value for the third word
    if (!convertWordFloat(words[2], &value, Error_ParameterNoFloat)) {
        return -4; // Third parameter is not a float
    }

    // Example handling for three words
    if (strcmp_P(words[0], CMD_SENS) == 0) {
        // Handle the sensitivity command
        // Use the first character of the second word to determine the axis
        bool isGT = false;               // Flag to indicate if the axis is a gate axis (GTX, GTY, GTZ, GRX, GRY, GRZ)
        bool isMF = false;               // Flag to indicate if the axis is a ModFunc axis (MTX, MTY, MTZ, MRX, MRY, MRZ)
        bool isINV = false;              // Flag to indicate it the axis is an Inversion axis (ITX, ITY, ITZ, IRX, IRY, IRZ) - Direction is ignored.
        uint8_t idx_pos_neg = 2;         // Index for the positive/negative command
        int8_t pos_neg = 0;              // Default to 0 (no positive/negative command)
        char axisName[3] = {0, 0, '\0'}; // Array to hold the axis names

        if (words[1][0] == 'G') {
            // Gate axis
            isGT = true;     // Set the GT flag
            idx_pos_neg = 3; // Set the index for the positive/negative command to 3
        } else if (words[1][0] == 'M') {
            // Modulation function axis
            isMF = true;     // Set the MF flag
            idx_pos_neg = 3; // Set the index for the positive/negative command to 3
        } else if (words[1][0] == 'I') {
            // Inversion axis
            isINV = true;    // Set the INV flag
            idx_pos_neg = 0; // Defacto ignore the pos/neg indicator, by pointing to the first character of the word.

        } else {
            // Assume it's a normal axis (TX, TY, TZ, RX, RY, RZ)
            idx_pos_neg = 2; // Set the index for the positive/negative command to 2
        }

        // Retrieve the axis name from the second word
        if (isGT || isMF || isINV) {
            axisName[0] = words[1][1]; // Skip the first character (G or M)
            axisName[1] = words[1][2]; // Skip the first character (G or M)
        } else {
            axisName[0] = words[1][0]; // Use the first character of the word (T or R)
            axisName[1] = words[1][1]; // Use the second character of the word (T or R)
        }

        // Use the fourth or third character to determine the direction (positive/negative)
        if (words[1][idx_pos_neg] == '+') {
            pos_neg = 1; // Positive
        } else if (words[1][idx_pos_neg] == '-') {
            pos_neg = -1; // Negative
        }

        // Now we can set the sensitivity for the specified axis
        Serial.print(F("Config "));
        Serial.print(words[1]);
        Serial.print(F(" -> "));
        Serial.println(value);

        _SMKIN->UpdateAxisConfig(axisName, isGT, isMF, isINV, pos_neg, value); // Call the function to set the sensitivity
        _SMKIN->PrintAxisConfigurations();                                     // Print the sensitivities after setting them
    } else if (strcmp_P(words[0], CMD_MINMAX) == 0) {
        // We should have an axisname, a sign indicating min or max and a value in the third word.
        _SMHW->UpdateMinMax(words[1], value); // Call the function to set the min/max values
        _SMHW->PrintMinMax();                 // Print the min/max values after setting them

    } else {
        return -1; // Unknown command
    }
    return 0; // Success
}