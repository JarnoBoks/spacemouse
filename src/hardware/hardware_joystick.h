#pragma once

#include <Arduino.h>
#include "hardware.h"

class Hardware_Joystick : public HardwareImpl<Hardware_Joystick> {
private:
    Hardware_Joystick();

public:
    static Hardware *getInstance() {
        if (_instance == nullptr) {
            _instance = new Hardware_Joystick(); // Create the hardware instance
        }
        return _instance; // Return a pointer to the instance
    }

    ~Hardware_Joystick() {}; // nothing to do in destructor

    int16_t calculateRawValue(AxisType_t axistype) override;
};
