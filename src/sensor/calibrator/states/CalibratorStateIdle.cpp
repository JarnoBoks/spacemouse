#include "CalibratorStateIdle.hpp"
#include <sensor/SensorCollection.hpp>                                      // For SensorCollection class
#include <observers/SensorCalibrations/SensorCollectionIdleCalibration.hpp> // For SensorIdleCalibration class
#include <common/esp_print.h>                                               // For ESP_PRINT and other print macros

#include <Arduino.h> // For millis() function
/**
 * @brief Activates the idle calibration process.
 * @details This function activates the idle calibration process by creating an instance of the SensorCollectionIdleCalibration class and attaching it to the sensor collection.
 */
void CalibratorStateIdle::start() {
    RETURN_E_IF_NULL(context, "Calibrator context is null");                       // Check if the context is set
    RETURN_E_IF_NULL(context->getSensorCollection(), "Sensor collection is null"); // Check if the sensor collection is set

    // Do nothing if the observer is already set (failsafe check, should not happen)
    if (m_sensorObserver) {
        ESP_PRINT("Idle calibration already active, skipping.");
        return;
    }

    // Output start of the Idle calibration to the console
    Serial.println(F("Starting Idle calibration..."));

    // Attach the idle calibration observer to the SensorCollection
    m_sensorObserver = new SensorCollectionIdleCalibration(this);     // The observer is deleted in the base class destructor
    context->getSensorCollection()->attachObserver(m_sensorObserver); // TODO - Rename context to m_calibrator or similar, to avoid confusion with the SensorCollection

    m_startCalibrationTime = millis();
}

/**
 * @brief Applies the idle calibration state.
 * @details This function applies the idle calibration state. It does not require any management logic, as the apply method is sufficient to start the idle calibration process.
 */

// Check if the calibration should finish
// For the Idle calibration, the apply method should check/update the number of iterations processed
// and call the finalizer if the requested number of iterations is reached.
void CalibratorStateIdle::update() {
    RETURN_E_IF_NULL(m_sensorObserver, "Idle calibration observer is null"); // Check if the observer is set

    if (++m_processedIterations >= m_requestedIterations) {
        finish();
    }
}

void CalibratorStateIdle::finish() {

    // Call the finish method of the observer, which will finalize the idle calibration and output the results to the console.
    static_cast<SensorCollectionIdleCalibration *>(m_sensorObserver)->_finishCalibration(context->getSensorCollection());

    // Output results of the Idle calibration to the console
    Serial.println(F("Calibration finished!"));
    Serial.print(F("Took "));
    Serial.print(millis() - m_startCalibrationTime);
    Serial.println(F(" ms for "));
    Serial.print(m_processedIterations);
    Serial.println(F(" iterations."));

    // Context will destruct us, while the destructor of our Base class will take care of detaching and deleting the observer.
    context->finish();
}
