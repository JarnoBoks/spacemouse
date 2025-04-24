#ifndef HARDWARE_HALL_H
#define HARDWARE_HALL_H

#define MAX_SENSORS 8

#include <Arduino.h>
#include "hardware_hall.h"
#include "sensor.h"
#include "axis.h" // for AxisType enum

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