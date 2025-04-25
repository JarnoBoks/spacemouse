#ifndef SENSORCALIBRATIONMANAGER_H
#define SENSORCALIBRATIONMANAGER_H
#include <Arduino.h>

class Sensor; // Forward declaration of Sensor class

class SensorCalibrationManager {
public:
    bool calibrateIdle(Sensor *sensors[], uint8_t count, int iterations = 500);

    void calibrateMinMax(Sensor *sensors[], uint8_t count);
    void calibrateDeadzone(Sensor *sensor, const uint8_t deadzone);
};

#endif // SENSORCALIBRATIONMANAGER_H