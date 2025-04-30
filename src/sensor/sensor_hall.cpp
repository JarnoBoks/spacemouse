#include "sensor_hall.h"
#include "defaults_hall.h" // For the HALL_SENSOR_NAMES macro

/**
 * @brief Constructor for the HallSensor class.
 * @param pin The pin number for the Hall sensor.
 * @param id The ID of the Hall sensor.
 */
HallSensor::HallSensor(const int8_t pin, HallSensorsId_t id) : Sensor(pin, id) {
    const char *names[HallSensorsId_t::HALL_LENGTH] = HALL_SENSOR_NAMES;
    this->name = names[id];
}

/**
 * @brief Retrieves the name of the Hall sensor.
 * @return The name of the Hall sensor as a string.
 *
 * NOTE - The names are defined in the HALL_SENSOR_NAMES macro.
 *        The values are defined in the defaultaxisconfig_joystick.h file.
 */
/* REMOVE
const char *HallSensor::getName() const {
    return name;
}
 */
/**
 * @brief Sets the idle position for the sensor configuration. Overrides the base class method.
 * @param val The new idle position to set.
 * @return True if the idle position is in the predefined normal zone, false otherwise.
 *
 * NOTE - For the moment the function uses pre processor macros to set the warning limits for the idle position.
 *        This is acceptable because the different sensors types are not used in the same spacemouse.
 */
bool HallSensor::setIdlePosition(int val) {

    bool res = Sensor::setIdlePosition(val); // Call the base class method to set the idle position
    return res && idlePositionWarning(val);  // Return true if the idle position is set and in the predefined normal zone, false otherwise
}

bool HallSensor::idlePositionWarning(const int val) const {
    return val >= IDLEPOINT_LOW_WARNINGLEVEL && val <= IDLEPOINT_HIGH_WARNINGLEVEL; // Return true if in normal zone, false otherwise
}
