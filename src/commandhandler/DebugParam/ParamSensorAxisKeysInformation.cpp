#include "ParamSensorAxisKeysInformation.hpp"

// Observable classes that are used in this file
#include "kinematics/kinematics.h"
#include "sensor/SensorCollection.hpp" // For SensorCollection class

// Observers that are used in this file.
#include "observers/DebugOutput/DebugOutputAxesModified.hpp"             // Implementation class for axes observer       //REVIEW - Can we use the Interface instead of the base class?
#include "observers/DebugOutput/DebugOutputSensorsCenteredNoNewline.hpp" // Implementation class for sensor observers     //REVIEW - Can we use the Interface instead of the base class?

#include <common/esp_print.h> // For ESP_PRINT

#include <Arduino.h>

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugParamSensorAxisKeysInformation::~DebugParamSensorAxisKeysInformation() {
    Kinematics::getInstance()->detachObserver(AxisObserver);          // Detach the observer from the hardware
    delete AxisObserver;                                              // Clean up the observer instance
    m_Context->getSensorCollection()->detachObserver(SensorObserver); // Detach the sensor observer from the sensor collection
    delete SensorObserver;                                            // Clean up the observer instance
}

void DebugParamSensorAxisKeysInformation::apply() {

    // Instantiate the Observers and attach them to the hardware
    SensorObserver = new DebugOutputSensorsCenteredNoNewline();
    AxisObserver = new DebugOutputAxesModified();

    m_Context->getSensorCollection()->attachObserver(SensorObserver); // Attach the sensor observer to the sensor collection
    Kinematics::getInstance()->attachObserver(AxisObserver);
}

void DebugParamSensorAxisKeysInformation::report() {
    ESP_PRINT("Debug Axis Information:");
}