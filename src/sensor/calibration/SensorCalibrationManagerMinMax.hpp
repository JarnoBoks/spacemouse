#pragma once

#include "SensorCalibrationManager.hpp"

class SensorCalibrationManagerMinMax : public SensorCalibrationManager {
private:
public:
    SensorCalibrationManagerMinMax() = delete;   // Delete the default constructor
    ~SensorCalibrationManagerMinMax() = default; // Default destructor

    /**
     * @brief Constructor for the SensorCalibrationManagerMinMax class.
     * @param sensorCollection Pointer to the SensorCollection instance.
     * @details Initializes the sensor calibration manager with the provided sensor collection.
     */
    SensorCalibrationManagerMinMax(SensorCollection *sensorCollection) : SensorCalibrationManager(sensorCollection) {};

    virtual void activate() override;
};
