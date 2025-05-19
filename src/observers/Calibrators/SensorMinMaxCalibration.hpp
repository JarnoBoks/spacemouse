#pragma once

#include "observers/IObserver.hpp"
#include "sensor/SensorCollection.hpp" // For cHW_MAX_SENSORS

// Forward declaration of classes to avoid circular dependencies
class SensorCalibrationManager;

/**
 * @brief Class to calibrate the minimum and maximum values of the hardware sensors of the spacemouse.
 * This class inherits from IObserver.
 *
 * @details The class is an observer of the SensorCollection class and is instantiated by the SensorCalibrationManager.
 */
class SensorMinMaxCalibration : public IObserver {
private:
    unsigned long m_startCalibrationTime = 0; // Calibration start time from millis()

    int m_minValue[cHW_MAX_SENSORS] = {1023};  // Array to store minimum values for each sensor, all items initialized to maximum possible value (1023)
    int m_maxValue[cHW_MAX_SENSORS] = {-1023}; // Array to store maximum values for each sensor, all items initialized to minimum possible value (-1023)

    SensorCalibrationManager *m_CalibrationManager = nullptr; // Pointer to the calibration manager

    void _initialize();                            // Initialize the calibration process
    void _finalize(IObservable *sensorCollection); // Finalize the calibration process

public:
    SensorMinMaxCalibration(SensorCalibrationManager *calibrationManager);
    virtual ~SensorMinMaxCalibration() {};

    void update(IObservable *sensorCollection) override;
};
