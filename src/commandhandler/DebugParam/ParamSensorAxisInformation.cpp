#include "ParamSensorAxisInformation.h"
#include <Arduino.h>
#include "kinematics/kinematics.h"
#include "hardware/hardware.h"
#include "..\..\observers\DebugOutput\DebugOutputAxesModified.hpp"             // Implementation of the ODebugOutputAxes class
#include "..\..\observers\DebugOutput\DebugOutputSensorsCenteredNoNewline.hpp" // Implementation of the ODebugOutputSensors class

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
DebugParamSensorAxisInformation::~DebugParamSensorAxisInformation() {
    Hardware::getInstance()->detachObserver(SensorObserver);
    Kinematics::getInstance()->detachObserver(AxisObserver);
    delete SensorObserver;
    delete AxisObserver;
}

/**
 * @brief Applies the debug parameters by instantiating the observers and attaching them to the hardware & kinematics.
 * @details This method creates instances of the Output_SensorValuesCenteredWithoutNewline and DebugOutputAxesModified classes,
 *          and attaches them to the hardware and kinematics instances respectively.
 *          This allows for monitoring and reporting of sensor values and axis values during debugging.
 */
void DebugParamSensorAxisInformation::apply() {

    // Instantiate the Observers and attach them to the hardware
    SensorObserver = new DebugOutputSensorsCenteredNoNewline();
    AxisObserver = new DebugOutputAxesModified();

    Hardware::getInstance()->attachObserver(SensorObserver);
    Kinematics::getInstance()->attachObserver(AxisObserver);
}

void DebugParamSensorAxisInformation::report() {
    ESP_PRINT("Debug Axis Information:");
}