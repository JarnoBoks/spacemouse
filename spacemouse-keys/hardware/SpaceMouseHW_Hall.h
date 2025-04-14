#ifndef SPACEMOUSEHWHALL_h
#define SPACEMOUSEHWHALL_h

#include <Arduino.h>
#include "SpaceMouseHW.h"

// The following constants are here for more readable access to the arrays. You don't need to change this values!
// When using HallE sensors in centered or rawValues array
/* REMOVE
#define HES0 0
#define HES1 1
#define HES2 2
#define HES3 3
#define HES6 4
#define HES7 5
#define HES8 6
#define HES9 7
*/

// The Hall effect sensors aren't centered arount zero, due to the nature of the hardware.
// In my version of the Spacemouse, the values vary between -425 and 285, the centerpoint is thus around -70
// The MIN and MAX warning levels have to be shifted accordingly.
#define MINMAX_MINWARNING (100 - centerPoint)
#define MINMAX_MAXWARNING (100 + centerPoint)

enum Sensors {
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
    void setAnalogReferenceVoltage(int debug);
    void _calculateKinematicSensors(int16_t *velocity);

    // Functions for writing stored data to the serial interface
    void printRawReads();
    void printCentered();

private:
    char const *_axisNames[NUM_SENSORS] = {"HES0:", "HES1:", "HES2:", "HES3:", "HES6:", "HES7:", "HES8:", "HES9:"}; // 8
};

#endif // SPACEMOUSEHWHALL_h