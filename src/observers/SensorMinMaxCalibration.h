#pragma once

#include "IObserver.hpp"
#include "sensor/SensorCollection.hpp" // For cHW_MAX_SENSORS

// Forward declaration of classes to avoid circular dependencies
class SensorCalibrationManager;

/**
 * @brief Class to calibrate the minimum and maximum values of the hardware sensors of the spacemouse.
 * This class inherits from IObserver.
 *
 * @details The class is an observer of the Hardware class and is instantiated by the SensorCalibrationManager.
 */
class SensorMinMaxCalibration : public IObserver {
private:
    unsigned long startCalibrationTime = 0; // Time from millis(), when the calibration was started

    int minValue[cHW_MAX_SENSORS] = {1023};  // Array to store minimum values for each sensor, all items set to maximum possible value
    int maxValue[cHW_MAX_SENSORS] = {-1023}; // Array to store maximum values for each sensor, all items set to minimum possible value

    SensorCalibrationManager *CalibrationManager = nullptr; // Pointer to the calibration manager

    void finish(SensorCollection *sensorCollection);

public:
    SensorMinMaxCalibration(SensorCalibrationManager *calibrationManager);
    virtual ~SensorMinMaxCalibration() {};

    void update(SensorCollection *sensorCollection) override;
};
