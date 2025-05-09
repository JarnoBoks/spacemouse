#pragma once

#include "Sensor.hpp"

#define HALL_SENSOR_NAMES {"HES0", "HES1", "HES2", "HES3", "HES6", "HES7", "HES8", "HES9"}

enum HallSensorsId_t : int8_t { HES0 = 0,
                                HES1,
                                HES2,
                                HES3,
                                HES6,
                                HES7,
                                HES8,
                                HES9,
                                HALL_LENGTH }; // Length of the enum for array size

/**
 * @brief Class representing a Hall effect sensor.
 * @details This class extends the Sensor class to provide specific functionality for Hall effect sensors.
 *          It includes methods for setting idle positions and handling idle position warnings.
 */
class HallSensor : public Sensor {
private:
public:
    HallSensor(const int8_t pin, HallSensorsId_t id);

    bool setIdlePosition(int val) override;
    bool idlePositionWarning(const int val) const override;
};
