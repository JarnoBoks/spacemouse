#include "DebugOutputAxes.hpp"

#include "kinematics/kinematics.h" // NOTE Included in header file too.
#include "TextHelper.h"

/**
 * @brief Writes the debug output for the axes, using the getAxisValue function to get the axis value.
 * @details This function is called by the Kinematics class to output the axis values to the serial monitor.
 *          The pure virtual getAxisValue function is used to get the axis value, which is  overridden in derived classes.
 *          The function iterates through all axes and prints their names and values to the serial monitor.
 * @param kinematics Pointer to the Kinematics object.
 */
void DebugOutputAxes::update(Kinematics *kinematics) {
    if (!isDebugOutputDue() || kinematics == nullptr) {
        return; // If the debug output is not due, do nothing
    }

    for (uint8_t id = 0; id < AxisType_t::LENGTH; id++) {
        TextHelper::printLeadingComma(id); // Print a komma if it's not the first axis

        Axis *axis = kinematics->getAxis(static_cast<AxisType_t>(id)); // Pointer to the axis

        Serial.print(axis->getName()); // Print the sensor name
        Serial.print(F(":"));
        const int value = getAxisValue(axis); // Get the axis value using the virtual function
        TextHelper::alignValue(value, 4);     // Align the value to the right with spaces
        Serial.print(value);
    }
}
