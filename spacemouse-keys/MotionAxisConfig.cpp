#include <Arduino.h>
#include <EEPROM.h>
#include "MotionAxisConfig.h"
#include "eepromStorage.h" // Include the EEPROM address map

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
 * @brief Calculate the velocity based on the current sensor readings.
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
    _value *= _config.invert;
}

/**
 * @brief Get the velocity value for this axis.
 * @return The velocity value for this axis.
 */
int16_t MotionAxisConfig::GetVelocity() const {
    return _value;
}

/**
 * @brief Set the velocity value for this axis.
 * @param value The velocity value to set.
 */
void MotionAxisConfig::SetVelocity(int16_t value) {
    _value = value;
}

/**
 * @brief Print the velocity value for this axis.
 * @details This function prints the velocity value for this axis to the serial port.
 *          The output format is: "TX: 0346" or "TY: -0125"
 */
void MotionAxisConfig::PrintVelocity() {
    char debugOutputBuffer[20];
    sprintf_P(debugOutputBuffer, PSTR("%2.2s: %4d "), _name, _value);
    Serial.print(debugOutputBuffer);
}

#if 0
boolean MotionAxisConfig::isAxis(const char *Name) {
    return (strcmp(Name, _name) == 0);
}
#endif

/**
 * @brief Updates the configuration for the specified axis.
 * @details This function updates the configuration for the specified axis based on the provided parameters.
 * @param axisName The name of the axis to update.
 * @param isGT Indicates if the gate configuration should be updated.
 * @param isMF Indicates if the modulation function should be updated.
 * @param isInversion Indicates if the inversion setting should be updated.
 * @param pos_neg Indicates the direction for the update (1 for positive, -1 for negative).
 * @param value The new value to set for the specified configuration.
 * @return boolean indicating success or failure of the update.
 */
boolean MotionAxisConfig::UpdateConfig(const char *axisName, boolean isGT, boolean isMF, boolean isInversion, int8_t pos_neg, float value) {
    // Check if we have to process this axis
    if ((strcmp(axisName, _name) != 0)) {
        return 0; // Axis name does not match, return false
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

    return 1; // Success
}

int8_t MotionAxisConfig::GetInvert() {
    return _config.invert;
}

static const char FMT_NOCOMMA[] PROGMEM = "%s%2.2s%s: ";
static const char FMT_COMMA[] PROGMEM = ", %s%2.2s%s: ";
#define NO_PREFIX ""
#define NO_SIGN ""

/**
 * @brief Helper function to print the configuration values.
 * @param posval The positive value to print.
 * @param negval The negative value to print.
 * @param format The format string for printing.
 * @param prefix The prefix to use in the output (ie. none, G, M, IF).
 * @param precision The number of decimal places to print.
 */
void MotionAxisConfig::_helper_PrintConfig(const float posval, const float negval, const char *format, const char *prefix, uint8_t precision) {
    char printBuffer[32];

    // Print the positive value
    if (posval == negval) {
        // If the values are equal, we can print the name, without a prefix & without a sign
        sprintf_P(printBuffer, format, prefix, _name, NO_SIGN);
    } else {
        // Otherwise we print the name , without a prefix, but with a "+"
        sprintf_P(printBuffer, format, prefix, _name, "+");
    }
    Serial.print(printBuffer);
    Serial.print(posval, precision);

    // If the positive sensititivity is not equal to the negative sensitivity, we have to print the negative sensitivity too.
    if (posval != negval) {
        sprintf_P(printBuffer, FMT_COMMA, prefix, _name, "-");
        Serial.print(printBuffer);
        Serial.print(negval, precision);
    }
}

#define TWO_DECIMALS 2
#define ONE_DECIMAL 1
#define NO_DECIMALS 0
/**
 * @brief Output the configuration of the axis to the Serial Monitor.
 * @details This function prints the configuration of the axis to the serial port.
 */
void MotionAxisConfig::PrintConfig() {
    // Print the sensitivity
    _helper_PrintConfig(_config.pos_sensitivity, _config.neg_sensitivity, FMT_NOCOMMA, NO_PREFIX, TWO_DECIMALS);

    // Print the gate
    _helper_PrintConfig(_config.pos_gate, _config.neg_gate, FMT_COMMA, "G", NO_DECIMALS);

    // Print the modfunc
    _helper_PrintConfig(_config.pos_modfunc, _config.neg_modfunc, FMT_COMMA, "M", NO_DECIMALS);

    // Print the inversion
    _helper_PrintConfig(_config.invert, _config.invert, FMT_COMMA, "IF", NO_DECIMALS);
}

/**
 * @brief Function to modify the input value according to different mathematic modes. Choose the mathematical function in config.h as modFunc
 * @param x input between -350 and +350
 * @return output between -350 and +350
 */
void MotionAxisConfig::_modifierFunction() {
    // making sure function input never exceeds range of -350 to 350
    _value = constrain(_value, -350, 350);
    uint8_t mfunc;

    mfunc = (_value < 0) ? _config.neg_modfunc : _config.pos_modfunc; // Get the modulation function from the class variable
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