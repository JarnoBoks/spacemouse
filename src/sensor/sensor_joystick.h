#ifndef SENSOR_JOYSTICK_H
#define SENSOR_JOYSTICK_H

#include "sensor.h"

#define IDLEPOINT_MIN_WARNINGLEVEL 384 // The idlePoint should be above this level.
#define IDLEPOINT_MAX_WARNINGLEVEL 640 // The idlePoint should be below this level.

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
    const char *name;

public:
    JoystickSensor(const int8_t pin, JoystickSensorsId_t id);
    ~JoystickSensor() override {}

    const char *getName() const override;

    bool setIdlePosition(int val) override;
    bool idlePositionWarning(const int val) const override;
};
#endif // SENSOR_JOYSTICK_H
