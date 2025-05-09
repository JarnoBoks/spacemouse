#include "SensorCalibrationManagerIdle.hpp"
#include "observers/SensorIdleCalibration.h" // For SensorIdleCalibration class

#include <Arduino.h> // For PROGMEM

// FIXME - Split into separate classes for each calibration type (and move common code to a base class)
void SensorCalibrationManagerIdle::activate(const int iterations) {
    // Call the base class calculate function
    SensorCalibrationManager::activate();

    // Attach the idle calibration observer to the hardware
    currentCalibration = new SensorIdleCalibration(this, iterations);
    m_SensorCollection->attachObserver(currentCalibration);
}

void SensorCalibrationManagerIdle::activate() {
    // Call the base class calculate function
    SensorCalibrationManager::activate();

    // Attach the idle calibration observer to the hardware
    currentCalibration = new SensorIdleCalibration(this, 500);
    m_SensorCollection->attachObserver(currentCalibration);
}
