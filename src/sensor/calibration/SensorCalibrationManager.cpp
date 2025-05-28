#include "SensorCalibrationManager.hpp"
#include "sensor/SensorCollection.hpp"
#include "observers/IObserver.hpp"

#include <Arduino.h> // For PROGMEM

#ifndef CF
#define CF(x) ((const __FlashStringHelper *)x)
#endif

// FIXME - Add deadzone warning in the output.
#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This should be changed in the future.

// Text messages for calibration
static const char Error_AlreadyCalibrating[] PROGMEM = "Calibration already in progress!"; // Error message when calibration is already in progress

void SensorCalibrationManager::activate() {
    if (currentCalibration != nullptr) {
        Serial.println(CF(Error_AlreadyCalibrating));
        return;
    }
}

/**
 * @brief Deactivates the current calibration process.
 * @param warningsOccurred *Ignored* Indicates if any warnings occurred during calibration.
 * @details This function detaches the current calibration observer from the sensor collection and deletes the observer instance.
 *         It is called when the calibration process is completed or canceled.
 * REVIEW -Is the parameter warningsOccurred necessary? It is not used in the current implementation.
 */
void SensorCalibrationManager::deactivate(const bool warningsOccurred) {
    m_SensorCollection->detachObserver(currentCalibration);
    delete currentCalibration;

    // The SensorCalibration Manager can be deleted when there are no more calibrations running.
    // TODO - Test if deleting this; works as expected.
}
