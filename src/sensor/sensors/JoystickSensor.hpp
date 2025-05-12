#pragma once

#include "Sensor.hpp"

#define JOYSTICK_SENSOR_NAMES {"AX", "AY", "BX", "BY", "CX", "CY", "DX", "DY"}

enum JoystickSensorsId_t : int8_t { AX = 0,
                                    AY,
                                    BX,
                                    BY,
                                    CX,
                                    CY,
                                    DX,
                                    DY,
                                    JS_LENGTH };

/**
 * @brief Class representing a joystick sensor.
 * @details This class extends the Sensor class to provide specific functionality for joystick sensors.
 */
class JoystickSensor : public Sensor {
private:
public:
    JoystickSensor(const int8_t pin, JoystickSensorsId_t id);
    ~JoystickSensor() override {}

    bool setIdlePosition(int val) override final;
    bool idlePositionWarning(const int val) const override final;
};
