#pragma once

#include "debugOutputAxes.hpp"
#include <knob/axis/KnobAxis.hpp>

/**
 * @brief Output class for modified axes values.
 * @details Writes the value of all the axes after applying the modifier function to the Serial monitor.
 *          This class inherits from OutputAxes and overrides the getAxisValue method to return the value of the axis after applying the modifier function.
 *          After outputting the values, it prints a newline character to the Serial monitor.
 */
class DebugOutputAxesModified : public DebugOutputAxes {
private:
    inline const int getAxisValue(const KnobAxis *axis) const override {
        return axis->getModValue(); // Get the axis value
    }

public:
    DebugOutputAxesModified(const bool outputNewline = true) : DebugOutputAxes(outputNewline) {}
    virtual ~DebugOutputAxesModified() = default;

    inline void update(IObservable *axisCollection) override final {
        DebugOutputAxes::update(axisCollection);
    }
};