#include "SensorCalibrationManagerMinMax.hpp"
#include "..\..\observers\Calibrators\SensorMinMaxCalibration.h" // For SensorMinMaxCalibration class

void SensorCalibrationManagerMinMax::activate() {
    SensorCalibrationManager::activate();

    // Attach the MinMax calibration observer to the hardware
    currentCalibration = new SensorMinMaxCalibration(this);
    m_SensorCollection->attachObserver(currentCalibration);
}
