#include "SensorCalibrationManagerIdle.hpp"
#include <observers/Calibrators/SensorIdleCalibration.hpp> // For SensorIdleCalibration class

/**
 * @brief Activates the idle calibration process.
 * @param iterations The number of iterations for the idle calibration process.
 * @details This function initializes the idle calibration process by creating an instance of the SensorIdleCalibration class and attaching it to the sensor collection.
 *          It also calls the base class activate function to set up the calibration manager.
 */
void SensorCalibrationManagerIdle::activate(const int iterations) {
    // Call the base class calculate function
    SensorCalibrationManager::activate();

    // Attach the idle calibration observer to the sensor collection
    currentCalibration = new SensorIdleCalibration(this, iterations);
    m_SensorCollection->attachObserver(currentCalibration);
}

/**
 * @brief Activates the idle calibration process with a default number of 500 iterations.
 * @see SensorCalibrationManagerIdle::activate(int iterations)
 */
void SensorCalibrationManagerIdle::activate() {
    activate(500); // Default number of iterations is 500
}
