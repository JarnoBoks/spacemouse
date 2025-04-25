
#include "sensorconfig.h"
#include "defaultsensorconfig.h"
#include "eeprom/eepromstore.h" // To load and save the sensor configuration to EEPROM

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
    // REVIEW - Check if the overload in the loadConfig function is correct or if the function for Axis/Sensor should have a different name.
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
 * @param compare If true, compares with the current minimum value.
 * @deprecated
 */

void SensorConfig::setMin(int val, bool compare) {
    minv = (compare && val < minv) ? val : minv;
}

/**
 * @brief Sets the maximum value for the sensor configuration.
 * @param val The new maximum value to set.
 * @param compare If true, compares with the current maximum value.
 * @deprecated
 */
void SensorConfig::setMax(int val, bool compare) {
    maxv = (compare && val > maxv) ? val : maxv;
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