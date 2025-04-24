#ifndef SENSOR_HALL_H
#define SENSOR_HALL_H

#include "sensor.h"

#define HALL_SENSOR_NAMES {"HES0", "HES1", "HES2", "HES3", "HES6", "HES7", "HES8", "HES9"}

enum HallSensorsId_t : int8_t { HES0 = 0,
                                HES1,
                                HES2,
                                HES3,
                                HES6,
                                HES7,
                                HES8,
                                HES9,
                                LENGTH }; // Length of the enum for array size

class HallSensor : public Sensor {
private:
    const HallSensorsId_t id;
    const char *name;

public:
    HallSensor(const int8_t pin, HallSensorsId_t id);

    uint8_t getId() const override;
    const char *getName() const override;
};

#endif // SENSOR_HALL_H