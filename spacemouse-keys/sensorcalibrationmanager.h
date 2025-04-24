#ifndef SENSORCALIBRATIONMANAGER_H
#define SENSORCALIBRATIONMANAGER_H
#include <Arduino.h>
#include "sensor.h"

class SensorCalibrationManager {
private:
    CalibrationCommand *commands[MAX_SENSOR_COMMANDS];
    uint8_t commandCount = 0;

public:
    void calibrateIdle(Sensor *sensors[], uint8_t count);
    void calibrateMinMax(Sensor *sensors[], uint8_t count);
    void calibrateDeadzone(Sensor *sensors[], uint8_t count);
    void addCommand(CalibrationCommand *cmd);
};

#endif // SENSORCALIBRATIONMANAGER_H