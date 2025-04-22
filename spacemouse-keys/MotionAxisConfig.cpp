#include <Arduino.h>
#include <EEPROM.h>
#include "MotionAxisConfig.h"
#include "eepromStorage.h" // Include the EEPROM address map
#include "text.h"          // Include the text library for printing text to the serial monitor

// Include math operators for doing better calculation algorithms. Arduino math is a standard library already included.
#include <math.h>
#define sign(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0)) // Define Signum Function

/**
 * @brief Constructor for MotionAxisConfig class.
 * @details Create the object & technically initialise members.Load the configuration data from the EEPROM.
 * @param Name The base name of the axis (e.g. TX, TY, TZ, RX, RY, RZ)
 * @param EEPROMAddress The address in the EEPROM where the axis configuration is stored
 *
 */
MotionAxisConfig::MotionAxisConfig(const char *Name, int EEPROMAddress) : _name{Name}, _EEPROMAddress(EEPROMAddress) {
    // Load the configuration from the EEProm storage
    EEPROM.get(EEPROMAddress, _config);
}

/**
 * @brief Constructor for MotionAxisConfig class with default configuration.
 * @details Create the object & technically initialise members. Update the default configuration data in the EEPROM.
 * @param Name The base name of the axis (e.g. TX, TY, TZ, RX, RY, RZ)
 * @param EEPROMAddress The address in the EEPROM where the axis configuration is stored
 * @param config The default configuration for the velocity axis
 */
MotionAxisConfig::MotionAxisConfig(const char *Name, int EEPROMAddress, VelocityConfig_t config) {
    // Constructor with configuration
    _name = Name;
    _EEPROMAddress = EEPROMAddress;
    _config = config;
    EEPROM.put(EEPROMAddress, _config); // Save the new config to the EEPROM
}

/**
 * @brief Destructor for MotionAxisConfig class.
 */
MotionAxisConfig::~MotionAxisConfig() {
    // Destructor
}

/**
 * @brief Calculates the velocity based on the current sensor readings.
 * @details This function calculates the velocity based on the current position and the sensitivity.
 *          It applies the sensitivity configured for this axis (&direction).
 *          After that it applies the configured modifier function for this axis, applies any gate for this axis.
 *          Finally it inverts the motion if necessary.
 * @param sensorInput Value that is read from the sensor (e.g. joystick, knob, etc.). The value is centered and the deadzone is applied.
 */
void MotionAxisConfig::CalculcateVelocity(int16_t sensorInput) {
    // Calculate the velocity based on the current position and the sensitivity
    // This is a placeholder function, the actual implementation will depend on the specific hardware and requirements
    float sens;
    int8_t gate;

    // If the velocity is negative, use the negative sensitivity and gate values
    // Otherwise, use the positive sensitivity and gate values
    if (sensorInput < 0) {
        sens = _config.neg_sensitivity;
        gate = _config.neg_gate;
    } else {
        sens = _config.pos_sensitivity;
        gate = _config.pos_gate;
    }

    // apply the sensitivity for this axis & direction
    _value = sensorInput / sens;

    // Apply the modifier function for this axis, override the default one if necessary
    _modifierFunction();

    // Apply any gate for this axis.
    if (abs(_value) < gate) {
        _value = 0;
    }

    // Invert the motion if necessary
    _value = (_config.invert == 1) ? -_value : _value; // Invert the value if necessary
}

/**
 * @brief Prints the velocity value for this axis.
 * @details This function prints the velocity value for this axis to the serial port.
 *          The output format is: "TX: 0346" or "TY: -0125"
 */
void MotionAxisConfig::PrintVelocity() {
    Serial.print(_name);   // Print the axis name (e.g. TX, TY, TZ, RX, RY, RZ)
    Serial.print(F(": ")); // Add a space between the axis name and the value
    alignValue(_value, 4); // Align the value to the right with spaces
    Serial.print(_value);  // Print the velocity value (value is never wider than 4 positions)
    Serial.print(F(" "));  // Add a space after the value
}

#if 0
boolean MotionAxisConfig::isAxis(const char *Name) {
    return (strcmp(Name, _name) == 0);
}
#endif

