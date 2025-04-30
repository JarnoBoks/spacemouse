#ifndef SENSORIDLECALIBRATION_H
#define SENSORIDLECALIBRATION_H

#include "IObserver.h"
#include "hardware/hardware.h"

// Forward declaration of classes to avoid circular dependencies
class SensorCalibrationManager;

/**
 * @brief Class to calibrate the idle position of the hardware sensors of the spacemouse.
 * This class inherits from DebugMonitor.
 *
 * @details This class is used to calibrate the idle position of the hardware sensors of the spacemouse.
 * It is used to set the idle position of the sensors to the average value of the readings.
 * The class is an observer of the Hardware class and is instantiated by the SensorCalibrationManager.
 */
class SensorIdleCalibration : public IObserver {
private:
    int requestedIterations = 500;          // Number of remaining iterations for the idle calibration
    int processedIterations = 0;            // Number of processed iterations for the idle calibration
    unsigned long startCalibrationTime = 0; // Time from millis(), when the calibration was started

    bool warningsOccurred = false;
    uint8_t maxDeadZone = 0;                // Maximum dead zone value (of all sensors)
    uint32_t sumReads[MAX_SENSORS] = {0};   // Array to store sum of reads, necessaru for the average calculation, all items initialized to 0
    int minIdleValue[MAX_SENSORS] = {1023}; // Array to store minimum idle values for each sensor, all items set to maximum possible value
    int maxIdleValue[MAX_SENSORS] = {0};    // Array to store maximum idle values for each sensor, all items set to minimum possible value

    // REVIEW - SessorCalibrationManager is a singleton. Is it necessary to store the pointer here? (Uses some memory).
    SensorCalibrationManager *CalibrationManager = nullptr; // Pointer to the calibration manager

    void finish(Hardware *hardware); // Finish the calibration process

protected:
public:
    SensorIdleCalibration(SensorCalibrationManager *calibrationManager, int numiterations); // Constructor
    ~SensorIdleCalibration() {};                                                            // nothing to do in destructor

    void update(const Axis *axis) override {
        // Doing nothing here, as this output class is only used for sensors and not for axes.
    }

    void update(Hardware *hardware) override;
};

#endif // SENSORIDLECALIBRATION_H