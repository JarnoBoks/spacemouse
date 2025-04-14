///  This file contains all functions specific to the Joystick sensors hardware

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
    velocity[TRANSX] = (-centered[CY] + centered[AY]);

    // calculate sensors transY
    velocity[TRANSY] = (-centered[BY] + centered[DY]);

    // calculate sensors transZ
    velocity[TRANSZ] = -centered[AX] - centered[BX] - centered[CX] - centered[DX];

    // rotX
    velocity[ROTX] = (-centered[CX] + centered[AX]);

    // rotY
    velocity[ROTY] = (-centered[BX] + centered[DX]);

    // rotZ
    velocity[ROTZ] = (centered[AY] + centered[BY] + centered[CY] + centered[DY]);
}