/**
 * @brief Updates the configuration for the specified axis.
 * @details The function checks if the axis name matches the current axis.
 * If it does, it updates the specified configuration parameter (gate, modifier function, or sensitivity) based on the provided value and direction.
 * @param axisName The name of the axis to update.
 * @param isGT Indicates if the gate configuration should be updated.
 * @param isMF Indicates if the modifier function should be updated.
 * @param isInversion Indicates if the inversion setting should be updated.
 * @param pos_neg Indicates the direction for the update (1 for positive, -1 for negative).
 * @param value The new value to set for the specified configuration.
 * @return Flag indicating success or failure of the update.
 * @retval false Axis name does not match or update failed.
 * @retval true Update was successful.

 */
boolean MotionAxisConfig::UpdateConfig(const char *axisName, boolean isGT, boolean isMF, boolean isInversion, int8_t pos_neg, float value) {
    // Check if we have to process this axis
    if ((strcmp(axisName, _name) != 0)) {
        return false; // Axis name does not match, return false
    }

    // Check which configuration parameter has to be updated (gate, modfunc or sensitivity)
    if (isGT) {
        // We want to update the gate configuration, now check if a direction is set.
        switch (pos_neg) {
        case 1:
            _config.pos_gate = value;
            break;
        case -1:
            _config.neg_gate = value;
            break;
        default:
            // If no specific direction is entered, set both gate directions to the same value
            _config.pos_gate = _config.neg_gate = (int8_t)value;
            break;
        }
    } else if (isMF) {
        // We want to update the modfunc configuration, now check if a direction is set.
        switch (pos_neg) {
        case 1:
            _config.pos_modfunc = (int8_t)value;
            break;
        case -1:
            _config.neg_modfunc = (int8_t)value;
            break;
        default:
            // If no specific direction is entered, set both modfunc directions to the same value
            _config.pos_modfunc = _config.neg_modfunc = (int8_t)value;
            break;
        }
    } else {
        // We don't want to update a gate or a modfunc: update the sensitivity value
        switch (pos_neg) {
        case 1:
            _config.pos_sensitivity = value;
            break;
        case -1:
            _config.neg_sensitivity = value;
            break;
        default:
            _config.pos_sensitivity = _config.neg_sensitivity = value;
            break;
        }
    }

    EEPROM.put(_EEPROMAddress, _config); // Save the new config to the EEPROM

    return true; // Success
}

#define FMT_NOCOMMA false
#define FMT_COMMA true
#define NO_PREFIX ""
#define NO_SIGN " "
#define TWO_DECIMALS 2
#define ONE_DECIMAL 1
#define NO_DECIMALS 0

/**
 * @brief
 * @param separator
 * @param typeprefix
 * @param name
 * @param sign
 * @return size_t The number of characters printed.
 */
