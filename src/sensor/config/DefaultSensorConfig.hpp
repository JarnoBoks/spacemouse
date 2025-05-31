#pragma once
#include <stdint.h>

class SensorConfig;
class Sensor;

class DefaultSensorConfig {
public:
    DefaultSensorConfig() = default;
    ~DefaultSensorConfig() = default;
    SensorConfig create(const Sensor *contextSensor) const;
};
