#pragma once

#include "Sensor.hpp"

#ifdef ARDUINO_ARCH_AVR
// Settings for the AVR architecture and PCB version of the Spacemouse
// FIXME - Check if this is still needed after the ESP32 migration
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
#endif

#ifdef ARDUINO_ARCH_ESP32
// Settings for the ESP32 architecture and PCB version of the Spacemouse
#define HALL_SENSOR_NAMES {"HES1", "HES2", "HES3", "HES4", "HES5", "HES6", "HES7", "HES8"}

enum HallSensorsId_t : int8_t { HES1 = 0,
                                HES2,
                                HES3,
                                HES4,
                                HES5,
                                HES6,
                                HES7,
                                HES8,
                                HALL_LENGTH }; // Length of the enum for array size
#endif

/**
 * @brief Class representing a Hall effect sensor.
 * @details This class extends the Sensor class to provide specific functionality for Hall effect sensors.
 *          It includes methods for setting idle positions and handling idle position warnings.
 */
class HallSensor : public Sensor {
private:
public:
    HallSensor(const int8_t pin, const HallSensorsId_t id);

    bool setIdlePosition(int val) override final;
    bool idlePositionOk() const override final;

    bool isDeadzoneOk() const override final;
    bool setDeadzone(const uint8_t dz) override final;
};
