#ifndef SPACEMOUSEHWHALL_h
#define SPACEMOUSEHWHALL_h

#include <Arduino.h>
#include "SpaceMouseHW.h"

// Contains setup values for the Hall effect hardware. These values are used for configuration of this controller software.
// As opposed to default configuration parameters, who are stored in "defaults_hall.h", these values are used for the calibration of the hardware.
#define HALL_WARN_CENTERPOINT_MIN 600 // Warning level for the minimal centerpoint value (centerpoint below this value throws a warning)
#define HALL_WARN_CENTERPOINT_MAX 800 // Warning level for the maximum centerpoint value (centerpoint above this value throws a warning)
#define HALL_WARN_MINMAX_MIN 300      // Warning level for the minimum value (absolute minimum below this value throws a warning)
#define HALL_WARN_MINMAX_MAX 170      // Warning level for the maximum value (absolute maximum below this value throws a warning)
#define HALL_WARN_MINMAX_RANGE 600    // Warning level for the minmax range calculation value (range below this value throws a warning)
#define HALL_SENSOR_NAMES {"HES0", "HES1", "HES2", "HES3", "HES6", "HES7", "HES8", "HES9"}

// Default motion configuration for Hall effect hardware
#include "defaults_hall.h"

// The following constants are here for more readable access to the arrays. You don't need to change this values!
// When using HallE sensors in centered or rawValues array
enum HallSensorsId_t : uint8_t {
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

    bool BusyZeroing(const unsigned int num_iterations, const bool do_serial_output);

    void SetAnalogReferenceVoltage(const uint8_t debug) override;
    void CalculateKinematicSensors(int16_t *velocities) override;
};
#endif // SPACEMOUSEHWHALL_h