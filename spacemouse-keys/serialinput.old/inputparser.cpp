#include "inputparser.h"
#include "serialoutput/text.h"

/**
 * @brief  Handle the input from the serial monitor. This function reads the input from the serial monitor and processes it.
 * @details The function reads the input until a newline character or the buffer limit is reached. It then tokenizes the input and handles the command based on the number of received words.
 */
void InputParser::Parse() {
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
 * @return 1 if the command was handled successfully, -1 if the command was unknown.
 * @retval 1 Success
 * @retval -1 Command unknown
 */
int8_t InputParser::_handleOneWord(char *words[]) {
    int8_t ret = 1; // Default return value for success

    // --------------- SHOW ----------------------------------------------------
    if (strcmp_P(words[0], CMD_SHOW) == 0) {

        /*         EEPROMStorage::printVersion(); // Show the version number of the EEPROM storage
                Serial.println(F("\nHARDWARE CONFIGURATION:"));
                _SMHW->PrintDeadzone();
                _SMHW->PrintMinMax();

                // Show all stored calibration values
                _SMKIN->PrintAxisConfigurations();
                _SMKIN->PrintSwitchYZ();      // Print the switch YZ flag
                _SMKIN->PrintExclusiveMode(); // Print the exclusive mode flag
         */
        // --------------- IDLE ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_IDLE) == 0) {
        // Tune Idle position & suggest deadzone value
        /*         _SMHW->BusyZeroing(2000, true); // Call the function to calibrate the idle position */

        // --------------- MINMAX ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_MINMAX) == 0) {
        // Calibrate Idle position
        /*         _SMHW->CalibrateMinMax(); // Call the function to start the min/max calibration without storing the values in EEPROM */

        // --------------- SENS ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_SENS) == 0) {
        /*         _SMKIN->PrintAxisConfigurations(); // Print the current sensitivities */

        // --------------- DZ ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_DEADZONE) == 0) {
        /*         _SMHW->PrintDeadzone(); */

        // --------------- SWITCHYZ ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_SWITCHYZ) == 0) {
        /*         _SMKIN->PrintSwitchYZ(); */

        // --------------- EXCL ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_EXCLUSIVEMODE) == 0) {
        /*         _SMKIN->PrintExclusiveMode(); */

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
 * @return The status of the requested command.
 * @retval 1 Success
 * @retval -1 Unknown command
 * @retval -2 No command given
 * @retval -3 No parameter given
 * @retval -4 Second parameter is not a number
 */
int8_t InputParser::_handleTwoWords(char *words[]) {
    int8_t ret = 1; // Default return value for success

    // When we have two words, the first word is the command and the second word is the parameter.
    // We have to check if both words are supplied and if the second word is a number or a string.
    if (isWordEmpty(words[0], Error_EmptyCommand)) {
        return -2; // No command given
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

        if (value == -1) {
            _debug = DebugLevel_t::DEBUG_OFF; // Set the debug level to off
        } else if (value == 0) {
            _debug = DebugLevel_t::DEBUG_SILENT; // Set the debug level to off
        } else if (value == 1) {
            _debug = DebugLevel_t::DEBUG_1; // Set the debug level to 1
        } else if (value == 2) {
            _debug = DebugLevel_t::DEBUG_2; // Set the debug level to 2
        } else if (value == 3) {
            _debug = DebugLevel_t::DEBUG_3; // Set the debug level to 3
        } else if (value == 4) {
            _debug = DebugLevel_t::DEBUG_4; // Set the debug level to 4
        } else if (value == 5) {
            _debug = DebugLevel_t::DEBUG_5; // Set the debug level to 5
        } else if (value == 6) {
            _debug = DebugLevel_t::DEBUG_6; // Set the debug level to 6
        } else if (value == 7) {
            _debug = DebugLevel_t::DEBUG_7; // Set the debug level to 7
        } else if (value == 8) {
            _debug = DebugLevel_t::DEBUG_8; // Set the debug level to 8
        } else if (value == 9) {
            _debug = DebugLevel_t::DEBUG_9; // Set the debug level to 9
        } else if (value == 10) {
            _debug = DebugLevel_t::DEBUG_10; // Set the debug level to 10
        } else if (value == 11) {
            _debug = DebugLevel_t::DEBUG_11;             // Set the debug level to 11
        } else {                                         // Check for valid range of values for debugging levels.
            Serial.println(CF(Error_ParameterNoNumber)); // Print error message if the value is out of range
            return -1;                                   // Return error code
        }

        _debug = (DebugLevel_t)value; // Set the debug level

        // Inform the hardware of the debuglevel update.
        _SMHW->SetAnalogReferenceVoltage(_debug);

        // Disable the modifier function & inversion if the debug level is 4 or 5.
        if (_debug == 4) {
            _SMKIN->SetIgnoreArithmetics(false); // Set the ignore modifier function and inversion flag
        } else {
            _SMKIN->SetIgnoreArithmetics(true); // Set the ignore modifier function and inversion flag
        }

        // --------------- MODFUNC x ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_MODFUNC) == 0) {
#if 0
        _SMKIN->SetModulationFunction(value); // Call the function to set the modulation function
#endif

        // --------------- DEADZONE x ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_DEADZONE) == 0) {
        /*         _SMHW->UpdateDeadzone(value);   // Call the function to set the deadzone
                Serial.print(CF(Info_Updated)); // Print message to the serial monitor
                _SMHW->PrintDeadzone();         // Print the deadzone value
         */
        // --------------- MINMAX 1 ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_MINMAX) == 0) {
        /*         // Call the function to start the min/max calibration and store the values in EEPROM if value is 1, otherwise do not store the values in EEPROM
                _SMHW->CalibrateMinMax((value == 1));

         */
        // --------------- SWITCHYZ 1|0 ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_SWITCHYZ) == 0) {
        /*         _SMKIN->UpdateSwitchYZ((value == 1)); // Call the function to set the switch YZ
                Serial.print(CF(Info_Updated));       // Print message to the serial monitor
                _SMKIN->PrintSwitchYZ();              // Print the switch YZ value
         */
        // --------------- SENS 99999 ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_SENS) == 0 && value == 99999) {
        /*         _SMKIN->RestoreDefaultAxisConfigurations(); // Call the function to restore the default axis configurations
                Serial.print(CF(Info_Updated));             // Print message to the serial monitor
                _SMKIN->PrintAxisConfigurations();          // Print the axis configurations to the serial monitor
         */
        // --------------- EXCL 1|0 ----------------------------------------------------
    } else if (strcmp_P(words[0], CMD_EXCLUSIVEMODE) == 0) {
        /*         _SMKIN->UpdateExclusiveMode((value == 1)); // Call the function to set the exclusive mode flag
                Serial.print(CF(Info_Updated));            // Print message to the serial monitor
                _SMKIN->PrintExclusiveMode();              // Print the exclusive mode value
         */
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
 * @return The status of the requested command.
 * @retval 1 Success
 * @retval -1 Unknown command
 * @retval -2 No command given
 * @retval -3 No parameter given
 * @retval -4 No value given
 * @retval -5 Third parameter is not a float
 *
 */
int8_t InputParser::_handleThreeWords(char *words[]) {
    int8_t ret = 1; // Default return value for success

    // Handle three words command. The first word is the command, the second word is the axis, and the third word is the value.
    // We have to check if all three words are supplied and if the third word is a float.
    if (isWordEmpty(words[0], Error_EmptyCommand)) {
        return -2; // No command given
    }
    if (isWordEmpty(words[1], Error_EmptyParameter)) {
        return -3; // No parameter given
    }
    if (isWordEmpty(words[2], Error_EmptyValue)) {
        return -4; // No value given
    }

    float value; // Default value for the third word
    if (!convertWordFloat(words[2], &value, Error_ParameterNoFloat)) {
        return -5; // Third parameter is not a float
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

        /*         _SMKIN->UpdateAxisConfiguration(axisName, isGT, isMF, isINV, pos_neg, value); // Call the function to set the sensitivity
                _SMKIN->PrintAxisConfigurations();                                            // Print the sensitivities after setting them
         */
    } else if (strcmp_P(words[0], CMD_MINMAX) == 0) {
        // We should have an axisname, a sign indicating min or max and a value in the third word.
        /*         _SMHW->UpdateMinMax(words[1], value); // Call the function to set the min/max values
                _SMHW->PrintMinMax();                 // Print the min/max values after setting them
         */
    } else {
        return -1; // Unknown command
    }
    return ret; // Success
}