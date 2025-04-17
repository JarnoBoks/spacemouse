// File for calibration specific functions

#include <Arduino.h>
#include "calibration.h"
#include "kinematics.h"
#include "config.h"
#include "hardware/SpaceMouseHW.h"

/// @brief Hold characters to plot them
char debugOutputBuffer[20];

/**
 * @brief Prints the current status of the key to the serial interface
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

/**
 * @brief Prints the raw ADC 10-bit values, if the output is due (every x miliseconds)
 * @param rawReads  pointer to the int array where the raw ADC values are stored
 * @param keyVals   pointer to the int array where the raw key readings are stored (no debouncing)
 */
void debugOutput1(SpaceMouseHW_ &SMHW, int *keyVals) {
    if (isDebugOutputDue()) {
        SMHW.PrintRawReads();
        // Report back 0-1023 raw ADC 10-bit values if enabled
        printKeys(keyVals);
        Serial.print(DEBUG_LINE_END);
    }
}

void debugOutput2(SpaceMouseHW_ &SMHW) {
    if (isDebugOutputDue()) {
        SMHW.PrintCentered();
        Serial.print(DEBUG_LINE_END);
    }
}

/**
 * @brief Report translation & rotation values, configured sensititivity parameters and status of the mouse buttons.
 * @param SMKIN Pointer to the Kinematics object containing the current translation & rotation values aswell as the sensitivity configuration.
 * @param keyOut Pointer to the array containing the status of the mousebuttons
 */
void debugOutput4(Kinematics &SMKIN, uint8_t *keyOut) {
    SMKIN.PrintVelocities();
    printKeys(keyOut);
    Serial.print(F(" || "));
    SMKIN.PrintSensitivities();
    Serial.print(DEBUG_LINE_END);
}

/**
 * @brief Report single axis and  translation & rotation values side by side for direct reference. Very useful if you need to alter which inputs are used in the arithmetic above.
 *
 * @param SMHW Pointer to the Hardware object for the eight axis
 * @param SMKIN Pointer to the Kinematics object for the six resulting translation & rotation values.
 */
