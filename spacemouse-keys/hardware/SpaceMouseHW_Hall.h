#ifndef SPACEMOUSEHWHALL_h
#define SPACEMOUSEHWHALL_h

#include <Arduino.h>
#include "SpaceMouseHW.h"

// Values for the calibration warning levels. These values will be stored in the Base class members.
// The Hall effect sensors aren't centered arount zero, due to the nature of the hardware.
// In my version of the Spacemouse, the values vary between -425 and 285, the centerpoint is thus around -70
// The MIN and MAX warning levels have to be shifted accordingly.
#define HALL_WARN_CENTERPOINT_MIN 600 // Warning level for the minimal centerpoint value (centerpoint below this value throws a warning)
#define HALL_WARN_CENTERPOINT_MAX 800 // Warning level for the maximum centerpoint value (centerpoint above this value throws a warning)
#define HALL_WARN_MINMAX_MIN 300      // Warning level for the minimum value (absolute minimum below this value throws a warning)
#define HALL_WARN_MINMAX_MAX 170      // Warning level for the maximum value (absolute maximum below this value throws a warning)
#define HALL_WARN_MINMAX_RANGE 600    // Warning level for the minmax range calculation value (range below this value throws a warning)
#define HALL_AXIS_NAMES {"HES0", "HES1", "HES2", "HES3", "HES6", "HES7", "HES8", "HES9"}

// Default motion configuration for Hall sensors hardware
// This file is included in the main sketch and the default values are used if the EEPROM is not set yet.
// The values differ between the various hardware options.
#define DEF_SENS_TRANSX 0.80
#define DEF_SENS_TRANSY 0.99
#define DEF_SENS_POSITIVE_TRANSZ 2.5
#define DEF_SENS_NEGATIVE_TRANSZ 1.5

#define DEF_SENS_ROTX 1.20
#define DEF_SENS_ROTY 1.20
#define DEF_SENS_ROTZ 0.90

#define DEF_GATE_NEG_TRANSZ 15
#define DEF_GATE_ROTX 15
#define DEF_GATE_ROTY 15
#define DEF_GATE_ROTZ 15

#define DEF_INVERT_TX -1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_TY -1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_TZ -1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_RX -1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_RY -1 // 1 = no inversion, -1 = inversion
#define DEF_INVERT_RZ -1 // 1 = no inversion, -1 = inversion

// The following constants are here for more readable access to the arrays. You don't need to change this values!
// When using HallE sensors in centered or rawValues array
enum Hall_Sensors {
    HES0 = 0,
    HES1,
    HES2,
    HES3,
    HES6,
    HES7,
    HES8,
    HES9
};

/**
 * @brief Class to abstract from used hardware
 *
 *
 */
class SpaceMouseHW_Hall_ : public SpaceMouseHW_ {
public:
    SpaceMouseHW_Hall_();
    ~SpaceMouseHW_Hall_();

    bool BusyZeroing(uint16_t numIterations, boolean serialOutput);

    void SetAnalogReferenceVoltage(int debug) override;
    void CalculateKinematicSensors(int16_t *velocities) override;
};
#endif // SPACEMOUSEHWHALL_h