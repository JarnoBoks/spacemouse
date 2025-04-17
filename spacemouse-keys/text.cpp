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

bool convertWordNumber(const char *str, long *n, const char *errorMsg) {
    // Check if the string is a number (integer or float)
    char *endptr;
    *n = strtol(str, &endptr, 10); // Convert to long integer
    if (*endptr != '\0') {
        Serial.print(CF(Error));
        Serial.println(CF(errorMsg));
        return false; // Not a valid number
    }
    return true; // Valid number
}

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