void debugOutput5(SpaceMouseHW_ &SMHW, Kinematics &SMKIN) {
    if (isDebugOutputDue()) {
        SMHW.PrintCentered();
        Serial.print(" || ");
        SMKIN.PrintVelocities();
        Serial.print(DEBUG_LINE_END);
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

/**
 * @brief
 *
 * @param dbg
 * @param SMKIN
 * @param SMHW

 */

#define CF(x) ((const __FlashStringHelper *)x)
/// @brief Command list for the serial interface. The commands are stored in program memory to save RAM space.
/// @details The commands are stored in program memory to save RAM space. The commands are compared with the input string using strcmp_P.
static const char CMD_DEBUG[] PROGMEM = "DEBUG";
static const char CMD_INVERT[] PROGMEM = "INVERT";
static const char CMD_MODFUNC[] PROGMEM = "MODFUNC";
static const char CMD_SENS[] PROGMEM = "SENS";
static const char CMD_HELP[] PROGMEM = "HELP";
static const char RESP_UNKNOWN_CMD[] PROGMEM = "Unknown command. Type HELP for a list of commands.";
static const char RESP_HELP[] PROGMEM = "Commands: DEBUG, INVERT, MODFUNC, SENS, HELP";

static const char TX[] PROGMEM = "TX";
static const char TY[] PROGMEM = "TY";
static const char TZ[] PROGMEM = "TZ";
static const char RX[] PROGMEM = "RX";
static const char RY[] PROGMEM = "RY";
static const char RZ[] PROGMEM = "RZ";
static const char GTX[] PROGMEM = "GTX";
static const char GTY[] PROGMEM = "GTY";
static const char GTZ[] PROGMEM = "GTZ";
static const char GRX[] PROGMEM = "GRX";
static const char GRY[] PROGMEM = "GRY";
static const char GRZ[] PROGMEM = "GRZ";
static const char MTX[] PROGMEM = "MTX";
static const char MTY[] PROGMEM = "MTY";
static const char MTZ[] PROGMEM = "MTZ";
static const char MRX[] PROGMEM = "MRX";
static const char MRY[] PROGMEM = "MRY";
static const char MRZ[] PROGMEM = "MRZ";
// INVERT: 0 = TX, 1 = TY, 2 = TZ, 3 = RX, 4 = RY, 5 = RZ
// MODFUNC: 0 = TX, 1 = TY, 2 = TZ, 3 = RX, 4 = RY, 5 = RZ

void debugInput(int &dbg, Kinematics &SMKIN, SpaceMouseHW_ &SMHW) {
    char inputBuffer[32]; // Buffer to store the input command
    int bytesRead = 0;

    // Read the input into the buffer until a newline character or buffer limit
    while (Serial.available() > 0 && bytesRead < sizeof(inputBuffer) - 1) {
        char c = Serial.read();
        if (c == '\n') {
            break; // Stop reading at newline
        }
        inputBuffer[bytesRead++] = c;
    }
    inputBuffer[bytesRead] = '\0'; // Null-terminate the string

    if (bytesRead > 0) {
        char word1[9] = ""; // Buffer for the first word (8 characters + null terminator)
        char word2[9] = ""; // Buffer for the second word (optional) (8 characters + null terminator)
        float value = 0.0;  // Float value

        // Try parsing two words and a float
        int parsedItems = sscanf(inputBuffer, "%8s %8s %f", word1, word2, &value);

        if (parsedItems == 3) {
            // Input contains two words and a float
            Serial.println(F("Parsed input:"));
            Serial.print(CF("Command: "));
            Serial.println(word1);
            Serial.print(F("Item: "));
            Serial.println(word2);
            Serial.print(F("Value: "));
            Serial.println(value, 6); // Print float with 6 decimal places

            // Handle the input
            handleTwoWordsAndFloat(word1, word2, value, SMKIN);
        } else if (parsedItems == 2) {
            // Input contains one word and a float
            Serial.println(F("Parsed input:"));
            Serial.print(F("Command: "));
            Serial.println(word1);
            Serial.print(F("Value: "));
            Serial.println(value, 6); // Print float with 6 decimal places

            // Handle the input
            handleOneWordAndFloat(word1, value);
        } else {
            Serial.println(F("Invalid input format. Use: <word> <word> <float> or <word> <float>"));
        }
    }
}

bool checkForAxis(const char *word, velocityAxis_t &axis, bool &isGT, bool &isMF) {
    // Check if the word matches any of the defined axis names
    // and set the axis variable accordingly
    if (strcmp_P(word, TX) == 0) {
        axis = transX;
    } else if (strcmp_P(word, TY) == 0) {
        axis = transY;
    } else if (strcmp_P(word, TZ) == 0) {
        axis = transZ;
    } else if (strcmp_P(word, RX) == 0) {
        axis = rotX;
    } else if (strcmp_P(word, RY) == 0) {
        axis = rotY;
    } else if (strcmp_P(word, RZ) == 0) {
        axis = rotZ;
    } else if (strcmp_P(word, GTX) == 0) {
        axis = transX;
        isGT = true;
    } else if (strcmp_P(word, GTY) == 0) {
        axis = transY;
        isGT = true;
    } else if (strcmp_P(word, GTZ) == 0) {
        axis = transZ;
        isGT = true;
    } else if (strcmp_P(word, GRX) == 0) {
        axis = rotX;
        isGT = true;
    } else if (strcmp_P(word, GRY) == 0) {
        axis = rotY;
        isGT = true;
    } else if (strcmp_P(word, GRZ) == 0) {
        axis = rotZ;
        isGT = true;
    } else if (strcasecmp_P(word, MTX) == 0) {
        axis = transX;
        isMF = true;
    } else if (strcasecmp_P(word, MTY) == 0) {
        axis = transY;
        isMF = true;
    } else if (strcasecmp_P(word, MTZ) == 0) {
        axis = transZ;
        isMF = true;
    } else if (strcasecmp_P(word, MRX) == 0) {
        axis = rotX;
        isMF = true;
    } else if (strcasecmp_P(word, MRY) == 0) {
        axis = rotY;
        isMF = true;
    } else if (strcasecmp_P(word, MRZ) == 0) {
        axis = rotZ;
        isMF = true;
    } else {

        Serial.println(F("Unknown axis."));
        return false; // Invalid axis
    }

    // If we reach here, the axis is valid
    return true;
}

void checkForPositiveNegative(const char *word, int8_t &value) {
    // Check if the third or the fourth character is a '+' or '-' sign
    // and set the value accordingly
    if (word[2] == '+' || word[2] == '-') {
        value = (word[2] == '+') ? 1 : -1; // Positive or negative
    } else if (word[3] == '+' || word[3] == '-') {
        value = (word[3] == '+') ? 1 : -1; // Positive or negative
    }
}

void handleTwoWordsAndFloat(const char *cmd, const char *item, float value, Kinematics &SMKIN) {
    // Example handling for two words and a float
    // This function can parse the commands "SENS" and "INVERT" with a float value and perform the corresponding actions.
    // For example, you can set the sensitivity or inversion for a specific axis.
    // The first word is the command, the second word is the axis, and the float value is the parameter.

    velocityAxis_t axis;
    bool isGT = false; // Flag to indicate if the item is a gate item (GTX, GTY, GTZ, GRX, GRY, GRZ)
    bool isMF = false; // Flag to indicate if the item is a ModFunc item (MTX, MTY, MTZ, MRX, MRY, MRZ)
    if (checkForAxis(item, axis, isGT, isMF)) {
        // Check if the command is for inversion or sensitivity function
        if (strcmp_P(cmd, CMD_SENS) == 0) {
            // Set sensitivity function for the specified axis
            Serial.print(F("Setting sensitivity for "));
            Serial.print(item);
            Serial.print(F(" to: "));
            Serial.println(value);

            int8_t pos_neg = 0;                      // Default to 0 (no positive/negative command)
            checkForPositiveNegative(item, pos_neg); // Check for '+' or '-' in the word2

            SMKIN.SetSensitivities(axis, isGT, isMF, pos_neg, value); // Set the sensitivity for the specified axis

        } else if (strcmp_P(item, CMD_INVERT) == 0 && !isGT && !isMF) {
            // Set inversion for the specified axis
            Serial.print(F("Setting inversion for "));
            Serial.print(cmd);
            Serial.print(F(" to: "));
            Serial.println(value);

            SMKIN.SetTransRotInversions(axis, (uint8_t)value);
        } else {
            Serial.println(CF(RESP_UNKNOWN_CMD));
        }
    } else {
        Serial.println(F("Unknown second parameter."));
    }
}

void handleOneWordAndFloat(const char *word, float value) {
    // Example handling for one word and a float
    if (strcmp(word, "DEBUG") == 0) {
        Serial.print("Setting debug level to: ");
        Serial.println(value);
    } else {
        Serial.println(CF("Unknown command"));
    }
}