#include "CalibratorStateMinMax.hpp"
#include <sensor/SensorCollection.hpp>                              // For SensorCollection class
#include <observers/SensorCalibrations/SensorMinMaxCalibration.hpp> // For SensorMinMaxCalibration class
#include <visitors/printers/SensorMinMaxPrinter.hpp>                // For SensorMinMaxPrinter class
#include <visitors/SensorPersistConfigVisitor.hpp>                  // For SensorPersistConfigVisitor class
#include <visitors/SensorResetMinMaxVisitor.hpp>                    // For SensorResetMinMaxVisitor class
#include <common/esp_print.h>                                       // For ESP_PRINT and other print macros

#define CAL_MINMAX_DURATION_SEC 15 // Duration for MinMax calibration in seconds.

void CalibratorStateMinMax::start() {
    RETURN_E_IF_NULL(context, "Calibrator context is null");                       // Check if the context is set
    RETURN_E_IF_NULL(context->getSensorCollection(), "Sensor collection is null"); // Check if the sensor collection is set

    // Do nothing if the observer is already set (failsafe check, should not happen)
    if (m_sensorObserver) {
        ESP_PRINT("MinMax calibration already active, skipping.");
        return; // Exit if the observer is already set
    }

    // Output start of the MinMax calibration to the console
    Serial.print(F("Move the knob for "));
    Serial.print(CAL_MINMAX_DURATION_SEC);
    Serial.println(F(" sec."));

    // Reset the current configuration settings for min and max values of all sensors
    SensorResetMinMaxVisitor resetVisitor;
    context->getSensorCollection()->accept(resetVisitor);

    // Attach the MinMaxCalibration observer to the SensorCollection
    m_sensorObserver = new SensorMinMaxCalibration(this);
    context->getSensorCollection()->attachObserver(m_sensorObserver);

    m_startCalibrationTime = millis();
}

// Check if the calibration should finish
// For the MinMax calibration, the apply method should check if the timeperiod has exceeded
// REFACTOR - This could be programmed more memory efficiently by converting to seconds immediately
void CalibratorStateMinMax::update() {
    // Finish calibration if the configured time has elapsed.
    unsigned long elapsedTimeMs = millis() - m_startCalibrationTime;
    if (elapsedTimeMs > (CAL_MINMAX_DURATION_SEC * 1000)) {
        finish();
    }

    if (elapsedTimeMs / 1000 > m_lastOutputSec) {
        m_lastOutputSec = (unsigned int)elapsedTimeMs / 1000; // Update the last output time in seconds
        if (m_lastOutputSec < CAL_MINMAX_DURATION_SEC) {
            Serial.print(F("Remaining: "));
            Serial.print(CAL_MINMAX_DURATION_SEC - m_lastOutputSec);
            Serial.println(F(" sec."));
        }
    }
}

/**
 * @brief Finishes the MinMax calibration process.
 */
void CalibratorStateMinMax::finish() {
    RETURN_E_IF_NULL(context, "Calibrator context is null");                       // Check if the context is set
    RETURN_E_IF_NULL(context->getSensorCollection(), "Sensor collection is null"); // Check if the sensor collection is set

    // Write results of the MinMax calibration for each sensor to the console
    SensorMinMaxPrinter printer;
    context->getSensorCollection()->accept(printer);

    // Persist the calibration data if requested and no warnings occurred
    if (m_persist && !printer.hasWarningsOccurred()) {
        Serial.print(F("Persisting minmax..."));
        SensorPersistConfigVisitor persistor;
        context->getSensorCollection()->accept(persistor);
    } else if (m_persist && printer.hasWarningsOccurred()) {
        Serial.println(F("Warnings: not persisting minmax."));
    } else {
        Serial.println(F("Not persisting minmax."));
    }

    // Context will destruct us, while the destructor of our Base class will take care of detaching and deleting the observer.
    context->finish();
}
