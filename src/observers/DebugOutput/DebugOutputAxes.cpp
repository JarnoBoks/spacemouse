#include "DebugOutputAxes.hpp"
#include <knob/KnobAxisCollection.hpp>
#include <knob/axis/KnobAxis.hpp>
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
    // DEVNOTE - The Observable is a KnobAxisCollection. Cast is necessary to access Collection functions.
    for (uint8_t i = 0; i < static_cast<KnobAxisCollection *>(knobMotionVectors)->getItemCount(); i++) {
        TextHelper::printLeadingComma(i); // Print a komma if it's not the first axis

        KnobAxis *axis = static_cast<KnobAxisCollection *>(knobMotionVectors)->getAxis(i); // Pointer to the axis
        if (!axis)
            continue;

        Serial.print(axis->getDescriptor());
        Serial.print(F(":"));
        const int value = getAxisValue(axis); // Get the value using the function defined in the derived class
        TextHelper::alignValue(value, 4);
        Serial.print(value);
    }
}
