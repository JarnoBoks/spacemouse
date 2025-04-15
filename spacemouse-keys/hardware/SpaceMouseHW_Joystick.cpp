#include <Arduino.h>
#include "SpaceMouseHW_Joystick.h"
#include "config.h"
#include "kinematics.h" // Definition of the velocity array positions (TRANSzz/ROTXzz)

/**
 * Constructor/Destructor
 */
SpaceMouseHW_Joystick_::SpaceMouseHW_Joystick_()
    : SpaceMouseHW_(JOYSTICK_WARN_CENTERPOINT_MIN, JOYSTICK_WARN_CENTERPOINT_MAX, JOYSTICK_WARN_MINMAX_MIN, JOYSTICK_WARN_MINMAX_MAX) {
}

SpaceMouseHW_Joystick_::~SpaceMouseHW_Joystick_() {}

/**
 * TODO
 */
void SpaceMouseHW_Joystick_::CalculateKinematicSensors(int16_t *velocity) {

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
void SpaceMouseHW_Joystick_::SetAnalogReferenceVoltage(int debug) {
    analogReference(DEFAULT);
    Serial.println(F("Setting analog reference to 5V."));

    // The first measurements after changing the reference voltage can be wrong. So take 100ms to let the voltage stabilize and
    // take some measurements afterwards just to be sure. Performancewise this shouldn't be a problem due to the debug/setup
    // nature of this function.
    delay(100);
    for (uint8_t i = 0; i <= 8; i++) {
        ReadAllFromSensors();
    }
}

bool SpaceMouseHW_Joystick_::BusyZeroing(uint16_t numIterations, boolean debugFlag) {
    if (debugFlag) {
        Serial.println(F("Zeroing Joysticks..."));
    }

    return SpaceMouseHW_::BusyZeroing(numIterations, debugFlag);
}