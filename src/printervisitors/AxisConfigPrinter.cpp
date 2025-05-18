#include "AxisConfigPrinter.h"

/**
 * @brief
 * @param separator
 * @param typeprefix
 * @param name
 * @param sign
 * @return size_t The number of characters printed.
 */
size_t AxisConfigPrinter::_helper_PrintItem(const char *separator, const char *typeprefix, const char *name, const char *sign) {
    //  size_t len = strlen(name); // Get the length of the axis name
    size_t nc = 0;

    nc += Serial.print(separator);  // Print the separator (pipe, comma or space) before the axis name
    nc += Serial.print(typeprefix); // Add the typeprefix (e.g. G, M, IF) if needed
    nc += Serial.print(name);       // Add the axis name
    nc += Serial.print(sign);       // Add the sign
    nc += Serial.print(F(": "));

    return nc; // Return the number of characters printed
}

/**
 * @brief Prints each configuration setting for the axis.
 * @param posval The positive value to print.
 * @param negval The negative value to print.
 * @param format The format string for printing.
 * @param typeprefix The typeprefix to use in the output (ie. none, G, M, IF).
 * @param precision The number of decimal places to print.
 * @param minwidth The minimum width of the output.
 * @param printseparator Flag to indicate if a separator should be printed.
 */
// TODO - Change order of the parameters to make it more readable.
void AxisConfigPrinter::_helper_PrintConfig(const char *name, const float posval, const float negval, const boolean printseparator, const char *typeprefix, const uint8_t precision, const int8_t minwidth) {
    size_t nc = 0;                                        // Number of characters printed
    const char *separator = (printseparator) ? "| " : ""; // Separator for the output

    //  Proces the positive direction
    if (posval == negval) {
        nc += _helper_PrintItem(separator, typeprefix, name, NO_SIGN); // No comma needed
    } else {
        // Otherwise we print the name, without a prefix, but with a "+" postfix
        nc += _helper_PrintItem(separator, typeprefix, name, "+"); // No comma needed
    }
    nc += Serial.print(posval, precision);

    // If the value for the positive direction does not equal the value for the negative direction, the negative direction value is printed too.
    if (posval != negval) {
        // Printing the negative value always gets preceded by a comma.
        nc += _helper_PrintItem(", ", typeprefix, name, "-");
        // Print the negative value
        nc += Serial.print(negval, precision);
    }

    // If the values have to be aligned, add spaces to the output until the minimum width is reached.
    if (minwidth > 0) {
        while (nc < (uint8_t)minwidth) {
            Serial.print(F(" "));
            nc++;
        }
    }
}
