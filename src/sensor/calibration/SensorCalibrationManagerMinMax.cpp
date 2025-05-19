#include "SensorCalibrationManagerMinMax.hpp"
#include <observers/Calibrators/SensorMinMaxCalibration.hpp> // For SensorMinMaxCalibration class

/**
 * @brief Activates the MinMax calibration process.
 * @details This function is called to start the MinMax calibration process.
 *          It initializes the calibration manager and attaches the MinMax calibration observer to the sensor collection.
 */
void SensorCalibrationManagerMinMax::activate() {
    SensorCalibrationManager::activate();

    // Attach the MinMax calibration observer to the sensor collection
    currentCalibration = new SensorMinMaxCalibration(this);
    m_SensorCollection->attachObserver(currentCalibration);
}
