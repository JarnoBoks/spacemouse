
#include "sensorconfig.h"
#include "defaultsensorconfig.h"
#include "eeprom/eepromstore.h" // To load and save the sensor configuration to EEPROM
#include <math.h>               // For abs() function
#include DEFAULTS_TYPE          // Include the default values for the specific hardware type

/**
 * @brief Default constructor for SensorConfig.
 * Initializes the sensor configuration with zeroed values.
 * @note This constructor is not used in the current implementation.
 */
SensorConfig::SensorConfig() : minv(0), maxv(0), invert(false), deadzone(0) {};

/**
 * @brief Constructor for SensorConfig with sensorId.
 * Loads the sensor configuration from EEPROM using the provided sensorId.
 * If loading fails, it sets up the configuration with default defined values.
 * @param sensorId The ID of the sensor to load the configuration for.
 */
SensorConfig::SensorConfig(int8_t sensorId) : minv(0), maxv(0), invert(false), deadzone(0) {
    if (!EEPROMStore::loadConfig(*this, sensorId)) {
        // If loading from EEPROM fails, setup the configuration with default defined values
        *this = DefaultSensorConfig::getInstance().getDefaultConfig(sensorId);
    }
};

/**
 * @brief Constructor for SensorConfig with parameters.
 * Initializes the sensor configuration with the provided min, max, invert, and deadzone values. Used when called from default sensor config.
 * @param min The minimum value for the sensor configuration.
 * @param max The maximum value for the sensor configuration.
 * @param invert If true, inverts the sensor values.
 * @param deadzone The deadzone value for the sensor configuration.
 */
SensorConfig::SensorConfig(const int min, const int max, const bool invert, const uint8_t deadzone)
    : minv(min), maxv(max), invert(invert), deadzone(deadzone) {};

/**
 * @brief Sets the minimum value for the sensor configuration.
 * @param val The new minimum value to set.
 * @note Used in calibration routines to update the minimum value.
 */
void SensorConfig::updateMin(int val) {
    minv = (val < minv) ? val : minv;
}

/**
 * @brief Sets the maximum value for the sensor configuration.
 * @param val The new maximum value to set.
 * @note Used in calibration routines to update the maximum value.
 */
void SensorConfig::updateMax(int val) {
    maxv = (val > maxv) ? val : maxv;
}

void SensorConfig::_minWarning(bool *warning) const {
    if (warning != nullptr) {
        *warning = (minv > MINIMUM_HIGH_WARNINGLEVEL); // If the minimum value is below the warning level, raise a warning
    }
}

void SensorConfig::_maxWarning(bool *warning) const {
    if (warning != nullptr) {
        *warning = (maxv < MAXIMUM_LOW_WARNINGLEVEL); // If the maximum value is below the warning level, raise a warning
    }
}

/**
 * @brief Retrieves the minimum value of the sensor configuration.
 * @details This function checks if the minimum value is above the warning level and sets the warning status accordingly.
 * @param warning Pointer to a boolean variable to store the warning status.
 *                If the minimum value is greater than the warning level, it will be set to true; otherwise, it will be set to false.
 * @return The minimum value of the sensor configuration.
 */
const int SensorConfig::getMin(bool *warning) const {
    _minWarning(warning); // Check if the minimum value is above the warning level
    return minv;
}

void SensorConfig::setMin(const int val, bool *warning) {
    minv = val;           // Set the minimum value
    _minWarning(warning); // Check if the minimum value is above the warning level
}

/**
 * @brief  Retrieves the maximum value of the sensor configuration.
 * @details This function checks if the maximum value is below the warning level and sets the warning status accordingly.
 * @param warning Pointer to a boolean variable to store the warning status.
 *                If the maximum value is below the warning level, it will be set to true; otherwise, it will be set to false.
 * @return The maximum value of the sensor configuration.
 */
const int SensorConfig::getMax(bool *warning) const {
    _maxWarning(warning); // Check if the maximum value is below the warning level
    return maxv;
}

void SensorConfig::setMax(const int val, bool *warning) {
    maxv = val;           // Set the maximum value
    _maxWarning(warning); // Check if the maximum value is below the warning level
}

/**
 * @brief  Gets the working range of the sensor configuration.
 * @details This function calculates the working range (difference between min and max) and checks if it is below the warning level.
 * @param warning Pointer to a boolean variable to store the warning status.
 *                If the working range is below the warning level, it will be set to true; otherwise, it will be set to false.
 * @return The working range of the sensor configuration.
 * @note If the maximum value is less than the minimum value, it raises a warning.
 */
const int SensorConfig::getRange(bool *warning) const {
    int range = abs(minv - maxv); // Calculate the working range
    if (warning != nullptr && maxv < minv) {
        *warning = true; // If the minimum value is greater than the maximum value, raise a warning
    } else if (warning != nullptr && range < WORKINGRANGE_WARNINGLEVEL) {
        *warning = true; // If the working range is below the warning level, raise a warning
    } else if (warning != nullptr) {
        *warning = false; // If the minimum value is greater than the maximum value, raise a warning
    }

    return (abs(minv - maxv)); // Calculate the working range}
}

/**
 * @brief Saves the sensor configuration to EEPROM.
 * @param id The ID of the sensor to save the configuration for.
 * @note This function uses the EEPROMStore class to save the configuration.
 */
void SensorConfig::saveSensorConfig(const int8_t id) {
    EEPROMStore::saveConfig(*this, id); // Store the configuration in the EEPROM
}

/**
 * @brief Loads the sensor configuration from EEPROM.
 * @param address The address to load the configuration from.
 */
void SensorConfig::loadSensorConfig(const uint8_t address) {
    EEPROMStore::loadConfig(*this, address);
}