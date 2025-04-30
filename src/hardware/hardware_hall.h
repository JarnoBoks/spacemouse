#ifndef HARDWARE_HALL_H
#define HARDWARE_HALL_H

#include <Arduino.h>
#include "hardware.h"

class Hardware_HALL : public HardwareImpl<Hardware_HALL> {
private:
    Hardware_HALL();

public:
    static Hardware *getInstance() {
        if (_instance == nullptr) {
            _instance = new Hardware_HALL(); // Create the hardware instance
        }
        return _instance; // Return a pointer to the instance
    }

    ~Hardware_HALL() {}; // nothing to do in destructor
    int16_t calculateRawValue(AxisType_t axistype) override;
};

#endif // HARDWARE_HALL_H