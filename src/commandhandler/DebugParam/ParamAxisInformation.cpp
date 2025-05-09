#include "ParamAxisInformation.hpp"
#include <Arduino.h>

#include "observers/DebugOutput/DebugOutputAxesSensitivity.hpp"
#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugParamAxisInformation::~DebugParamAxisInformation() {
    Kinematics::getInstance()->detachObserver(AxisObserver); // Detach the observer from the hardware
    delete AxisObserver;                                     // Clean up the observer instance
}

void DebugParamAxisInformation::apply() {
    ESP_PRINT("Debug Axis Information applied");

    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    AxisObserver = new DebugOutputAxesSensitivity();
    Kinematics::getInstance()->attachObserver(AxisObserver);
}

void DebugParamAxisInformation::report() {
    ESP_PRINT("Debug Axis Information:");
}