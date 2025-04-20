#include <Arduino.h>
#include "SpaceMouseHW_Hall.h"
#include "eepromStorage.h"
#include "text.h"
#include "config.h"
#include "kinematics.h" // Definition of the velocity array positions (TRANSzz/ROTXzz)

static const char *Hall_axisNames[8] = HALL_AXIS_NAMES;

/**
 * Constructor / Destructor
 */
SpaceMouseHW_Hall_::SpaceMouseHW_Hall_()
    : SpaceMouseHW_(HALL_WARN_CENTERPOINT_MIN, HALL_WARN_CENTERPOINT_MAX, HALL_WARN_MINMAX_MIN, HALL_WARN_MINMAX_MAX, HALL_WARN_MINMAX_RANGE, Hall_axisNames) {}

SpaceMouseHW_Hall_::~SpaceMouseHW_Hall_() {}

/**
 * TODO
 */
void SpaceMouseHW_Hall_::CalculateKinematicSensors(int16_t *velocities) {

    // calculate sensors transX
    velocities[transX] = (centered[HES1] - centered[HES0] + centered[HES6] - centered[HES7]) / 2;

    // calculate sensors transY
    velocities[transY] = (centered[HES2] - centered[HES3] + centered[HES9] - centered[HES8]) / 2;

    // calculate sensors transZ
    velocities[transZ] = (centered[HES0] + centered[HES1] + centered[HES2] + centered[HES3] + centered[HES6] + centered[HES7] + centered[HES8] + centered[HES9]) / 4;

    // rotX
    velocities[rotX] = (centered[HES0] + centered[HES1] - centered[HES6] - centered[HES7]) / 2;

    // rotY
    velocities[rotY] = (centered[HES8] + centered[HES9] - centered[HES2] - centered[HES3]) / 2;

    // rotZ
    velocities[rotZ] = (centered[HES0] + centered[HES2] + centered[HES6] + centered[HES8] - centered[HES1] - centered[HES3] - centered[HES7] - centered[HES9]) / 4;
}

/**
 * @brief Set the analog reference voltage to 5V for debug 1 and to 2.56V otherwise
 * @param debug The current debug level of the spacemouse.
 */
void SpaceMouseHW_Hall_::SetAnalogReferenceVoltage(int debug) {

    //.print(F("Setting analog reference voltage to "));
    Serial.print(CF(Info_AnalogVoltage));
    if (debug == 1) {
        // Set the reference voltage for the AD Convertor to 5V only for the first calibration step (pinout/inversion calibration).
        analogReference(DEFAULT);
        Serial.println(F("5V"));
    } else {
        // Set the reference voltage for the AD Convertor to 2.56V in order to get larger sensitivity.
        analogReference(INTERNAL);
        Serial.println(F("2.56V"));
    }

    // The first measurements after changing the reference voltage can be wrong. So take 100ms to let the voltage stabilize and
    // take some measurements afterwards just to be sure. Performancewise this shouldn't be a problem due to the debug/setup
    // nature of this function.
    delay(100);

    // Read all sensors 8 times
    for (uint8_t i = 0; i < 8; i++) { // Changed from <= to < for correct iteration
        ReadAllFromSensors();
    }
}

bool SpaceMouseHW_Hall_::BusyZeroing(uint16_t numIterations, boolean serialOutput) {
    if (serialOutput) {
        Serial.println(F("Zeroing Sensors..."));
    }

    return SpaceMouseHW_::BusyZeroing(numIterations, serialOutput);
}