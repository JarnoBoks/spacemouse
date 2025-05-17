#include "CommandBase.h"
#include <Arduino.h>

/**
 * @brief Checks if a word is empty.
 * @param str The word to check.
 * @param errorMsg The error message to display if the word is empty.
 * @return True if the word is empty, false otherwise.
 */
const bool CommandBase::isWordEmpty(const char *str, const char *errorMsg) const {
    // Check if the string is empty or null
    return (!str || *str == '\0');
}

/**
 * @brief Converts a string to a long integer.
 * @param str The string to convert.
 * @param n Pointer to the long integer to store the result.
 * @return The result of the conversion.
 * @retval true Conversion successful
 * @retval false Conversion failed
 * @note The function uses strtod to convert the string to a long integer for code size purposes.
 *       The strtod function is already used somewhere else in the code and to preserve space in the compiled code
 *       strtol or atof is not used.
 */
const bool CommandBase::convertWordNumber(const char *str, long *n) const {
    // Check if the string is a number (integer or float)
    char *endptr = nullptr;
    *n = (long)strtod(str, &endptr); // Convert to long integer

    return (*endptr == '\0'); // Check if the conversion was successful
}

/**
 * @brief Converts a string to a float.
 * @param str The string to convert.
 * @param value Pointer to the float to store the result.
 * @return The result of the conversion.
 * @retval true Conversion successful
 * @retval false Conversion failed
 * @note The function uses strtod to convert the string to a long integer for code size purposes.
 *       The strtod function is already used somewhere else in the code and to preserve space in the compiled code
 *       strtol or atof is not used.
 */
const bool CommandBase::convertWordFloat(const char *str, float *value) const {

    // Check if the string is a number (integer or float)
    char *endptr = nullptr;
    *value = float(strtod(str, &endptr)); // Convert to float

    return (*endptr == '\0'); // Check if the conversion was successful
}