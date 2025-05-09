#pragma once

#include "IObserver.hpp"
#include "sensor/SensorCollection.hpp" // For cHW_MAX_SENSORS

// Forward declaration of classes to avoid circular dependencies
class SensorCalibrationManager;

/**
 * @brief Class to calibrate the idle position of the hardware sensors of the spacemouse.
 * This class inherits from IObserver.
 *
 * @details The class is an observer of the Hardware class and is instantiated by the SensorCalibrationManager.
 */
class SensorIdleCalibration : public IObserver {
private:
    int requestedIterations = 500;          // Number of remaining iterations for the idle calibration
    int processedIterations = 0;            // Number of processed iterations for the idle calibration
    unsigned long startCalibrationTime = 0; // Time from millis(), when the calibration was started

    bool warningsOccurred = false;
    uint8_t maxDeadZone = 0; // Maximum dead zone value (of all sensors)

    uint32_t sumReads[cHW_MAX_SENSORS]; // Array to store sum of reads, necessaru for the average calculation
    int minIdleValue[cHW_MAX_SENSORS];  // Array to store minimum idle values for each sensor
    int maxIdleValue[cHW_MAX_SENSORS];  // Array to store maximum idle values for each sensor

    // REVIEW - SessorCalibrationManager is a singleton. Is it necessary to store the pointer here? (Uses some memory).
    SensorCalibrationManager *CalibrationManager = nullptr; // Pointer to the calibration manager

    void finish(Hardware *hardware); // Finish the calibration process

protected:
public:
    SensorIdleCalibration(SensorCalibrationManager *calibrationManager, int numiterations); // Constructor
    virtual ~SensorIdleCalibration() {};                                                    // nothing to do in destructor

    void update(Hardware *hardware) override;
};
