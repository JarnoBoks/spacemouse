#include "ParamLoopFrequency.h"
#include "..\..\observers\DebugOutput\DebugOutputLoopFrequency.hpp"
#include <Arduino.h>

// Only log to serial if not using Arduino AVR architecture
#ifndef ARDUINO_ARCH_AVR
#ifndef ESP_PRINT(x)
#define ESP_PRINT(x) Serial.println(x)
#endif
#else
#define ESP_PRINT(x)
#endif

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugParamLoopFrequency::~DebugParamLoopFrequency() {
    Kinematics::getInstance()->detachObserver(AxisObserver); // Detach the observer from the kinematics object
    delete AxisObserver;                                     // Clean up the observer instance
    AxisObserver = nullptr;
}

void DebugParamLoopFrequency::apply() {
    ESP_PRINT("Debug Loop Frequency applied");

    // Instantiate the Observer for the Loop Frequency values and attach it to the hardware
    AxisObserver = new DebugOutputLoopFrequency();
    Kinematics::getInstance()->attachObserver(AxisObserver);
}

void DebugParamLoopFrequency::report() {
    ESP_PRINT("Debug Loop Frequency Information:");
}