/**
 *  This file contains all functions specific to the Hall Effect sensors hardware
 *
 *
 * When using Hall Effect sensors the reading of the sensors has to be translated
 * to the joystick values
 *
 * Default assembly when looking from above on top of the space mouse
 *
 *    back(USB)     resulting axis (not from the single sensors)
 *
 *      7   6              Y+
 *        |                .
 *   8    |    3           .
 *     ---+---        X-...Z+...X+
 *   9    |    2           .
 *        |                .
 *      0   1              Y-
 *
 *
 * Movement table (expected)
 *
 * Movement     Plane           AX  AY  BX  BY  CX  CY  DX  DY  ||  H0  H1  H2  H3  H6  H7  H8  H9
 * West         Horizontal      0   +   0   0   0   -   0   0   ||  0   0   +   +   0   0   -   -
 * East         Horizontal      0   -   0   0   0   +   0   0   ||  0   0   -   -   0   0   +   +
 * North        Horizontal      0   0   0   -   0   0   0   +   ||  +   +   0   0   -   -   0   0
 * South        Horizontal      0   0   0   +   0   0   0   -   ||  -   -   0   0   +   +   0   0
 * Top          Vertical        -   0   -   0   -   0   -   0   ||  +   +   +   +   +   +   +   +               (all magnets further away)
 * Bottom       Vertical        +   0   +   0   +   0   +   0   ||  -   -   -   -   -   -   -   -               (all magnets closer by)
 * Rotx-fw      Vertical        -   0   0   0   +   0   0   0   ||  +   +   0   0   -   -   0   0
 * Rotx-bw      Vertical        +   0   0   0   -   0   0   0   ||  -   -   0   0   +   +   0   0
 * Roty-left    Vertical        0   0   +   0   0   0   -   0   ||  0   0   +   +   0   0   -   -
 * Roty-right   Vertical        0   0   -   0   0   0   +   0   ||  0   0   -   -   0   0   +   +
 * Rotz-clock   Horizontal      -   0   -   0   -   0   -   0   ||  +   -   +   -   +   -   +   -
 * Rotz-cclock  Horizontal      +   0   +   0   +   0   +   0   ||  -   +   -   +   -   +   -   +
 *
 */

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"

#include "hardware_hall.h" // External definitions for this file

#include "kinematics.h" // Definition of the velocity array positions

#include "../adc.h" // Definition of readAllFromSensors

#ifdef HALLEFFECT
/**
 * TODO
 */
void _calculateKinematicSensors(int *centered, int16_t *velocity) {

    // calculate sensors transX
    velocity[TRANSX] = (centered[HES1] - centered[HES0] + centered[HES6] - centered[HES7]) / 2;

    // calculate sensors transY
    velocity[TRANSY] = (centered[HES2] - centered[HES3] + centered[HES9] - centered[HES8]) / 2;

    // calculate sensors transZ
    velocity[TRANSZ] = (centered[HES0] + centered[HES1] + centered[HES2] + centered[HES3] + centered[HES6] + centered[HES7] + centered[HES8] + centered[HES9]) / 4;

    // rotX
    velocity[ROTX] = (centered[HES0] + centered[HES1] - centered[HES6] - centered[HES7]) / 2;

    // rotY
    velocity[ROTY] = (centered[HES8] + centered[HES9] - centered[HES2] - centered[HES3]) / 2;

    // rotZ
    velocity[ROTZ] = (centered[HES0] + centered[HES2] + centered[HES6] + centered[HES8] - centered[HES1] - centered[HES3] - centered[HES7] - centered[HES9]) / 4;
}

/**
 * @brief Set the analog reference voltage to 5V for debug 1 and to 2.56V otherwise
 * @param debug The current debug level of the spacemouse.
 */
void setAnalogReferenceVoltage(int debug) {
    if (debug == 1) {
        // Set the reference voltage for the AD Convertor to 5V only for the first calibration step (pinout/inversion calibration).
        analogReference(DEFAULT);
        Serial.println(F("Setting analog reference to 5V."));
    } else {
        // Set the reference voltage for the AD Convertor to 2.56V in order to get larger sensitivity.
        analogReference(INTERNAL);
        Serial.println(F("Setting analog reference to 2.56V."));
    }

    // The first measurements after changing the reference voltage can be wrong. So take 100ms to let the voltage stabilize and
    // take some measurements afterwards just to be sure. Performancewise this shouldn't be a problem due to the debug/setup
    // nature of this function.
    delay(100);
    int tempReads[8];
    for (int i = 0; i <= 8; i++) {
        readAllFromSensors(tempReads);
    }
}

#endif // #ifdef HALLEFFECT