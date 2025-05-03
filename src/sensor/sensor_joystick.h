#ifndef SENSOR_JOYSTICK_H
#define SENSOR_JOYSTICK_H

#include "sensor.h"

// -- Calibration warning levels for the joystick sensors --

#define JOYSTICK_SENSOR_NAMES {"AX:", "AY:", "BX:", "BY:", "CX:", "CY:", "DX:", "DY:"}

enum JoystickSensorsId_t : int8_t { AX = 0,
                                    AY,
                                    BX,
                                    BY,
                                    CX,
                                    CY,
                                    DX,
                                    DY,
                                    JS_LENGTH };

class JoystickSensor : public Sensor {
private:
public:
    JoystickSensor(const int8_t pin, JoystickSensorsId_t id);
    ~JoystickSensor() override {}

    bool setIdlePosition(int val) override;
    bool idlePositionWarning(const int val) const override;
};
#endif // SENSOR_JOYSTICK_H
