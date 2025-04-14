#include <Arduino.h>
#include "SpaceMouseHW_Joystick.h"
#include "config.h"
#include "kinematics.h" // Definition of the velocity array positions (TRANSzz/ROTXzz)

/**
 * Constructor
 */
SpaceMouseHW_Joystick_::SpaceMouseHW_Joystick_() {
}

/**
 * TODO
 */
void SpaceMouseHW_Joystick_::_calculateKinematicSensors(int16_t *velocity) {

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
void SpaceMouseHW_Joystick_::setAnalogReferenceVoltage(int debug) {
    analogReference(DEFAULT);
    Serial.println(F("Setting analog reference to 5V."));

    // The first measurements after changing the reference voltage can be wrong. So take 100ms to let the voltage stabilize and
    // take some measurements afterwards just to be sure. Performancewise this shouldn't be a problem due to the debug/setup
    // nature of this function.
    delay(100);
    for (int i = 0; i <= 8; i++) {
        readAllFromSensors();
    }
}

void SpaceMouseHW_Joystick_::printRawReads() {
    // Report back 0-1023 raw ADC 10-bit values if enabled
    for (int i = 0; i < NUM_SENSORS; i++) {
        _printRawRead(_axisNames[i], i);
    }
}

void SpaceMouseHW_Joystick_::printCentered() {
    // Report back 0-1023 raw ADC 10-bit values if enabled
    for (int i = 0; i < NUM_SENSORS; i++) {
        _printCentered(_axisNames[i], i);
    }
}