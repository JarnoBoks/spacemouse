#include "CalibratorStateMinMax.hpp"
#include <sensor/SensorCollection.hpp>                              // For SensorCollection class
#include <observers/SensorCalibrations/SensorMinMaxCalibration.hpp> // For SensorMinMaxCalibration class
#include <visitors/printers/SensorConfigMinMaxPrinter.hpp>          // For SensorConfigMinMaxPrinter class
#include <visitors/SensorPersistConfigVisitor.hpp>                  // For SensorPersistConfigVisitor class
#include <common/esp_print.h>                                       // For ESP_PRINT and other print macros

#define CALIBRATION_MINMAX_DURATION 15 // Duration for MinMax calibration in seconds

void CalibratorStateMinMax::start() {
    RETURN_E_IF_NULL(context, "Calibrator context is null");                       // Check if the context is set
    RETURN_E_IF_NULL(context->getSensorCollection(), "Sensor collection is null"); // Check if the sensor collection is set

    // Do nothing if the observer is already set (failsafe check, should not happen)
    if (m_sensorObserver) {
        ESP_PRINT("MinMax calibration already active, skipping.");
        return; // Exit if the observer is already set
    }

    // Output start of the MinMax calibration to the console
    Serial.print(F("Move the spacemouse for "));
    Serial.print(CALIBRATION_MINMAX_DURATION);
    Serial.println(F(" sec."));

    // Attach the MinMaxCalibration observer to the SensorCollection
    m_sensorObserver = new SensorMinMaxCalibration(this);
    context->getSensorCollection()->attachObserver(m_sensorObserver);

    m_startCalibrationTime = millis();
}

// Check if the calibration should finish
// For the MinMax calibration, the apply method should check if the timeperiod has exceeded
void CalibratorStateMinMax::update() {
    // Finish calibration if the configured time has elapsed.
    if (millis() - m_startCalibrationTime > (CALIBRATION_MINMAX_DURATION * 1000)) {
        finish();
    }
}

/**
 * @brief Finishes the MinMax calibration process.
 */
void CalibratorStateMinMax::finish() {
    RETURN_E_IF_NULL(context, "Calibrator context is null");                       // Check if the context is set
    RETURN_E_IF_NULL(context->getSensorCollection(), "Sensor collection is null"); // Check if the sensor collection is set

    // Write results of the MinMax calibration to the console
    SensorConfigMinMaxPrinter printer;
    context->getSensorCollection()->accept(printer);

    // Persist the calibration data if requested
    if (m_persist) {
        SensorPersistConfigVisitor persistor;
        context->getSensorCollection()->accept(persistor);
    }

    // Context will destruct us, while the destructor of our Base class will take care of detaching and deleting the observer.
    context->finish();
}
