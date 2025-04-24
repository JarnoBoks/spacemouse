
#include "sensor_joystick.h"

JoystickSensor::JoystickSensor(const int8_t pin, JoystickSensorsId_t id) : Sensor(pin), id(id) {
    const char *names[JoystickSensorsId_t::LENGTH] = JOYSTICK_SENSOR_NAMES;
    this->name = names[static_cast<uint8_t>(id)];
}

uint8_t JoystickSensor::getId() const {
    return static_cast<uint8_t>(id);
}

const char *JoystickSensor::getName() const {
    return name;
}
