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

// Default motion configuration for Joystick hardware
// This file is included in the main sketch and the default values are used if the EEPROM is not set yet.
// The values differ between the various hardware options.
#define DEF_SENS_TRANSX 2.0
#define DEF_SENS_TRANSY 2.0
#define DEF_SENS_POSITIVE_TRANSZ 0.5
#define DEF_SENS_NEGATIVE_TRANSZ 2.5

#define DEF_SENS_ROTX 1.5
#define DEF_SENS_ROTY 1.5
#define DEF_SENS_ROTZ 2.0

#define DEF_GATE_NEG_TRANSZ 15
#define DEF_GATE_ROTX 15
#define DEF_GATE_ROTY 15
#define DEF_GATE_ROTZ 15

#define DEF_INVERT_TX 1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_TY 1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_TZ 1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_RX 1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_RY 1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_RZ 1 // 1 = no inversion, -1 = inversion

// The following constants are here for more readable access to the arrays. You don't need to change this values!
// Axes in centered or rawValues array
enum Joystick_Sensors {
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