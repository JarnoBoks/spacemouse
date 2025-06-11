
#include "JoystickSensor.hpp"
#include "defaults_joystick.h" // For the Warning levels

JoystickSensor::JoystickSensor(const int8_t pin, JoystickSensorsId_t id) : Sensor(pin, id) {
    const char *names[JoystickSensorsId_t::JS_LENGTH] = JOYSTICK_SENSOR_NAMES;
    this->descriptor = names[static_cast<uint8_t>(id)];
}

/**
 * @brief Sets the idle position for the sensor configuration. Overrides the base class method.
 * @param val The new idle position to set.
 * @return True if the idle position is set and in the predefined normal zone, false otherwise.
 *
 * NOTE - For the moment the function uses pre processor macros to set the warning limits for the idle position.
 *        The values are defined in the defaultaxisconfig_joystick.h file.
 *        This is acceptable while the Joystick & Hall Effect sensors are not used in the same spacemouse.
 */
bool JoystickSensor::setIdlePosition(int val) {

    bool res = Sensor::setIdlePosition(val); // Call the base class method to set the idle position
    return res && idlePositionOk(val);       // Return true if the idle position is set and in the predefined normal zone, false otherwise.
}

/**
 * @brief Checks if the idle position is within the warning limits.
 * @param val The idle position value to check.
 * @return The warning status of the idle position.
 * @retval True if the idle position is in the normal zone (between IDLEPOINT_LOW_WARNINGLEVEL and IDLEPOINT_HIGH_WARNINGLEVEL) (ok).
 * @retval False if the idle position is outside the normal zone (warning).
 */
bool JoystickSensor::idlePositionOk(const int val) const {
    return val >= IDLEPOINT_LOW_WARNINGLEVEL && val <= IDLEPOINT_HIGH_WARNINGLEVEL; // Return true if in normal zone, false otherwise
}

bool JoystickSensor::setDeadzone(const uint8_t dz) {
    const uint8_t dz_surplus = dz + DEADZONE_SURPLUS; // Calculate the deadzone with the surplus added
    bool res = Sensor::setDeadzone(dz_surplus);       // Call the base class method to set the deadzone
    return res && isDeadzoneOk();                     // Return true if the deadzone is set and within the warning limits, false otherwise
}

/**
 * @brief Checks if the deadzone value is within the warning limits.
 * @param val The deadzone value to check.
 * @return The warning status of the deadzone.
 * @retval True if the deadzone is below DEADZONE_WARNINGLEVEL (ok).
 * @retval False if the deadzone is above or equal to DEADZONE_WARNINGLEVEL (warning).
 */
bool JoystickSensor::isDeadzoneOk() const {
    return (getDeadzone() < DEADZONE_WARNINGLEVEL);
}