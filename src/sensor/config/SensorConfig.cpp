
#include "SensorConfig.hpp"
#include "config.h" // Include the config file to know the hardware type
#include <sensor/sensors/Sensor.hpp>
#include "DefaultSensorConfig.hpp"

#if defined(ARDUINO_ARCH_AVR)
#include "eeprom/eepromstore.h"                    // To load and save the sensor configuration to EEPROM
constexpr uint8_t EEPROM_SENSORCONFIG_VERSION = 1; // Define the version number for the SensorConfig in EEPROM.     // TODO: Add versioning
#endif
#if defined(ARDUINO_ARCH_ESP32)
#include "eeprom/preferencesstore.h" // To load and save the sensor configuration to Preferences
#endif

#include <Arduino.h> // For abs() function

#if defined(HW_HALLEFFECT)
#include "defaults_hall.h"
#elif defined(HW_JOYSTICK)
#include "defaults_joystick.h"
#else
#error "No hardwaretype defined"
#endif

/**
 * @brief   Constructor for SensorConfig class with sensorId.
 * @details This constructor initializes the SensorConfig object with the given sensor Id.
 *          It retrieves the configuration from EEPROM using the EEPROMStore class. If loading fails, it sets the configuration to default values.
 * @see config.h for overriding the default values.
 * @see defaults_hall.h for the default values for the HALL Effect hardware.
 * @see defaults_joystick.h for the default values for the JOYSTICK hardware.
 * @param sensorId The ID of the sensor to load the configuration for.
 */
SensorConfig::SensorConfig(const Sensor *contextSensor)
    : m_contextSensor(contextSensor) {
    if (!retrieve()) {
        DefaultSensorConfig defaultConfig;
        *this = defaultConfig.create(contextSensor);
    }
}

/**
 * @brief Constructor for SensorConfig with parameters.
 * @details Initializes the sensor configuration with the provided min, max, invert, and deadzone values. Used when called from DefaultSensorConfig.
 * @param min The minimum value for the sensor configuration.
 * @param max The maximum value for the sensor configuration.
 * @param invert If true, inverts the sensor values.
 */
SensorConfig::SensorConfig(const Sensor *contextSensor, const int min, const int max, const bool invert) {
    data.invert = invert;
    data.minv = min;
    data.maxv = max;
};

/**
 * @brief Sets the minimum value for the sensor configuration if the new value is less than the current value.
 * @param val The new minimum value to set.
 * @note Used in calibration routines to update the minimum value.
 */
void SensorConfig::updateMin(int val) {
    data.minv = (val < data.minv) ? val : data.minv;
}

/**
 * @brief Updates the maximum value for the sensor configuration if the new value is greater than the current value.
 * @param val The new maximum value requested to set.
 * @note Used in calibration routines to update the maximum value.
 */
void SensorConfig::updateMax(int val) {
    data.maxv = (val > data.maxv) ? val : data.maxv;
}

void SensorConfig::_minWarning(bool *warning) const {
    if (warning != nullptr) {
        *warning = (data.minv > MINIMUM_HIGH_WARNINGLEVEL); // If the minimum value is below the warning level, raise a warning
    }
}

void SensorConfig::_maxWarning(bool *warning) const {
    if (warning != nullptr) {
        *warning = (data.maxv < MAXIMUM_LOW_WARNINGLEVEL); // If the maximum value is below the warning level, raise a warning
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
    return data.minv;
}

/**
 * @brief Sets the minimum value of the sensor configuration and will update the warning status if necessary.
 * @param val The new minimum value to set.
 * @param warning Pointer to a boolean variable to store the warning status.
 */
void SensorConfig::setMin(const int val, bool *warning) {
    data.minv = val;      // Set the minimum value
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
    return data.maxv;
}

/**
 * @brief Sets the maximum value of the sensor configuration and will update the warning status if necessary.
 * @param val The new maximum value to set.
 * @param warning Pointer to a boolean variable to store the warning status.
 */
void SensorConfig::setMax(const int val, bool *warning) {
    data.maxv = val;      // Set the maximum value
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
    int range = abs(data.minv - data.maxv); // Calculate the working range
    if (warning != nullptr && data.maxv < data.minv) {
        *warning = true; // If the minimum value is greater than the maximum value, raise a warning
    } else if (warning != nullptr && range < WORKINGRANGE_WARNINGLEVEL) {
        *warning = true; // If the working range is below the warning level, raise a warning
    } else if (warning != nullptr) {
        *warning = false; // If the minimum value is greater than the maximum value, raise a warning
    }

    return (abs(data.minv - data.maxv)); // Calculate the working range
}

#if defined(ARDUINO_ARCH_ESP32)
#define KEY_PREF_SENSORCFG "snsr%d" // Key prefix for sensor configuration in Preferences

/**
 * @brief Saves the sensor configuration to EEPROM.
 * @param sensorId The ID of the sensor to save the configuration for.
 * @note This function uses the PreferencesStore class to save the configuration to the non-volatile memory of the ESP32.
 */
void SensorConfig::persist() const {
    const int8_t sensorId = static_cast<int8_t>(m_contextSensor->getId());

    char buffer[8] = "\0"; // Ensure the buffer is null-terminated
    sprintf(buffer, KEY_PREF_SENSORCFG, sensorId);
    PreferencesStore::save(buffer, &data, sizeof(data)); // Store the data structure in the Preferences
}

/**
 * @brief Loads the sensor configuration from EEPROM.
 * @param sensorId The ID of the sensor to load the configuration for.
 * @return The result of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool SensorConfig::retrieve() {
    const int8_t sensorId = static_cast<int8_t>(m_contextSensor->getId());

    char buffer[8] = "\0"; // Ensure the buffer is null-terminated
    sprintf(buffer, KEY_PREF_SENSORCFG, sensorId);
    return (PreferencesStore::load(buffer, &data, sizeof(data)) == ERR_PREFSTORE_SUCCESS);
}

#endif // End of ESP32 specific code

#if defined(ARDUINO_ARCH_AVR)
/**
 * @brief Saves the sensor configuration to EEPROM.
 * @param sensorId The ID of the sensor to save the configuration for.
 * @note This function uses the EEPROMStore class to save the configuration to the Arduino EEPROM.
 */
void SensorConfig::persist() const {
    const int8_t sensorId = static_cast<int8_t>(m_contextSensor->getId());

    // Calculate the EEPROM tableId for the SensorConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (sensorId * EEPROM_SENSOR_ID_RESERVATIONS) + EEPROM_SENSOR_ID_BASE; // Calculated Id for the SensorConfig in EEPROM

    // Persist the data stored in this class
    EEPROMStore::save(tableId, &data, sizeof(data)); // Store the data structure in the EEPROM
}

/**
 * @brief Loads the sensor configuration from EEPROM.
 * @param sensorId The ID of the sensor to load the configuration for.
 * @return The result of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool SensorConfig::retrieve() {
    const int8_t sensorId = static_cast<int8_t>(m_contextSensor->getId());

    // Calculate the EEPROM tableId for the SensorConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (sensorId * EEPROM_SENSOR_ID_RESERVATIONS) + EEPROM_SENSOR_ID_BASE; // Calculated Id for the SensorConfig in EEPROM

    // Retrieve the data stored in the EEPROM
    return (EEPROMStore::load(tableId, &data, sizeof(data)) == ERR_EEPROMSTORE_SUCCESS);
}
#endif
