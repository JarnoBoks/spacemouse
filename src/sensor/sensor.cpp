#include "sensor.h"
#include "sensorconfig.h"

/**
 * @brief Constructor for Sensor.
 * @details Initializes the sensor with the provided pin and ID.
 *          The constructor also creates a new SensorConfig object for this axis.
 * @param pin The pin number for the sensor.
 * @param id The ID of the sensor.
 */
Sensor::Sensor(const int8_t pin, const int8_t id)
    : pin(pin),
      name(nullptr),
      id(id),
      config(new SensorConfig(id)),
      rawvalue(0),
      centered(0),
      filtered(0),
      idleposition(0) // Initialize the member variables
{
    // REVIEW - Move the config init to the initializer list.
    //   this->config = new SensorConfig(id); // Create a new SensorConfig object for this sensor (identified by id)
}

Sensor::~Sensor() {
    delete config; // Clean up the SensorConfig object to avoid memory leaks
}

const bool Sensor::isCurrentSensor(const char *name) const {
    // Check if the provided name matches the sensor's name
    if (this->name != nullptr) {
        return (strcmp(this->name, name) == 0); // Compare the names and return true if they match
    }
    return false; // Return false if the names do not match or if the sensor name is null
}

/**
 * @brief Reads the raw value from the sensor pin.
 * @details This function reads the analog value from the specified pin and calculates the centered value.
 *          If a configuration is set, it adjusts the centered value based on the idle position.
 */
void Sensor::readValue() {
    rawvalue = analogRead(pin);

    if (config) {
        centered = rawvalue - idleposition;
    }
}

/**
 * @brief Retrieves the configuration of the sensor.
 * @return A pointer to the SensorConfig object associated with this sensor.
 */
SensorConfig *Sensor::getConfig() const {
    return config;
}

/**
 * @brief Sets the idle position for the sensor configuration.
 * @param val The new idle position to set.
 * @return True if the idle position was set successfully, false otherwise.
 */
bool Sensor::setIdlePosition(int val) {
    idleposition = val; // Set the idle position to the provided value
    return true;        // Return true to indicate success
}

/**
 * @brief Retrieves the raw value of the sensor.
 * @return The raw value of the sensor.
 */
int Sensor::getRawValue() const {
    return rawvalue;
}

/**
 * @brief Retrieves the raw value of the sensor.
 * @return The raw value of the sensor.
 */
int Sensor::getCenteredValue() const {
    return centered;
}

/**
 * @brief Retrieves the filtered value of the sensor.
 * @return The filtered value of the sensor.
 */
int Sensor::getFilteredValue() const {
    return filtered;
}

/**
 * @brief Applies calibration to the sensor value.
 * @details This function adjusts the filtered value based on the sensor's configuration.
 */
void Sensor::applyCalibration() {
    filtered = centered;

    if (config) {
        filtered = map(filtered, config->getMin(), config->getMax(), 0, 1023);
        if (config->isInverted()) {
            filtered = 1023 - filtered;
        }
        if (abs(filtered) < config->getDeadzone()) {
            filtered = 0;
        }
    }
}

const uint8_t Sensor::getId() const {
    return static_cast<uint8_t>(id);
}