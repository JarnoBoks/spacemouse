#pragma once

// Include local headers
#include "observers/IObserver.hpp" // For IDebugMonitor class

// Include necessary system headers
#include <Arduino.h>

class Sensor; // Forward declaration of Sensor class

class SensorCalibrationManager {
private:
    static SensorCalibrationManager *instance; // Singleton instance
    SensorCalibrationManager() {};             // Private constructor for singleton pattern

    IObserver *currentCalibration = nullptr; // Pointer to the current calibration observer
public:
    static SensorCalibrationManager *getInstance(); // Moved getInstance() method here

    void activateIdleCalibration(const int iterations = 500);
    void deactivateIdleCalibration(const bool warningsOccurred);

    void activateMinMaxCalibration();
    void deactivateMinMaxCalibration(const bool warningsOccurred);
};
