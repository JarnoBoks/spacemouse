#include "DebugOutputAxes.hpp"
#include <axis/KnobMotionVectorCollection.hpp>
#include <axis/axes/Axis.hpp>
#include <common/TextHelper.h>

/**
 * @brief   Writes the debug output for the axes, using the getAxisValue function to get the axis value.
 * @details This function is called by the Kinematics class to output the axis values to the serial monitor.
 *          The pure virtual getAxisValue function is used to get the axis value, which is  overridden in derived classes.
 *          The function iterates through all axes and prints their names and values to the serial monitor.
 * @param kinematics Pointer to the Kinematics object.
 */
void DebugOutputAxes::update(IObservable *knobMotionVectors) {
    if (!isDebugOutputDue() || !knobMotionVectors) {
        return;
    }

    // REFACTOR - Do the loop constraint(itemCount) for all collections (sensors, keys, axes) in the base class
    // REVIEW - Test - static cast is not necessary, but it is more readable
    for (uint8_t id = 0; id < static_cast<KnobMotionVectorCollection *>(knobMotionVectors)->getItemCount(); id++) {
        TextHelper::printLeadingComma(id); // Print a komma if it's not the first axis

        KnobMotionVector *axis = static_cast<KnobMotionVectorCollection *>(knobMotionVectors)->getMotionVector(static_cast<MotionVector_t>(id)); // Pointer to the axis
        if (!axis)
            continue;

        Serial.print(axis->getName()); // Print the sensor name
        Serial.print(F(":"));
        const int value = getAxisValue(axis); // Get the axis value using the virtual function
        TextHelper::alignValue(value, 4);     // Align the value to the right with spaces
        Serial.print(value);
    }
}
