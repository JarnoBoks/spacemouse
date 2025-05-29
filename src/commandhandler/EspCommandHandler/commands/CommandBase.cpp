#include "CommandBase.hpp"
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
 * @brief Converts a string to a boolean.
 * @details The function checks if the first character of the string is '0' or '1'.
 *          If the first character is '0', it sets the boolean to false, otherwise it sets it to true.
 * @param str The string to convert.
 * @param n Pointer to the bool to store the result.
 * @return The result of the conversion.
 * @retval true Conversion successful
 * @retval false Conversion failed
 */
const bool CommandBase::convertWordBool(const char *str, bool *n) const {
    if (str == nullptr || *str == '\0') {
        return false; // If the string is null or empty, conversion fails
    }

    if (*str == '0') {
        *n = false; // If the first character is '0', set boolean to false
    } else if (*str == '1') {
        *n = true; // If the first character is '1', set boolean to true
    } else {
        return false; // If the first character is not '0' or '1', conversion fails
    }

    return true; // Conversion successful
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