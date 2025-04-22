#ifndef SPACEMOUSEHWJOYSTICK_h
#define SPACEMOUSEHWJOYSTICK_h

#include <Arduino.h>
#include "SpaceMouseHW.h"

// Contains setup values for the Hall effect hardware. These values are used for configuration of this controller software.
// As opposed to default configuration parameters, who are stored in "defaults_hall.h", these values are used for the calibration of the hardware.
#define JOYSTICK_WARN_CENTERPOINT_MIN 384 // Warning level for the minimal centerpoint value (centerpoint below this value throws a warning)
#define JOYSTICK_WARN_CENTERPOINT_MAX 640 // Warning level for the maximum centerpoint value (centerpoint above this value throws a warning)
#define JOYSTICK_WARN_MINMAX_MIN 250      // Warning level for the minimum value (absolute minimum below this value throws a warning)       // TODO - Necessary?
#define JOYSTICK_WARN_MINMAX_MAX 250      // Warning level for the maximum value (absolute maximum below this value throws a warning)       // TODO - Necessary?
#define JOYSTICK_WARN_MINMAX_RANGE 600    // Warning level for the minmax range calculation value (range below this value throws a warning)
#define JOYSTICK_SENSOR_NAMES {"AX:", "AY:", "BX:", "BY:", "CX:", "CY:", "DX:", "DY:"}

// Default motion configuration for Joystick hardware
#include "defaults_joystick.h"

// The following constants are here for more readable access to the arrays.
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

    bool BusyZeroing(const unsigned int num_iterations, const bool do_serial_output);

    void SetAnalogReferenceVoltage(const uint8_t debug) override;
    void CalculateKinematicSensors(int16_t *velocities) override;
};

#endif // SPACEMOUSEHWJOYSTICK_h