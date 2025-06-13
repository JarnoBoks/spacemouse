#pragma once
#include <observers/IObserver.hpp>

class ICalibratorState;

// REFACTOR - For ESP32 there should be an interface class for Calibrator Observers.

/**
 * @brief Class to calibrate the minimum and maximum values of the sensors of the spacemouse.
 * @details The class is an observer of the SensorCollection class and is instantiated by the SensorCalibrationManager.
 *          It listens for updates from the SensorCollection and performs calibration by tracking the minimum and maximum
 *          values of each sensor.
 */
class SensorMinMaxCalibration : public IObserver {
private:
    ICalibratorState *m_CalibratorState = nullptr; // Pointer to the CalibratorState instance that manages this calibration

public:
    SensorMinMaxCalibration(ICalibratorState *calibratorState);
    virtual ~SensorMinMaxCalibration() {};

    void update(IObservable *sensorCollection) override;
};
