#include "DebugOutputAxes.hpp"
#include <Knob/KnobMotionVectorCollection.hpp>
#include <Knob/MotionVector/KnobMotionVector.hpp>
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

        KnobMotionVector *motionVector = static_cast<KnobMotionVectorCollection *>(knobMotionVectors)->getMotionVector(static_cast<MotionVector_t>(id)); // Pointer to the axis
        if (!motionVector)
            continue;

        Serial.print(motionVector->getDescriptor()); // Print the MotionVector name
        Serial.print(F(":"));
        const int value = getAxisValue(motionVector); // Get the value using the function defined in the derived class
        TextHelper::alignValue(value, 4);             // Align the value to the right with spaces
        Serial.print(value);
    }
}
