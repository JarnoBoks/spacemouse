#include <Arduino.h>
#include "SpaceMouseHW_Hall.h"
#include "config.h"
#include "kinematics.h" // Definition of the velocity array positions (TRANSzz/ROTXzz)

/**
 * Constructor
 */
SpaceMouseHW_Hall_::SpaceMouseHW_Hall_() {
}

/**
 * TODO
 */
void SpaceMouseHW_Hall_::_calculateKinematicSensors(int16_t *velocity) {

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
void SpaceMouseHW_Hall_::setAnalogReferenceVoltage(int debug) {
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

    // Read all sensors 8 times
    for (int i = 0; i <= 8; i++) {
        readAllFromSensors();
    }
}

void SpaceMouseHW_Hall_::printRawReads() {
    // Report back 0-1023 raw ADC 10-bit values if enabled
    for (int i = 0; i < NUM_SENSORS; i++) {
        _printRawRead(_axisNames[i], i);
    }
}

void SpaceMouseHW_Hall_::printCentered() {
    // Report back 0-1023 raw ADC 10-bit values if enabled
    for (int i = 0; i < NUM_SENSORS; i++) {
        _printCentered(_axisNames[i], i);
    }
}

bool SpaceMouseHW_Hall_::busyZeroing(uint16_t numIterations, boolean debugFlag) {
    if (debugFlag == true) {
        Serial.println(F("Zeroing HALL Sensors..."));
    }
    return SpaceMouseHW_::busyZeroing(numIterations, debugFlag);
}