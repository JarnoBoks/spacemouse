#ifndef HARDWARE_HALL_H
#define HARDWARE_HALL_H

#include <Arduino.h>
#include "hardware.h"
#include "sensor/sensor_hall.h"
#include "kinematics/axis.h" // for AxisType enum

class Hardware_HALL : public Hardware {
private:
    static Hardware_HALL *instance; // Singleton instance
    Hardware_HALL();

public:
    static Hardware_HALL *getInstance();

    ~Hardware_HALL() {}; // nothing to do in destructor
    int16_t calculateRawValue(AxisType_t axistype) override;
};

#endif // HARDWARE_HALL_H