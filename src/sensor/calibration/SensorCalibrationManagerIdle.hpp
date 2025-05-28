#pragma once

#include "SensorCalibrationManager.hpp"

#ifndef IDLE_ITERATIONS
#define IDLE_ITERATIONS 500 // Default number of iterations for idle calibration during startup
#endif

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
