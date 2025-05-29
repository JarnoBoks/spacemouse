#include "SensorMinMaxCalibration.hpp"

#include <sensor/SensorCollection.hpp>
#include <sensor/calibrator/states/ICalibratorState.hpp> // For CalibratorStateBase class
#include <visitors/SensorUpdateMinMaxVisitor.hpp>

#include <common/esp_print.h> // For ESP_PRINT and other print macros

SensorMinMaxCalibration::SensorMinMaxCalibration(ICalibratorState *calibratorState)
    : m_CalibratorState(calibratorState) {
}

void SensorMinMaxCalibration::update(IObservable *Observable) {

    SensorCollection *sensorCollection = static_cast<SensorCollection *>(Observable);
    RETURN_E_IF_NULL(sensorCollection, "Sensor collection null");

    // Read each Sensor and update minimum and maximum values in the SensorConfig if applicable.
    SensorUpdateMinMaxVisitor updater;
    sensorCollection->accept(updater); // Accept the visitor to update the min/max values

    // Notify the calibration manager that an update has been processed
    m_CalibratorState->update();
}