#include "ParamSensorAxisInformation.h"
#include <Arduino.h>
#include "kinematics/kinematics.h"
#include "hardware/hardware.h"
#include "observers/output_axisvalues.h"
#include "observers/output_sensorvalues.h"

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

    if (AxisObserver != nullptr) {
        Kinematics::getInstance()->detachObserver(AxisObserver); // Detach the observer from the hardware
        delete AxisObserver;                                     // Clean up the observer instance
        AxisObserver = nullptr;
    }
    if (SensorObserver != nullptr) {
        Kinematics::getInstance()->detachObserver(SensorObserver); // Detach the observer from the hardware
        delete SensorObserver;                                     // Clean up the observer instance
        SensorObserver = nullptr;
    }
}

void DebugParamSensorAxisInformation::apply() {

    // Instantiate the Observers and attach them to the hardware
    // SensorObserver = new Output_SensorValuesCenteredWithoutNewline();
    AxisObserver = new Output_AxisValuesModified();

    // Hardware::getInstance()->attachObserver(SensorObserver);
    Kinematics::getInstance()->attachObserver(AxisObserver);
}

void DebugParamSensorAxisInformation::report() {
    ESP_PRINT("Debug Axis Information:");
}