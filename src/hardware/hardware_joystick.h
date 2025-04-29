#ifndef HARDWARE_JOYSTICK_H
#define HARDWARE_JOYSTICK_H

#include <Arduino.h>
#include "hardware.h"

class Hardware_Joystick : public Hardware {
private:
    // REMOVE static Hardware_Joystick *instance; // Singleton instance
    Hardware_Joystick();

public:
    // REMOVE static Hardware_Joystick *getInstance();
    // REMOVE static Hardware *getInstance();
    ~Hardware_Joystick() {}; // nothing to do in destructor

    int16_t calculateRawValue(AxisType_t axistype) override;
};

#endif // HARDWARE_JOYSTICK_H