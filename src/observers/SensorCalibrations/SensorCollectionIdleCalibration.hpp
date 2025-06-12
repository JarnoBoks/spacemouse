#pragma once

#include <observers/IObserver.hpp>
#include <sensor/SensorCollection.hpp> // For cHW_MAX_SENSORS

class ICalibratorState;

// REFACTOR - Attach this to the sensor instead of the SensorCollection, so that it can be used for all sensors.

/**
 * @brief Class to calibrate the idle position of the hardware sensors of the spacemouse. This class inherits from IObserver.
 * @details The class is an observer of the SensorCollection class and is instantiated by the SensorCalibrationManager.
 *          It calculates the average idle position of the sensors over a number of iterations and sets the idle position.
 *
 */
class SensorCollectionIdleCalibration : public IObserver {
private:
    int m_processedIterations = 0; // Number of processed iterations for the idle calibration, used to calculate the average idle position

    bool m_warningsOccurred = false;
    uint8_t m_maxDeadZone = 0; // Maximum dead zone value (of all sensors)

    uint32_t m_sumReads[cHW_MAX_SENSORS]; // Array to store sum of reads, used for the average calculation
    int m_minIdleValue[cHW_MAX_SENSORS];  // Array to store minimum idle values for each sensor
    int m_maxIdleValue[cHW_MAX_SENSORS];  // Array to store maximum idle values for each sensor

    ICalibratorState *m_CalibratorState = nullptr; // Pointer to the CalibratorState instance that manages this calibration

protected:
public:
    SensorCollectionIdleCalibration(ICalibratorState *calibratorState);
    virtual ~SensorCollectionIdleCalibration() = default;

    void update(IObservable *sensorCollection) override;
    void _finishCalibration(IObservable *sensorCollection); // Finish the calibration process
};
