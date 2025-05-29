
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
    return res && idlePositionWarning(val);  // Return true if the idle position is set and in the predefined normal zone, false otherwise.
}

bool JoystickSensor::idlePositionWarning(const int val) const {
    return val >= IDLEPOINT_LOW_WARNINGLEVEL && val <= IDLEPOINT_HIGH_WARNINGLEVEL; // Return true if in normal zone, false otherwise
}
