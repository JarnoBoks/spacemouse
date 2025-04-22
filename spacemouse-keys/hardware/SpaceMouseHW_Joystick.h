#ifndef SPACEMOUSEHWJOYSTICK_h
#define SPACEMOUSEHWJOYSTICK_h

#include <Arduino.h>
#include "SpaceMouseHW.h"

// Values for the calibration warning levels. These values will be stored in the Base class members.
#define JOYSTICK_WARN_CENTERPOINT_MIN 384 // Warning level for the minimal centerpoint value (centerpoint below this value throws a warning)
#define JOYSTICK_WARN_CENTERPOINT_MAX 640 // Warning level for the maximum centerpoint value (centerpoint above this value throws a warning)
#define JOYSTICK_WARN_MINMAX_MIN 250      // Warning level for the minimum value (absolute minimum below this value throws a warning)       // TODO - Necessary?
#define JOYSTICK_WARN_MINMAX_MAX 250      // Warning level for the maximum value (absolute maximum below this value throws a warning)       // TODO - Necessary?
#define JOYSTICK_WARN_MINMAX_RANGE 600    // Warning level for the minmax range calculation value (range below this value throws a warning)
#define JOYSTICK_AXIS_NAMES {"AX:", "AY:", "BX:", "BY:", "CX:", "CY:", "DX:", "DY:"}

// Default motion configuration for Hall effect hardware
#include "defaults_hall.h"

// The following constants are here for more readable access to the arrays. You don't need to change this values!
// Axes in centered or rawValues array
enum Joystick_Sensors : uint8_t {
    AX = 0,
    AY,
    BX,
    BY,
    CX,
    CY,
    DX,
    DY
};

/**
 * @brief Class to abstract from used hardware
 *
 *
 */
class SpaceMouseHW_Joystick_ : public SpaceMouseHW_ {
public:
    SpaceMouseHW_Joystick_();
    ~SpaceMouseHW_Joystick_();

    bool BusyZeroing(uint16_t numIterations, boolean serialOutput);

    void SetAnalogReferenceVoltage(int debug) override;
    void CalculateKinematicSensors(int16_t *velocities) override;
};

#endif // SPACEMOUSEHWJOYSTICK_h