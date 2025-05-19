#pragma once

#include "observers/IObserver.hpp"
#include "sensor/SensorCollection.hpp" // For cHW_MAX_SENSORS

// Forward declaration of classes to avoid circular dependencies
class SensorCalibrationManager;

/**
 * @brief Class to calibrate the idle position of the hardware sensors of the spacemouse.
 * This class inherits from IObserver.
 *
 * @details The class is an observer of the SensorCollection class and is instantiated by the SensorCalibrationManager.
 */
class SensorIdleCalibration : public IObserver {
private:
    int m_requestedIterations = 500;          // Number of remaining iterations for the idle calibration
    int m_processedIterations = 0;            // Number of processed iterations for the idle calibration
    unsigned long m_startCalibrationTime = 0; // Time from millis(), when the calibration was started

    bool m_warningsOccurred = false;
    uint8_t m_maxDeadZone = 0; // Maximum dead zone value (of all sensors)

    uint32_t m_sumReads[cHW_MAX_SENSORS]; // Array to store sum of reads, necessaru for the average calculation
    int m_minIdleValue[cHW_MAX_SENSORS];  // Array to store minimum idle values for each sensor
    int m_maxIdleValue[cHW_MAX_SENSORS];  // Array to store maximum idle values for each sensor

    // REVIEW - SensorCalibrationManager is a singleton. Is it necessary to store the pointer here? (Uses some memory).
    SensorCalibrationManager *m_CalibrationManager = nullptr; // Pointer to the calibration manager

    void _initialize();                            // Initialize the calibration process
    void _finalize(IObservable *sensorCollection); // Finish the calibration process

protected:
public:
    SensorIdleCalibration(SensorCalibrationManager *calibrationManager, int numiterations);
    virtual ~SensorIdleCalibration() {};

    void update(IObservable *sensorCollection) override;
};
