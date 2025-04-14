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

#include "hardware.h" // External definitions for this file

#include "kinematics.h" // Definition of the velocity array

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