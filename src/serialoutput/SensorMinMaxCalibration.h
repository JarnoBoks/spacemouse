#ifndef SENSORMINMAXCALIBRATION_H
#define SENSORMINMAXCALIBRATION_H

#include "idebugmonitor.h"
#include "hardware/hardware.h"
#include "text.h"

// Forward declaration of classes to avoid circular dependencies
class SensorCalibrationManager;

/**
 * @brief  *
 *
 */
class SensorMinMaxCalibration : public IDebugMonitor {
private:
    unsigned long startCalibrationTime = 0; // Time from millis(), when the calibration was started

    int minValue[MAX_SENSORS] = {1023};  // Array to store minimum values for each sensor, all items set to maximum possible value
    int maxValue[MAX_SENSORS] = {-1023}; // Array to store maximum values for each sensor, all items set to minimum possible value

    // REVIEW - SessorCalibrationManager is a singleton. Is it necessary to store the pointer here? (Uses some memory).
    SensorCalibrationManager *CalibrationManager = nullptr; // Pointer to the calibration manager

    void start();
    void finish(Hardware *hardware);                  // Finish the calibration process
    void printCalibrationResults(Hardware *hardware); // Print the calibration results for each sensor to the serial monitor

protected:
public:
    SensorMinMaxCalibration(SensorCalibrationManager *calibrationManager, int numiterations); // Constructor
    ~SensorMinMaxCalibration() {};                                                            // nothing to do in destructor

    void update(const Axis *axis) override {
        // Doing nothing here, as this output class is only used for sensors and not for axes.
    }

    void update(Hardware *hardware) override;
};

#endif // SENSORMINMAXCALIBRATION_H