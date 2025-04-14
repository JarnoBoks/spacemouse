///  This file contains all functions specific to the Joystick sensors hardware

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"

#include "hardware.h" // External definitions for this file

#include "kinematics.h" // Definition of the velocity array positions

#include "../adc.h" // Definition of readAllFromSensors

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

/**
 * @brief Set the analog reference voltage to 5V for debug 1 and to 2.56V otherwise
 * @param debug The current debug level of the spacemouse.
 */
void setAnalogReferenceVoltage(int debug) {
    Serial.println(F("Setting analog reference to 5V."));

    // The first measurements after changing the reference voltage can be wrong. So take 100ms to let the voltage stabilize and
    // take some measurements afterwards just to be sure. Performancewise this shouldn't be a problem due to the debug/setup
    // nature of this function.
    delay(100);
    int tempReads[8];
    for (int i = 0; i <= 8; i++) {
        readAllFromSensors(tempReads);
    }
}