#include "ICommand.h"
#include <Arduino.h>

#define CF(x) ((const __FlashStringHelper *)x)
static const char Error_CommandUnkown[] PROGMEM = "Unknown command or -format.";
static const char Error[] PROGMEM = "Err: Invalid ";
static const char Error_EmptyCommand[] PROGMEM = "command";
static const char Error_EmptyParameter[] PROGMEM = "parameter";
static const char Error_ParameterNoNumber[] PROGMEM = "integer";
static const char Error_ParameterNoFloat[] PROGMEM = "float";
static const char Error_EmptyValue[] PROGMEM = "value";

const bool ICommand::isWordEmpty(const char *str, const char *errorMsg) const {
    // Check if the string is empty or null
    if (!str || *str == '\0') {
        Serial.print(CF(Error));
        Serial.println(CF(errorMsg));
        return true; // String is empty or null
    }

    return false; // String is not empty or null
}

// NOTE - The number conversion functions are both written with the usage of the strtod function, for code size purposes.
//        strtod is already used by somewhere else and to preserve space in the compiled code we do not use strtol or atof.
const bool ICommand::convertWordNumber(const char *str, long *n) const {
    // Check if the string is a number (integer or float)
    char *endptr = nullptr;
    *n = (long)strtod(str, &endptr); // Convert to long integer

    if (*endptr != '\0') {
        // TODO: Check if compile size is smaller with the usage of F() macro
        Serial.print(CF(Error));
        Serial.print(CF(Error_ParameterNoNumber));
        return false; // Not a valid number
    }

    return true; // Valid number
}

const bool ICommand::convertWordFloat(const char *str, float *value) const {

    // Check if the string is a number (integer or float)
    char *endptr = nullptr;
    *value = float(strtod(str, &endptr)); // Convert to float

    if (*endptr != '\0') {
        // TODO: Check if compile size is smaller with the usage of F() macro
        Serial.print(CF(Error));
        Serial.println(CF(Error_ParameterNoFloat));
        return false; // Not a valid float
    }

    return true; // Valid float
}