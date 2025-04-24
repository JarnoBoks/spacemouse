#ifndef HARDWARE_JOYSTICK_H
#define HARDWARE_JOYSTICK_H

#define MAX_SENSORS 8

#include <Arduino.h>
#include "hardware.h"

class Hardware_Joystick : public Hardware {
private:
    static Hardware_Joystick *instance; // Singleton instance
    Hardware_Joystick();

public:
    static Hardware_Joystick *getInstance();
    ~Hardware_Joystick() {}; // nothing to do in destructor

    int16_t calculateRawValue(AxisType_t axistype) override;
};

#endif // HARDWARE_JOYSTICK_H