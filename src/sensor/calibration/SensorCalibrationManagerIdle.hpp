#pragma once

#include "SensorCalibrationManager.hpp"

class SensorCalibrationManagerIdle : public SensorCalibrationManager {
protected:
public:
    SensorCalibrationManagerIdle() = delete;   // Delete the default constructor
    ~SensorCalibrationManagerIdle() = default; // Default destructor

    /**
     * @brief Constructor for the SensorCalibrationManagerIdle class.
     * @param sensorCollection Pointer to the SensorCollection instance.
     * @details Initializes the sensor calibration manager with the provided sensor collection.
     */
    SensorCalibrationManagerIdle(SensorCollection *sensorCollection) : SensorCalibrationManager(sensorCollection) {};

    void activate() override final;
    void activate(const int iterations);
};
