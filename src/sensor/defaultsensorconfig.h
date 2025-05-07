#pragma once
#include <Arduino.h>

class SensorConfig; // Forward declaration of SensorConfig class

class DefaultSensorConfig {
private:
    static DefaultSensorConfig *instance;
    DefaultSensorConfig(); // Private constructor to prevent instantiation

public:
    static DefaultSensorConfig &getInstance();
    SensorConfig getDefaultConfig(const int8_t sensorId) const;
};