size_t _helper_PrintItem(const char *separator, const char *typeprefix, const char *name, const char *sign) {
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
void MotionAxisConfig::_helper_PrintConfig(const float posval, const float negval, const boolean printseparator, const char *typeprefix, const uint8_t precision, const int8_t minwidth) {
    size_t nc = 0;                                        // Number of characters printed
    const char *separator = (printseparator) ? "| " : ""; // Separator for the output

    //  Proces the positive direction
    if (posval == negval) {
        nc += _helper_PrintItem(separator, typeprefix, _name, NO_SIGN); // No comma needed
    } else {
        // Otherwise we print the name, without a prefix, but with a "+" postfix
        nc += _helper_PrintItem(separator, typeprefix, _name, "+"); // No comma needed
    }
    nc += Serial.print(posval, precision);

    // If the value for the positive direction does not equal the value for the negative direction, the negative direction value is printed too.
    if (posval != negval) {
        // Printing the negative value always gets preceded by a comma.
        nc += _helper_PrintItem(", ", typeprefix, _name, "-");
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

/**
 * @brief Outputs the configuration of the axis to the Serial monitor.
 * @details Prints the configuration for the axis to the Serial monitor.
 *          The output format is: "TX: 0346 | G: 0 | M: 0 | I: 0"
 */
void MotionAxisConfig::PrintConfig() {

    // Print the sensitivity
    _helper_PrintConfig(_config.pos_sensitivity, _config.neg_sensitivity, FMT_NOCOMMA, NO_PREFIX, TWO_DECIMALS, 21);

    // Print the gate
    _helper_PrintConfig(_config.pos_gate, _config.neg_gate, FMT_COMMA, "G", NO_DECIMALS, 21);

    // Print the modfunc
    _helper_PrintConfig(_config.pos_modfunc, _config.neg_modfunc, FMT_COMMA, "M", NO_DECIMALS, 19);

    // Print the inversion
    _helper_PrintConfig(_config.invert, _config.invert, FMT_COMMA, "I", NO_DECIMALS, -1);
}

/**
 * @brief Modifies the _value for this axis by applying a mathematical mode. Choose the mathematical function in config.h as modFunc.
 * @details The modifier function is used to modify the output of the axis based on the configured function.
 *        The function is applied to the velocity value before it is sent to the host.
 *          The available functions are:
 *          0: linear y = x [Standard behaviour: No modification]
 *          1: squared function y = x^2*sign(x) [altered squared function working in positive and negative direction]
 *          2: tangent function: y = tan(x) [Results in a linear curve near zero but increases the more you are away from zero]
 *          3: squared tangent function: y = tan(x^2*sign(X)) [Results in a flatter curve near zero but increases a lot the more you are away from zero]
 *          4: cubed tangent function: y = tan(x^3) [Results in a very flat curve near zero but increases drastically the more you are away from zero]
 * @result The private member '_value' is modified using the configured modifier function for this axis.
 */
void MotionAxisConfig::_modifierFunction() {
    // making sure function input never exceeds range of -350 to 350
    _value = constrain(_value, -350, 350);
    uint8_t mfunc;

    mfunc = (_value < 0) ? _config.neg_modfunc : _config.pos_modfunc; // Get the modifier function from the class variable
    switch (mfunc) {
    case 1:
        // using squared function y = x^2*sign(x)
        _value = 350 * pow(_value / 350.0, 2) * sign(_value); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
        break;
    case 2:
        // using tan function: tan(x)
        _value = 350 * tan(_value / 350.0);
        break;
    case 3:
        // using squared tan function: tan(x^2*sign(x))
        _value = 350 * tan(pow(_value / 350.0, 2) * sign(_value)); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
        break;
    case 4:
        // using cubed tan function: tan(x^3)
        _value = 350 * tan(pow(_value / 350.0, 3));
        break;
    default:
        // using linear function: y = x
        // nothing to do here, just use the value as it is
        break;
    }

    // make sure values between-350 and 350 are allowed
    _value = constrain(_value, -350, 350);
}

// ---------------------- GETTERS AND SETTERS ----------------------

/**
 * @brief Retrieves the velocity value for this axis.
 * @return The velocity value for this axis.
 */
int16_t MotionAxisConfig::GetVelocity() const {
    return _value;
}

/**
 * @brief Sets the velocity value for this axis.
 * @param value The velocity value to set.
 */
void MotionAxisConfig::SetVelocity(int16_t value) {
    _value = value;
}

/**
 * @brief Sets the modifier function for this axis.
 * @param modFunc The new modifier function to set for this axis (0-4).
 */
void MotionAxisConfig::SetModFunc(uint8_t modFunc) {
    if (modFunc >= 0 && modFunc <= 4) {
        _config.pos_modfunc = _config.neg_modfunc = modFunc;
    }
}

/**
 * @brief Retrieves the inversion value for this axis.
 * @return The current inversion value for this axis.
 * @retval 0 = no inversion, 1 = inversion
 */
int8_t MotionAxisConfig::GetInvert() {
    return _config.invert;
}

/**
 * @brief Sets the inversion value for this axis.
 * @param invert The new inversion value to set for this axis (0 = no inversion, 1 = inversion).
 */
void MotionAxisConfig::SetInvert(int8_t invert) {
    if (invert == 0 || invert == 1) {
        _config.invert = invert; // Set the inversion value for this axis
    }
}