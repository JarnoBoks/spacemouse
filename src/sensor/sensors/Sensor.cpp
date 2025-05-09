#include "Sensor.hpp"
#include "sensor/config/SensorConfig.h"
#include "visitors/IPrinterVisitor.h"

#include <wiring_private.h> // for analogRead & map function

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
}

Sensor::~Sensor() {
    delete config;
}

/**
 * @brief Checks if the provided name matches the sensor's name.
 * @param name The name to compare against the sensor's name.
 * @return True if the names match, false otherwise.
 */
const bool Sensor::isCurrentSensor(const char *name) const {
    if (this->name != nullptr) {
        return (strcmp(this->name, name) == 0);
    }
    return false;
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
 * @brief Sets the idle position for the sensor configuration.
 * @param val The new idle position to set.
 * @return True if the idle position was set successfully, false otherwise.
 */
bool Sensor::setIdlePosition(int val) {
    idleposition = val; // Set the idle position to the provided value
    return true;        // Return true to indicate success
}

/**
 * @brief Applies calibration to the sensor value.
 * @details This function adjusts the filtered value based on the sensor's configuration.
 */
#define TOTALSENSITIVITY 350
void Sensor::applyCalibration() {
    filtered = centered;

    if (config) {
        uint8_t _deadzone = config->getDeadzone(); // Get the deadzone value from the configuration
        if (abs(centered) < _deadzone) {
            filtered = 0;
        } else if (centered > _deadzone) {
            filtered = map(centered, _deadzone, config->getMax(), 0, TOTALSENSITIVITY);
        } else { // if the value is smaller than -DEADZONE
            filtered = map(centered, config->getMin(), (-1 * _deadzone), -TOTALSENSITIVITY, 0);
        }
    }
}

void Sensor::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this); // Call the visit method of the visitor with this sensor as an argument
}