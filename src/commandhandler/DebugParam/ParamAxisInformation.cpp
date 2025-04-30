#include "ParamAxisInformation.h"
#include <Arduino.h>

#include "observers/output_axisvalues.h"

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugParamAxisInformation::~DebugParamAxisInformation() {
    if (AxisObserver != nullptr) {
        Kinematics::getInstance()->detachObserver(AxisObserver); // Detach the observer from the hardware
        delete AxisObserver;                                     // Clean up the observer instance
        AxisObserver = nullptr;
    }
}

void DebugParamAxisInformation::apply() {
    Serial.println("Debug Axis Information applied");

    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    AxisObserver = new Output_AxisValuesRaw();
    Kinematics::getInstance()->attachObserver(AxisObserver);
}

void DebugParamAxisInformation::report() {
    Serial.println("Debug Axis Information:");
}