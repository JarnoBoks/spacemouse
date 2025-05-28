#include "Sensor.hpp"
#include <sensor/config/SensorConfig.hpp>
#include <sensor/sensors/ArchitectureADC.hpp> // Abstraction from Architecture specific ADC functions

#include <wiring_private.h> // for 'map' function

/**
 * @brief Constructor for Sensor.
 * @details Initializes the sensor with the provided pin and ID.
 *          The constructor also creates a new SensorConfig object for this axis.
 * @param pin The pin number for the sensor.
 * @param id The ID of the sensor.
 */
Sensor::Sensor(const int8_t pin, const int8_t id)
    : pin(pin),
      id(id),
      config(new SensorConfig(id)) {
    // Configure this sensor's pin for ADC reading
    ArchitectureADC::connectADC(pin); // Setup the ADC for the specified pin
}

Sensor::~Sensor() {
    delete config;
}

/**
 * @brief Checks if the provided name matches the sensor's name.
 * @param name The name to compare against the sensor's name.
 * @return True if the names match, false otherwise.
 */
const bool Sensor::hasDescriptor(const char *descriptor) const {
    if (this->name != nullptr) {
        return (strcmp(this->name, descriptor) == 0);
    }
    return false;
}

/**
 * @brief Reads the raw value from the sensor pin and applies centering if the configuration is set.
 * @details This function reads the analog value from the specified pin and calculates the centered value.
 *          If a configuration is set, it adjusts the centered value based on the idle position.
 */
void Sensor::readValue() {
    m_rawValue = ArchitectureADC::readADC(pin);

    if (config) {
        m_cntValue = m_rawValue - idleposition;
    }
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
 * @brief Applies calibration values (deadzone, min/max & inversion) to the sensor value.
 * @details This function sets the final value of the reading based on the sensor's configuration.
 *          Apply deadzone correction, mapping, and inversion based on the configuration
 *          If the absolute value of the centered value is within the deadzone, set the final value to 0
 *          If the centered value is greater than the deadzone, map it to a positive range
 *          If the centered value is less than the negative deadzone, map it to a negative range
 *          The mapping is done to a range of -TOTALSENSITIVITY to TOTALSENSITIVITY
 */
#define TOTALSENSITIVITY 350
void Sensor::applyCalibration() {
    m_finValue = m_cntValue;

    if (!config) {
        return; // If no configuration is set, exit the function
    }

    uint8_t deadZone = config->getDeadzone();

    if (abs(m_cntValue) <= deadZone) {
        m_finValue = 0;
    } else if (m_cntValue > deadZone) {
        m_finValue = map(m_cntValue, deadZone, config->getMax(), 0, TOTALSENSITIVITY);
    } else {
        m_finValue = map(m_cntValue, config->getMin(), (-1 * deadZone), -TOTALSENSITIVITY, 0);
    }

    // Invert the final value if the configuration is set to inverted
    m_finValue = (config->isInverted()) ? -m_finValue : m_finValue;
}