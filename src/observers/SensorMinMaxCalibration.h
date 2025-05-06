#pragma once

#include "IObserver.h"
#include "hardware/hardware.h"

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

    int minValue[MAX_SENSORS] = {1023};  // Array to store minimum values for each sensor, all items set to maximum possible value
    int maxValue[MAX_SENSORS] = {-1023}; // Array to store maximum values for each sensor, all items set to minimum possible value

    // REVIEW - SessorCalibrationManager is a singleton. Is it necessary to store the pointer here? (Uses some memory).
    SensorCalibrationManager *CalibrationManager = nullptr; // Pointer to the calibration manager

    void finish(Hardware *hardware); // Finish the calibration process
public:
    SensorMinMaxCalibration(SensorCalibrationManager *calibrationManager); // Constructor
    virtual ~SensorMinMaxCalibration() {};                                 // nothing to do in destructor

    void update(Hardware *hardware) override;
};
