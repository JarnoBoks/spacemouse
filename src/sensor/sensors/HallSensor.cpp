#include "HallSensor.hpp"
#include "defaults_hall.h" // For the HALL_SENSOR_NAMES macro

/**
 * @brief Constructor for the HallSensor class.
 * @param pin The pin number for the Hall sensor.
 * @param id The ID of the Hall sensor.
 */
HallSensor::HallSensor(const int8_t pin, HallSensorsId_t id) : Sensor(pin, id) {
    const char *names[HallSensorsId_t::HALL_LENGTH] = HALL_SENSOR_NAMES;
    this->descriptor = names[id];
}

/**
 * @brief Sets the idle position for the sensor configuration. Overrides the base class method.
 * @param val The new idle position to set.
 * @return True if the idle position is in the predefined normal zone, false otherwise.
 */
bool HallSensor::setIdlePosition(int val) {

    bool res = Sensor::setIdlePosition(val); // Call the base class method to set the idle position
    return res && idlePositionOk();          // Return true if the idle position is set and in the predefined normal zone, false otherwise
}

/**
 * @brief Checks if the idle position is within the warning limits.
 * @param val The idle position value to check.
 * @return The warning status of the idle position.
 * @retval True if the idle position is in the normal zone (between IDLEPOINT_LOW_WARNINGLEVEL and IDLEPOINT_HIGH_WARNINGLEVEL) (ok).
 * @retval False if the idle position is outside the normal zone (warning).
 */
bool HallSensor::idlePositionOk() const {
    return getIdlePosition() >= IDLEPOINT_LOW_WARNINGLEVEL && getIdlePosition() <= IDLEPOINT_HIGH_WARNINGLEVEL; // Return true if in normal zone, false otherwise
}

bool HallSensor::setDeadzone(const uint8_t dz) {
    const uint8_t dz_surplus = dz + DEADZONE_SURPLUS; // Calculate the deadzone with the surplus added
    bool res = Sensor::setDeadzone(dz_surplus);       // Call the base class method to set the deadzone
    return res && isDeadzoneOk();                     // Return true if the deadzone is set and within the warning limits, false otherwise}
}

/**
 * @brief Checks if the deadzone value is within the warning limits.
 * @param val The deadzone value to check.
 * @return The warning status of the deadzone.
 * @retval True if the deadzone is below DEADZONE_WARNINGLEVEL (ok).
 * @retval False if the deadzone is above or equal to DEADZONE_WARNINGLEVEL (warning).
 */
bool HallSensor::isDeadzoneOk() const {
    return (getDeadzone() < DEADZONE_WARNINGLEVEL);
}