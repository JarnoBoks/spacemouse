#include "sensor_hall.h"

HallSensor::HallSensor(const int8_t pin, HallSensorsId_t id) : Sensor(pin), id(id) {
    const char *names[HallSensorsId_t::LENGTH] = HALL_SENSOR_NAMES;
    this->name = names[id];
}

uint8_t HallSensor::getId() const {
    return static_cast<uint8_t>(id);
}

const char *HallSensor::getName() const {
    return name;
}