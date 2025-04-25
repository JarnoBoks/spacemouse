#ifndef OUTPUT_RAWSENSORVALUES_H
#define OUTPUT_RAWSENSORVALUES_H

#include "idebugmonitor.h"
#include "text.h"

/**
 * @brief Class to output raw sensor values for debugging purposes.
 * This class inherits from DebugMonitor and overrides the logSensorValues method to output raw sensor values.
 *
 * DEBUG_LEVEL = 1
 */
class Output_RawSensorValues : public IDebugMonitor {
private:
public:
    Output_RawSensorValues() {};  // Constructor to initialize currentLevel
    ~Output_RawSensorValues() {}; // nothing to do in destructor

    void update(const Axis *axis) override {
        // Doing nothing here, as this class is only used for sensors and not for axes.
    }

    void update(Hardware *hardware) override;
};

#endif // OUTPUT_RAWSENSORVALUES_H