#include "Calibrator.hpp"
#include <sensor/calibrator/states/ICalibratorState.hpp>

#include <Arduino.h> // For Serial class

/**
 * @brief Constructor for the Calibrator class.
 * @details Initializes the Calibrator with a pointer to the SensorCollection.
 * @param sensorCollection Pointer to the SensorCollection instance.
 */
Calibrator::Calibrator(SensorCollection *sensorCollection)
    : m_SensorCollection(sensorCollection),
      m_currentState(nullptr) {
}

/**
 * @brief Starts a new calibration, but only if there isn't a calibration in progress.
 * @param state Pointer to the new calibrator state to set.
 * @return The result of the start operation.
 * @retval true, the new calibration was started successfully.
 * @retval false, calibration is already in progress, so the new calibration was not set.

 */
const bool Calibrator::start(ICalibratorState *state) {
    if (m_currentState) {
        Serial.println(F("Wait for current calibration to finish.")); // Print a warning message
        return false;                                                 // Return false if a calibration is already in progress
    }

    m_currentState = state;
    m_currentState->set_context(this);
    m_currentState->start(); // Start the new calibration state
    return true;
}

/**
 * @brief Finishes the current calibration process.
 * @details This function cleans up the current state by deleting it and setting the pointer to nullptr.
 * @warning This function should only be called from the current running state.
 */
void Calibrator::finish() {
    if (m_currentState) {
        delete m_currentState;
        m_currentState = nullptr; // Clear the current state pointer
    }
}
