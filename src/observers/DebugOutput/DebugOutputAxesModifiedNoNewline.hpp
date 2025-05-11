#pragma once

#include "DebugOutputAxes.hpp"
#include "axis/axes/Axis.hpp"

/**
 * @brief Output class for modified axes values.
 * @details Writes the value of all the axes after applying the modifier function to the Serial monitor.
 *          This class inherits from DebugOutputAxes and overrides the getAxisValue method to return the value of the axis after applying the modifier function.
 *          After outputting the values, it does not print a newline character to the Serial monitor.
 */
class DebugOutputAxesModifiedNoNewline : public DebugOutputAxes {
protected:
    // REVIEW - Can this be made private?
    /**
     * @brief Get the axis value after applying the modifier function.
     * @details This function overrides the getAxisValue method from the base class to return the value of the axis after applying the modifier function.
     * @param axis Pointer to the Axis object.
     * @return The modified value of the axis.
     */
    inline const int getAxisValue(const Axis *axis) const override {
        return axis->getModValue(); // Get the axis value
    }
};