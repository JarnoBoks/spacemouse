#ifndef SENSORCALIBRATIONMANAGER_H
#define SENSORCALIBRATIONMANAGER_H
#include <Arduino.h>

#include "serialoutput/idebugmonitor.h" // For IDebugMonitor class  //TODO - Rename class to IAxisHardware_Observer
class Sensor;                           // Forward declaration of Sensor class

class SensorCalibrationManager {
private:
    static SensorCalibrationManager *instance; // Singleton instance
    SensorCalibrationManager() {};             // Private constructor for singleton pattern

    IDebugMonitor *currentCalibration = nullptr; // Pointer to the current calibration observer
public:
    static SensorCalibrationManager *getInstance(); // Moved getInstance() method here

    void startIdleCalibration(const int iterations = 500);
    void finishIdleCalibration(const bool warningsOccurred);

    void calibrateMinMax(Sensor *sensors[], uint8_t count);
    void calibrateDeadzone(Sensor *sensor, const uint8_t deadzone);
};

#endif // SENSORCALIBRATIONMANAGER_H
