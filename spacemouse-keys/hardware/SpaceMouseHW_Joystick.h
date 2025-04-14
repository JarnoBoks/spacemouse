#ifndef SPACEMOUSEHWJOYSTICK_h
#define SPACEMOUSEHWJOYSTICK_h

#include <Arduino.h>
#include "SpaceMouseHW.h"

/* REMOVE
// The following constants are here for more readable access to the arrays. You don't need to change this values!
// Axes in centered or rawValues array
#define AX 0
#define AY 1
#define BX 2
#define BY 3
#define CX 4
#define CY 5
#define DX 6
#define DY 7
*/

enum Sensors {
    AX = 0,
    AY,
    BX,
    BY,
    CX,
    CY,
    DX,
    DY
};

#define MINMAX_MINWARNING 250
#define MINMAX_MAXWARNING 250

/**
 * @brief Class to abstract from used hardware
 *
 *
 */
class SpaceMouseHW_Joystick_ : public SpaceMouseHW_ {
public:
    SpaceMouseHW_Joystick_();

    bool busyZeroing(uint16_t numIterations, boolean debugFlag);

    void setAnalogReferenceVoltage(int debug);
    void _calculateKinematicSensors(int16_t *velocity);

    // Functions for writing stored data to the serial interface
    void printRawReads();
    void printCentered();

private:
    char const *_axisNames[8] = {"AX:", "AY:", "BX:", "BY:", "CX:", "CY:", "DX:", "DY:"}; // 8
};

#endif // SPACEMOUSEHWJOYSTICK_h