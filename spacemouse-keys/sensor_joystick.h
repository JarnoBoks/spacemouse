#ifndef SENSOR_JOYSTICK_H
#define SENSOR_JOYSTICK_H

#include "sensor.h"

#define JOYSTICK_SENSOR_NAMES {"AX:", "AY:", "BX:", "BY:", "CX:", "CY:", "DX:", "DY:"}

enum JoystickSensorsId_t : int8_t { AX = 0,
                                    AY,
                                    BX,
                                    BY,
                                    CX,
                                    CY,
                                    DX,
                                    DY,
                                    LENGTH };

class JoystickSensor : public Sensor {
private:
    const JoystickSensorsId_t id;
    const char *name;

public:
    JoystickSensor(const int8_t pin, JoystickSensorsId_t id);
    ~JoystickSensor() override {}

    uint8_t getId() const override;
    const char *getName() const override;
};
#endif // SENSOR_JOYSTICK_H
