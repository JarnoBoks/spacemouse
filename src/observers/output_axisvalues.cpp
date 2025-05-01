#include "output_axisvalues.h"
#include "kinematics/kinematics.h"
#include "TextHelper.h"

// REVIEW - Move to Visitor?

void Output_AxisValues::update(Kinematics *kinematics) {
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
