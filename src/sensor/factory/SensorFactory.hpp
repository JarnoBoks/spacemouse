#pragma once

#include "sensor/sensors/Sensor.hpp" // Base class for the sensors to create          // REVIEW - Should we use the interface instead of the base class?
#include <stdint.h>

/**
 * @brief SensorFactory base class for creating sensor instances.
 * This class is responsible for creating and managing sensor instances.
 * It provides methods to create sensors, set up their functionality, and evaluate their state.
 */
class SensorFactory {
protected:
    // REMOVE void setupFunctionality(Sensor *sensor);

public:
    SensorFactory() = default;
    virtual ~SensorFactory() {};

    virtual Sensor *create(const uint8_t id) = 0; // Pure virtual function to create a sensor instance.
};
