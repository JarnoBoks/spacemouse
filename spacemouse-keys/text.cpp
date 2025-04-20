#include "text.h"

bool isWordEmpty(const char *str, const char *errorMsg) {
    // Check if the string is empty or null
    if (str == NULL || *str == '\0') {
        Serial.print(CF(Error));
        Serial.println(CF(errorMsg));
        return true; // String is empty or null
    }

    return false; // String is not empty or null
}

// REVIEW - Test the conversion functions. They are both written with the usage of the strtod function, for code size purposes.
//  strtod is already used by somewhere else and to preserve space in the compiled code we do not use strtol or atof.
bool convertWordNumber(const char *str, long *n, const char *errorMsg) {
    // Check if the string is a number (integer or float)
    char *endptr;
    //*n = strtol(str, &endptr, 10); // Convert to long integer
    *n = (long)strtod(str, &endptr); // Convert to long integer
    if (*endptr != '\0') {
        Serial.print(CF(Error));
        Serial.println(CF(errorMsg));
        return false; // Not a valid number
    }
    return true; // Valid number
}

#if 0
bool convertWordFloat(const char *str, float *const &f, const char *errorMsg) {
    // Check if the string is a float
    *f = float(atof(str)); // Convert to float
    if (*f == 0.0 && str[0] != '0') {
        Serial.print(CF(Error));
        Serial.println(CF(errorMsg));
        return false; // Not a valid float
    }
    return true; // Valid float
}
#endif

bool convertWordFloat(const char *str, float *const &f, const char *errorMsg) {
    // Check if the string is a number (integer or float)
    char *endptr;
    *f = float(strtod(str, &endptr)); // Convert to long integer
    if (*endptr != '\0') {
        Serial.print(CF(Error));
        Serial.println(CF(errorMsg));
        return false; // Not a valid number
    }
    return true; // Valid number
}

void alignValue(const int value, const uint8_t width) {
    int8_t spaces = width - 1;
    if (value < 0) {
        spaces--; // Remove a space to compensate for the sign
    }
    if (abs(value) > 99) {
        spaces--; // Remove a space to compensatie for the 3rd
    }
    if (abs(value) > 9) {
        spaces--; // Remove a space to compensatie for the 1st digit
    }
    for (int8_t i = 0; i < spaces; i++) {
        Serial.print(F(" ")); // Add spaces to align the output
    }
}