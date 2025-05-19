#pragma once
#include <stdint.h>

class SensorConfig; // Forward declaration of SensorConfig class

class DefaultSensorConfig {
public:
    DefaultSensorConfig() = default;
    ~DefaultSensorConfig() = default;
    SensorConfig create(const int8_t sensorId) const;
};
